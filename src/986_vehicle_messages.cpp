#include "986_vehicle_messages.h"
#include "986_can_codec.h"
#include "can_utils.h"

namespace params {
VehicleSignals vehicle;
}

bool decode986Vehicle0x2A8(const CANMessage &frame) {
    if (frame.id != 0x2A8 || frame.len < 8) {
        return false;
    }

    params::vehicle.frei_bremse_3_1 = readBitsLE(frame.data, 0, 1) != 0;
    const uint16_t rawLegacyFL = (uint16_t)readBitsLE(frame.data, 1, 15);
    params::vehicle.frei_bremse_3_2 = readBitsLE(frame.data, 16, 1) != 0;
    const uint16_t rawLegacyFR = (uint16_t)readBitsLE(frame.data, 17, 15);
    params::vehicle.frei_bremse_3_3 = readBitsLE(frame.data, 32, 1) != 0;
    const uint16_t rawLegacyRL = (uint16_t)readBitsLE(frame.data, 33, 15);
    params::vehicle.frei_bremse_3_4 = readBitsLE(frame.data, 48, 1) != 0;
    const uint16_t rawLegacyRR = (uint16_t)readBitsLE(frame.data, 49, 15);
    params::vehicle.wheel_speed_fl_legacy = rawLegacyFL * 0.01f;
    params::vehicle.wheel_speed_fr_legacy = rawLegacyFR * 0.01f;
    params::vehicle.wheel_speed_rl_legacy = rawLegacyRL * 0.01f;
    params::vehicle.wheel_speed_rr_legacy = rawLegacyRR * 0.01f;

    return true;
}

void encode986Vehicle0x2A8(CANMessage &frame) {
    frame.id = 0x2A8;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint16_t rawLegacyFL = clampU16((int32_t)(params::vehicle.wheel_speed_fl_legacy / 0.01f + 0.5f));
    const uint16_t rawLegacyFR = clampU16((int32_t)(params::vehicle.wheel_speed_fr_legacy / 0.01f + 0.5f));
    const uint16_t rawLegacyRL = clampU16((int32_t)(params::vehicle.wheel_speed_rl_legacy / 0.01f + 0.5f));
    const uint16_t rawLegacyRR = clampU16((int32_t)(params::vehicle.wheel_speed_rr_legacy / 0.01f + 0.5f));

    writeBitsLE(frame.data, 0, 1, params::vehicle.frei_bremse_3_1 ? 1U : 0U);
    writeBitsLE(frame.data, 1, 15, rawLegacyFL);
    writeBitsLE(frame.data, 16, 1, params::vehicle.frei_bremse_3_2 ? 1U : 0U);
    writeBitsLE(frame.data, 17, 15, rawLegacyFR);
    writeBitsLE(frame.data, 32, 1, params::vehicle.frei_bremse_3_3 ? 1U : 0U);
    writeBitsLE(frame.data, 33, 15, rawLegacyRL);
    writeBitsLE(frame.data, 48, 1, params::vehicle.frei_bremse_3_4 ? 1U : 0U);
    writeBitsLE(frame.data, 49, 15, rawLegacyRR);
}

