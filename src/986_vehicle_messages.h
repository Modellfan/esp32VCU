#ifndef PORSCHE_986_VEHICLE_MESSAGES_H
#define PORSCHE_986_VEHICLE_MESSAGES_H

#include <Arduino.h>
#include <ACAN2515.h>

namespace params {

struct VehicleSignals {
    float wheel_speed_fl_legacy = 0.0f;
    float wheel_speed_fr_legacy = 0.0f;
    float wheel_speed_rl_legacy = 0.0f;
    float wheel_speed_rr_legacy = 0.0f;
    float wheel_speed_fl = 0.0f;
    float wheel_speed_fr = 0.0f;
    float wheel_speed_rl = 0.0f;
    float wheel_speed_rr = 0.0f;

    bool tcs_intervention = false;
    bool msr_request = false;
    bool abs_intervention = false;
    bool abd_intervention = false;
    bool fdr_intervention = false;
    uint8_t asr_control_mode = 0;
    bool abs_warning_lamp = false;
    bool brake_warning_lamp = false;
    bool brake_switch = false;
    bool brake_switch_inverted = false;
    bool handbrake_switch = false;
    float vehicle_reference_speed = 0.0f;
    float tcs_intervention_slow = 0.0f;
    float tcs_intervention_fast = 0.0f;
    float intervention_torque = 0.0f;
    float lateral_acceleration = 0.0f;

    float ambient_temperature = 0.0f;
    float oil_pressure = 0.0f;
    uint8_t light_dimmer = 0;
    uint8_t cluster_counter = 0;

    float steering_angle_old_deg = 0.0f;
    float steering_angle_deg = 0.0f;
};

extern VehicleSignals vehicle;

} // namespace params

bool decode0x2A8(const CANMessage &frame);
bool decode0x1A0(const CANMessage &frame);
bool decode0x520(const CANMessage &frame);
bool decode0x00C0(const CANMessage &frame);

void encode0x2A8(CANMessage &frame);
void encode0x1A0(CANMessage &frame);
void encode0x520(CANMessage &frame);
void encode0x00C0(CANMessage &frame);

#endif
