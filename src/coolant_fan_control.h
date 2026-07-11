#ifndef COOLANT_FAN_CONTROL_H
#define COOLANT_FAN_CONTROL_H

#include <Arduino.h>

void coolantFanBegin();
void coolantFanSetLevel(uint8_t level);
uint8_t coolantFanLevel();
bool coolantFanLowOutput();
bool coolantFanHighOutput();

#endif
