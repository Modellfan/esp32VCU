/**************************************************************************
 *  main.cpp — Dual-file CAN logger for ESP32-S3 (SavvyCAN ASCII)
 *  - Uses your existing canmanager.* to init MCP2515 (no CAN SPI here)
 *  - One SD card on HSPI (separate SPI bus)
 *  - Writes one file per bus (bus1 + bus2)
 *  - Format: "<sec>.<usec6> <bus>R11|R29 <8hexID> <b0> ... <b7>"
 **************************************************************************/

#include <Arduino.h>
#include "USB.h"
#include "USBCDC.h"
#include <TaskScheduler.h>

#define GVRET_PORT   Serial
#define MONITOR_PORT USBSerial1
USBCDC USBSerial1(0);

#include "gvret.h"
#include "canmanager.h"   // must provide: ACAN2515 can, can2, CANMessage, canManager_setup()

#include <SPI.h>
#include <SD.h>
#include <string.h>

/**************************************************************************
 *  SD card on separate SPI host (HSPI) — your tested pins
 **************************************************************************/
#define SD_MOSI 35
#define SD_MISO 37
#define SD_SCK  36
#define SD_CS   39

SPIClass SPI_SD(HSPI);   // dedicated SPI host for SD

/**************************************************************************
 *  Built-in LED (ESP32-S3)
 **************************************************************************/
#define RGB_BUILTIN 48

/**************************************************************************
 *  Logging parameters
 **************************************************************************/
static constexpr size_t   LOG_BLOCK  = 16 * 1024;       // 16 KB buffered writes per file
static constexpr uint32_t LOG_FLUSH_INTERVAL_MS = 200;  // flush rhythm
static constexpr size_t   RING_CAP  = 4096;             // frames per bus ring
static constexpr size_t   LOG_LINE_MAX  = 64;           // worst-case line length

/**************************************************************************
 *  Scheduler (blink)
 **************************************************************************/
Scheduler runner;
void blinkLED();
Task taskBlinkLED(500, TASK_FOREVER, &blinkLED, &runner, true);

/**************************************************************************
 *  Types & ring buffers
 **************************************************************************/
struct CanFrameLite {
  uint32_t sec;
  uint32_t usec;
  uint8_t  bus;        // 0 = can, 1 = can2
  uint32_t id;         // raw ID
  uint8_t  dlc;        // 0..8
  bool     extended;   // EFF?
  bool     rtr;        // RTR?
  uint8_t  data[8];
};

template<size_t CAP>
class Ring {
public:
  bool push(const CanFrameLite& f) {
    size_t n = (head_ + 1) & mask_;
    if (n == tail_) return false;
    buf_[head_] = f;
    head_ = n;
    return true;
  }
  bool pop(CanFrameLite& f) {
    if (tail_ == head_) return false;
    f = buf_[tail_];
    tail_ = (tail_ + 1) & mask_;
    return true;
  }
  size_t size() const { return (head_ - tail_) & mask_; }
private:
  static_assert((CAP & (CAP-1)) == 0, "CAP must be power of two");
  static constexpr size_t mask_ = CAP - 1;
  volatile size_t head_ = 0, tail_ = 0;
  CanFrameLite buf_[CAP];
};

Ring<RING_CAP> ringBus0;
Ring<RING_CAP> ringBus1;
volatile uint32_t drops[2] = {0,0};

/**************************************************************************
 *  Time helper
 **************************************************************************/
static inline void now_sec_usec(uint32_t& sec, uint32_t& usec) {
  uint64_t us = esp_timer_get_time(); // since boot (µs)
  sec  = us / 1000000ULL;
  usec = us % 1000000ULL;
}

/**************************************************************************
 *  SavvyCAN ASCII formatter
 *  "<sec>.<usec6> <bus>R11|R29 <8hexID> <b0> ... <b7>\n"
 *  - uppercase hex, ID zero-padded to 8 hex
 *  - always 8 bytes (pad with 00 if DLC < 8)
 **************************************************************************/
