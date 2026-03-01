#include "cluster_calc.h"

#include <Arduino.h>
#include <math.h>

#include "986_ecu_messages.h"
#include "tesla_sdu_messages.h"

// =====================================================
// Configuration
// =====================================================

#define REF_MODE_MAX_POWER 1
#define REF_MODE_DYN_POWER 2

// Fixed reference voltage (used in REF_MODE_MAX_POWER)
#define REF_VOLTAGE_V 348.0f

// "Absolute" allowed currents (positive values)
#define MAX_DRIVE_CURRENT_A 300.0f
#define MAX_REGEN_CURRENT_A 100.0f

// Dynamic limits (positive values)
#define DYN_DRIVE_CURRENT_A 300.0f
#define DYN_REGEN_CURRENT_A 100.0f

// Critical temperature limits
#define MAX_HEATSINK_TEMP_C 80.0f
#define MAX_MOTOR_TEMP_C 160.0f

// DBC: MO2_Kuehlm_T = raw_u8 * 0.75 - 48
#define MO2_KUEHLM_T_RAW_MIN 0.0f
#define MO2_KUEHLM_T_RAW_MAX 255.0f
#define MO2_KUEHLM_T_FACTOR 0.75f
#define MO2_KUEHLM_T_OFFSET -48.0f
#define MO2_KUEHLM_T_MIN_C ((MO2_KUEHLM_T_RAW_MIN * MO2_KUEHLM_T_FACTOR) + MO2_KUEHLM_T_OFFSET)
#define MO2_KUEHLM_T_MAX_C ((MO2_KUEHLM_T_RAW_MAX * MO2_KUEHLM_T_FACTOR) + MO2_KUEHLM_T_OFFSET)

// Needle values
#define NEEDLE_REVERSE_VALUE 35
#define NEEDLE_NEUTRAL_VALUE 822
#define NEEDLE_OFFSET_VALUE 4100
#define NEEDLE_DRIVE_MAX_VALUE 7950
#define NEEDLE_REGEN_MIN_VALUE 1600
#define NEEDLE_LAG 30

// Fuel simulation for MO5 consumption.
#define FUEL_STEP_UL 500U
#define FUEL_MAX_L 20.0f
#define FUEL_SLICE_TICKS 5U
#define NUM_FUEL_OVERFLOW_FLAG 5U
#define MO5_VERBRAUCH_MAX 0x7FFFU
#define MO5_VERBRAUCH_WRAP (MO5_VERBRAUCH_MAX + 1U)

namespace params {
ClusterSignals cluster;
} // namespace params

// =====================================================
// Helpers
// =====================================================

