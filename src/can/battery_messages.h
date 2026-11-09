// Communication layer: BMS / charger codec for CAN 3.
// The message layout is not defined yet (VCU-REQ-018); decodeBattery() accepts nothing.
#ifndef BATTERY_MESSAGES_H
#define BATTERY_MESSAGES_H

#include <ACAN2515.h>

bool decodeBattery(const CANMessage &frame);

#endif
