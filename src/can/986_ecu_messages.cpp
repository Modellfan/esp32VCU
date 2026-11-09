#include "986_ecu_messages.h"
#include "986_can_codec.h"
#include "can_utils.h"

namespace params {
EcuSignals ecu;
}

// 0x280 Engine_1 (ECU) - nominal time slice: 30 ms
bool decode986Ecu0x280(const CANMessage &frame) {
    if (frame.id != 0x280 || frame.len < 8) {
        return false;
    }

    const uint16_t rawEngineSpeed = (uint16_t)readBitsLE(frame.data, 16, 16);
    params::ecu.engine_speed_rpm = rawEngineSpeed * 0.25f;
    params::ecu.throttle_position_280 = readBitsLE(frame.data, 40, 8) * 0.390625f;
    params::ecu.pedal_position_1 = readBitsLE(frame.data, 32, 8) * 1.0f;
    params::ecu.pedal_position_2 = readBitsLE(frame.data, 56, 8) * 1.0f;
    return true;
}

void encode986Ecu0x280(CANMessage &frame) {
    frame.id = 0x280;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint16_t rawEngineSpeed = clampU16((int32_t)(params::ecu.engine_speed_rpm / 0.25f + 0.5f));
    const uint8_t rawThrottle = clampU8((int32_t)(params::ecu.throttle_position_280 / 0.390625f + 0.5f));
    const uint8_t rawPedal1 = clampU8((int32_t)(params::ecu.pedal_position_1 + 0.5f));
    const uint8_t rawPedal2 = clampU8((int32_t)(params::ecu.pedal_position_2 + 0.5f));

    writeBitsLE(frame.data, 16, 16, rawEngineSpeed);
    writeBitsLE(frame.data, 40, 8, rawThrottle);
    writeBitsLE(frame.data, 32, 8, rawPedal1);
    writeBitsLE(frame.data, 56, 8, rawPedal2);

    // Enforce standard fixed bit pattern from struct defaults.
    for (uint8_t i = 0; i < params::ecu.standard_280_force_one_count && i < 13; i++) {
        writeBitsLE(frame.data, params::ecu.standard_280_force_one_bits[i], 1, 1U);
    }
}

// 0x289 Motor_2 (ECU/PQ mapping) - nominal time slice: 30 ms
bool decode986Ecu0x289(const CANMessage &frame) {
    if (frame.id != 0x289 || frame.len < 8) {
        return false;
    }

    const uint8_t rawMuxPayload = (uint8_t)readBitsLE(frame.data, 0, 6);
    params::ecu.motor2_multiplex_selector = (uint8_t)readBitsLE(frame.data, 6, 2);
    switch (params::ecu.motor2_multiplex_selector) {
        case 0:
            params::ecu.motor2_can_version = rawMuxPayload;
            break;
        case 1:
            params::ecu.motor2_engine_code = rawMuxPayload;
            break;
        case 2:
            params::ecu.motor2_transmission_code = rawMuxPayload;
            break;
        case 3:
            params::ecu.motor2_max_torque_nm = rawMuxPayload * 10.0f;
            break;
        default:
            break;
    }

    params::ecu.motor2_coolant_temperature = readBitsLE(frame.data, 8, 8) * 0.75f - 48.0f;
    params::ecu.motor2_brake_light_switch = readBitsLE(frame.data, 16, 1) != 0;
    params::ecu.motor2_brake_test_switch = readBitsLE(frame.data, 17, 1) != 0;
    params::ecu.motor2_coolant_temp_not_ok = readBitsLE(frame.data, 18, 1) != 0;
    params::ecu.motor2_ac_compressor_active = readBitsLE(frame.data, 19, 1) != 0;
    params::ecu.motor2_normal_operation_active = readBitsLE(frame.data, 20, 1) != 0;
    params::ecu.motor2_drivetrain_coordinator_active = readBitsLE(frame.data, 21, 1) != 0;
    params::ecu.motor2_cruise_control_status = (uint8_t)readBitsLE(frame.data, 22, 2);
    params::ecu.motor2_speed_limiter_selected = readBitsLE(frame.data, 25, 1) != 0;
    params::ecu.motor2_cruise_control_regulating = readBitsLE(frame.data, 26, 1) != 0;
    params::ecu.motor2_sport_error = readBitsLE(frame.data, 27, 1) != 0;
    params::ecu.motor2_offroad_mode_active = readBitsLE(frame.data, 28, 1) != 0;
    params::ecu.motor2_biodiesel_content_percent = readBitsLE(frame.data, 29, 3) * 12.5f;
    params::ecu.motor2_cruise_target_speed_kmh = readBitsLE(frame.data, 32, 8) * 1.28f;
    params::ecu.motor2_idle_target_speed_rpm = readBitsLE(frame.data, 40, 8) * 10.0f;
    params::ecu.motor2_limited_torque_percent = readBitsLE(frame.data, 48, 8) * 0.39f;
    params::ecu.motor2_min_torque_ignition_retard_percent = readBitsLE(frame.data, 56, 8) * 0.39f;
    return true;
}

