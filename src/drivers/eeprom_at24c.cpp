#include "eeprom_at24c.h"

#include <I2C_eeprom.h>
#include <Wire.h>
#include <new>
#include <string.h>

#include "board.h"

namespace {
constexpr uint32_t I2C_CLOCK_HZ = 400000;

uint8_t gAddress = 0;
uint32_t gSize = 0;
alignas(I2C_eeprom) uint8_t gStorage[sizeof(I2C_eeprom)];  // created once the size is known
I2C_eeprom *gChip = nullptr;
}  // namespace

bool eepromBegin() {
    Wire.begin(board::GPIO_EEPROM_SDA, board::GPIO_EEPROM_SCL, I2C_CLOCK_HZ);
    for (uint8_t address = 0x50; address <= 0x57 && gAddress == 0; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            gAddress = address;
        }
    }
    if (gAddress == 0) {
        VCU_LOG("[EEPROM] no AT24C found at 0x50..0x57 (SDA GPIO%d, SCL GPIO%d)\n", board::GPIO_EEPROM_SDA,
                board::GPIO_EEPROM_SCL);
        return false;
    }
    I2C_eeprom probe(gAddress, &Wire);
    probe.begin();
    gSize = probe.determineSizeNoWrite();
    if (gSize == 0) {
        VCU_LOG("[EEPROM] AT24C at 0x%02X, size unknown\n", gAddress);
        return false;
    }
    gChip = new (gStorage) I2C_eeprom(gAddress, gSize, &Wire);
    gChip->begin();
    VCU_LOG("[EEPROM] AT24C at 0x%02X, %lu bytes, page %u\n", gAddress, (unsigned long)gSize, gChip->getPageSize());
    return true;
}

bool eepromReady() {
    return gChip != nullptr;
}

uint32_t eepromSize() {
    return gSize;
}

bool eepromRead(uint32_t address, void *data, size_t length) {
    if (gChip == nullptr || address + length > gSize) {
        return false;
    }
    return gChip->readBlock((uint16_t)address, static_cast<uint8_t *>(data), (uint16_t)length) == length;
}

bool eepromWrite(uint32_t address, const void *data, size_t length) {
    if (gChip == nullptr || address + length > gSize) {
        return false;
    }
    const uint8_t *bytes = static_cast<const uint8_t *>(data);
    // Verify after the write cycle (tWR max 5 ms): the ACK polling of the library reports the
    // chip ready too early on the ESP32-S3 (bench 2026-11-10), an early read-back fails.
    if (gChip->updateBlock((uint16_t)address, bytes, (uint16_t)length) > 0) {
        delay(6);
    }
    uint8_t chunk[32];
    for (size_t done = 0; done < length; done += sizeof chunk) {
        const size_t n = length - done < sizeof chunk ? length - done : sizeof chunk;
        if (!eepromRead(address + done, chunk, n) || memcmp(chunk, bytes + done, n) != 0) {
            return false;
        }
    }
    return true;
}

void eepromSelfTest() {
    if (gChip == nullptr) {
        VCU_LOG("[EEPROM] test: no chip\n");
        return;
    }
    constexpr size_t N = 16;
    const uint32_t address = gSize - N;
    uint8_t old[N];
    uint8_t pattern[N];
    uint8_t back[N];
    const uint8_t seed = (uint8_t)micros();
    for (size_t i = 0; i < N; i++) {
        pattern[i] = (uint8_t)(seed + i * 37U);
    }
    const bool readOld = eepromRead(address, old, N);
    const uint32_t t0 = micros();
    const bool written = eepromWrite(address, pattern, N);
    const uint32_t writeUs = micros() - t0;
    const bool readBack = eepromRead(address, back, N) && memcmp(back, pattern, N) == 0;
    const bool restored = eepromWrite(address, old, N);
    const bool ok = readOld && written && readBack && restored;
    VCU_LOG("[EEPROM] test %s: addr 0x%04lX, %u bytes, write+verify %lu us, read back %s, restored %s\n",
            ok ? "PASS" : "FAIL", (unsigned long)address, (unsigned)N, (unsigned long)writeUs,
            readBack ? "ok" : "MISMATCH", restored ? "ok" : "FAILED");
}