bool decode986Vehicle0x1A0(const CANMessage &frame) {
    if (frame.id != 0x1A0 || frame.len < 8) {
        return false;
    }

    params::vehicle.tcs_intervention = readBitsLE(frame.data, 0, 1) != 0;
    params::vehicle.msr_request = readBitsLE(frame.data, 1, 1) != 0;
    params::vehicle.abs_intervention = readBitsLE(frame.data, 2, 1) != 0;
    params::vehicle.abd_intervention = readBitsLE(frame.data, 3, 1) != 0;
    params::vehicle.fdr_intervention = readBitsLE(frame.data, 4, 1) != 0;
    params::vehicle.asr_control_mode = (uint8_t)readBitsLE(frame.data, 5, 2);
    params::vehicle.fdr_lamp = readBitsLE(frame.data, 7, 1) != 0;
    params::vehicle.abs_warning_lamp = readBitsLE(frame.data, 8, 1) != 0;
    params::vehicle.fdr_lamp2 = readBitsLE(frame.data, 9, 1) != 0;
    params::vehicle.brake_warning_lamp = readBitsLE(frame.data, 10, 1) != 0;
    params::vehicle.brake_switch = readBitsLE(frame.data, 11, 1) != 0;
    params::vehicle.brake_switch_inverted = readBitsLE(frame.data, 12, 1) != 0;
    params::vehicle.rough_road_suppression = readBitsLE(frame.data, 13, 1) != 0;
    params::vehicle.rough_road_suppression_status = readBitsLE(frame.data, 14, 1) != 0;
    params::vehicle.handbrake_switch = readBitsLE(frame.data, 15, 1) != 0;
    params::vehicle.psm_button_lamp = readBitsLE(frame.data, 16, 1) != 0;
    params::vehicle.vehicle_reference_speed = readBitsLE(frame.data, 17, 15) * 0.01f;
    params::vehicle.tcs_intervention_slow = readBitsLE(frame.data, 32, 8) * 0.39f;
    const uint8_t rawByte40 = (uint8_t)readBitsLE(frame.data, 40, 8);
    params::vehicle.tcs_intervention_fast = rawByte40 * 0.39f;
    params::vehicle.msr_torque_inverse = rawByte40 * -0.39f + 99.45f;
    params::vehicle.intervention_torque = readBitsLE(frame.data, 48, 8) * 0.39f;
    params::vehicle.lateral_acceleration = 0.0f;
    params::vehicle.br1_counter = (uint8_t)readBitsLE(frame.data, 56, 4);
    params::vehicle.br1_asr_esp = readBitsLE(frame.data, 60, 1) != 0;
    params::vehicle.br1_espasr_passive = readBitsLE(frame.data, 61, 1) != 0;
    params::vehicle.br1_sta_esp = readBitsLE(frame.data, 62, 1) != 0;
    params::vehicle.br1_ersatz_kmh = readBitsLE(frame.data, 63, 1) != 0;
    return true;
}

void encode986Vehicle0x1A0(CANMessage &frame) {
    frame.id = 0x1A0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint16_t rawVRef = (uint16_t)(clampU16((int32_t)(params::vehicle.vehicle_reference_speed / 0.01f + 0.5f)) & 0x7FFFU);
    const uint8_t rawTcsSlow = clampU8((int32_t)(params::vehicle.tcs_intervention_slow / 0.39f + 0.5f));
    const uint8_t rawTcsFast = clampU8((int32_t)(params::vehicle.tcs_intervention_fast / 0.39f + 0.5f));
    const uint8_t rawMsrInv = clampU8((int32_t)((99.45f - params::vehicle.msr_torque_inverse) / 0.39f + 0.5f));
    const uint8_t rawMsrTorque = clampU8((int32_t)(params::vehicle.intervention_torque / 0.39f + 0.5f));

    writeBitsLE(frame.data, 0, 1, params::vehicle.tcs_intervention ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vehicle.msr_request ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vehicle.abs_intervention ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vehicle.abd_intervention ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::vehicle.fdr_intervention ? 1U : 0U);
    writeBitsLE(frame.data, 5, 2, params::vehicle.asr_control_mode);
    writeBitsLE(frame.data, 7, 1, params::vehicle.fdr_lamp ? 1U : 0U);
    writeBitsLE(frame.data, 8, 1, params::vehicle.abs_warning_lamp ? 1U : 0U);
    writeBitsLE(frame.data, 9, 1, params::vehicle.fdr_lamp2 ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::vehicle.brake_warning_lamp ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::vehicle.brake_switch ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vehicle.brake_switch_inverted ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vehicle.rough_road_suppression ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vehicle.rough_road_suppression_status ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vehicle.handbrake_switch ? 1U : 0U);
    writeBitsLE(frame.data, 16, 1, params::vehicle.psm_button_lamp ? 1U : 0U);
    writeBitsLE(frame.data, 17, 15, rawVRef);
    writeBitsLE(frame.data, 32, 8, rawTcsSlow);
    writeBitsLE(frame.data, 40, 8, params::vehicle.msr_request ? rawMsrInv : rawTcsFast);
    writeBitsLE(frame.data, 48, 8, rawMsrTorque);
    writeBitsLE(frame.data, 56, 4, params::vehicle.br1_counter & 0x0FU);
    writeBitsLE(frame.data, 60, 1, params::vehicle.br1_asr_esp ? 1U : 0U);
    writeBitsLE(frame.data, 61, 1, params::vehicle.br1_espasr_passive ? 1U : 0U);
    writeBitsLE(frame.data, 62, 1, params::vehicle.br1_sta_esp ? 1U : 0U);
    writeBitsLE(frame.data, 63, 1, params::vehicle.br1_ersatz_kmh ? 1U : 0U);
}

