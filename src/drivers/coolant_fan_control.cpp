// Driver layer: coolant fan relay outputs. Pins come from board.h.
// Without a confirmed stage-1 pin both outputs stay disabled (board::FAN_OUTPUTS_ENABLED).
#include "coolant_fan_control.h"

#include "board.h"

namespace {
bool gStage1 = false;
bool gStage2 = false;
}  // namespace

bool coolantFanOutputsEnabled() {
    return board::FAN_OUTPUTS_ENABLED;
}

void coolantFanBegin() {
    if (!board::FAN_OUTPUTS_ENABLED) {
        VCU_LOG("[FAN] outputs disabled: stage 1 pin not assigned in board.h\n");
        return;
    }
    pinMode(board::GPIO_FAN_STAGE1, OUTPUT);
    pinMode(board::GPIO_FAN_STAGE2, OUTPUT);
    coolantFanWrite(false, false);
}

void coolantFanWrite(bool stage1, bool stage2) {
    if (!board::FAN_OUTPUTS_ENABLED) {
        gStage1 = false;
        gStage2 = false;
        return;
    }
    gStage1 = stage1;
    gStage2 = stage2;
    digitalWrite(board::GPIO_FAN_STAGE1, stage1 ? HIGH : LOW);
    digitalWrite(board::GPIO_FAN_STAGE2, stage2 ? HIGH : LOW);
}

bool coolantFanStage1() {
    return gStage1;
}

bool coolantFanStage2() {
    return gStage2;
}
