// Data layer (RTE) additions: actuator set points, charging parameters, ECU overrides,
// CAN receive timestamps and the lock shared by the real-time task and the web service.
// Signal groups of the codecs stay in their modules (params::vehicle, params::ecu, ...).
#ifndef VCU_PARAMS_H
#define VCU_PARAMS_H

#include <Arduino.h>

namespace params {

enum FanMode : uint8_t { FAN_MODE_MANUAL = 0, FAN_MODE_AUTO = 1 };

// Actuator set points. Writers: web service (manual values, mode) and application
// (automatic fan level). Only the real-time output step drives the hardware.
struct OutputSignals {
    uint8_t fan_mode = FAN_MODE_MANUAL;
    uint8_t fan_level_manual = 0;
    uint8_t fan_level_auto = 0;
    uint8_t fan_level = 0;          // applied level
    bool fan_stage1 = false;        // applied relay states
    bool fan_stage2 = false;
    bool fan_outputs_enabled = false;
    uint8_t digipot_pct = 0;        // requested fuel gauge position
    uint8_t digipot_applied_pct = 0;
    uint8_t digipot_wiper = 0;
};

struct ChargingSignals {
    uint8_t target_charge_pct = 80;   // 50..100
    uint8_t ac_current_limit_a = 16;  // 6..32
};

// Values set through /api/set-ecu that the cluster calculation would otherwise overwrite.
// Priority: override > calculation > default value.
struct EcuOverrides {
    bool engine_speed_active = false;
    float engine_speed_rpm = 0.0f;
    bool coolant_temp_active = false;
    float coolant_temp_c = 0.0f;
    bool coolant_warning_active = false;
    bool coolant_warning = false;
    bool fuel_active = false;
    uint32_t fuel_ul = 0;
};

// BMS / charger on CAN 3 (TWAI). Message layout not defined yet.
struct BatterySignals {
    float soc_pct = 0.0f;
    float pack_voltage_v = 0.0f;
    float pack_current_a = 0.0f;
    bool plug_connected = false;
    bool charging_active = false;
};

extern OutputSignals outputs;
extern ChargingSignals charging;
extern EcuOverrides ecu_override;
extern BatterySignals battery;

}  // namespace params

// ---------------------------------------------------------------------------
// Receive monitor: last reception time per known CAN message (VCU-REQ-013).

enum CanBus : uint8_t { BUS_VEHICLE = 1, BUS_DRIVE = 2, BUS_BATTERY = 3 };

enum RxSource : uint8_t {
    RX_NONE = 0,  // value is produced by the VCU itself: always valid
    RX_VEH_1A0, RX_VEH_2A8, RX_VEH_4A1, RX_VEH_510, RX_VEH_520, RX_VEH_560, RX_VEH_440, RX_VEH_0C0,
    RX_SDU_500, RX_SDU_501, RX_SDU_502, RX_SDU_503, RX_SDU_504, RX_SDU_505, RX_SDU_506, RX_SDU_507, RX_SDU_508,
    RX_BATTERY,   // placeholder until the CAN 3 messages are defined
    RX_COUNT
};

// Timeout after which a value counts as stale. Draft: 500 ms for every message
// (cycle times of the vehicle and inverter messages are not documented yet).
constexpr uint32_t RX_TIMEOUT_MS = 500;

void rxMark(CanBus bus, uint32_t id);
bool rxFresh(RxSource source, uint32_t nowMs);
void rxSnapshot(uint32_t *lastRxMs);  // copies RX_COUNT entries

// ---------------------------------------------------------------------------
// Lock for params::* shared between the real-time task (core 1) and the web task (core 0).
// Hold it only for short copies or single writes.

void paramsLockInit();
void paramsLock();
void paramsUnlock();

class ParamsGuard {
public:
    ParamsGuard() { paramsLock(); }
    ~ParamsGuard() { paramsUnlock(); }
    ParamsGuard(const ParamsGuard &) = delete;
    ParamsGuard &operator=(const ParamsGuard &) = delete;
};

#endif  // VCU_PARAMS_H
