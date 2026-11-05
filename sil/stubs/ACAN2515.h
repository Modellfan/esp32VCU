// SIL stub: only the CANMessage type, so that the signal headers (which also declare the
// codec functions) compile. No CAN code is built or run in SIL.
#ifndef SIL_ACAN2515_H
#define SIL_ACAN2515_H

#include <Arduino.h>

class CANMessage {
public:
    uint32_t id = 0;
    bool ext = false;
    bool rtr = false;
    uint8_t idx = 0;
    uint8_t len = 0;
    union {
        uint64_t data64;
        uint32_t data32[2];
        uint16_t data16[4];
        uint8_t data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    };
};

#endif
