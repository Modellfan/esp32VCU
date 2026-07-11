#ifndef DIGIPOT_CONTROL_H
#define DIGIPOT_CONTROL_H

#include <Arduino.h>

void digipotBegin();
void digipotSetPercent(uint8_t percent);
uint8_t digipotPercent();
uint8_t digipotWiper();

#endif
