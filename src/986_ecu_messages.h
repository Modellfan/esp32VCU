#ifndef PORSCHE_986_ECU_MESSAGES_H
#define PORSCHE_986_ECU_MESSAGES_H

#include <Arduino.h>
#include <ACAN2515.h>

namespace params {

struct EcuSignals {
    // Standard/default controls used by encoder functions.
    uint8_t standard_280_force_one_bits[13] = {8, 11, 13, 14, 32, 37, 38, 50, 52, 54, 58, 59, 60};
    uint8_t standard_280_force_one_count = 13;
    uint8_t standard_298_force_one_bits[11] = {8, 16, 21, 22, 24, 32, 34, 35, 36, 37, 38};
    uint8_t standard_298_force_one_count = 11;
    uint8_t standard_289_byte0_sequence[4] = {0x00, 0x40, 0x80, 0xF3};
    uint8_t standard_289_byte0_sequence_len = 4;
    uint8_t standard_289_frames_per_step = 4;
    uint32_t standard_289_frame_counter = 0;

    // PQ Motor_2 mapped to CAN ID 0x289 (30 ms).
    uint8_t motor2_multiplex_selector = 0;
    uint8_t motor2_can_version = 0;
    uint8_t motor2_engine_code = 0;
    uint8_t motor2_transmission_code = 0;
    float motor2_max_torque_nm = 510.0f;
    float motor2_coolant_temperature = -48.0f;
    bool motor2_brake_light_switch = false;
    bool motor2_brake_test_switch = false;
    bool motor2_coolant_temp_not_ok = true;
    bool motor2_ac_compressor_active = false;
    bool motor2_normal_operation_active = true;
    bool motor2_drivetrain_coordinator_active = false;
    uint8_t motor2_cruise_control_status = 0;
    bool motor2_speed_limiter_selected = false;
    bool motor2_cruise_control_regulating = true;
    bool motor2_sport_error = true;
    bool motor2_offroad_mode_active = true;
    float motor2_biodiesel_content_percent = 37.5f;
    float motor2_cruise_target_speed_kmh = 174.08f;
    float motor2_idle_target_speed_rpm = 1230.0f;
    float motor2_limited_torque_percent = 39.78f;
    float motor2_min_torque_ignition_retard_percent = 37.83f;

    float engine_speed_rpm = 0.0f;
    float throttle_position_280 = 37.5f;
    float pedal_position_1 = 1.0f;
    float pedal_position_2 = 28.0f;

    float inlet_air_temperature = -47.25f;
    float throttle_position_1_298 = 37.890625f;
    float throttle_position_2_298 = 48.828125f;
    float barometric_pressure_mbar = 0.0f;
    uint8_t vehicle_speed_298_raw = 0;

    bool check_engine_light = false;
    bool check_engine_light_bit1 = false;
    bool engine_stat_bool2 = false;
    bool reduced_power = false;
    bool fan_error = false;
    bool engine_stat_bool5 = false;
    bool engine_stat_bool6 = false;
    bool engine_stat_bool7 = false;

    bool mo5_vorgluehen = false;
    bool mo5_e_gas = false;
    bool mo5_obd_2 = false;
    bool mo5_heissl = false;
    bool mo5_klimakompr = false;
    bool mo5_feld_kuehl = false;
    bool mo5_kliko_red = false;
    uint16_t mo5_verbrauch_ul = 0;
    bool mo5_ueberlverb = false;

    uint8_t unknown_4f8[8] = {0};
    uint8_t unknown_7a0[8] = {0x85, 0x0C, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x02};
};

extern EcuSignals ecu;

} // namespace params

bool decode986Ecu0x280(const CANMessage &frame);
bool decode986Ecu0x289(const CANMessage &frame);
bool decode986Ecu0x298(const CANMessage &frame);
bool decode986Ecu0x4E0(const CANMessage &frame);
bool decode986Ecu0x4F8(const CANMessage &frame);
bool decode986Ecu0x7A0(const CANMessage &frame);
bool decode986Ecu(const CANMessage &frame);

void encode986Ecu0x280(CANMessage &frame);
void encode986Ecu0x289(CANMessage &frame);
void encode986Ecu0x298(CANMessage &frame);
void encode986Ecu0x4E0(CANMessage &frame);
void encode986Ecu0x4F8(CANMessage &frame);
void encode986Ecu0x7A0(CANMessage &frame);

#endif
