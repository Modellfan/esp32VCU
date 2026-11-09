#include "can_rx_monitor.h"

namespace {

struct RxEntry {
    CanBus bus;
    uint32_t id;
    RxSource source;
};

constexpr RxEntry kRxTable[] = {
    {BUS_VEHICLE, 0x1A0, RX_VEH_1A0}, {BUS_VEHICLE, 0x2A8, RX_VEH_2A8}, {BUS_VEHICLE, 0x4A1, RX_VEH_4A1},
    {BUS_VEHICLE, 0x510, RX_VEH_510}, {BUS_VEHICLE, 0x520, RX_VEH_520}, {BUS_VEHICLE, 0x560, RX_VEH_560},
    {BUS_VEHICLE, 0x440, RX_VEH_440}, {BUS_VEHICLE, 0x0C0, RX_VEH_0C0},
    {BUS_DRIVE, 0x500, RX_SDU_500}, {BUS_DRIVE, 0x501, RX_SDU_501}, {BUS_DRIVE, 0x502, RX_SDU_502},
    {BUS_DRIVE, 0x503, RX_SDU_503}, {BUS_DRIVE, 0x504, RX_SDU_504}, {BUS_DRIVE, 0x505, RX_SDU_505},
    {BUS_DRIVE, 0x506, RX_SDU_506}, {BUS_DRIVE, 0x507, RX_SDU_507}, {BUS_DRIVE, 0x508, RX_SDU_508},
};

// 0 = never received. Written by the CAN task only; 32-bit stores are atomic on the S3.
volatile uint32_t gLastRxMs[RX_COUNT] = {0};

}  // namespace

void rxMark(CanBus bus, uint32_t id) {
    uint32_t now = millis();
    if (now == 0) {
        now = 1;
    }
    for (const RxEntry &entry : kRxTable) {
        if (entry.bus == bus && entry.id == id) {
            gLastRxMs[entry.source] = now;
            return;
        }
    }
}

bool rxFresh(RxSource source, uint32_t nowMs) {
    if (source == RX_NONE || source >= RX_COUNT) {
        return true;
    }
    const uint32_t last = gLastRxMs[source];
    return last != 0 && (uint32_t)(nowMs - last) <= RX_TIMEOUT_MS;
}

void rxSnapshot(uint32_t *lastRxMs) {
    for (int i = 0; i < RX_COUNT; i++) {
        lastRxMs[i] = gLastRxMs[i];
    }
}