void encode986Ecu0x289(CANMessage &frame) {
    frame.id = 0x289;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    // Standard byte-0 sequence from struct defaults.
    uint8_t sequenceLen = params::ecu.standard_289_byte0_sequence_len;
    if (sequenceLen == 0 || sequenceLen > 4) {
        sequenceLen = 4;
    }
    uint8_t framesPerStep = params::ecu.standard_289_frames_per_step;
    if (framesPerStep == 0) {
        framesPerStep = 1;
    }
    const uint8_t sequenceIndex = (uint8_t)((params::ecu.standard_289_frame_counter / framesPerStep) % sequenceLen);
    const uint8_t rawByte0 = params::ecu.standard_289_byte0_sequence[sequenceIndex];
    params::ecu.standard_289_frame_counter++;

    const uint8_t rawMuxPayload = (uint8_t)(rawByte0 & 0x3FU);
    const uint8_t rawMuxSelector = (uint8_t)((rawByte0 >> 6U) & 0x03U);
    params::ecu.motor2_multiplex_selector = rawMuxSelector;
    if (rawMuxSelector == 0U) {
        params::ecu.motor2_can_version = rawMuxPayload;
    } else if (rawMuxSelector == 1U) {
        params::ecu.motor2_engine_code = rawMuxPayload;
    } else if (rawMuxSelector == 2U) {
        params::ecu.motor2_transmission_code = rawMuxPayload;
    } else {
        params::ecu.motor2_max_torque_nm = rawMuxPayload * 10.0f;
    }

    const uint8_t rawCoolantTemp = clampU8((int32_t)((params::ecu.motor2_coolant_temperature + 48.0f) / 0.75f + 0.5f));
    const uint8_t rawRme = (uint8_t)(clampU8((int32_t)(params::ecu.motor2_biodiesel_content_percent / 12.5f + 0.5f)) & 0x07U);
    const uint8_t rawCruiseSetSpeed = clampU8((int32_t)(params::ecu.motor2_cruise_target_speed_kmh / 1.28f + 0.5f));
    const uint8_t rawIdleSetSpeed = clampU8((int32_t)(params::ecu.motor2_idle_target_speed_rpm / 10.0f + 0.5f));
    const uint8_t rawLimitedTorque = clampU8((int32_t)(params::ecu.motor2_limited_torque_percent / 0.39f + 0.5f));
    const uint8_t rawMinTorque = clampU8((int32_t)(params::ecu.motor2_min_torque_ignition_retard_percent / 0.39f + 0.5f));

    writeBitsLE(frame.data, 0, 6, rawMuxPayload);
    writeBitsLE(frame.data, 6, 2, rawMuxSelector);
    writeBitsLE(frame.data, 8, 8, rawCoolantTemp);
    writeBitsLE(frame.data, 16, 1, params::ecu.motor2_brake_light_switch ? 1U : 0U);
    writeBitsLE(frame.data, 17, 1, params::ecu.motor2_brake_test_switch ? 1U : 0U);
    writeBitsLE(frame.data, 18, 1, params::ecu.motor2_coolant_temp_not_ok ? 1U : 0U);
    writeBitsLE(frame.data, 19, 1, params::ecu.motor2_ac_compressor_active ? 1U : 0U);
    writeBitsLE(frame.data, 20, 1, params::ecu.motor2_normal_operation_active ? 1U : 0U);
    writeBitsLE(frame.data, 21, 1, params::ecu.motor2_drivetrain_coordinator_active ? 1U : 0U);
    writeBitsLE(frame.data, 22, 2, params::ecu.motor2_cruise_control_status & 0x03U);
    writeBitsLE(frame.data, 25, 1, params::ecu.motor2_speed_limiter_selected ? 1U : 0U);
    writeBitsLE(frame.data, 26, 1, params::ecu.motor2_cruise_control_regulating ? 1U : 0U);
    writeBitsLE(frame.data, 27, 1, params::ecu.motor2_sport_error ? 1U : 0U);
    writeBitsLE(frame.data, 28, 1, params::ecu.motor2_offroad_mode_active ? 1U : 0U);
    writeBitsLE(frame.data, 29, 3, rawRme);
    writeBitsLE(frame.data, 32, 8, rawCruiseSetSpeed);
    writeBitsLE(frame.data, 40, 8, rawIdleSetSpeed);
    writeBitsLE(frame.data, 48, 8, rawLimitedTorque);
    writeBitsLE(frame.data, 56, 8, rawMinTorque);
}

