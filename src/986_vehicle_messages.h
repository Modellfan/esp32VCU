#ifndef PORSCHE_986_VEHICLE_MESSAGES_H
#define PORSCHE_986_VEHICLE_MESSAGES_H

#include <Arduino.h>
#include <ACAN2515.h>

namespace params {

struct VehicleSignals {
    bool frei_bremse_3_1 = false;
    float wheel_speed_fl_legacy = 0.0f;
    bool frei_bremse_3_2 = false;
    float wheel_speed_fr_legacy = 0.0f;
    bool frei_bremse_3_3 = false;
    float wheel_speed_rl_legacy = 0.0f;
    bool frei_bremse_3_4 = false;
    float wheel_speed_rr_legacy = 0.0f;

    bool tcs_intervention = false;
    bool msr_request = false;
    bool abs_intervention = false;
    bool abd_intervention = false;
    bool fdr_intervention = false;
    uint8_t asr_control_mode = 0;
    bool fdr_lamp = false;
    bool abs_warning_lamp = false;
    bool fdr_lamp2 = false;
    bool brake_warning_lamp = false;
    bool brake_switch = false;
    bool brake_switch_inverted = false;
    bool rough_road_suppression = false;
    bool rough_road_suppression_status = false;
    bool handbrake_switch = false;
    bool psm_button_lamp = false;
    float vehicle_reference_speed = 0.0f;
    float tcs_intervention_slow = 0.0f;
    float tcs_intervention_fast = 0.0f;
    float msr_torque_inverse = 0.0f;
    float intervention_torque = 0.0f;
    float lateral_acceleration = 0.0f;
    uint8_t br1_counter = 0;
    bool br1_asr_esp = false;
    bool br1_espasr_passive = false;
    bool br1_sta_esp = false;
    bool br1_ersatz_kmh = false;

    float ambient_temperature = 0.0f;
    float oil_pressure = 0.0f;
    uint8_t light_dimmer = 0;
    uint8_t cluster_counter = 0;

    bool vorzeichen_rb = false;
    float lenkwinkel_rb = 0.0f;
    bool lws_ok_rb = false;
    bool lws_abgleich_rb = false;
    bool frei_lenkwinkel_1_1_rb = false;
    bool fine_check_rb = false;
    bool coarse_check_rb = false;

    bool yaw_rate_sign = false;
    float yaw_rate = 0.0f;
    bool no_actively_built_up_brake_pressure = false;
    bool acc_related_error = false;
    bool receipt_for_acc_message = false;
    bool driver_braking = false;
    bool regulator_error_fdr = false;
    float acceleration = 0.0f;
    bool error_driver_brake_detection = false;
    uint8_t multiplex_byte4 = 0;
    float brake_normal_or_offset = 0.0f;

    float vehicle_speed_510 = 0.0f;
    bool indicator_left = false;
    bool indicator_right = false;
    bool tempomat_button = false;
    bool steeringwheel_button_up = false;
    bool steeringwheel_button_down = false;

    uint8_t prnd_pos = 0;

    uint8_t unknown_560[8] = {0};
};

extern VehicleSignals vehicle;

} // namespace params

bool decode986Vehicle0x2A8(const CANMessage &frame);
bool decode986Vehicle0x1A0(const CANMessage &frame);
bool decode986Vehicle0x520(const CANMessage &frame);
bool decode986Vehicle0x00C0(const CANMessage &frame);
bool decode986Vehicle0x4A1(const CANMessage &frame);
bool decode986Vehicle0x510(const CANMessage &frame);
bool decode986Vehicle0x440(const CANMessage &frame);
bool decode986Vehicle0x560(const CANMessage &frame);
bool decode986Vehicle(const CANMessage &frame);

void encode986Vehicle0x2A8(CANMessage &frame);
void encode986Vehicle0x1A0(CANMessage &frame);
void encode986Vehicle0x520(CANMessage &frame);
void encode986Vehicle0x00C0(CANMessage &frame);
void encode986Vehicle0x4A1(CANMessage &frame);
void encode986Vehicle0x510(CANMessage &frame);
void encode986Vehicle0x440(CANMessage &frame);
void encode986Vehicle0x560(CANMessage &frame);

#endif
