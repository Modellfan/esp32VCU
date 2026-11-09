// SIL shim: C interface between unchanged application components (src/) and Python (ctypes).
//
// Components are tested on the application level: inputs and outputs are parameters of
// the central signal table (src/data/signal_table.cpp), written and read by name. No CAN layer.
//
//   component 1 = cluster   updateCluster()          (src/app/cluster_calc.cpp)
//   component 2 = app       appStep(now)             (src/app/vcu_app.cpp: cluster, overrides, fan)
#include <Arduino.h>

#include "cluster_calc.h"
#include "signal_table.h"
#include "vcu_app.h"
#include "vcu_params.h"

#define SIL_API extern "C" __declspec(dllexport)

namespace {
uint32_t gNowMs = 0;
constexpr uint32_t kCycleMs = 6;  // RT_PERIOD_MS in src/services/vcu_rt.h

uint8_t *liveBase(SigGroup group) {
    switch (group) {
        case G_VEHICLE: return reinterpret_cast<uint8_t *>(&params::vehicle);
        case G_ECU: return reinterpret_cast<uint8_t *>(&params::ecu);
        case G_SDU: return reinterpret_cast<uint8_t *>(&params::tesla_sdu);
        case G_CLUSTER: return reinterpret_cast<uint8_t *>(&params::cluster);
        case G_OUTPUTS: return reinterpret_cast<uint8_t *>(&params::outputs);
        case G_CHARGING: return reinterpret_cast<uint8_t *>(&params::charging);
        case G_BATTERY: return reinterpret_cast<uint8_t *>(&params::battery);
        default: return nullptr;
    }
}

template <typename T> void storeAs(uint8_t *p, double value) {
    const T v = (T)value;
    memcpy(p, &v, sizeof v);
}
}  // namespace

uint32_t millis() { return gNowMs; }
uint32_t micros() { return gNowMs * 1000U; }

// ---------------------------------------------------------------- time and components

SIL_API uint32_t sil_now_ms() { return gNowMs; }
SIL_API uint32_t sil_cycle_ms() { return kCycleMs; }

// Advances the simulated clock by one cycle and runs the selected component once.
// Returns -1 for an unknown component.
SIL_API int sil_step(int component) {
    gNowMs += kCycleMs;
    switch (component) {
        case 1: updateCluster(); return 0;
        case 2: appStep(gNowMs); return 0;
        default: return -1;
    }
}

// ---------------------------------------------------------------- parameters

SIL_API int sil_signal_count() { return (int)kSignalCount; }
SIL_API const char *sil_signal_name(int index) {
    return index >= 0 && (size_t)index < kSignalCount ? kSignals[index].name : nullptr;
}

// 0 = ok, -1 = unknown name.
SIL_API int sil_get(const char *name, double *value) {
    const SignalDef *def = findSignal(name);
    if (def == nullptr) {
        return -1;
    }
    *value = readSignal(*def, liveBase(def->group));
    return 0;
}

// Writes a value with the C++ storage type of the signal. 0 = ok, -1 = unknown name.
// Not range-checked: a test may drive the component with any value.
SIL_API int sil_set(const char *name, double value) {
    const SignalDef *def = findSignal(name);
    if (def == nullptr) {
        return -1;
    }
    uint8_t *p = liveBase(def->group) + def->offset;
    switch (def->storage) {
        case S_BOOL: storeAs<bool>(p, value != 0.0); break;
        case S_U8: storeAs<uint8_t>(p, value); break;
        case S_I8: storeAs<int8_t>(p, value); break;
        case S_U16: storeAs<uint16_t>(p, value); break;
        case S_I16: storeAs<int16_t>(p, value); break;
        case S_U32: storeAs<uint32_t>(p, value); break;
        case S_I32: storeAs<int32_t>(p, value); break;
        case S_F32: storeAs<float>(p, value); break;
    }
    return 0;
}

// ---------------------------------------------------------------- ECU overrides (VCU-DEC-005)

SIL_API void sil_override_engine_speed(int active, double rpm) {
    params::ecu_override.engine_speed_active = active != 0;
    params::ecu_override.engine_speed_rpm = (float)rpm;
}
