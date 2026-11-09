// Coolant fan: data, automatic level and relay outputs (VCU-REQ-008, VCU-DEC-004).
#ifndef COOLANT_FAN_CONTROL_H
#define COOLANT_FAN_CONTROL_H

#include <Arduino.h>

namespace params {

enum FanMode : uint8_t { FAN_MODE_MANUAL = 0, FAN_MODE_AUTO = 1 };

// Writers: web service (manual level, mode) and coolantFanAutoUpdate() (automatic level).
// Only the CAN task drives the relays.
struct FanSignals {
    uint8_t fan_mode = FAN_MODE_MANUAL;
    uint8_t fan_level_manual = 0;
    uint8_t fan_level_auto = 0;
    uint8_t fan_level = 0;          // applied level
    bool fan_stage1 = false;        // applied relay states
    bool fan_stage2 = false;
    bool fan_outputs_enabled = false;
};

extern FanSignals fan;

}  // namespace params

// Automatic level from motor and heatsink temperature with hysteresis.
// fresh == false (no inverter data) returns the fail-safe level 2.
uint8_t fanAutoLevel(float motorTempC, float heatsinkTempC, bool fresh, uint8_t previousLevel);

// Every 100 ms (updateOutputs in main.cpp), params lock held: automatic and applied level.
void coolantFanAutoUpdate(uint32_t nowMs);

// Relay outputs. Without a confirmed stage-1 pin both outputs stay disabled (board.h).
void coolantFanBegin();
void coolantFanWrite(bool stage1, bool stage2);
// Copies the applied relay states into params::fan (params lock held).
void coolantFanReportState();

#endif