static inline float clampf(float x, float lo, float hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static inline float lerpf(float a, float b, float t) {
    return a + (b - a) * t;
}

static uint32_t s_mo5LastOverflowCount = 0U;
static uint8_t s_mo5OverflowFlagTicksRemaining = 0U;

static void resetMo5FuelOverflowState() {
    s_mo5LastOverflowCount = 0U;
    s_mo5OverflowFlagTicksRemaining = 0U;
    params::ecu.mo5_ueberlverb = false;
}

static void updateMo5FuelFromTotalFuel(float totalFuelL) {
    const float fuelL = (totalFuelL < 0.0f) ? 0.0f : totalFuelL;
    const uint32_t totalFuel_uL = (uint32_t)lroundf(fuelL * 1000000.0f);
    const uint32_t overflowCount = totalFuel_uL / MO5_VERBRAUCH_WRAP;
    const uint32_t rawAfterOverflow = totalFuel_uL % MO5_VERBRAUCH_WRAP;

    if (overflowCount != s_mo5LastOverflowCount) {
        s_mo5OverflowFlagTicksRemaining = NUM_FUEL_OVERFLOW_FLAG;
        s_mo5LastOverflowCount = overflowCount;
    }

    params::ecu.mo5_verbrauch_ul = rawAfterOverflow;
    params::ecu.mo5_ueberlverb = (s_mo5OverflowFlagTicksRemaining > 0U);
    if (s_mo5OverflowFlagTicksRemaining > 0U) {
        s_mo5OverflowFlagTicksRemaining--;
    }
}

float powerPercent(float battVoltage_V, float motorCurrent_A, uint8_t refMode) {
    const bool isRegen = (motorCurrent_A < 0.0f);
    const float vRef = (refMode == REF_MODE_MAX_POWER) ? (float)REF_VOLTAGE_V : battVoltage_V;

    float iLimit;
    if (refMode == REF_MODE_MAX_POWER) {
        iLimit = isRegen ? (float)MAX_REGEN_CURRENT_A : (float)MAX_DRIVE_CURRENT_A;
    } else {
        iLimit = isRegen ? (float)DYN_REGEN_CURRENT_A : (float)DYN_DRIVE_CURRENT_A;
    }

    const float pRef_W = fabsf(vRef * iLimit);
    if (pRef_W <= 2000.0f) return 0.0f;

    const float pAbs_W = fabsf(battVoltage_V * motorCurrent_A);
    float percent = pAbs_W / pRef_W;
    percent = clampf(percent, 0.0f, 1.0f);

    return isRegen ? -percent : percent;
}

int needleFromPercent(int8_t dir, float percentSigned) {
    if (dir > 0) return NEEDLE_REVERSE_VALUE;
    if (dir == 0) return NEEDLE_NEUTRAL_VALUE;

    const float p = clampf(percentSigned, -1.0f, 1.0f);
    if (fabsf(p) < 0.05f) return NEEDLE_OFFSET_VALUE;

    if (p > 0.0f) {
        const int base = (int)lroundf(lerpf((float)NEEDLE_OFFSET_VALUE, (float)NEEDLE_DRIVE_MAX_VALUE, p));
        const int lagged = base + NEEDLE_LAG;
        return (lagged > NEEDLE_DRIVE_MAX_VALUE) ? NEEDLE_DRIVE_MAX_VALUE : lagged;
    }
    return (int)lroundf(lerpf((float)NEEDLE_OFFSET_VALUE, (float)NEEDLE_REGEN_MIN_VALUE, -p));
}

void updateCluster() {
    const float battVoltage_V = params::tesla_sdu.udc;
    const float motorCurrent_A = params::tesla_sdu.idc;
    const int8_t direction = params::tesla_sdu.seldir;
    static uint8_t s_fuelTickDivider = 0U;

    params::cluster.power_percent_max = powerPercent(battVoltage_V, motorCurrent_A, REF_MODE_MAX_POWER);
    params::cluster.power_percent_dyn = powerPercent(battVoltage_V, motorCurrent_A, REF_MODE_DYN_POWER);
    params::cluster.needle_position = needleFromPercent(direction, params::cluster.power_percent_max);
    params::cluster.heatsink_temp_critical = params::tesla_sdu.tmphs >= (float)MAX_HEATSINK_TEMP_C;
    params::cluster.motor_temp_critical = params::tesla_sdu.tmpm >= (float)MAX_MOTOR_TEMP_C;

    if (params::cluster.activated) {
        // Cluster mode intentionally overwrites ECU values; UI reflects this through /api/live.
        params::ecu.engine_speed_rpm = (float)params::cluster.needle_position;
        params::ecu.motor2_coolant_temperature =
            clampf(params::tesla_sdu.tmpm, (float)MO2_KUEHLM_T_MIN_C, (float)MO2_KUEHLM_T_MAX_C);
        params::ecu.motor2_coolant_temp_not_ok =
            params::cluster.heatsink_temp_critical || params::cluster.motor_temp_critical;

        s_fuelTickDivider++;
        if (s_fuelTickDivider >= FUEL_SLICE_TICKS) {
            s_fuelTickDivider = 0U;
            if (params::cluster.total_fuel < (float)FUEL_MAX_L) {
                params::cluster.total_fuel += ((float)FUEL_STEP_UL) * 0.000001f;
                if (params::cluster.total_fuel > (float)FUEL_MAX_L) {
                    params::cluster.total_fuel = (float)FUEL_MAX_L;
                }
            }
            updateMo5FuelFromTotalFuel(params::cluster.total_fuel);
        }
    } else {
        s_fuelTickDivider = 0U;
        resetMo5FuelOverflowState();
    }
}
