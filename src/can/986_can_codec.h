#ifndef PORSCHE_986_CAN_CODEC_H
#define PORSCHE_986_CAN_CODEC_H

#include <Arduino.h>
#include <ACAN2515.h>

namespace can986 {

static inline int32_t signExtend(uint32_t value, uint8_t bitLen) {
    if (bitLen == 0 || bitLen >= 32) {
        return (int32_t)value;
    }
    const uint32_t signBit = 1UL << (bitLen - 1);
    if (value & signBit) {
        value |= (~0UL << bitLen);
    }
    return (int32_t)value;
}

} // namespace can986

#endif
