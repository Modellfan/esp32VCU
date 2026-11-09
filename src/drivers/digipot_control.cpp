// Driver layer: X9C103S digital potentiometer (fuel gauge sender emulation).
#include "digipot_control.h"

#include <X9C103S.h>

#include "board.h"

namespace {
X9C103S digipot(board::GPIO_DIGIPOT_CS, board::GPIO_DIGIPOT_INC, board::GPIO_DIGIPOT_UD);
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
