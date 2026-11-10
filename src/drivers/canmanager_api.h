// Public interface of the CAN manager, free of hardware dependencies (also used by the
// signal table and the SIL build). The controllers themselves: canmanager.h.
//
//   Receive monitor (VCU-REQ-013, VCU-DEC-006): last reception time per known message. The
//   receive loop marks every decoded frame; the web API and the fan automatic ask whether a
//   value is fresh.
//   params::can: settings of the CAN controllers (read once at start-up) and their live status
//   (written by loop() every error_refresh_ms under the params lock); in /api/live and on /can.
#ifndef CANMANAGER_API_H
#define CANMANAGER_API_H

#include <Arduino.h>
#include <stdint.h>

constexpr uint8_t CAN_BUS_COUNT = 4;  // CAN0 (TWAI), CAN1..CAN3 (MCP2515)

// ---------------------------------------------------------------- receive monitor

enum CanBus : uint8_t { BUS_VEHICLE = 1, BUS_DRIVE = 2, BUS_BATTERY = 3 };

enum RxSource : uint8_t {
    RX_NONE = 0,  // value is produced by the VCU itself: always valid
    RX_VEH_1A0, RX_VEH_2A8, RX_VEH_4A1, RX_VEH_510, RX_VEH_520, RX_VEH_560, RX_VEH_440, RX_VEH_0C0,
    RX_SDU_500, RX_SDU_501, RX_SDU_502, RX_SDU_503, RX_SDU_504, RX_SDU_505, RX_SDU_506, RX_SDU_507, RX_SDU_508,
    RX_BATTERY,   // placeholder until the CAN0 messages are defined
    RX_COUNT
};

// Timeout after which a value counts as stale. Draft: 500 ms for every message
// (cycle times of the vehicle and inverter messages are not documented yet).
constexpr uint32_t RX_TIMEOUT_MS = 500;

void rxMark(CanBus bus, uint32_t id);
bool rxFresh(RxSource source, uint32_t nowMs);
void rxSnapshot(uint32_t *lastRxMs);  // copies RX_COUNT entries

// ---------------------------------------------------------------- params::can

namespace params {

struct CanBusStatus {
    bool up = false;        // controller initialised
    uint32_t error = 0;     // init result: 0 ok; ACAN2515 / ACAN_ESP32 error code
    uint32_t rx_frames = 0;
    uint8_t tec = 0;
    uint8_t rec = 0;
    uint8_t eflg = 0;       // MCP2515 EFLG; CAN0: TWAI status register
    bool bus_off = false;
};

struct CanManagerSignals {
    // Basic software settings
    uint32_t bitrate = 500000;                 // all buses, bit/s
    uint32_t spi_clock_hz = 10000000;          // shared SPI bus of the MCP2515
    uint32_t mcp2515_quartz_hz = 8000000;
    uint16_t mcp2515_rx_buffer = 32;           // driver receive buffer, frames
    uint16_t mcp2515_tx_buffer = 16;           // driver transmit buffer, frames
    uint16_t can0_rx_buffer = 64;              // TWAI driver receive buffer, frames
    uint16_t can0_tx_buffer = 8;
    uint8_t rx_budget_per_bus = 32;            // frames per bus and loop() pass
    uint8_t ecu_period_ms = 6;                 // ECU task: 5 frames x 6 ms = 30 ms (VCU-REQ-001)
    uint16_t error_refresh_ms = 300;           // error counters and this status block
    uint8_t ecu_task_priority = 23;            // configMAX_PRIORITIES - 2, core 1
    uint8_t web_task_priority = 1;             // WiFi/web/GVRET/log, core 0, lowest

    // Status
    CanBusStatus can0;
    CanBusStatus can1;
    CanBusStatus can2;
    CanBusStatus can3;
    uint32_t ecu_cycles = 0;
    uint32_t ecu_overruns = 0;                 // cycles longer than ecu_period_ms
    uint32_t ecu_max_cycle_us = 0;
    uint32_t ecu_tx_failed = 0;
    uint32_t ecu_started_ms = 0;
    uint16_t can0_rx_peak = 0;                 // TWAI driver receive buffer high-water mark
    bool can0_rx_overflow = false;
};

extern CanManagerSignals can;

}  // namespace params

#endif  // CANMANAGER_API_H