// 0x298 Engine_2 (ECU) - nominal time slice: 30 ms
bool decode986Ecu0x298(const CANMessage &frame) {
    if (frame.id != 0x298 || frame.len < 8) {
        return false;
    }

    const uint8_t rawIat = (uint8_t)readBitsLE(frame.data, 8, 8);
    params::ecu.inlet_air_temperature = rawIat * 0.75f - 48.0f;
    params::ecu.throttle_position_1_298 = readBitsLE(frame.data, 16, 8) * 0.390625f;
    params::ecu.throttle_position_2_298 = readBitsLE(frame.data, 32, 8) * 0.390625f;
    params::ecu.barometric_pressure_mbar = readBitsLE(frame.data, 40, 8) * 5.0f;
    params::ecu.vehicle_speed_298_raw = (uint8_t)readBitsLE(frame.data, 56, 8);
    return true;
}

void encode986Ecu0x298(CANMessage &frame) {
    frame.id = 0x298;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint8_t rawIat = clampU8((int32_t)((params::ecu.inlet_air_temperature + 48.0f) / 0.75f + 0.5f));
    const uint8_t rawThrottle1 = clampU8((int32_t)(params::ecu.throttle_position_1_298 / 0.390625f + 0.5f));
    const uint8_t rawThrottle2 = clampU8((int32_t)(params::ecu.throttle_position_2_298 / 0.390625f + 0.5f));
    const uint8_t rawBaro = clampU8((int32_t)(params::ecu.barometric_pressure_mbar / 5.0f + 0.5f));

    writeBitsLE(frame.data, 8, 8, rawIat);
    writeBitsLE(frame.data, 16, 8, rawThrottle1);
    writeBitsLE(frame.data, 32, 8, rawThrottle2);
    writeBitsLE(frame.data, 40, 8, rawBaro);
    writeBitsLE(frame.data, 56, 8, params::ecu.vehicle_speed_298_raw);

    // Enforce standard fixed bit pattern from struct defaults.
    for (uint8_t i = 0; i < params::ecu.standard_298_force_one_count && i < 11; i++) {
        writeBitsLE(frame.data, params::ecu.standard_298_force_one_bits[i], 1, 1U);
    }
}

// 0x4E0 EngineStat (ECU) - nominal time slice: 30 ms
bool decode986Ecu0x4E0(const CANMessage &frame) {
    if (frame.id != 0x4E0 || frame.len < 8) {
        return false;
    }

    params::ecu.check_engine_light = readBitsLE(frame.data, 0, 1) != 0;
    params::ecu.check_engine_light_bit1 = readBitsLE(frame.data, 1, 1) != 0;
    params::ecu.engine_stat_bool2 = readBitsLE(frame.data, 2, 1) != 0;
    params::ecu.reduced_power = readBitsLE(frame.data, 3, 1) != 0;
    params::ecu.fan_error = readBitsLE(frame.data, 4, 1) != 0;
    params::ecu.engine_stat_bool5 = readBitsLE(frame.data, 5, 1) != 0;
    params::ecu.engine_stat_bool6 = readBitsLE(frame.data, 6, 1) != 0;
    params::ecu.engine_stat_bool7 = readBitsLE(frame.data, 7, 1) != 0;

    params::ecu.mo5_vorgluehen = readBitsLE(frame.data, 9, 1) != 0;
    params::ecu.mo5_e_gas = readBitsLE(frame.data, 10, 1) != 0;
    params::ecu.mo5_obd_2 = readBitsLE(frame.data, 11, 1) != 0;
    params::ecu.mo5_heissl = readBitsLE(frame.data, 12, 1) != 0;
    params::ecu.mo5_klimakompr = readBitsLE(frame.data, 13, 1) != 0;
    params::ecu.mo5_feld_kuehl = readBitsLE(frame.data, 14, 1) != 0;
    params::ecu.mo5_kliko_red = readBitsLE(frame.data, 15, 1) != 0;
    params::ecu.mo5_verbrauch_ul = readBitsLE(frame.data, 16, 15);
    params::ecu.mo5_ueberlverb = readBitsLE(frame.data, 31, 1) != 0;
    return true;
}

