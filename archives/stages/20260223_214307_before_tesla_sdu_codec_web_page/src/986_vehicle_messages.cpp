#include "986_vehicle_messages.h"
#include "986_can_codec.h"
#include "can_utils.h"

namespace params {
VehicleSignals vehicle;
}

bool decode0x2A8(const CANMessage &frame) {
    if (frame.id != 0x2A8 || frame.len < 8) {
        return false;
    }

    const uint16_t rawLegacyFL = (uint16_t)can986::readBitsLE(frame.data, 1, 15);
    const uint16_t rawLegacyFR = (uint16_t)can986::readBitsLE(frame.data, 17, 15);
    const uint16_t rawLegacyRL = (uint16_t)can986::readBitsLE(frame.data, 33, 15);
    const uint16_t rawLegacyRR = (uint16_t)can986::readBitsLE(frame.data, 49, 15);
    params::vehicle.wheel_speed_fl_legacy = rawLegacyFL * 0.01f;
    params::vehicle.wheel_speed_fr_legacy = rawLegacyFR * 0.01f;
    params::vehicle.wheel_speed_rl_legacy = rawLegacyRL * 0.01f;
    params::vehicle.wheel_speed_rr_legacy = rawLegacyRR * 0.01f;

    const uint16_t rawNewFL = (uint16_t)can986::readBitsLE(frame.data, 0, 16);
    const uint16_t rawNewFR = (uint16_t)can986::readBitsLE(frame.data, 16, 16);
    const uint16_t rawNewRL = (uint16_t)can986::readBitsLE(frame.data, 32, 16);
    const uint16_t rawNewRR = (uint16_t)can986::readBitsLE(frame.data, 48, 16);
    params::vehicle.wheel_speed_fl = rawNewFL * 0.005218f - 1.6338f;
    params::vehicle.wheel_speed_fr = rawNewFR * 0.005218f - 1.6338f;
    params::vehicle.wheel_speed_rl = rawNewRL * 0.005218f - 1.6338f;
    params::vehicle.wheel_speed_rr = rawNewRR * 0.005218f - 1.6338f;
    return true;
}

void encode0x2A8(CANMessage &frame) {
    frame.id = 0x2A8;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint16_t rawLegacyFL = canutils::clampU16((int32_t)(params::vehicle.wheel_speed_fl_legacy / 0.01f + 0.5f));
    const uint16_t rawLegacyFR = canutils::clampU16((int32_t)(params::vehicle.wheel_speed_fr_legacy / 0.01f + 0.5f));
    const uint16_t rawLegacyRL = canutils::clampU16((int32_t)(params::vehicle.wheel_speed_rl_legacy / 0.01f + 0.5f));
    const uint16_t rawLegacyRR = canutils::clampU16((int32_t)(params::vehicle.wheel_speed_rr_legacy / 0.01f + 0.5f));

    const uint16_t rawNewFL = canutils::clampU16((int32_t)((params::vehicle.wheel_speed_fl + 1.6338f) / 0.005218f + 0.5f));
    const uint16_t rawNewFR = canutils::clampU16((int32_t)((params::vehicle.wheel_speed_fr + 1.6338f) / 0.005218f + 0.5f));
    const uint16_t rawNewRL = canutils::clampU16((int32_t)((params::vehicle.wheel_speed_rl + 1.6338f) / 0.005218f + 0.5f));
    const uint16_t rawNewRR = canutils::clampU16((int32_t)((params::vehicle.wheel_speed_rr + 1.6338f) / 0.005218f + 0.5f));

    can986::writeBitsLE(frame.data, 1, 15, rawLegacyFL);
    can986::writeBitsLE(frame.data, 17, 15, rawLegacyFR);
    can986::writeBitsLE(frame.data, 33, 15, rawLegacyRL);
    can986::writeBitsLE(frame.data, 49, 15, rawLegacyRR);

    can986::writeBitsLE(frame.data, 0, 16, rawNewFL);
    can986::writeBitsLE(frame.data, 16, 16, rawNewFR);
    can986::writeBitsLE(frame.data, 32, 16, rawNewRL);
    can986::writeBitsLE(frame.data, 48, 16, rawNewRR);
}

bool decode0x1A0(const CANMessage &frame) {
    if (frame.id != 0x1A0 || frame.len < 8) {
        return false;
    }

    params::vehicle.tcs_intervention = can986::readBitsLE(frame.data, 0, 1) != 0;
    params::vehicle.msr_request = can986::readBitsLE(frame.data, 1, 1) != 0;
    params::vehicle.abs_intervention = can986::readBitsLE(frame.data, 2, 1) != 0;
    params::vehicle.abd_intervention = can986::readBitsLE(frame.data, 3, 1) != 0;
    params::vehicle.fdr_intervention = can986::readBitsLE(frame.data, 4, 1) != 0;
    params::vehicle.asr_control_mode = (uint8_t)can986::readBitsLE(frame.data, 5, 2);
    params::vehicle.abs_warning_lamp = can986::readBitsLE(frame.data, 8, 1) != 0;
    params::vehicle.brake_warning_lamp = can986::readBitsLE(frame.data, 10, 1) != 0;
    params::vehicle.brake_switch = can986::readBitsLE(frame.data, 11, 1) != 0;
    params::vehicle.brake_switch_inverted = can986::readBitsLE(frame.data, 12, 1) != 0;
    params::vehicle.handbrake_switch = can986::readBitsLE(frame.data, 15, 1) != 0;
    params::vehicle.vehicle_reference_speed = can986::readBitsLE(frame.data, 17, 16) * 0.01f;
    params::vehicle.tcs_intervention_slow = can986::readBitsLE(frame.data, 32, 8) * 0.39f;
    params::vehicle.tcs_intervention_fast = can986::readBitsLE(frame.data, 40, 8) * 0.39f;
    params::vehicle.intervention_torque = can986::readBitsLE(frame.data, 48, 8) * 0.39f;
    params::vehicle.lateral_acceleration = can986::readBitsLE(frame.data, 48, 8) * 0.01f - 1.27f;
    return true;
}

