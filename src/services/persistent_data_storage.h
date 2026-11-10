// Persistent data storage, taken over from TeensyVCU (src/persistent_data_storage.h): records
// with magic, version, sequence number and FNV-1a checksum in 16 slots used in turn (wear
// levelling). The newest valid slot wins at start-up; a damaged or half-written slot is
// skipped. Backend: AT24C EEPROM (eeprom_at24c.h) instead of the Teensy EEPROM.
#ifndef PERSISTENT_DATA_STORAGE_H
#define PERSISTENT_DATA_STORAGE_H

#include <Arduino.h>

#include "eeprom_at24c.h"

class PersistentDataStorage {
public:
    // Stored values (VCU-REQ-020). Change kVersion when the layout changes: old records are
    // then ignored and the defaults apply. No padding bytes (compared with memcmp, covered
    // by the checksum): floats first, the reserved bytes fill up to 16.
    struct PersistentData {
        float acceleration_0_50_best_s = 0.0f;
        float acceleration_0_100_best_s = 0.0f;
        uint8_t target_charge_pct = 80;
        uint8_t ac_current_limit_a = 16;
        uint8_t fan_mode = 0;          // params::FAN_MODE_*
        uint8_t fan_level_manual = 0;
        uint8_t acceleration_0_50_valid = 0;
        uint8_t acceleration_0_100_valid = 0;
        uint8_t reserved[2] = {0, 0};
    };
    static_assert(sizeof(PersistentData) == 16, "PersistentData must not contain padding");

    // false when no valid record was found (defaults are used).
    bool begin() {
        if (!initialized) {
            initialized = true;
            scan_slots();
        }
        return has_valid_data;
    }

    PersistentData load() {
        begin();
        return cached_data;
    }

    bool save(const PersistentData &data) {
        begin();
        const uint32_t next_sequence = has_valid_data ? (last_sequence + 1U) : 1U;
        const size_t next_slot = has_valid_data ? ((last_slot + 1U) % kSlotCount) : 0U;
        if (!write_slot(next_slot, next_sequence, data)) {
            return false;
        }
        cached_data = data;
        has_valid_data = true;
        last_sequence = next_sequence;
        last_slot = next_slot;
        return true;
    }

    uint32_t sequence() const { return last_sequence; }
    size_t slot() const { return last_slot; }

private:
    struct RecordHeader {
        uint32_t magic;
        uint16_t version;
        uint16_t payload_size;
        uint32_t sequence;
        uint32_t crc;
    };

    static constexpr uint32_t kMagic = 0x45564355UL;  // 'EVCU'
    static constexpr uint16_t kVersion = 1U;
    static constexpr size_t kSlotCount = 16U;
    static constexpr uint32_t kBaseAddress = 0U;      // EEPROM start; 16 slots use 512 bytes

    bool initialized = false;
    bool has_valid_data = false;
    uint32_t last_sequence = 0U;
    size_t last_slot = kSlotCount - 1U;
    PersistentData cached_data;

    static constexpr size_t slot_size() { return sizeof(RecordHeader) + sizeof(PersistentData); }

    void scan_slots() {
        has_valid_data = false;
        last_sequence = 0U;
        last_slot = kSlotCount - 1U;
        cached_data = PersistentData{};
        for (size_t i = 0; i < kSlotCount; ++i) {
            RecordHeader header;
            PersistentData data;
            if (!read_slot(i, header, data)) {
                continue;
            }
            if (!has_valid_data || header.sequence > last_sequence) {
                has_valid_data = true;
                last_sequence = header.sequence;
                last_slot = i;
                cached_data = data;
            }
        }
    }

    static uint32_t compute_crc(const RecordHeader &header, const PersistentData &data) {
        uint32_t hash = 2166136261UL;
        auto update = [&hash](const uint8_t *bytes, size_t len) {
            for (size_t i = 0; i < len; ++i) {
                hash ^= bytes[i];
                hash *= 16777619UL;
            }
        };
        RecordHeader header_copy = header;
        header_copy.crc = 0U;
        update(reinterpret_cast<const uint8_t *>(&header_copy), sizeof(header_copy));
        update(reinterpret_cast<const uint8_t *>(&data), sizeof(data));
        return hash;
    }

    bool read_slot(size_t index, RecordHeader &header, PersistentData &data) const {
        const uint32_t base = kBaseAddress + index * slot_size();
        if (!eepromRead(base, &header, sizeof header) || header.magic != kMagic || header.version != kVersion ||
            header.payload_size != sizeof(PersistentData)) {
            return false;
        }
        if (!eepromRead(base + sizeof(RecordHeader), &data, sizeof data)) {
            return false;
        }
        return compute_crc(header, data) == header.crc;
    }

    bool write_slot(size_t index, uint32_t sequence, const PersistentData &data) {
        const uint32_t base = kBaseAddress + index * slot_size();
        RecordHeader header;
        header.magic = kMagic;
        header.version = kVersion;
        header.payload_size = sizeof(PersistentData);
        header.sequence = sequence;
        header.crc = 0U;
        header.crc = compute_crc(header, data);
        // Data first, header last: a power loss in between leaves an invalid slot, the
        // previous record stays the newest.
        return eepromWrite(base + sizeof(RecordHeader), &data, sizeof data) && eepromWrite(base, &header, sizeof header);
    }
};

#endif  // PERSISTENT_DATA_STORAGE_H
