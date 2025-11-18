/*
  ESP32-S3 SD Card -> Google Drive (Apps Script) STREAMING uploader
  - Raw bytes body (NOT base64)
  - Params: token, name, mimeType, folderId (query string)
  - Streams with HTTPClient::sendRequest(Stream*, size)
  - Handles Google 302/301/etc with manual re-POST to the Location URL
  - Increases timeouts and disables connection reuse to avoid -11 timeouts
  - Parses JSON { ok, id, name, size, webViewUrl, ... }
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <ctype.h>  // isspace/isdigit

// ===== WiFi =====
const char* WIFI_SSID     = "Blacknet@Ueberlingen";
const char* WIFI_PASSWORD = "Ueberlingen2019";

// ===== Apps Script Web App =====
// Your working deployment (the /exec endpoint)
const char* WEBAPP_BASE =
  "https://script.google.com/macros/s/AKfycbz8dWlOPF_wcNfHRwrzWKuc6QB-jBiv7S6ODfcoz9uqy7vKSUGqdleDthApVtRmBQmj/exec";
const char* TOKEN     = "my_secret";
const char* FOLDER_ID = "1ealnpCAoC9H5pLFLJfjuo5vqwT0XOa84";

// ===== SD SPI pins (ESP32-S3) =====
#define SD_MOSI 35
#define SD_MISO 37
#define SD_SCK  36
#define SD_CS   39

// ---------- helpers ----------
String urlEncode(const String& s) {
  String out; char buf[4];
  for (size_t i = 0; i < s.length(); i++) {
    unsigned char c = (unsigned char)s[i];
    if (isalnum(c) || c=='-'||c=='_'||c=='.'||c=='~') out += (char)c;
    else { snprintf(buf, sizeof(buf), "%%%02X", c); out += buf; }
  }
  return out;
}
String toLowerExt(const String& name) {
  int dot = name.lastIndexOf('.');
  String ext = (dot >= 0) ? name.substring(dot + 1) : "";
  ext.toLowerCase(); return ext;
}
String guessMimeType(const String& filename) {
  String ext = toLowerExt(filename);
  if (ext=="jpg"||ext=="jpeg") return "image/jpeg";
  if (ext=="png") return "image/png";
  if (ext=="gif") return "image/gif";
  if (ext=="bmp") return "image/bmp";
  if (ext=="txt") return "text/plain";
  if (ext=="csv") return "text/csv";
  if (ext=="json") return "application/json";
  if (ext=="pdf") return "application/pdf";
  if (ext=="mp4") return "video/mp4";
  if (ext=="mp3") return "audio/mpeg";
  return "application/octet-stream";
}
String driveSafeName(const String& sdAbsPath) {
  String n = sdAbsPath.startsWith("/") ? sdAbsPath.substring(1) : sdAbsPath;
  n.replace('/', '_'); if (n.length()==0) n="upload.bin"; return n;
}

// --- tiny JSON readers (sufficient for your response) ---
String jsonGetString(const String& json, const char* key) {
  String pat = "\"" + String(key) + "\":";
  int i = json.indexOf(pat);
  if (i < 0) return "";
  i += pat.length();
  while (i < (int)json.length() && isspace((unsigned char)json[i])) i++;
  if (i >= (int)json.length() || json[i] != '\"') return "";
  i++; // skip opening quote
  String out;
  while (i < (int)json.length()) {
    char ch = json[i++];
    if (ch == '\\') {
      if (i >= (int)json.length()) break;
      char n = json[i++];
      if (n=='\"'||n=='\\'||n=='/') out += n;
      else if (n=='n') out += '\n';
      else if (n=='r') out += '\r';
      else if (n=='t') out += '\t';
      else out += n;
    } else if (ch == '\"') {
      break;
    } else {
      out += ch;
    }
  }
  return out;
}
bool jsonGetBool(const String& json, const char* key, bool def=false) {
  String pat = "\"" + String(key) + "\":";
  int i = json.indexOf(pat);
  if (i < 0) return def;
  i += pat.length();
  while (i < (int)json.length() && isspace((unsigned char)json[i])) i++;
  if (json.startsWith("true", i)) return true;
  if (json.startsWith("false", i)) return false;
  return def;
}
long jsonGetLong(const String& json, const char* key, long def=-1) {
  String pat = "\"" + String(key) + "\":";
  int i = json.indexOf(pat);
  if (i < 0) return def;
  i += pat.length();
  while (i < (int)json.length() && (isspace((unsigned char)json[i]) || json[i]=='\"')) i++;
  long sign = 1; if (i < (int)json.length() && json[i]=='-') { sign=-1; i++; }
  long v=0; bool any=false;
  while (i < (int)json.length() && isdigit((unsigned char)json[i])) { v=v*10+(json[i]-'0'); any=true; i++; }
  return any ? v*sign : def;
}

// --- paths / filter ---
String joinPath(const String& base, const String& name) { return (base=="/")?"/"+name:base+"/"+name; }
bool isSkippableDir(const String& name) {
  return (name=="System Volume Information") || name.startsWith("FOUND.");
}

// forward
void walkAndUpload(const String& dirPath);

// ------ low-level request that follows redirects correctly ------
// - For 301/302/303 -> follow with GET (no body)
// - For 307/308     -> repeat the same POST with body
int postStreamFollowingRedirects(const String& url,
                                 const String& mimeType,
                                 File& f, size_t fsize,
                                 String& outBody) {
  const int MAX_HOPS = 3;
  String curUrl = url;
  outBody = "";

  for (int hop = 0; hop < MAX_HOPS; ++hop) {
    // Always reset file for a retry (only used if we POST again)
    f.seek(0);

    WiFiClientSecure client;
    client.setInsecure(); // TODO: load proper root CA for production

    HTTPClient http;
    http.setFollowRedirects(HTTPC_DISABLE_FOLLOW_REDIRECTS); // manual handling
    http.setTimeout(25000);
    http.setReuse(false);
    if (!http.begin(client, curUrl)) {
      return -100; // couldn't start
    }
    http.addHeader("Accept", "application/json");
    http.addHeader("Connection", "close");

    // First request is POST with the file stream
    http.addHeader("Content-Type", mimeType);
    int code = http.sendRequest("POST", &f, fsize);

    if (code == 301 || code == 302 || code == 303 || code == 307 || code == 308) {
      // Get redirect target
      String loc = http.getLocation();
      http.end();
      if (!loc.length()) return -101; // redirect but no Location

      // 307/308 must repeat POST + body; 301/302/303 switch to GET (no body)
      bool repeatPost = (code == 307 || code == 308);

      if (!repeatPost) {
        // Follow with GET (no body) and return that response
        if (!http.begin(client, loc)) return -102;
        http.setTimeout(25000);
        http.setReuse(false);
        http.addHeader("Accept", "application/json");
        http.addHeader("Connection", "close");
        int gcode = http.GET();
        outBody = http.getString();
        http.end();
        return gcode;
      } else {
        // Repeat POST with the same file
        curUrl = loc;
        // loop and try again with POST to new URL
        continue;
      }
    }

    // Not a redirect → read and return
    outBody = http.getString();
    http.end();
    return code;
  }

  return -103; // too many redirects
}

// --- streaming upload (uses the redirect-aware POST) ---
bool uploadOneFile(const String& sdPathIn) {
  String sdPath = sdPathIn; if (!sdPath.startsWith("/")) sdPath = "/" + sdPath;

  File f = SD.open(sdPath, FILE_READ);
  if (!f) { Serial.printf("  ❌ Cannot open: %s\n", sdPath.c_str()); return false; }
  size_t fsize = f.size();
  if (fsize == 0) { Serial.printf("  ⚠️ Empty file, skipping: %s\n", sdPath.c_str()); f.close(); return false; }

  String fname    = driveSafeName(sdPath);
  String mimeType = guessMimeType(fname);
  Serial.printf("→ Uploading: %s  (MIME: %s, %u bytes)\n", fname.c_str(), mimeType.c_str(), (unsigned)fsize);

  String url = String(WEBAPP_BASE)
               + "?token="    + urlEncode(String(TOKEN))
               + "&name="     + urlEncode(fname)
               + "&mimeType=" + urlEncode(mimeType)
               + "&folderId=" + urlEncode(String(FOLDER_ID));

  String resp;
  int code = postStreamFollowingRedirects(url, mimeType, f, fsize, resp);
  Serial.printf("  HTTP %d\n", code);
  f.close();

  if (!resp.isEmpty()) {
    bool ok = jsonGetBool(resp, "ok", false);
    if (!ok) {
      String err = jsonGetString(resp, "error");
      if (err.isEmpty()) {
        // If we accidentally got HTML (e.g., 302 page), show a hint
        if (resp.indexOf("<HTML>") >= 0 || resp.indexOf("<html") >= 0) {
          Serial.println("  ❌ Server returned HTML (likely a redirect page).");
        }
      }
      Serial.printf("  ❌ Server error: %s\n", err.length()?err.c_str():"(unknown)");
      Serial.printf("  Raw response: %s\n", resp.c_str());
      return false;
    }
    String id   = jsonGetString(resp, "id");
    String n    = jsonGetString(resp, "name");
    long   sz   = jsonGetLong(resp,    "size", -1);
    String vurl = jsonGetString(resp, "webViewUrl");
    Serial.printf("  ✅ Uploaded OK: id=%s name=%s size=%ld\n", id.c_str(), n.c_str(), sz);
    if (vurl.length()) Serial.printf("  🔗 View: %s\n", vurl.c_str());
    return true;
  } else {
    Serial.println("  ⚠️ Empty response body");
    return (code == 200); // Apps Script always returns 200; we just didn't read it
  }
}

void walkAndUpload(const String& dirPath) {
  String abs = dirPath; if (abs.isEmpty() || abs[0] != '/') abs = "/" + abs;
  File dir = SD.open(abs);
  if (!dir || !dir.isDirectory()) { Serial.printf("⚠️ Not a directory: %s\n", abs.c_str()); return; }

  File entry;
  while ((entry = dir.openNextFile())) {
    String name = String(entry.name());
    String path = joinPath(abs, name);
    if (entry.isDirectory()) {
      Serial.printf("📁 Dir: %s\n", name.c_str());
      if (!isSkippableDir(name)) walkAndUpload(path); else Serial.println("  ↪️ Skipping system folder.");
    } else {
      Serial.printf("📄 File: %s  (%u bytes)\n", path.c_str(), (unsigned)entry.size());
      bool ok = uploadOneFile(path);
      Serial.printf("  %s %s\n", ok ? "✅" : "❌", path.c_str());
      delay(200);
    }
    entry.close();
  }
  dir.close();
}

void connectWiFi() {
  Serial.printf("Connecting to WiFi: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 25000) { Serial.print("."); delay(500); }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) Serial.printf("✅ WiFi connected. IP: %s\n", WiFi.localIP().toString().c_str());
  else Serial.println("❌ WiFi connect failed.");
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\nESP32-S3 SD → Google Drive Uploader (streaming + redirect-safe)");

  connectWiFi();
  if (WiFi.status() != WL_CONNECTED) { Serial.println("Stopping: no WiFi."); while (true) delay(1000); }

  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SPI)) { Serial.println("❌ SD Card initialization failed!"); while (true) delay(1000); }
  Serial.println("✅ SD Card initialized.");

  if (!SD.exists("/test.txt")) {
    File f = SD.open("/test.txt", FILE_WRITE);
    if (f) { f.println("Hello from ESP32-S3!"); f.println("Streaming upload test."); f.close(); Serial.println("✅ Created /test.txt"); }
  }

  walkAndUpload("/");
  Serial.println("🎉 Done walking SD card.");
}

void loop() {}
