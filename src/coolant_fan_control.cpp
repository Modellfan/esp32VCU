#include "coolant_fan_control.h"

#ifndef COOLANT_FAN_LOW_PIN
#define COOLANT_FAN_LOW_PIN 7
#endif

#ifndef COOLANT_FAN_HIGH_PIN
#define COOLANT_FAN_HIGH_PIN 15
#endif

namespace {
uint8_t fanLevel = 0;
bool lowOutput = false;
bool highOutput = false;

void applyFanOutputs() {
    lowOutput = fanLevel >= 1U;
    highOutput = fanLevel >= 2U;
    digitalWrite(COOLANT_FAN_LOW_PIN, lowOutput ? HIGH : LOW);
    digitalWrite(COOLANT_FAN_HIGH_PIN, highOutput ? HIGH : LOW);
}
}

void coolantFanBegin() {
    pinMode(COOLANT_FAN_LOW_PIN, OUTPUT);
    pinMode(COOLANT_FAN_HIGH_PIN, OUTPUT);
    applyFanOutputs();
}

void coolantFanSetLevel(uint8_t level) {
    fanLevel = level > 2U ? 2U : level;
    applyFanOutputs();
}

uint8_t coolantFanLevel() {
    return fanLevel;
}

bool coolantFanLowOutput() {
    return lowOutput;
}

bool coolantFanHighOutput() {
    return highOutput;
}
