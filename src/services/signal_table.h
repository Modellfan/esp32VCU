// Central signal table of the web API (VCU-DEC-007): one entry per value exposed by
// /api/live. /api/live, /api/signals and the set endpoints are generated from it, so a
// field name exists in exactly one place in the firmware. The snapshot copies every
// module's data under the params lock, so the web task builds JSON without holding it.
#ifndef SIGNAL_TABLE_H
#define SIGNAL_TABLE_H

#include <Arduino.h>
#include <stddef.h>

#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "battery_messages.h"
#include "can_rx_monitor.h"
#include "cluster_calc.h"
#include "coolant_fan_control.h"
#include "digipot_control.h"
#include "tesla_sdu_messages.h"

// Every data group of the modules, listed once: X(group id, struct type, params:: instance).
#define SIGNAL_GROUPS(X)                          \
    X(G_VEHICLE, VehicleSignals, vehicle)         \
    X(G_ECU, EcuSignals, ecu)                     \
    X(G_ECU_OVERRIDE, EcuOverrides, ecu_override) \
    X(G_SDU, TeslaSduSignals, tesla_sdu)          \
    X(G_CLUSTER, ClusterSignals, cluster)         \
    X(G_FAN, FanSignals, fan)                     \
    X(G_DIGIPOT, DigipotSignals, digipot)         \
    X(G_CHARGING, ChargingSignals, charging)      \
    X(G_BATTERY, BatterySignals, battery)

#define SIGNAL_GROUP_ID(id, Type, name) id,
enum SigGroup : uint8_t { SIGNAL_GROUPS(SIGNAL_GROUP_ID) G_GROUP_COUNT };
#undef SIGNAL_GROUP_ID
enum SigStorage : uint8_t { S_BOOL, S_U8, S_I8, S_U16, S_I16, S_U32, S_I32, S_F32 };
enum SigFormat : uint8_t { F_NUM, F_INT, F_UINT, F_BOOL };

// Flags
constexpr uint8_t W_ECU = 0x01;               // settable via /api/set-ecu (writes the value directly)
constexpr uint8_t OVR_ENGINE_SPEED = 0x02;     // settable via /api/set-ecu, stored as override
constexpr uint8_t OVR_COOLANT_TEMP = 0x04;
constexpr uint8_t OVR_COOLANT_WARNING = 0x08;
constexpr uint8_t OVR_FUEL = 0x10;
constexpr uint8_t OVR_ANY = OVR_ENGINE_SPEED | OVR_COOLANT_TEMP | OVR_COOLANT_WARNING | OVR_FUEL;

struct SignalDef {
    const char *name;
    SigGroup group;
    uint16_t offset;
    SigStorage storage;
    SigFormat format;
    uint8_t decimals;
    RxSource rx;
    uint8_t flags;
};

extern const SignalDef kSignals[];
extern const size_t kSignalCount;

const SignalDef *findSignal(const char *name);

// Copy of every signal group, taken under the params lock.
#define SIGNAL_GROUP_MEMBER(id, Type, name) params::Type name;
struct ParamsSnapshot {
    SIGNAL_GROUPS(SIGNAL_GROUP_MEMBER)
    uint32_t last_rx_ms[RX_COUNT];
};
#undef SIGNAL_GROUP_MEMBER

void takeSnapshot(ParamsSnapshot &snap);
const uint8_t *snapshotGroupBase(const ParamsSnapshot &snap, SigGroup group);
// Live data of a group (caller holds the params lock).
uint8_t *liveGroupBase(SigGroup group);

double readSignal(const SignalDef &def, const uint8_t *base);
// Writes a parsed value into the live params (caller holds the params lock).
// Rejects text that is not a valid value for the storage type.
bool writeSignal(const SignalDef &def, const String &text);
bool parseSignalBool(const String &text, bool &out);
bool parseSignalNumber(const String &text, double &out);

const char *sigStorageName(SigStorage storage);

#endif  // SIGNAL_TABLE_H
