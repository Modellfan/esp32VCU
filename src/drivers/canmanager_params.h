// CAN manager parameters (params::can): basic software settings of the CAN controllers and
// their live status. Own header without SPI/ACAN dependencies so the signal table and the
// SIL build can include it. Shown in /api/live and on the test page /can.
//
// Settings are read once at start-up (canManager_setup, ecuTask); the status block is
// written by loop() every error_refresh_ms under the params lock.
#ifndef CANMANAGER_PARAMS_H
#define CANMANAGER_PARAMS_H

#include <stdint.h>

constexpr uint8_t CAN_BUS_COUNT = 4;  // CAN0 (TWAI), CAN1..CAN3 (MCP2515)

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

#endif  // CANMANAGER_PARAMS_H
