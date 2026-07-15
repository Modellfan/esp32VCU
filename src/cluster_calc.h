#ifndef CLUSTER_CALC_H
#define CLUSTER_CALC_H

#include <Arduino.h>

namespace params {

struct ClusterSignals {
    bool activated = true;
    float total_fuel = 0.0f;
    float power_percent_max = 0.0f;
    float power_percent_dyn = 0.0f;
    int needle_position = 0;
    bool heatsink_temp_critical = false;
    bool motor_temp_critical = false;
    bool acceleration_run_active = false;
    float acceleration_run_time_s = 0.0f;
    float acceleration_0_50_last_s = 0.0f;
    float acceleration_0_50_best_s = 0.0f;
    float acceleration_0_100_last_s = 0.0f;
    float acceleration_0_100_best_s = 0.0f;
    bool acceleration_0_50_valid = false;
    bool acceleration_0_100_valid = false;
};

extern ClusterSignals cluster;

} // namespace params

float powerPercent(float battVoltage_V, float motorCurrent_A, uint8_t refMode);
int needleFromPercent(int8_t dir, float percentSigned);
void updateCluster();

#endif
