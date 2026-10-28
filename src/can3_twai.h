// Driver layer: CAN 3 (BMS / charger) on the ESP32-S3 TWAI controller.
// Inactive until board::GPIO_CAN3_TX/RX are assigned (VCU-REQ-018, open).
#ifndef CAN3_TWAI_H
#define CAN3_TWAI_H

#include <ACAN2515.h>

bool can3Begin();
bool can3Active();
// Non-blocking. Returns true and fills frame when a frame was received.
bool can3Receive(CANMessage &frame);

#endif
