// Service layer: WiFi (access point + home network) and HTTP server.
// Runs in its own task on core 0; reads params only through snapshots and writes
// them only under the params lock, so it never delays the CAN task noticeably.
#include "webinterface.h"

#include <FS.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>

#include "board.h"
#include "canmanager.h"
#include "gvret_bridge.h"
#include "signal_table.h"
#include "status_led.h"
#include "params_lock.h"

#if __has_include("secrets.h")
#include "secrets.h"
#else
#warning "src/secrets.h missing: using secrets.example.h (placeholder credentials)"
#include "secrets.example.h"
#endif

namespace {

constexpr size_t MAX_FIELDS = 256;
constexpr uint16_t HTTP_PORT = 80;

WebServer server(HTTP_PORT);
bool gFsOk = false;
ParamsSnapshot gSnap;  // only used by the web task

String ipToString(const IPAddress &ip) {
    return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

// ---------------------------------------------------------------- JSON helpers

void appendJsonString(String &out, const char *text) {
    out += '"';
    for (const char *p = text; *p; p++) {
        const char c = *p;
        if (c == '"' || c == '\\') {
            out += '\\';
            out += c;
        } else if ((uint8_t)c < 0x20) {
            char buf[8];
            snprintf(buf, sizeof buf, "\\u%04x", (unsigned)(uint8_t)c);
            out += buf;
        } else {
            out += c;
        }
    }
    out += '"';
}

void appendKey(String &out, const char *key) {
    if (out.length() > 1) {
        out += ',';
    }
    appendJsonString(out, key);
    out += ':';
}

void appendNumber(String &out, double value, uint8_t decimals) {
    if (!isfinite(value)) {
        out += "null";  // never emit nan/inf: they are not valid JSON
        return;
    }
    out += String(value, (unsigned int)decimals);
}

struct FieldFilter {
    String raw;
    bool active = false;

    bool wants(const char *key) const {
        if (!active) {
            return true;
        }
        const size_t keyLen = strlen(key);
        int start = 0;
        while (start < (int)raw.length()) {
            int end = raw.indexOf(',', start);
            if (end < 0) {
                end = raw.length();
            }
            if ((size_t)(end - start) == keyLen && strncmp(raw.c_str() + start, key, keyLen) == 0) {
                return true;
            }
            start = end + 1;
        }
        return false;
    }
};

void appendSignalValue(String &out, const SignalDef &def, const uint8_t *base) {
    const double value = readSignal(def, base);
    switch (def.format) {
        case F_BOOL: out += value != 0.0 ? "true" : "false"; break;
        case F_INT: out += String((long)lround(value)); break;
        case F_UINT: out += String((unsigned long)(value < 0 ? 0 : lround(value))); break;
        case F_NUM:
        default: appendNumber(out, value, def.decimals); break;
    }
}

String buildLiveJson(const FieldFilter &filter, const String &extra) {
    takeSnapshot(gSnap);
    const uint32_t now = millis();
    String payload;
    payload.reserve(9000);
    payload += '{';

    String stale;
    for (size_t i = 0; i < kSignalCount; i++) {
        const SignalDef &def = kSignals[i];
        if (!filter.wants(def.name)) {
            continue;
        }
        appendKey(payload, def.name);
        appendSignalValue(payload, def, snapshotGroupBase(gSnap, def.group));
        if (def.rx != RX_NONE) {
            const uint32_t last = gSnap.last_rx_ms[def.rx];
            if (last == 0 || (uint32_t)(now - last) > RX_TIMEOUT_MS) {
                if (stale.length()) {
                    stale += ',';
                }
                appendJsonString(stale, def.name);
            }
        }
    }

    const bool staConnected = WiFi.status() == WL_CONNECTED;
    auto addStr = [&](const char *key, const String &value) {
        if (filter.wants(key)) {
            appendKey(payload, key);
            appendJsonString(payload, value.c_str());
        }
    };
    auto addBool = [&](const char *key, bool value) {
        if (filter.wants(key)) {
            appendKey(payload, key);
            payload += value ? "true" : "false";
        }
    };
    auto addUInt = [&](const char *key, uint32_t value) {
        if (filter.wants(key)) {
            appendKey(payload, key);
            payload += String((unsigned long)value);
        }
    };

    // Legacy UI flags (the set fields are always writable).
    addBool("set_rpm_active", true);
    addBool("set_coolant_active", true);
    addBool("set_oil_active", true);
    addBool("rpm_override", gSnap.ecu_override.engine_speed_active);
    addBool("coolant_override", gSnap.ecu_override.coolant_temp_active);
    addBool("coolant_warning_override", gSnap.ecu_override.coolant_warning_active);
    addBool("fuel_override", gSnap.ecu_override.fuel_active);

    // Network status
    addStr("ap_ssid", VCU_WIFI_AP_SSID);
    addStr("ap_ip", ipToString(WiFi.softAPIP()));
    addUInt("ap_clients", WiFi.softAPgetStationNum());
    addStr("sta_ssid", VCU_WIFI_HOME_SSID);
    addBool("sta_connected", staConnected);
    addStr("sta_ip", staConnected ? ipToString(WiFi.localIP()) : String(""));
    addBool("lte_connected", false);
    addStr("lte_operator", "");
    addUInt("lte_signal", 0);
    // Values known to the touch UI: wifi, lte, offline ("offline" = no home network).
    addStr("network_mode", staConnected ? "wifi" : "offline");
    addStr("network_label", staConnected ? "WiFi" : "Offline");

    // Diagnostics
    const CanTaskStats rt = canTaskStats();
    addUInt("uptime_ms", now);
    addUInt("rt_cycles", rt.cycles);
    addUInt("rt_overruns", rt.overruns);
    addUInt("rt_max_cycle_us", rt.max_cycle_us);
    addUInt("rt_tx_failed", rt.tx_failed);
    addUInt("rt_started_ms", rt.started_ms);
    addUInt("can1_error", canManager_error(1));
    for (uint8_t bus = 0; bus < 3; bus++) {
        static const char *const kPrefix[3] = {"can0_", "can1_", "can2_"};
        const CanBusDiag d = canBusDiag(bus);
        const String p = kPrefix[bus];
        addUInt((p + "rx_frames").c_str(), d.rx_frames);
        addUInt((p + "tec").c_str(), d.tec);
        addUInt((p + "rec").c_str(), d.rec);
        addUInt((p + "eflg").c_str(), d.eflg);
        addBool((p + "bus_off").c_str(), d.bus_off);
    }
    addUInt("can2_error", canManager_error(2));
    addBool("can0_active", can0Active());
    addUInt("gvret_dropped", gvretBridgeDropped());
    addUInt("free_heap", ESP.getFreeHeap());

    if (filter.wants("stale")) {
        appendKey(payload, "stale");
        payload += '[';
        payload += stale;
        payload += ']';
    }
    payload += extra;
    payload += '}';
    return payload;
}

FieldFilter filterFromRequest() {
    FieldFilter filter;
    if (server.hasArg("fields")) {
        filter.raw = server.arg("fields");
        filter.active = filter.raw.length() > 0;
        if (filter.raw.length() > MAX_FIELDS * 48) {
            filter.raw = "";
            filter.active = false;
        }
    }
    return filter;
}

void sendJson(int status, const String &body) {
    server.sendHeader("Cache-Control", "no-store");
    server.send(status, "application/json", body);
}

void sendLive(const String &extra = String()) {
    sendJson(200, buildLiveJson(filterFromRequest(), extra));
}

String rejectedJson(const String &rejected) {
    if (rejected.length() == 0) {
        return String();
    }
    return String(",\"rejected\":[") + rejected + "]";
}

void addRejected(String &rejected, const String &key) {
    if (rejected.length()) {
        rejected += ',';
    }
    appendJsonString(rejected, key.c_str());
}

bool parseLevel(const String &text, long lo, long hi, long &out) {
    double v;
    if (!parseSignalNumber(text, v)) {
        return false;
    }
    out = lround(v);
    if (out < lo) {
        out = lo;
    }
    if (out > hi) {
        out = hi;
    }
    return true;
}

// Override fields: "auto" (or empty) releases the override.
bool setOverride(uint8_t flag, const String &text) {
    params::EcuOverrides &o = params::ecu_override;
    const bool release = text.length() == 0 || text.equalsIgnoreCase("auto");
    double number = 0;
    bool flagValue = false;
    if (!release) {
        if (flag == OVR_COOLANT_WARNING) {
            if (!parseSignalBool(text, flagValue)) {
                return false;
            }
        } else if (!parseSignalNumber(text, number) || fabs(number) > 1.0e7) {
            return false;
        }
    }
    switch (flag) {
        case OVR_ENGINE_SPEED:
            o.engine_speed_active = !release;
            o.engine_speed_rpm = (float)number;
            return true;
        case OVR_COOLANT_TEMP:
            o.coolant_temp_active = !release;
            o.coolant_temp_c = (float)number;
            return true;
        case OVR_COOLANT_WARNING:
            o.coolant_warning_active = !release;
            o.coolant_warning = flagValue;
            return true;
        case OVR_FUEL:
            if (number < 0) {
                return false;
            }
            o.fuel_active = !release;
            o.fuel_ul = (uint32_t)lround(number);
            return true;
        default:
            return false;
    }
}

// ---------------------------------------------------------------- API handlers

void handleLive() {
    sendLive();
}

void handleSignals() {
    String body;
    body.reserve(kSignalCount * 80);
    body += "{\"timeout_ms\":";
    body += String((unsigned long)RX_TIMEOUT_MS);
    body += ",\"signals\":[";
    for (size_t i = 0; i < kSignalCount; i++) {
        const SignalDef &def = kSignals[i];
        if (i) {
            body += ',';
        }
        body += "{\"name\":";
        appendJsonString(body, def.name);
        body += ",\"type\":\"";
        body += sigStorageName(def.storage);
        body += "\",\"source\":";
        body += def.rx == RX_NONE ? "\"vcu\"" : "\"can\"";
        body += ",\"writable\":";
        body += (def.flags & (W_ECU | OVR_ANY)) ? "\"set-ecu\"" : "false";
        body += '}';
    }
    body += "]}";
    sendJson(200, body);
}

// Outputs are applied by the next CAN task cycle; wait two cycles so the response
// already reports the new state.
void waitForOutputs() {
    vTaskDelay(pdMS_TO_TICKS(2 * CAN_TASK_PERIOD_MS + 2));
}

// /api/set: charging parameters, actuators and legacy ECU shortcuts.
void handleSet() {
    String rejected;
    bool outputs = false;
    {
        ParamsGuard guard;
        for (int i = 0; i < server.args(); i++) {
            const String key = server.argName(i);
            const String value = server.arg(i);
            long level = 0;
            if (key == "fields") {
                continue;
            } else if (key == "target_charge_pct" && parseLevel(value, 50, 100, level)) {
                params::charging.target_charge_pct = (uint8_t)level;
            } else if (key == "ac_current_limit_a" && parseLevel(value, 6, 32, level)) {
                params::charging.ac_current_limit_a = (uint8_t)level;
            } else if (key == "digital_pot_pct" && parseLevel(value, 0, 100, level)) {
                params::digipot.digipot_pct = (uint8_t)level;
                outputs = true;
            } else if (key == "coolant_fan_level" && parseLevel(value, 0, 2, level)) {
                // A manual level switches the fan to manual mode.
                params::fan.fan_level_manual = (uint8_t)level;
                params::fan.fan_mode = params::FAN_MODE_MANUAL;
                outputs = true;
            } else if (key == "coolant_fan_mode" && (value == "auto" || value == "1")) {
                params::fan.fan_mode = params::FAN_MODE_AUTO;
                outputs = true;
            } else if (key == "coolant_fan_mode" && (value == "manual" || value == "0")) {
                params::fan.fan_mode = params::FAN_MODE_MANUAL;
                outputs = true;
            } else if (key == "rpm" && setOverride(OVR_ENGINE_SPEED, value)) {
            } else if (key == "coolant" && setOverride(OVR_COOLANT_TEMP, value)) {
            } else if (key == "oil" && setOverride(OVR_FUEL, value)) {
            } else {
                addRejected(rejected, key);
            }
        }
    }
    if (outputs) {
        waitForOutputs();
    }
    sendLive(rejectedJson(rejected));
}

// /api/set-ecu: every signal of the table marked writable.
void handleSetEcu() {
    String rejected;
    bool outputs = false;
    {
        ParamsGuard guard;
        for (int i = 0; i < server.args(); i++) {
            const String key = server.argName(i);
            if (key == "fields") {
                continue;
            }
            const String value = server.arg(i);
            const SignalDef *def = findSignal(key.c_str());
            bool ok = false;
            if (def != nullptr && (def->flags & OVR_ANY)) {
                ok = setOverride(def->flags & OVR_ANY, value);
            } else if (def != nullptr && (def->flags & W_ECU)) {
                ok = writeSignal(*def, value);
            } else if (key == "digital_pot_pct" || key == "coolant_fan_level") {
                long level = 0;
                const bool pot = key == "digital_pot_pct";
                ok = parseLevel(value, 0, pot ? 100 : 2, level);
                if (ok && pot) {
                    params::digipot.digipot_pct = (uint8_t)level;
                } else if (ok) {
                    params::fan.fan_level_manual = (uint8_t)level;
                    params::fan.fan_mode = params::FAN_MODE_MANUAL;
                }
                outputs = outputs || ok;
            }
            if (!ok) {
                addRejected(rejected, key);
            }
        }
    }
    if (outputs) {
        waitForOutputs();
    }
    sendLive(rejectedJson(rejected));
}

void handleSetCluster() {
    String rejected;
    if (server.hasArg("activated")) {
        bool value;
        if (parseSignalBool(server.arg("activated"), value)) {
            ParamsGuard guard;
            params::cluster.activated = value;
        } else {
            addRejected(rejected, "activated");
        }
    }
    sendLive(rejectedJson(rejected));
}

// ---------------------------------------------------------------- static files (PWA)

const char *contentTypeFor(const String &path) {
    if (path.endsWith(".html")) return "text/html; charset=utf-8";
    if (path.endsWith(".css")) return "text/css";
    if (path.endsWith(".js")) return "application/javascript";
    if (path.endsWith(".json")) return "application/json";
    if (path.endsWith(".svg")) return "image/svg+xml";
    if (path.endsWith(".png")) return "image/png";
    if (path.endsWith(".webp")) return "image/webp";
    if (path.endsWith(".jpg")) return "image/jpeg";
    if (path.endsWith(".ico")) return "image/x-icon";
    return "application/octet-stream";
}

// Serves pwa/dist from SPIFFS: x.js -> x.js.gz with Content-Encoding gzip.
bool serveStaticFile(String path) {
    if (!gFsOk || path.indexOf("..") >= 0 || path.length() > 40) {
        return false;
    }
    if (path.endsWith("/")) {
        path += "index.html";
    }
    String stored = path;
    if (!SPIFFS.exists(stored)) {
        stored = path + ".gz";
        if (!SPIFFS.exists(stored)) {
            return false;
        }
    }
    File file = SPIFFS.open(stored, FILE_READ);
    if (!file) {
        return false;
    }
    // Assets are referenced with a content hash (?v=...) by pwa/tools/build.py: cache them
    // for a year. index.html, sw.js and manifest.json must revalidate after an update.
    const bool shell = path.endsWith(".html") || path.endsWith("sw.js") || path.endsWith("manifest.json");
    const char *cache = shell ? "no-cache" : server.hasArg("v") ? "public, max-age=31536000, immutable" : "max-age=300";
    server.sendHeader("Cache-Control", cache);
    server.streamFile(file, contentTypeFor(path));
    file.close();
    return true;
}

void handleNotFound() {
    if (server.method() == HTTP_GET && serveStaticFile(server.uri())) {
        return;
    }
    server.send(404, "text/plain", "Not found");
}

// ---------------------------------------------------------------- task

void webTask(void *) {
    uint32_t lastLed = 0;
    uint32_t lastDiag = 0;
    bool ledOn = false;
    bool staWasConnected = false;
    for (;;) {
        server.handleClient();
        gvretBridgeService();
        const uint32_t now = millis();
        const bool staConnected = WiFi.status() == WL_CONNECTED;
        if (staConnected != staWasConnected) {  // log home network connect / loss once
            staWasConnected = staConnected;
            webinterfaceLogWifiStatus();
        }
        if (now - lastDiag >= 1000U) {  // one diagnostic line per second on COM10
            lastDiag = now;
            canManagerLogStatus();
        }
        if (now - lastLed >= 500U) {  // VCU-REQ-019: 1 Hz green blink at 10 %
            lastLed = now;
            ledOn = !ledOn;
            FastLED.setBrightness((uint8_t)((255U * 10U) / 100U));
            if (ledOn) {
                fastled(LED_GREEN);
            } else {
                fastledOff();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

// Last home network disconnect reason (wifi_err_reason_t, e.g. 201 no AP found,
// 15 handshake timeout / wrong password); set by the WiFi event task.
volatile uint8_t gStaReason = 0;

void startWifi() {
    WiFi.persistent(false);
    WiFi.onEvent(
        [](WiFiEvent_t, WiFiEventInfo_t info) {
            const uint8_t reason = info.wifi_sta_disconnected.reason;
            if (reason != gStaReason) {  // log changes only, retries repeat the same reason
                gStaReason = reason;
                VCU_LOG("[WIFI] home disconnected, reason %u (%s)\n", reason,
                        WiFi.disconnectReasonName((wifi_err_reason_t)reason));
            }
        },
        ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.mode(WIFI_AP_STA);
    WiFi.setHostname("eboxster-vcu");
    const bool apOk = WiFi.softAP(VCU_WIFI_AP_SSID, VCU_WIFI_AP_PASSWORD);
    VCU_LOG("[WEB] AP %s %s ip=%s\n", VCU_WIFI_AP_SSID, apOk ? "up" : "FAILED", ipToString(WiFi.softAPIP()).c_str());
    if (strlen(VCU_WIFI_HOME_SSID) > 0) {
        WiFi.setAutoReconnect(true);
        WiFi.begin(VCU_WIFI_HOME_SSID, VCU_WIFI_HOME_PASSWORD);  // non-blocking
    }
}

}  // namespace

void webinterfaceLogWifiStatus() {
    const bool staConnected = WiFi.status() == WL_CONNECTED;
    VCU_LOG("[WIFI] ap=%s ip=%s clients=%u | home=%s %s ip=%s rssi=%d last_reason=%u host=eboxster-vcu\n",
            VCU_WIFI_AP_SSID, ipToString(WiFi.softAPIP()).c_str(), (unsigned)WiFi.softAPgetStationNum(),
            VCU_WIFI_HOME_SSID, staConnected ? "connected" : "not connected",
            staConnected ? ipToString(WiFi.localIP()).c_str() : "-", staConnected ? (int)WiFi.RSSI() : 0,
            (unsigned)gStaReason);
}

void webinterfaceBegin() {
    startWifi();
    gFsOk = SPIFFS.begin(false);
    VCU_LOG("[WEB] SPIFFS %s\n", gFsOk ? "ok" : "FAILED (run uploadfs)");

    server.on("/api/live", HTTP_GET, handleLive);
    server.on("/api/signals", HTTP_GET, handleSignals);
    server.on("/api/set", HTTP_GET, handleSet);
    server.on("/api/set-ecu", HTTP_GET, handleSetEcu);
    server.on("/api/set-cluster", HTTP_GET, handleSetCluster);
    server.onNotFound(handleNotFound);
    server.begin();

    xTaskCreatePinnedToCore(webTask, "vcu_web", 12288, nullptr, 2, nullptr, 0);
}