void encode986Ecu0x4E0(CANMessage &frame) {
    frame.id = 0x4E0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 1, params::ecu.check_engine_light ? 1U : 0U);
    writeBitsLE(frame.data, 1, 1, params::ecu.check_engine_light_bit1 ? 1U : 0U);
    writeBitsLE(frame.data, 2, 1, params::ecu.engine_stat_bool2 ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::ecu.reduced_power ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::ecu.fan_error ? 1U : 0U);
    writeBitsLE(frame.data, 5, 1, params::ecu.engine_stat_bool5 ? 1U : 0U);
    writeBitsLE(frame.data, 6, 1, params::ecu.engine_stat_bool6 ? 1U : 0U);
    writeBitsLE(frame.data, 7, 1, params::ecu.engine_stat_bool7 ? 1U : 0U);

    const uint32_t cappedVerbrauch = (params::ecu.mo5_verbrauch_ul > 0x7FFFU) ? 0x7FFFU : params::ecu.mo5_verbrauch_ul;
    const uint16_t rawVerbrauch = (uint16_t)cappedVerbrauch;
    writeBitsLE(frame.data, 9, 1, params::ecu.mo5_vorgluehen ? 1U : 0U);
    writeBitsLE(frame.data, 10, 1, params::ecu.mo5_e_gas ? 1U : 0U);
    writeBitsLE(frame.data, 11, 1, params::ecu.mo5_obd_2 ? 1U : 0U);
    writeBitsLE(frame.data, 12, 1, params::ecu.mo5_heissl ? 1U : 0U);
    writeBitsLE(frame.data, 13, 1, params::ecu.mo5_klimakompr ? 1U : 0U);
    writeBitsLE(frame.data, 14, 1, params::ecu.mo5_feld_kuehl ? 1U : 0U);
    writeBitsLE(frame.data, 15, 1, params::ecu.mo5_kliko_red ? 1U : 0U);
    writeBitsLE(frame.data, 16, 15, rawVerbrauch);
    writeBitsLE(frame.data, 31, 1, params::ecu.mo5_ueberlverb ? 1U : 0U);
}

bool decode986Ecu0x4F8(const CANMessage &frame) {
    if (frame.id != 0x4F8 || frame.len < 8) {
        return false;
    }
    for (uint8_t i = 0; i < 8; i++) {
        params::ecu.unknown_4f8[i] = frame.data[i];
    }
    return true;
}

void encode986Ecu0x4F8(CANMessage &frame) {
    frame.id = 0x4F8;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = params::ecu.unknown_4f8[i];
    }
}

// 0x7A0 Unknown_7A0 (ECU) - nominal time slice: 30 ms
bool decode986Ecu0x7A0(const CANMessage &frame) {
    if (frame.id != 0x7A0 || frame.len < 8) {
        return false;
    }
    for (uint8_t i = 0; i < 8; i++) {
        params::ecu.unknown_7a0[i] = frame.data[i];
    }
    return true;
}

void encode986Ecu0x7A0(CANMessage &frame) {
    frame.id = 0x7A0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = params::ecu.unknown_7a0[i];
    }
}

bool decode986Ecu(const CANMessage &frame) {
    switch (frame.id) {
        case 0x280: return decode986Ecu0x280(frame);
        case 0x289: return decode986Ecu0x289(frame);
        case 0x298: return decode986Ecu0x298(frame);
        case 0x4E0: return decode986Ecu0x4E0(frame);
        case 0x4F8: return decode986Ecu0x4F8(frame);
        case 0x7A0: return decode986Ecu0x7A0(frame);
        default: return false;
    }
}
