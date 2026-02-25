#ifndef PORSCHE_986_ECU_MESSAGES_H
#define PORSCHE_986_ECU_MESSAGES_H

#include <Arduino.h>
#include <ACAN2515.h>

namespace params {

struct EcuSignals {
    float engine_speed_rpm = 0.0f;
    float throttle_position_280 = 0.0f;
    float pedal_position_1 = 0.0f;
    float pedal_position_2 = 0.0f;

    float coolant_temperature = 0.0f;
    bool coolant_level_switch = false;
    bool cruise_control_active = false;

    float inlet_air_temperature = 0.0f;
    float throttle_position_1_298 = 0.0f;
    float throttle_position_2_298 = 0.0f;
    float barometric_pressure_mbar = 0.0f;
    uint8_t vehicle_speed_298_raw = 0;

    bool check_engine_light = false;
    bool reduced_power = false;
    bool fan_error = false;
    uint16_t fuel_used_raw = 0;
    float boost_pressure = 0.0f;
    float oil_temperature = 0.0f;
};

extern EcuSignals ecu;

} // namespace params

bool decode0x280(const CANMessage &frame);
bool decode0x289(const CANMessage &frame);
bool decode0x298(const CANMessage &frame);
bool decode0x4E0(const CANMessage &frame);

void encode0x280(CANMessage &frame);
void encode0x289(CANMessage &frame);
void encode0x298(CANMessage &frame);
void encode0x4E0(CANMessage &frame);

#endif
