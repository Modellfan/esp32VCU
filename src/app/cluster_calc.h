#ifndef CLUSTER_CALC_H
#define CLUSTER_CALC_H

#include <Arduino.h>

namespace params {

struct ClusterSignals {
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
    bool sdu_data_ok = false;  // 0x500 and 0x504 fresh; otherwise the cluster shows the fallback
    // Calibration (engineering page ECU > Needle), stored in the EEPROM (VCU-REQ-020);
    // the values here are the defaults (/api/set?reset_calibration=1).
    uint16_t needle_reverse = 35;       // direction reverse
    uint16_t needle_neutral = 822;      // direction neutral, also when SDU data is stale
    uint16_t needle_offset = 4100;      // drive, no power
    uint16_t needle_drive_max = 7950;   // drive, full power
    uint16_t needle_regen_min = 1600;   // drive, full regeneration
    int16_t needle_drive_offset = 30;   // drive with power: added to the needle (was NEEDLE_LAG)
    int16_t heatsink_critical_c = 80;   // inverter heatsink: coolant warning from here
    int16_t motor_critical_c = 160;     // motor: coolant warning from here
};

extern ClusterSignals cluster;

} // namespace params

float powerPercent(float battVoltage_V, float motorCurrent_A, uint8_t refMode);
int needleFromPercent(int8_t dir, float percentSigned);
// Every ECU task cycle (main.cpp): power, needle, acceleration timing, fuel; then the ECU overrides.
void updateCluster();
// Clears the stored best acceleration times (/api/set?reset_best_times=1).
void clusterResetBestTimes();
// Needle and temperature calibration back to the firmware defaults (/api/set?reset_calibration=1).
void clusterResetCalibration();

#endif
