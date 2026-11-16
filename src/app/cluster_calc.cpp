#include "cluster_calc.h"

#include <Arduino.h>
#include <math.h>

#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "tesla_sdu_messages.h"
#include "canmanager_api.h"

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

// Automatic acceleration timing. Separate stop/start thresholds prevent a
// noisy speed signal around 0 km/h from repeatedly starting a new run.
#define ACCEL_STOP_SPEED_KMH 0.5f
#define ACCEL_START_SPEED_KMH 1.0f
// Plausibility (review B-04): a run is dropped when it is faster than these times (speed glitch),
// when the speed falls this far below the run's maximum, or when 0x510 is no longer received.
#define ACCEL_MIN_0_50_S 1.0f
#define ACCEL_MIN_0_100_S 2.0f
#define ACCEL_ABORT_DROP_KMH 5.0f
#define ACCEL_TARGET_50_KMH 50.0f
#define ACCEL_TARGET_100_KMH 100.0f

// DBC: MO2_Kuehlm_T = raw_u8 * 0.75 - 48
#define MO2_KUEHLM_T_RAW_MIN 0.0f
#define MO2_KUEHLM_T_RAW_MAX 255.0f
#define MO2_KUEHLM_T_FACTOR 0.75f
#define MO2_KUEHLM_T_OFFSET -48.0f
#define MO2_KUEHLM_T_MIN_C ((MO2_KUEHLM_T_RAW_MIN * MO2_KUEHLM_T_FACTOR) + MO2_KUEHLM_T_OFFSET)
#define MO2_KUEHLM_T_MAX_C ((MO2_KUEHLM_T_RAW_MAX * MO2_KUEHLM_T_FACTOR) + MO2_KUEHLM_T_OFFSET)

// Needle values
// Needle calibration: params::cluster.needle_* (settable in engineering mode, not stored).
#define NEEDLE_LAG 30

// Fuel simulation for MO5 consumption.
#define FUEL_STEP_UL 500U
#define FUEL_MAX_L 20.0f
#define FUEL_SLICE_MS 30U  // fuel simulation step, independent of the ECU task period
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

static void updateAccelerationTiming(float vehicleSpeedKmh, bool speedFresh) {
    static bool armed = false;
    static bool reached50 = false;
    static uint32_t runStartMs = 0U;
    static float runMaxKmh = 0.0f;

    const uint32_t nowMs = millis();
    const float speedKmh = isfinite(vehicleSpeedKmh) ? vehicleSpeedKmh : 0.0f;

    // No speed or clear deceleration: drop the run; a new one needs a stop first.
    if (params::cluster.acceleration_run_active &&
        (!speedFresh || speedKmh < runMaxKmh - ACCEL_ABORT_DROP_KMH)) {
        params::cluster.acceleration_run_active = false;
        params::cluster.acceleration_run_time_s = 0.0f;
        armed = false;
    }
    if (!speedFresh) {
        return;
    }

    if (speedKmh <= ACCEL_STOP_SPEED_KMH) {
        armed = true;
        reached50 = false;
        params::cluster.acceleration_run_active = false;
        params::cluster.acceleration_run_time_s = 0.0f;
        return;
    }

    if (armed && speedKmh >= ACCEL_START_SPEED_KMH) {
        armed = false;
        reached50 = false;
        runStartMs = nowMs;
        runMaxKmh = speedKmh;
        params::cluster.acceleration_run_active = true;
        params::cluster.acceleration_run_time_s = 0.0f;
    }

    if (!params::cluster.acceleration_run_active) return;

    const float elapsedS = (float)(nowMs - runStartMs) * 0.001f;
    params::cluster.acceleration_run_time_s = elapsedS;
    if (speedKmh > runMaxKmh) {
        runMaxKmh = speedKmh;
    }

    if (!reached50 && speedKmh >= ACCEL_TARGET_50_KMH && elapsedS < ACCEL_MIN_0_50_S) {
        params::cluster.acceleration_run_active = false;  // implausible: speed glitch
        return;
    }
    if (!reached50 && speedKmh >= ACCEL_TARGET_50_KMH) {
        reached50 = true;
        params::cluster.acceleration_0_50_last_s = elapsedS;
        if (!params::cluster.acceleration_0_50_valid ||
            elapsedS < params::cluster.acceleration_0_50_best_s) {
            params::cluster.acceleration_0_50_best_s = elapsedS;
        }
        params::cluster.acceleration_0_50_valid = true;
    }

    if (speedKmh >= ACCEL_TARGET_100_KMH && elapsedS < ACCEL_MIN_0_100_S) {
        params::cluster.acceleration_run_active = false;  // implausible: speed glitch
        return;
    }
    if (speedKmh >= ACCEL_TARGET_100_KMH) {
        params::cluster.acceleration_0_100_last_s = elapsedS;
        if (!params::cluster.acceleration_0_100_valid ||
            elapsedS < params::cluster.acceleration_0_100_best_s) {
            params::cluster.acceleration_0_100_best_s = elapsedS;
        }
        params::cluster.acceleration_0_100_valid = true;
        params::cluster.acceleration_run_active = false;
    }
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
    const params::ClusterSignals &c = params::cluster;
    if (dir > 0) return c.needle_reverse;
    if (dir == 0) return c.needle_neutral;

    const float p = clampf(percentSigned, -1.0f, 1.0f);
    if (fabsf(p) < 0.05f) return c.needle_offset;

    if (p > 0.0f) {
        const int base = (int)lroundf(lerpf((float)c.needle_offset, (float)c.needle_drive_max, p));
        const int lagged = base + NEEDLE_LAG;
        return (lagged > c.needle_drive_max) ? c.needle_drive_max : lagged;
    }
    return (int)lroundf(lerpf((float)c.needle_offset, (float)c.needle_regen_min, -p));
}