bool decode986Vehicle0x520(const CANMessage &frame) {
    if (frame.id != 0x520 || frame.len < 8) {
        return false;
    }

    params::vehicle.ambient_temperature = readBitsLE(frame.data, 24, 8) * 0.5f - 40.0f;
    params::vehicle.oil_pressure = readBitsLE(frame.data, 32, 8) * 0.04f;
    params::vehicle.light_dimmer = (uint8_t)readBitsLE(frame.data, 8, 8);
    params::vehicle.cluster_counter = (uint8_t)readBitsLE(frame.data, 40, 8);
    return true;
}

void encode986Vehicle0x520(CANMessage &frame) {
    frame.id = 0x520;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint8_t rawAmbient = clampU8((int32_t)((params::vehicle.ambient_temperature + 40.0f) / 0.5f + 0.5f));
    const uint8_t rawOilPressure = clampU8((int32_t)(params::vehicle.oil_pressure / 0.04f + 0.5f));

    writeBitsLE(frame.data, 24, 8, rawAmbient);
    writeBitsLE(frame.data, 32, 8, rawOilPressure);
    writeBitsLE(frame.data, 8, 8, params::vehicle.light_dimmer);
    writeBitsLE(frame.data, 40, 8, params::vehicle.cluster_counter);
}

bool decode986Vehicle0x00C0(const CANMessage &frame) {
    if (frame.id != 0x00C0 || frame.len < 2) {
        return false;
    }

    params::vehicle.vorzeichen_rb = readBitsLE(frame.data, 15, 1) != 0;
    params::vehicle.lenkwinkel_rb = readBitsLE(frame.data, 5, 10) * 2.5f - 720.0f;
    params::vehicle.lws_ok_rb = readBitsLE(frame.data, 4, 1) != 0;
    params::vehicle.lws_abgleich_rb = readBitsLE(frame.data, 3, 1) != 0;
    params::vehicle.frei_lenkwinkel_1_1_rb = readBitsLE(frame.data, 2, 1) != 0;
    params::vehicle.fine_check_rb = readBitsLE(frame.data, 1, 1) != 0;
    params::vehicle.coarse_check_rb = readBitsLE(frame.data, 0, 1) != 0;
    return true;
}

void encode986Vehicle0x00C0(CANMessage &frame) {
    frame.id = 0x00C0;
    frame.len = 2;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const int32_t rawAngle = (int32_t)((params::vehicle.lenkwinkel_rb + 720.0f) / 2.5f + 0.5f);
    uint16_t rawAngleClamped = 0;
    if (rawAngle > 0) {
        rawAngleClamped = (uint16_t)rawAngle;
    }
    if (rawAngleClamped > 0x03FFU) {
        rawAngleClamped = 0x03FFU;
    }

    writeBitsLE(frame.data, 15, 1, params::vehicle.vorzeichen_rb ? 1U : 0U);
    writeBitsLE(frame.data, 5, 10, rawAngleClamped);
    writeBitsLE(frame.data, 4, 1, params::vehicle.lws_ok_rb ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::vehicle.lws_abgleich_rb ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::vehicle.frei_lenkwinkel_1_1_rb ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::vehicle.fine_check_rb ? 1U : 0U);
    writeBitsLE(frame.data, 0, 1, params::vehicle.coarse_check_rb ? 1U : 0U);
}

bool decode986Vehicle0x4A1(const CANMessage &frame) {
    if (frame.id != 0x4A1 || frame.len < 8) {
        return false;
    }

    params::vehicle.yaw_rate_sign = readBitsLE(frame.data, 0, 1) != 0;
    params::vehicle.yaw_rate = readBitsLE(frame.data, 1, 9) * 0.0021326f;
    params::vehicle.no_actively_built_up_brake_pressure = readBitsLE(frame.data, 10, 1) != 0;
    params::vehicle.acc_related_error = readBitsLE(frame.data, 12, 1) != 0;
    params::vehicle.receipt_for_acc_message = readBitsLE(frame.data, 13, 1) != 0;
    params::vehicle.driver_braking = readBitsLE(frame.data, 14, 1) != 0;
    params::vehicle.regulator_error_fdr = readBitsLE(frame.data, 15, 1) != 0;
    params::vehicle.acceleration = readBitsLE(frame.data, 16, 9) * 0.3125f;
    params::vehicle.error_driver_brake_detection = readBitsLE(frame.data, 25, 1) != 0;
    params::vehicle.multiplex_byte4 = (uint8_t)readBitsLE(frame.data, 30, 2);
    params::vehicle.brake_normal_or_offset = readBitsLE(frame.data, 32, 8) * 0.1f;
    return true;
}

