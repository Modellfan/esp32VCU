// Real-time task (core 1): CAN receive, application step, ECU emulation and outputs
// in a fixed 6 ms cycle. Independent of WiFi and the web server (VCU-REQ-002).
#ifndef VCU_RT_H
#define VCU_RT_H

#include <Arduino.h>

constexpr uint32_t RT_PERIOD_MS = 6;  // 5 ECU frames x 6 ms = 30 ms per frame

struct RtStats {
    uint32_t cycles;
    uint32_t overruns;       // cycles that took longer than RT_PERIOD_MS
    uint32_t max_cycle_us;
    uint32_t tx_failed;      // tryToSend() returned false
    uint32_t started_ms;     // millis() of the first cycle
};

void rtBegin();
RtStats rtStats();

#endif
