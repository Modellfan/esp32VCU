// Receive monitor: last reception time per known CAN message (VCU-REQ-013, VCU-DEC-006).
// The CAN task marks every successfully decoded frame; the web API and the fan automatic
// ask whether a message is fresh.
#ifndef CAN_RX_MONITOR_H
#define CAN_RX_MONITOR_H

#include <Arduino.h>

enum CanBus : uint8_t { BUS_VEHICLE = 1, BUS_DRIVE = 2, BUS_BATTERY = 3 };

enum RxSource : uint8_t {
    RX_NONE = 0,  // value is produced by the VCU itself: always valid
    RX_VEH_1A0, RX_VEH_2A8, RX_VEH_4A1, RX_VEH_510, RX_VEH_520, RX_VEH_560, RX_VEH_440, RX_VEH_0C0,
    RX_SDU_500, RX_SDU_501, RX_SDU_502, RX_SDU_503, RX_SDU_504, RX_SDU_505, RX_SDU_506, RX_SDU_507, RX_SDU_508,
    RX_BATTERY,   // placeholder until the CAN 3 messages are defined
    RX_COUNT
};

// Timeout after which a value counts as stale. Draft: 500 ms for every message
// (cycle times of the vehicle and inverter messages are not documented yet).
constexpr uint32_t RX_TIMEOUT_MS = 500;

void rxMark(CanBus bus, uint32_t id);
bool rxFresh(RxSource source, uint32_t nowMs);
void rxSnapshot(uint32_t *lastRxMs);  // copies RX_COUNT entries

#endif  // CAN_RX_MONITOR_H
