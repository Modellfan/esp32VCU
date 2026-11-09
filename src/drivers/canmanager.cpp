// Driver layer: CAN0 (TWAI) and two MCP2515 CAN controllers (CAN1, CAN2) on one SPI bus.
// Pins from board.h.
#include "canmanager.h"

#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>
#include <driver/twai.h>

#include "board.h"

namespace {
constexpr uint32_t SPI_CLOCK = 10UL * 1000UL * 1000UL;
constexpr uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;
constexpr uint32_t BITRATE = 500UL * 1000UL;

bool gCan0Active = false;
uint16_t gCan1Error = 0xFFFF;
uint16_t gCan2Error = 0xFFFF;

portMUX_TYPE gStatsMux = portMUX_INITIALIZER_UNLOCKED;
CanTaskStats gStats = {0, 0, 0, 0, 0};
CanBusDiag gDiag[3] = {};  // index = bus number, guarded by gStatsMux

constexpr uint8_t EFLG_TXBO = 0x20;  // MCP2515: transmitter bus-off
}  // namespace

ACAN2515 can1(board::GPIO_CAN1_CS, SPI, board::GPIO_CAN1_INT);
ACAN2515 can2(board::GPIO_CAN2_CS, SPI, board::GPIO_CAN2_INT);

void canManager_setup() {
    SPI.setFrequency(SPI_CLOCK);
    SPI.begin(board::GPIO_SPI_SCK, board::GPIO_SPI_MISO, board::GPIO_SPI_MOSI);

    ACAN2515Settings settings(QUARTZ_FREQUENCY, BITRATE);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    gCan1Error = can1.begin(settings, [] { can1.isr(); });
    VCU_LOG("[CAN] CAN1 (vehicle) init %s (0x%X)\n", gCan1Error == 0 ? "ok" : "FAILED", gCan1Error);

    ACAN2515Settings settings2(QUARTZ_FREQUENCY, BITRATE);
    settings2.mRequestedMode = ACAN2515Settings::NormalMode;
    gCan2Error = can2.begin(settings2, [] { can2.isr(); });
    VCU_LOG("[CAN] CAN2 (drive) init %s (0x%X)\n", gCan2Error == 0 ? "ok" : "FAILED", gCan2Error);

    if (board::CAN0_ENABLED) {
        // tx queue 8, rx queue 32 frames
        gCan0Active = ESP32Can.begin(ESP32Can.convertSpeed(500), board::GPIO_CAN0_TX, board::GPIO_CAN0_RX, 8, 32);
        VCU_LOG("[CAN] CAN0 (battery, TWAI) init %s\n", gCan0Active ? "ok" : "FAILED");
    }
}

bool can0Active() {
    return gCan0Active;
}

bool can0Receive(CANMessage &frame) {
    if (!gCan0Active) {
        return false;
    }
    CanFrame message;
    if (!ESP32Can.readFrame(message, 0)) {
        return false;
    }
    frame.id = message.identifier;
    frame.ext = message.extd != 0;
    frame.rtr = message.rtr != 0;
    frame.len = message.data_length_code > 8 ? 8 : message.data_length_code;
    for (uint8_t i = 0; i < frame.len; i++) {
        frame.data[i] = message.data[i];
    }
    return true;
}

uint16_t canManager_error(uint8_t bus) {
    return bus == 1 ? gCan1Error : gCan2Error;
}

void canTaskStatsRecord(uint32_t nowMs, uint32_t cycleUs, bool sent) {
    portENTER_CRITICAL(&gStatsMux);
    if (gStats.cycles == 0) {
        gStats.started_ms = nowMs;
    }
    gStats.cycles++;
    if (!sent) {
        gStats.tx_failed++;
    }
    if (cycleUs > gStats.max_cycle_us) {
        gStats.max_cycle_us = cycleUs;
    }
    if (cycleUs > CAN_TASK_PERIOD_MS * 1000U) {
        gStats.overruns++;
    }
    portEXIT_CRITICAL(&gStatsMux);
}

CanTaskStats canTaskStats() {
    portENTER_CRITICAL(&gStatsMux);
    const CanTaskStats copy = gStats;
    portEXIT_CRITICAL(&gStatsMux);
    return copy;
}

void canCountRx(uint8_t bus) {
    if (bus > 2) {
        return;
    }
    portENTER_CRITICAL(&gStatsMux);
    gDiag[bus].rx_frames++;
    portEXIT_CRITICAL(&gStatsMux);
}

void canRefreshErrors() {
    CanBusDiag d[3];
    portENTER_CRITICAL(&gStatsMux);
    for (int i = 0; i < 3; i++) {
        d[i] = gDiag[i];
    }
    portEXIT_CRITICAL(&gStatsMux);

    // MCP2515 registers over SPI (Arduino SPI transactions are locked).
    if (gCan1Error == 0) {
        d[1].tec = can1.transmitErrorCounter();
        d[1].rec = can1.receiveErrorCounter();
        d[1].eflg = can1.errorFlagRegister();
        d[1].bus_off = (d[1].eflg & EFLG_TXBO) != 0;
    }
    if (gCan2Error == 0) {
        d[2].tec = can2.transmitErrorCounter();
        d[2].rec = can2.receiveErrorCounter();
        d[2].eflg = can2.errorFlagRegister();
        d[2].bus_off = (d[2].eflg & EFLG_TXBO) != 0;
    }
    if (gCan0Active) {
        twai_status_info_t status;
        if (twai_get_status_info(&status) == ESP_OK) {
            d[0].tec = (uint8_t)(status.tx_error_counter > 255 ? 255 : status.tx_error_counter);
            d[0].rec = (uint8_t)(status.rx_error_counter > 255 ? 255 : status.rx_error_counter);
            d[0].bus_off = status.state == TWAI_STATE_BUS_OFF;
        }
    }

    portENTER_CRITICAL(&gStatsMux);
    for (int i = 0; i < 3; i++) {
        d[i].rx_frames = gDiag[i].rx_frames;  // keep counts that arrived meanwhile
        gDiag[i] = d[i];
    }
    portEXIT_CRITICAL(&gStatsMux);
}

CanBusDiag canBusDiag(uint8_t bus) {
    CanBusDiag copy = {};
    if (bus > 2) {
        return copy;
    }
    portENTER_CRITICAL(&gStatsMux);
    copy = gDiag[bus];
    portEXIT_CRITICAL(&gStatsMux);
    return copy;
}

void canManagerLogStatus() {
    const CanTaskStats t = canTaskStats();
    char line[320];
    int n = snprintf(line, sizeof line, "[DIAG] t=%lu ms cyc=%lu ovr=%lu max=%lu us txfail=%lu",
                     (unsigned long)millis(), (unsigned long)t.cycles, (unsigned long)t.overruns,
                     (unsigned long)t.max_cycle_us, (unsigned long)t.tx_failed);
    for (uint8_t bus = 0; bus < 3 && n > 0 && n < (int)sizeof line; bus++) {
        const CanBusDiag d = canBusDiag(bus);
        const bool up = bus == 0 ? gCan0Active : canManager_error(bus) == 0;
        n += snprintf(line + n, sizeof line - n, " | CAN%u %s rx=%lu tec=%u rec=%u eflg=0x%02X%s", bus,
                      up ? "up" : "DOWN", (unsigned long)d.rx_frames, d.tec, d.rec, d.eflg,
                      d.bus_off ? " BUS-OFF" : "");
    }
    VCU_LOG("%s\n", line);
}
