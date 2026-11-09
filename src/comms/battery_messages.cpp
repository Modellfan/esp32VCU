#include "battery_messages.h"

#include "vcu_params.h"

bool decodeBattery(const CANMessage &frame) {
    // TODO(VCU-REQ-018): decode SoC, pack voltage/current, plug and charging state once the
    // BMS and charger messages are known; write params::battery and return true.
    (void)frame;
    return false;
}
