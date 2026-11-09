#include "battery_messages.h"

namespace params {
BatterySignals battery;
ChargingSignals charging;
}  // namespace params

bool decodeBattery(const CANMessage &frame) {
    // TODO(VCU-REQ-018): decode SoC, pack voltage/current, plug and charging state once the
    // BMS and charger messages are known; write params::battery and return true.
    (void)frame;
    return false;
}
