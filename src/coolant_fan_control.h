#ifndef COOLANT_FAN_CONTROL_H
#define COOLANT_FAN_CONTROL_H

#include <Arduino.h>

// Driver layer: relay outputs only. The fan level is chosen in the application layer
// (fan_logic) and stored in params::outputs.
void coolantFanBegin();
bool coolantFanOutputsEnabled();
void coolantFanWrite(bool stage1, bool stage2);
bool coolantFanStage1();
bool coolantFanStage2();

#endif