void encode986Vehicle0x4A1(CANMessage &frame) {
    frame.id = 0x4A1;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint16_t rawYaw = clampU16((int32_t)(params::vehicle.yaw_rate / 0.0021326f + 0.5f));
    const uint16_t rawAcc = clampU16((int32_t)(params::vehicle.acceleration / 0.3125f + 0.5f));
    const uint8_t rawBrake = clampU8((int32_t)(params::vehicle.brake_normal_or_offset / 0.1f + 0.5f));

    writeBitsLE(frame.data, 0, 1, params::vehicle.yaw_rate_sign ? 1U : 0U);
    writeBitsLE(frame.data, 1, 9, rawYaw);
    writeBitsLE(frame.data, 10, 1, params::vehicle.no_actively_built_up_brake_pressure ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::vehicle.acc_related_error ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::vehicle.receipt_for_acc_message ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::vehicle.driver_braking ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::vehicle.regulator_error_fdr ? 1U : 0U);
    writeBitsLE(frame.data, 16, 9, rawAcc);
    writeBitsLE(frame.data, 25, 1, params::vehicle.error_driver_brake_detection ? 1U : 0U);
    writeBitsLE(frame.data, 30, 2, params::vehicle.multiplex_byte4 & 0x03U);
    writeBitsLE(frame.data, 32, 8, rawBrake);
}

bool decode986Vehicle0x510(const CANMessage &frame) {
    if (frame.id != 0x510 || frame.len < 8) {
        return false;
    }

    params::vehicle.vehicle_speed_510 = readBitsLE(frame.data, 0, 16) * 0.250264f - 4100.17f;
    params::vehicle.indicator_left = readBitsLE(frame.data, 20, 1) != 0;
    params::vehicle.indicator_right = readBitsLE(frame.data, 21, 1) != 0;
    params::vehicle.tempomat_button = readBitsLE(frame.data, 23, 1) != 0;
    params::vehicle.steeringwheel_button_up = readBitsLE(frame.data, 48, 1) != 0;
    params::vehicle.steeringwheel_button_down = readBitsLE(frame.data, 49, 1) != 0;
    return true;
}

void encode986Vehicle0x510(CANMessage &frame) {
    frame.id = 0x510;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint16_t rawSpeed = clampU16((int32_t)((params::vehicle.vehicle_speed_510 + 4100.17f) / 0.250264f + 0.5f));
    writeBitsLE(frame.data, 0, 16, rawSpeed);
    writeBitsLE(frame.data, 20, 1, params::vehicle.indicator_left ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::vehicle.indicator_right ? 1U : 0U);
    writeBitsLE(frame.data, 23, 1, params::vehicle.tempomat_button ? 1U : 0U);
    writeBitsLE(frame.data, 48, 1, params::vehicle.steeringwheel_button_up ? 1U : 0U);
    writeBitsLE(frame.data, 49, 1, params::vehicle.steeringwheel_button_down ? 1U : 0U);
}

bool decode986Vehicle0x440(const CANMessage &frame) {
    if (frame.id != 0x440 || frame.len < 8) {
        return false;
    }
    params::vehicle.prnd_pos = (uint8_t)readBitsLE(frame.data, 16, 8);
    return true;
}

void encode986Vehicle0x440(CANMessage &frame) {
    frame.id = 0x440;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }
    writeBitsLE(frame.data, 16, 8, params::vehicle.prnd_pos);
}

bool decode986Vehicle0x560(const CANMessage &frame) {
    if (frame.id != 0x560 || frame.len < 8) {
        return false;
    }
    for (uint8_t i = 0; i < 8; i++) {
        params::vehicle.unknown_560[i] = frame.data[i];
    }
    return true;
}

void encode986Vehicle0x560(CANMessage &frame) {
    frame.id = 0x560;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = params::vehicle.unknown_560[i];
    }
}

bool decode986Vehicle(const CANMessage &frame) {
    switch (frame.id) {
        case 0x2A8: return decode986Vehicle0x2A8(frame);
        case 0x1A0: return decode986Vehicle0x1A0(frame);
        case 0x520: return decode986Vehicle0x520(frame);
        case 0x00C0: return decode986Vehicle0x00C0(frame);
        case 0x4A1: return decode986Vehicle0x4A1(frame);
        case 0x510: return decode986Vehicle0x510(frame);
        case 0x440: return decode986Vehicle0x440(frame);
        case 0x560: return decode986Vehicle0x560(frame);
        default: return false;
    }
}
