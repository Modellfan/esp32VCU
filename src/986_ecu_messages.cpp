#include "986_ecu_messages.h"
#include "986_can_codec.h"
#include "can_utils.h"

namespace params {
EcuSignals ecu;
}

bool decode0x280(const CANMessage &frame) {
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

void encode0x280(CANMessage &frame) {
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
}

bool decode0x289(const CANMessage &frame) {
    if (frame.id != 0x289 || frame.len < 8) {
        return false;
    }

    const uint8_t rawCoolantTemp = (uint8_t)readBitsLE(frame.data, 8, 8);
    params::ecu.coolant_temperature = rawCoolantTemp * 0.75f - 48.0f;
    params::ecu.coolant_level_switch = readBitsLE(frame.data, 16, 1) != 0;
    params::ecu.cruise_control_active = readBitsLE(frame.data, 22, 1) != 0;
    return true;
}

void encode0x289(CANMessage &frame) {
    frame.id = 0x289;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint8_t rawCoolantTemp = clampU8((int32_t)((params::ecu.coolant_temperature + 48.0f) / 0.75f + 0.5f));
    writeBitsLE(frame.data, 8, 8, rawCoolantTemp);
    writeBitsLE(frame.data, 16, 1, params::ecu.coolant_level_switch ? 1U : 0U);
    writeBitsLE(frame.data, 22, 1, params::ecu.cruise_control_active ? 1U : 0U);
}

bool decode0x298(const CANMessage &frame) {
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

void encode0x298(CANMessage &frame) {
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
}

bool decode0x4E0(const CANMessage &frame) {
    if (frame.id != 0x4E0 || frame.len < 8) {
        return false;
    }

    params::ecu.check_engine_light = readBitsLE(frame.data, 0, 1) != 0;
    params::ecu.reduced_power = readBitsLE(frame.data, 3, 1) != 0;
    params::ecu.fan_error = readBitsLE(frame.data, 4, 1) != 0;
    params::ecu.fuel_used_raw = (uint16_t)readBitsLE(frame.data, 16, 16);
    params::ecu.boost_pressure = readBitsLE(frame.data, 32, 8) * 10.0f;
    params::ecu.oil_temperature = readBitsLE(frame.data, 40, 8) * 0.75f - 48.0f;
    return true;
}

void encode0x4E0(CANMessage &frame) {
    frame.id = 0x4E0;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const uint8_t rawBoost = clampU8((int32_t)(params::ecu.boost_pressure / 10.0f + 0.5f));
    const uint8_t rawOilTemp = clampU8((int32_t)((params::ecu.oil_temperature + 48.0f) / 0.75f + 0.5f));

    writeBitsLE(frame.data, 0, 1, params::ecu.check_engine_light ? 1U : 0U);
    writeBitsLE(frame.data, 3, 1, params::ecu.reduced_power ? 1U : 0U);
    writeBitsLE(frame.data, 4, 1, params::ecu.fan_error ? 1U : 0U);
    writeBitsLE(frame.data, 16, 16, params::ecu.fuel_used_raw);
    writeBitsLE(frame.data, 32, 8, rawBoost);
    writeBitsLE(frame.data, 40, 8, rawOilTemp);
}


