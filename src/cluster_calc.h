#ifndef CLUSTER_CALC_H
#define CLUSTER_CALC_H

#include <Arduino.h>

namespace params {

struct ClusterSignals {
    bool activated = false;
    float total_fuel = 0.0f;
    float power_percent_max = 0.0f;
    float power_percent_dyn = 0.0f;
    int needle_position = 0;
    bool heatsink_temp_critical = false;
    bool motor_temp_critical = false;
};

extern ClusterSignals cluster;

} // namespace params

float powerPercent(float battVoltage_V, float motorCurrent_A, uint8_t refMode);
int needleFromPercent(int8_t dir, float percentSigned);
void updateCluster();

#endif