// Values set through /api/set-ecu win over the calculation above (VCU-DEC-005).
static void applyEcuOverrides() {
    const params::EcuOverrides &o = params::ecu_override;
    if (o.engine_speed_active) {
        params::ecu.engine_speed_rpm = o.engine_speed_rpm;
    }
    if (o.coolant_temp_active) {
        params::ecu.motor2_coolant_temperature = o.coolant_temp_c;
    }
    if (o.coolant_warning_active) {
        params::ecu.motor2_coolant_temp_not_ok = o.coolant_warning;
    }
    if (o.fuel_active) {
        params::ecu.mo5_verbrauch_ul = o.fuel_ul;
    }
}

void updateCluster() {
    const uint32_t nowMs = millis();
    static uint32_t s_lastFuelMs = 0U;
    // Fallback when the inverter values are stale (review B-01): needle neutral, gauge at rest,
    // coolant warning on, so the driver sees that drive data is missing.
    const bool sduOk = rxFresh(RX_SDU_500, nowMs) && rxFresh(RX_SDU_504, nowMs);
    params::cluster.sdu_data_ok = sduOk;
    const float battVoltage_V = sduOk ? params::tesla_sdu.udc : 0.0f;
    const float motorCurrent_A = sduOk ? params::tesla_sdu.idc : 0.0f;
    const int8_t direction = sduOk ? params::tesla_sdu.seldir : (int8_t)0;

    updateAccelerationTiming(params::vehicle.vehicle_speed_510, rxFresh(RX_VEH_510, nowMs));

    params::cluster.power_percent_max = powerPercent(battVoltage_V, motorCurrent_A, REF_MODE_MAX_POWER);
    params::cluster.power_percent_dyn = powerPercent(battVoltage_V, motorCurrent_A, REF_MODE_DYN_POWER);
    params::cluster.needle_position = needleFromPercent(direction, params::cluster.power_percent_max);
    params::cluster.heatsink_temp_critical = sduOk && params::tesla_sdu.tmphs >= (float)params::cluster.heatsink_critical_c;
    params::cluster.motor_temp_critical = sduOk && params::tesla_sdu.tmpm >= (float)params::cluster.motor_critical_c;

    // Engine emulation always runs; single values are fixed with overrides (/api/set-ecu,
    // value "auto" releases them), applied below (2026-11-16: switch 'activated' removed).
    params::ecu.engine_speed_rpm = (float)params::cluster.needle_position;
    params::ecu.motor2_coolant_temperature = sduOk
        ? clampf(params::tesla_sdu.tmpm, (float)MO2_KUEHLM_T_MIN_C, (float)MO2_KUEHLM_T_MAX_C)
        : (float)MO2_KUEHLM_T_MIN_C;
    params::ecu.motor2_coolant_temp_not_ok =
        !sduOk || params::cluster.heatsink_temp_critical || params::cluster.motor_temp_critical;

    if (nowMs - s_lastFuelMs >= FUEL_SLICE_MS) {
        s_lastFuelMs = nowMs;
        if (params::cluster.total_fuel < (float)FUEL_MAX_L) {
            params::cluster.total_fuel += ((float)FUEL_STEP_UL) * 0.000001f;
            if (params::cluster.total_fuel > (float)FUEL_MAX_L) {
                params::cluster.total_fuel = (float)FUEL_MAX_L;
            }
        }
        updateMo5FuelFromTotalFuel(params::cluster.total_fuel);
    }

    applyEcuOverrides();
}

void clusterResetCalibration() {
    const params::ClusterSignals defaults;
    params::cluster.needle_reverse = defaults.needle_reverse;
    params::cluster.needle_neutral = defaults.needle_neutral;
    params::cluster.needle_offset = defaults.needle_offset;
    params::cluster.needle_drive_max = defaults.needle_drive_max;
    params::cluster.needle_regen_min = defaults.needle_regen_min;
    params::cluster.heatsink_critical_c = defaults.heatsink_critical_c;
    params::cluster.motor_critical_c = defaults.motor_critical_c;
}

void clusterResetBestTimes() {
    params::cluster.acceleration_0_50_best_s = 0.0f;
    params::cluster.acceleration_0_100_best_s = 0.0f;
    params::cluster.acceleration_0_50_valid = false;
    params::cluster.acceleration_0_100_valid = false;
}
