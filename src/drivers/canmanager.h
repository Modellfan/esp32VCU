// Driver layer: all CAN controllers of the VCU. Pins from board.h. Public interface without
// hardware (receive monitor, params::can): canmanager_api.h.
//   can0  TWAI controller of the ESP32-S3 (BMS / charger), library ACAN_ESP32, no termination
//   can1  MCP2515 #1: vehicle (Porsche 986)
//   can2  MCP2515 #2: drive (Tesla SDU / OpenInverter)
//   can3  MCP2515 #3: spare (receive and GVRET only, role open)
// CAN1..CAN3 share one SPI bus; their 120 ohm termination is selected by jumper.
#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <SPI.h>
#include <ACAN2515.h>

#include "canmanager_api.h"

extern ACAN2515 can1;  // CAN1: vehicle (Porsche 986)
extern ACAN2515 can2;  // CAN2: drive (Tesla SDU / OpenInverter)
extern ACAN2515 can3;  // CAN3: spare

// Starts can0..can3 (setup(), before any other task, so reception starts at once).
void canManager_setup();
// Init result per bus: 0 = ok (see params::CanBusStatus::error).
uint32_t canManager_error(uint8_t bus);

// CAN0 (TWAI). Non-blocking receive: returns true and fills frame when one was waiting.
bool can0Active();
bool can0Receive(CANMessage &frame);
// CAN0 send (loop() only): false when CAN0 is down or the driver buffer is full.
bool can0Send(const CANMessage &frame);

// Bus diagnostics per controller (bus 0..3).
struct CanBusDiag {
    uint32_t rx_frames;  // frames received since start
    uint8_t tec;         // transmit error counter
    uint8_t rec;         // receive error counter
    uint8_t eflg;        // MCP2515 EFLG register (bit 5 TXBO = bus-off); CAN0: TWAI status
    bool bus_off;
};

// Receive side (loop() in main.cpp): count one received frame.
void canCountRx(uint8_t bus);
// loop(), every params::can.error_refresh_ms, without the params lock: reads the error
// registers (SPI), then copies the status into params::can under the lock.
void canRefreshErrors();
// Any task: last known values.
CanBusDiag canBusDiag(uint8_t bus);
// Web task: one diagnostic text line per call (uptime, buses, CAN task timing).
void canManagerLogStatus();
// Serial command "can0-status": full TWAI status and the RX pin level.
void canManagerLogCan0Status();
// Serial command "can0-send": one test frame 0x6C0 on CAN0 (bench only), then the status.
void canManagerCan0SendTest();
// Serial command "can0-pintest": TWAI stopped, RX sampled floating/pull-up/pull-down,
// TX driven low as plain GPIO for 20 ms (echo on RX, errors on CAN1/CAN2), TWAI restarted.
// Disturbs the bench bus for 20 ms; never on the vehicle.
void canManagerCan0PinTest();

// ECU task cycle (src/main.cpp): 5 ECU frames x 6 ms = 30 ms per frame (VCU-REQ-001).
// Default of params::can.ecu_period_ms.
constexpr uint32_t CAN_TASK_PERIOD_MS = 6;

struct CanTaskStats {
    uint32_t cycles;
    uint32_t overruns;       // cycles that took longer than the period
    uint32_t max_cycle_us;
    uint32_t tx_failed;      // tryToSend() returned false
    uint32_t started_ms;     // millis() of the first cycle
};

void canTaskStatsRecord(uint32_t nowMs, uint32_t cycleUs, bool sent);
CanTaskStats canTaskStats();

#endif // CANMANAGER_H
