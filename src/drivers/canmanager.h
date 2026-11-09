#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <SPI.h>
#include <ACAN2515.h>

extern ACAN2515 can;   // CAN 1: vehicle (Porsche 986)
extern ACAN2515 can2;  // CAN 2: drive (Tesla SDU / OpenInverter)

void canManager_setup();
// 0 = initialised; anything else is the ACAN2515 error code (0xFFFF = not started).
uint16_t canManager_error(uint8_t bus);

// CAN task cycle (src/main.cpp): 5 ECU frames x 6 ms = 30 ms per frame (VCU-REQ-001).
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
