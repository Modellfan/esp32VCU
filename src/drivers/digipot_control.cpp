#include "digipot_control.h"

#include <X9C103S.h>

#include "board.h"

namespace params {
DigipotSignals digipot;
}  // namespace params

namespace {
X9C103S digipot(board::GPIO_DIGIPOT_CS, board::GPIO_DIGIPOT_INC, board::GPIO_DIGIPOT_UD);
uint8_t gApplied = 255;  // forces the first write
}  // namespace

void digipotBegin() {
    digipot.begin();
}

void digipotApply(uint8_t percent) {
    if (percent != gApplied) {
        digipot.setPercent(percent);
        gApplied = percent;
    }
}

void digipotReportState() {
    params::digipot.digipot_applied_pct = digipot.percent();
    params::digipot.digipot_wiper = digipot.wiper();
}
