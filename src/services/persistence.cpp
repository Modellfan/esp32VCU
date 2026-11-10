#include "persistence.h"

#include <string.h>

#include "battery_messages.h"
#include "board.h"
#include "cluster_calc.h"
#include "coolant_fan_control.h"
#include "params_lock.h"
#include "persistent_data_storage.h"

namespace {
constexpr uint32_t CHECK_PERIOD_MS = 1000;

PersistentDataStorage gStorage;
PersistentDataStorage::PersistentData gSaved;  // content of the newest record
bool gReady = false;
uint32_t gLastCheck = 0;

// Caller holds the params lock.
PersistentDataStorage::PersistentData collect() {
    PersistentDataStorage::PersistentData d;
    d.target_charge_pct = params::charging.target_charge_pct;
    d.ac_current_limit_a = params::charging.ac_current_limit_a;
    d.fan_mode = params::fan.fan_mode;
    d.fan_level_manual = params::fan.fan_level_manual;
    d.acceleration_0_50_valid = params::cluster.acceleration_0_50_valid ? 1U : 0U;
    d.acceleration_0_100_valid = params::cluster.acceleration_0_100_valid ? 1U : 0U;
    d.acceleration_0_50_best_s = params::cluster.acceleration_0_50_best_s;
    d.acceleration_0_100_best_s = params::cluster.acceleration_0_100_best_s;
    return d;
}

// Caller holds the params lock. Values out of range keep the defaults.
void apply(const PersistentDataStorage::PersistentData &d) {
    if (d.target_charge_pct >= 50U && d.target_charge_pct <= 100U) {
        params::charging.target_charge_pct = d.target_charge_pct;
    }
    if (d.ac_current_limit_a >= 6U && d.ac_current_limit_a <= 32U) {
        params::charging.ac_current_limit_a = d.ac_current_limit_a;
    }
    if (d.fan_mode <= params::FAN_MODE_AUTO && d.fan_level_manual <= 2U) {
        params::fan.fan_mode = d.fan_mode;
        params::fan.fan_level_manual = d.fan_level_manual;
    }
    if (d.acceleration_0_50_valid && d.acceleration_0_50_best_s > 0.0f) {
        params::cluster.acceleration_0_50_best_s = d.acceleration_0_50_best_s;
        params::cluster.acceleration_0_50_valid = true;
    }
    if (d.acceleration_0_100_valid && d.acceleration_0_100_best_s > 0.0f) {
        params::cluster.acceleration_0_100_best_s = d.acceleration_0_100_best_s;
        params::cluster.acceleration_0_100_valid = true;
    }
}
}  // namespace

void persistenceBegin() {
    if (!eepromReady()) {
        VCU_LOG("[PERSIST] no EEPROM: values are not kept over a restart\n");
        return;
    }
    const bool found = gStorage.begin();
    gSaved = gStorage.load();
    {
        ParamsGuard guard;
        if (found) {
            apply(gSaved);
        }
        gSaved = collect();  // what is in params now; differences get saved
    }
    gReady = true;
    VCU_LOG("[PERSIST] %s (slot %u, seq %lu): charge %u %% %u A, fan mode %u level %u, best 0-50 %.2f s 0-100 %.2f s\n",
            found ? "loaded" : "no record, defaults", (unsigned)gStorage.slot(), (unsigned long)gStorage.sequence(),
            gSaved.target_charge_pct, gSaved.ac_current_limit_a, gSaved.fan_mode, gSaved.fan_level_manual,
            gSaved.acceleration_0_50_valid ? gSaved.acceleration_0_50_best_s : 0.0f,
            gSaved.acceleration_0_100_valid ? gSaved.acceleration_0_100_best_s : 0.0f);
}

void persistenceService(uint32_t nowMs) {
    if (!gReady || nowMs - gLastCheck < CHECK_PERIOD_MS) {
        return;
    }
    gLastCheck = nowMs;
    PersistentDataStorage::PersistentData now;
    {
        ParamsGuard guard;
        now = collect();
    }
    if (memcmp(&now, &gSaved, sizeof now) == 0) {
        return;
    }
    // EEPROM write outside the lock (about 15 ms for one record).
    const bool ok = gStorage.save(now);
    if (ok) {
        gSaved = now;
    }
    VCU_LOG("[PERSIST] save %s (slot %u, seq %lu)\n", ok ? "ok" : "FAILED", (unsigned)gStorage.slot(),
            (unsigned long)gStorage.sequence());
}
