#include "digipot_control.h"

#include <X9C103S.h>

#ifndef X9C103S_CS_PIN
#define X9C103S_CS_PIN 4
#endif

#ifndef X9C103S_INC_PIN
#define X9C103S_INC_PIN 5
#endif

#ifndef X9C103S_UD_PIN
#define X9C103S_UD_PIN 6
#endif

namespace {
X9C103S digipot(X9C103S_CS_PIN, X9C103S_INC_PIN, X9C103S_UD_PIN);
}

void digipotBegin() {
    digipot.begin();
}

void digipotSetPercent(uint8_t percent) {
    digipot.setPercent(percent);
}

uint8_t digipotPercent() {
    return digipot.percent();
}

uint8_t digipotWiper() {
    return digipot.wiper();
}
