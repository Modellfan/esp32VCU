// Application layer (high level): runs inside the real-time step with the params lock held.
#ifndef VCU_APP_H
#define VCU_APP_H

#include <Arduino.h>

// Cluster calculation, ECU overrides and automatic fan level. Call every 6 ms.
void appStep(uint32_t nowMs);

// Pure function, exposed for tests: automatic fan level from temperatures with hysteresis.
// fresh == false (no inverter data) returns the fail-safe level 2.
uint8_t fanAutoLevel(float motorTempC, float heatsinkTempC, bool fresh, uint8_t previousLevel);

#endif
