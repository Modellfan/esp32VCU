#include "coolant_fan_control.h"

#include "board.h"
#include "can_rx_monitor.h"
#include "tesla_sdu_messages.h"

namespace params {
FanSignals fan;
}  // namespace params

namespace {

// Automatic fan thresholds. DRAFT values, not confirmed by the user (VCU-REQ-008 open):
// the default fan mode is manual until they are confirmed.
constexpr float FAN1_ON_MOTOR_C = 70.0f;
constexpr float FAN1_ON_HEATSINK_C = 50.0f;
constexpr float FAN2_ON_MOTOR_C = 110.0f;
constexpr float FAN2_ON_HEATSINK_C = 65.0f;
constexpr float FAN_HYSTERESIS_K = 5.0f;

bool gStage1 = false;
bool gStage2 = false;

uint8_t clampLevel(uint8_t level) {
    return level > 2U ? 2U : level;
}

}  // namespace

uint8_t fanAutoLevel(float motorTempC, float heatsinkTempC, bool fresh, uint8_t previousLevel) {
    if (!fresh) {
        return 2U;
    }
    const float h = FAN_HYSTERESIS_K;
    const bool stage2On = motorTempC >= FAN2_ON_MOTOR_C || heatsinkTempC >= FAN2_ON_HEATSINK_C;
    const bool stage2Hold = motorTempC > FAN2_ON_MOTOR_C - h || heatsinkTempC > FAN2_ON_HEATSINK_C - h;
    const bool stage1On = motorTempC >= FAN1_ON_MOTOR_C || heatsinkTempC >= FAN1_ON_HEATSINK_C;
    const bool stage1Hold = motorTempC > FAN1_ON_MOTOR_C - h || heatsinkTempC > FAN1_ON_HEATSINK_C - h;

    if (stage2On || (previousLevel >= 2U && stage2Hold)) {
        return 2U;
    }
    if (stage1On || (previousLevel >= 1U && stage1Hold)) {
        return 1U;
    }
    return 0U;
}

void coolantFanAutoUpdate(uint32_t nowMs) {
    params::FanSignals &f = params::fan;
    const bool fresh = rxFresh(RX_SDU_504, nowMs);  // 0x504 carries tmpm and tmphs
    f.fan_level_auto = fanAutoLevel((float)params::tesla_sdu.tmpm, (float)params::tesla_sdu.tmphs, fresh,
                                    f.fan_level_auto);
    f.fan_level_manual = clampLevel(f.fan_level_manual);
    f.fan_level = f.fan_mode == params::FAN_MODE_AUTO ? f.fan_level_auto : f.fan_level_manual;
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

void coolantFanReportState() {
    params::fan.fan_stage1 = gStage1;
    params::fan.fan_stage2 = gStage2;
    params::fan.fan_outputs_enabled = board::FAN_OUTPUTS_ENABLED;
}