size_t frameToSavvyASCII(const CanFrameLite& f, char* out, size_t outCap) {
  char* p = out;
  auto left = [&](){ return outCap - (size_t)(p - out); };

  // timestamp
  p += snprintf(p, left(), "%lu.%06lu ", (unsigned long)f.sec, (unsigned long)f.usec);

  // bus + R11/R29
  p += snprintf(p, left(), "%uR%s ", (unsigned)(f.bus + 1), f.extended ? "29" : "11");

  // ID: 8 hex, zero-padded (mask width)
  uint32_t idMasked = f.extended ? (f.id & 0x1FFFFFFF) : (f.id & 0x7FFu);
  p += snprintf(p, left(), "%08X ", (unsigned)idMasked);

  // 8 data bytes (pad with 00)
  for (uint8_t i = 0; i < 8; ++i) {
    uint8_t b = (i < f.dlc) ? f.data[i] : 0u;
    p += snprintf(p, left(), (i == 7) ? "%02X" : "%02X ", b);
  }

  if (left() > 0) *p++ = '\n';
  return (size_t)(p - out);
}

/**************************************************************************
 *  SD: two files (one per bus) with independent buffers
 **************************************************************************/
File logFile1; // bus 1
File logFile2; // bus 2

static uint8_t  writeBuf1[LOG_BLOCK];
static size_t   writeFill1 = 0;

static uint8_t  writeBuf2[LOG_BLOCK];
static size_t   writeFill2 = 0;

static uint32_t lastFlushMs = 0;

inline void sd_append_bus(File& lf, uint8_t* buf, size_t& fill, const char* data, size_t n) {
  if (n > (LOG_BLOCK - fill)) {
    if (fill) { lf.write(buf, fill); fill = 0; }
  }
  memcpy(buf + fill, data, n);
  fill += n;
}

inline void sd_flush_if_needed() {
  uint32_t now = millis();
  if (now - lastFlushMs >= LOG_FLUSH_INTERVAL_MS) {
    if (writeFill1) { logFile1.write(writeBuf1, writeFill1); writeFill1 = 0; }
    if (writeFill2) { logFile2.write(writeBuf2, writeFill2); writeFill2 = 0; }
    logFile1.flush();
    logFile2.flush();
    lastFlushMs = now;
  }
}

inline void sd_flush_now() {
  if (writeFill1) { logFile1.write(writeBuf1, writeFill1); writeFill1 = 0; }
  if (writeFill2) { logFile2.write(writeBuf2, writeFill2); writeFill2 = 0; }
  logFile1.flush();
  logFile2.flush();
}

/**************************************************************************
 *  CAN passthrough + ring fill (no CAN SPI defined here)
 **************************************************************************/
void passthroughCAN() {
  CANMessage msg;

  // CAN bus 0
  if (can.available()) {
    can.receive(msg);
    can2.tryToSend(msg);       // passthrough
    sendFrameToUSB(msg, 0);    // GVRET

    CanFrameLite f{};
    now_sec_usec(f.sec, f.usec);
    f.bus = 0;
    f.id = msg.id;
    f.dlc = msg.len;
    f.extended = msg.ext;
    f.rtr = msg.rtr;
    memcpy(f.data, msg.data, msg.len);
    if (!ringBus0.push(f)) drops[0]++;
  }

  // CAN bus 1
  if (can2.available()) {
    can2.receive(msg);
    can.tryToSend(msg);        // passthrough
    sendFrameToUSB(msg, 1);    // GVRET

    CanFrameLite f{};
    now_sec_usec(f.sec, f.usec);
    f.bus = 1;
    f.id = msg.id;
    f.dlc = msg.len;
    f.extended = msg.ext;
    f.rtr = msg.rtr;
    memcpy(f.data, msg.data, msg.len);
    if (!ringBus1.push(f)) drops[1]++;
  }
}

/**************************************************************************
 *  Logger pump: pop from rings, write to the corresponding file
 **************************************************************************/
