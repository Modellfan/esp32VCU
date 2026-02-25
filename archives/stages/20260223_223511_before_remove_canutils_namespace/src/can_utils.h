#ifndef CAN_UTILS_H
#define CAN_UTILS_H

#include <Arduino.h>

namespace canutils {

static inline uint32_t readBitsLE(const uint8_t *data, uint16_t startBit, uint8_t bitLen) {
    uint32_t value = 0;
    for (uint8_t i = 0; i < bitLen; i++) {
        const uint16_t bitIndex = startBit + i;
        const uint8_t byteIndex = bitIndex / 8;
        const uint8_t bitInByte = bitIndex % 8;
        const uint8_t bit = (data[byteIndex] >> bitInByte) & 0x01;
        value |= (uint32_t)bit << i;
    }
    return value;
}

static inline void writeBitsLE(uint8_t *data, uint16_t startBit, uint8_t bitLen, uint32_t value) {
    for (uint8_t i = 0; i < bitLen; i++) {
        const uint16_t bitIndex = startBit + i;
        const uint8_t byteIndex = bitIndex / 8;
        const uint8_t bitInByte = bitIndex % 8;
        const uint8_t bit = (value >> i) & 0x01;
        data[byteIndex] &= ~(1U << bitInByte);
        data[byteIndex] |= (bit << bitInByte);
    }
}

static inline uint8_t clampU8(int32_t value) {
    if (value < 0) {
        return 0;
    }
    if (value > 255) {
        return 255;
    }
    return (uint8_t)value;
}

static inline uint16_t clampU16(int32_t value) {
    if (value < 0) {
        return 0;
    }
    if (value > 65535) {
        return 65535;
    }
    return (uint16_t)value;
}

} // namespace canutils

#endif
