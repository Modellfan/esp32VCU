// Driver layer: all CAN controllers of the VCU. Pins from board.h.
//   can0  TWAI controller of the ESP32-S3 (BMS / charger), library ACAN_ESP32
//   can1  MCP2515 #1: vehicle (Porsche 986)
//   can2  MCP2515 #2: drive (Tesla SDU / OpenInverter)
#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <SPI.h>
#include <ACAN2515.h>

extern ACAN2515 can1;  // CAN1: vehicle (Porsche 986)
extern ACAN2515 can2;  // CAN2: drive (Tesla SDU / OpenInverter)

// Starts can0, can1 and can2.
void canManager_setup();
// 0 = initialised; anything else is the ACAN2515 error code (0xFFFF = not started).
uint16_t canManager_error(uint8_t bus);

// CAN0 (TWAI). Non-blocking receive: returns true and fills frame when one was waiting.
bool can0Active();
bool can0Receive(CANMessage &frame);

// Bus diagnostics per controller (bus 0 = CAN0/TWAI, 1 = CAN1, 2 = CAN2).
// eflg: MCP2515 EFLG register (bit 5 TXBO = bus-off); 0 for CAN0.
struct CanBusDiag {
    uint32_t rx_frames;  // frames received since start
    uint8_t tec;         // transmit error counter
    uint8_t rec;         // receive error counter
    uint8_t eflg;
    bool bus_off;
};

// Receive side (loop() in main.cpp): count one received frame / read the error registers (SPI).
void canCountRx(uint8_t bus);
void canRefreshErrors();
// Any task: last known values.
CanBusDiag canBusDiag(uint8_t bus);
// Web task: one diagnostic text line per call (uptime, buses, CAN task timing).
void canManagerLogStatus();
// Serial command "can0-status": full TWAI status and the TX/RX pin levels.
void canManagerLogCan0Status();
// Serial command "can0-send": one test frame 0x6C0 on CAN0 (bench only), then the status.
void canManagerCan0SendTest();
// Serial command "can0-pintest": TWAI stopped, RX sampled floating/pull-up/pull-down,
// TX driven low as plain GPIO for 20 ms (echo on RX, errors on CAN1/CAN2), TWAI restarted.
// Disturbs the bench bus for 20 ms; never on the vehicle.
void canManagerCan0PinTest();

// ECU task cycle (src/main.cpp): 5 ECU frames x 6 ms = 30 ms per frame (VCU-REQ-001).
constexpr uint32_t CAN_TASK_PERIOD_MS = 6;

struct CanTaskStats {
    uint32_t cycles;
    uint32_t overruns;       // cycles that took longer than CAN_TASK_PERIOD_MS
    uint32_t max_cycle_us;
    uint32_t tx_failed;      // tryToSend() returned false
    uint32_t started_ms;     // millis() of the first cycle
};

void canTaskStatsRecord(uint32_t nowMs, uint32_t cycleUs, bool sent);
CanTaskStats canTaskStats();

#endif // CANMANAGER_H
