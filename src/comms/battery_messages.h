// Communication layer: BMS / charger codec for CAN 3 (TWAI).
// The message layout is not defined yet (VCU-REQ-018); decodeBattery() accepts nothing.
#ifndef BATTERY_MESSAGES_H
#define BATTERY_MESSAGES_H

#include <ACAN2515.h>
#include <Arduino.h>

namespace params {

// BMS / charger values from CAN 3. Message layout not defined yet.
struct BatterySignals {
    float soc_pct = 0.0f;
    float pack_voltage_v = 0.0f;
    float pack_current_a = 0.0f;
    bool plug_connected = false;
    bool charging_active = false;
};

// Charging parameters set through /api/set, to be sent to the charger once defined.
struct ChargingSignals {
    uint8_t target_charge_pct = 80;   // 50..100
    uint8_t ac_current_limit_a = 16;  // 6..32
};

extern BatterySignals battery;
extern ChargingSignals charging;

}  // namespace params

bool decodeBattery(const CANMessage &frame);

#endif