void encode0x1A0(CANMessage &frame) {
    frame.id = 0x1A0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint16_t rawVRef = canutils::clampU16((int32_t)(params::vehicle.vehicle_reference_speed / 0.01f + 0.5f));
    const uint8_t rawTcsSlow = canutils::clampU8((int32_t)(params::vehicle.tcs_intervention_slow / 0.39f + 0.5f));
    const uint8_t rawTcsFast = canutils::clampU8((int32_t)(params::vehicle.tcs_intervention_fast / 0.39f + 0.5f));
    // Bit 48 is variant-dependent in references. Use intervention_torque mapping by default.
    const uint8_t rawBit48 = canutils::clampU8((int32_t)(params::vehicle.intervention_torque / 0.39f + 0.5f));

    can986::writeBitsLE(frame.data, 0, 1, params::vehicle.tcs_intervention ? 1U : 0U);
    can986::writeBitsLE(frame.data, 1, 1, params::vehicle.msr_request ? 1U : 0U);
    can986::writeBitsLE(frame.data, 2, 1, params::vehicle.abs_intervention ? 1U : 0U);
    can986::writeBitsLE(frame.data, 3, 1, params::vehicle.abd_intervention ? 1U : 0U);
    can986::writeBitsLE(frame.data, 4, 1, params::vehicle.fdr_intervention ? 1U : 0U);
    can986::writeBitsLE(frame.data, 5, 2, params::vehicle.asr_control_mode);
    can986::writeBitsLE(frame.data, 8, 1, params::vehicle.abs_warning_lamp ? 1U : 0U);
    can986::writeBitsLE(frame.data, 10, 1, params::vehicle.brake_warning_lamp ? 1U : 0U);
    can986::writeBitsLE(frame.data, 11, 1, params::vehicle.brake_switch ? 1U : 0U);
    can986::writeBitsLE(frame.data, 12, 1, params::vehicle.brake_switch_inverted ? 1U : 0U);
    can986::writeBitsLE(frame.data, 15, 1, params::vehicle.handbrake_switch ? 1U : 0U);
    can986::writeBitsLE(frame.data, 17, 16, rawVRef);
    can986::writeBitsLE(frame.data, 32, 8, rawTcsSlow);
    can986::writeBitsLE(frame.data, 40, 8, rawTcsFast);
    can986::writeBitsLE(frame.data, 48, 8, rawBit48);
}

bool decode0x520(const CANMessage &frame) {
    if (frame.id != 0x520 || frame.len < 8) {
        return false;
    }

    params::vehicle.ambient_temperature = can986::readBitsLE(frame.data, 24, 8) * 0.5f - 40.0f;
    params::vehicle.oil_pressure = can986::readBitsLE(frame.data, 32, 8) * 0.04f;
    params::vehicle.light_dimmer = (uint8_t)can986::readBitsLE(frame.data, 8, 8);
    params::vehicle.cluster_counter = (uint8_t)can986::readBitsLE(frame.data, 40, 8);
    return true;
}

void encode0x520(CANMessage &frame) {
    frame.id = 0x520;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint8_t rawAmbient = canutils::clampU8((int32_t)((params::vehicle.ambient_temperature + 40.0f) / 0.5f + 0.5f));
    const uint8_t rawOilPressure = canutils::clampU8((int32_t)(params::vehicle.oil_pressure / 0.04f + 0.5f));

    can986::writeBitsLE(frame.data, 24, 8, rawAmbient);
    can986::writeBitsLE(frame.data, 32, 8, rawOilPressure);
    can986::writeBitsLE(frame.data, 8, 8, params::vehicle.light_dimmer);
    can986::writeBitsLE(frame.data, 40, 8, params::vehicle.cluster_counter);
}

bool decode0x00C0(const CANMessage &frame) {
    if (frame.id != 0x00C0 || frame.len < 8) {
        return false;
    }

    const int32_t rawOld = can986::signExtend(can986::readBitsLE(frame.data, 4, 12), 12);
    const int32_t rawNew = can986::signExtend(can986::readBitsLE(frame.data, 0, 16), 16);
    params::vehicle.steering_angle_old_deg = rawOld * 0.390625f;
    params::vehicle.steering_angle_deg = rawNew * 0.0773f - 1.1668f;
    return true;
}

void encode0x00C0(CANMessage &frame) {
    frame.id = 0x00C0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const int32_t rawOldSigned = (int32_t)(params::vehicle.steering_angle_old_deg / 0.390625f);
    const int32_t rawNewSigned = (int32_t)((params::vehicle.steering_angle_deg + 1.1668f) / 0.0773f);
    const uint16_t rawOld = (uint16_t)(rawOldSigned & 0x0FFF);
    const uint16_t rawNew = (uint16_t)(rawNewSigned & 0xFFFF);

    can986::writeBitsLE(frame.data, 4, 12, rawOld);
    can986::writeBitsLE(frame.data, 0, 16, rawNew);
}
