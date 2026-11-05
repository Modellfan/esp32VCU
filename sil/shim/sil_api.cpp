// SIL shim: C interface between the unchanged firmware code (src/) and Python (ctypes).
//
// Coupling happens on the parameter level: every value of the central signal table
// (src/signal_table.cpp) can be written and read by name. sil_step() runs the same
// application step as the real-time task (src/vcu_rt.cpp) with a simulated 6 ms clock.
#include <Arduino.h>

#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "cluster_calc.h"
#include "signal_table.h"
#include "tesla_sdu_messages.h"
#include "vcu_app.h"
#include "vcu_params.h"

#define SIL_API extern "C" __declspec(dllexport)

namespace {
uint32_t gNowMs = 0;
constexpr uint32_t kCycleMs = 6;  // RT_PERIOD_MS in src/vcu_rt.h

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

// ---------------------------------------------------------------- time

SIL_API uint32_t sil_now_ms() { return gNowMs; }
SIL_API uint32_t sil_cycle_ms() { return kCycleMs; }

// Advances the simulated clock by one cycle and runs the application step
// (cluster calculation, ECU overrides, fan logic) exactly like vcu_rt.cpp.
SIL_API void sil_step() {
    gNowMs += kCycleMs;
    appStep(gNowMs);
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

// Writes a value with the C++ storage type of the signal (environment model input).
// 0 = ok, -1 = unknown name. Values are not range-checked: the environment model may
// drive the code with anything a real CAN bus could deliver.
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

// Marks a CAN message as received now (signal freshness, VCU-REQ-013).
SIL_API void sil_mark_rx(int bus, uint32_t id) { rxMark((CanBus)bus, id); }

// ---------------------------------------------------------------- ECU overrides (VCU-DEC-005)

SIL_API void sil_override_engine_speed(int active, double rpm) {
    params::ecu_override.engine_speed_active = active != 0;
    params::ecu_override.engine_speed_rpm = (float)rpm;
}

// ---------------------------------------------------------------- CAN frames

// Encodes one ECU emulation frame from the current parameters. Returns the length,
// or -1 for an id the firmware does not send.
SIL_API int sil_encode_ecu(uint32_t id, uint8_t *out8) {
    CANMessage frame;
    switch (id) {
        case 0x280: encode986Ecu0x280(frame); break;
        case 0x289: encode986Ecu0x289(frame); break;
        case 0x298: encode986Ecu0x298(frame); break;
        case 0x4E0: encode986Ecu0x4E0(frame); break;
        case 0x7A0: encode986Ecu0x7A0(frame); break;
        default: return -1;
    }
    memcpy(out8, frame.data, 8);
    return frame.len;
}

// Feeds a received frame into the decoders like the real-time task does.
// bus 1 = vehicle CAN, 2 = drive CAN. Returns 1 when a decoder accepted it.
SIL_API int sil_receive(int bus, uint32_t id, const uint8_t *data8, int len) {
    CANMessage frame;
    frame.id = id;
    frame.len = (uint8_t)(len > 8 ? 8 : len);
    memcpy(frame.data, data8, frame.len);
    const bool ok = bus == BUS_VEHICLE ? decode986Vehicle(frame) : bus == BUS_DRIVE ? decodeTeslaSdu(frame) : false;
    if (ok) {
        rxMark((CanBus)bus, id);
    }
    return ok ? 1 : 0;
}
