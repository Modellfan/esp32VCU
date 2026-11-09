#include "vcu_app.h"

#include "986_ecu_messages.h"
#include "cluster_calc.h"
#include "tesla_sdu_messages.h"
#include "vcu_params.h"

namespace {

// Automatic fan thresholds. DRAFT values, not confirmed by the user (VCU-REQ-008 open):
// the default fan mode is manual until they are confirmed.
constexpr float FAN1_ON_MOTOR_C = 70.0f;
constexpr float FAN1_ON_HEATSINK_C = 50.0f;
constexpr float FAN2_ON_MOTOR_C = 110.0f;
constexpr float FAN2_ON_HEATSINK_C = 65.0f;
constexpr float FAN_HYSTERESIS_K = 5.0f;

void applyEcuOverrides() {
    const params::EcuOverrides &o = params::ecu_override;
    if (o.engine_speed_active) {
        params::ecu.engine_speed_rpm = o.engine_speed_rpm;
    }
    if (o.coolant_temp_active) {
        params::ecu.motor2_coolant_temperature = o.coolant_temp_c;
    }
    if (o.coolant_warning_active) {
        params::ecu.motor2_coolant_temp_not_ok = o.coolant_warning;
    }
    if (o.fuel_active) {
        params::ecu.mo5_verbrauch_ul = o.fuel_ul;
    }
}

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

void appStep(uint32_t nowMs) {
    updateCluster();
    applyEcuOverrides();

    params::OutputSignals &out = params::outputs;
    const bool fresh = rxFresh(RX_SDU_504, nowMs);  // 0x504 carries tmpm and tmphs
    out.fan_level_auto = fanAutoLevel((float)params::tesla_sdu.tmpm, (float)params::tesla_sdu.tmphs, fresh,
                                      out.fan_level_auto);
    out.fan_level_manual = clampLevel(out.fan_level_manual);
    out.fan_level = out.fan_mode == params::FAN_MODE_AUTO ? out.fan_level_auto : out.fan_level_manual;
    if (out.digipot_pct > 100U) {
        out.digipot_pct = 100U;
    }
}