void pumpLoggerOnce() {
  CanFrameLite f;
  bool any = false;

  if (ringBus0.pop(f)) {
    any = true;
    char line[LOG_LINE_MAX];
    size_t n = frameToSavvyASCII(f, line, sizeof(line));
    sd_append_bus(logFile1, writeBuf1, writeFill1, line, n);
  }
  if (ringBus1.pop(f)) {
    any = true;
    char line[LOG_LINE_MAX];
    size_t n = frameToSavvyASCII(f, line, sizeof(line));
    sd_append_bus(logFile2, writeBuf2, writeFill2, line, n);
  }

  // periodic flush for both files
  sd_flush_if_needed();

  if (!any) {
    // tiny breather so we don't spin when idle
    delayMicroseconds(200);
  }
}

/**************************************************************************
 *  SD init (HSPI) + open two log files (one per bus)
 **************************************************************************/
bool sd_init_and_open() {
  // Bind SD to its own SPI host & pins (HSPI)
  SPI_SD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  // IMPORTANT: pass SPI_SD (not default SPI)
  if (!SD.begin(SD_CS, SPI_SD)) {
    MONITOR_PORT.println("⚠️ SD Card initialization failed!");
    return false;
  }

  char fname1[48], fname2[48];
  unsigned long ms = millis();
  snprintf(fname1, sizeof(fname1), "/log_%lu_bus1.sav", ms);
  snprintf(fname2, sizeof(fname2), "/log_%lu_bus2.sav", ms);

  logFile1 = SD.open(fname1, FILE_WRITE);
  logFile2 = SD.open(fname2, FILE_WRITE);

  if (!logFile1) { MONITOR_PORT.println("⚠️ Failed to open logfile for bus 1!"); return false; }
  if (!logFile2) { MONITOR_PORT.println("⚠️ Failed to open logfile for bus 2!"); return false; }

  // Optional headers (SavvyCAN ignores unknown lines)
  logFile1.print(F("# SavvyCAN ASCII log — bus 1 (R11/R29)\n"));
  logFile2.print(F("# SavvyCAN ASCII log — bus 2 (R11/R29)\n"));
  return true;
}

/**************************************************************************
 *  LED Task
 **************************************************************************/
void blinkLED() {
  digitalWrite(RGB_BUILTIN, !digitalRead(RGB_BUILTIN));
}

/**************************************************************************
 *  Setup / Loop
 **************************************************************************/
void setup() {
  GVRET_PORT.begin(115200);
  USBSerial1.begin();
  USB.begin();

  pinMode(RGB_BUILTIN, OUTPUT);
  digitalWrite(RGB_BUILTIN, LOW);

  // Initialize CAN buses (SPI pinning for MCP2515 is done inside canmanager)
  canManager_setup();

  // Initialize SD and open two files (one per bus)
  if (!sd_init_and_open()) {
    MONITOR_PORT.println("SD not ready. Logging disabled.");
  } else {
    MONITOR_PORT.println("SD ready, logging started for bus1 + bus2.");
  }

  MONITOR_PORT.println("System Initialized. Starting tasks...");
}

void loop() {
  // Blink task
  runner.execute();

  // CAN passthrough + rings
  passthroughCAN();

  // Move from rings → SD (two files)
  pumpLoggerOnce();

  // GVRET protocol handler
  gvret_loop();

  // Status every 2 seconds
  static uint32_t t0 = 0;
  if (millis() - t0 > 2000) {
    t0 = millis();
    MONITOR_PORT.printf("r0:%u r1:%u drops:%u/%u\n",
      (unsigned)ringBus0.size(), (unsigned)ringBus1.size(),
      (unsigned)drops[0], (unsigned)drops[1]);
  }
}

/**************************************************************************
 *  Optional: manual flush/close
 **************************************************************************/
void stopLoggingAndFlush() {
  sd_flush_now();
  if (logFile1) { logFile1.close(); }
  if (logFile2) { logFile2.close(); }
}
