// Driver layer: CAN0 (TWAI) and two MCP2515 CAN controllers (CAN1, CAN2) on one SPI bus.
// Pins from board.h.
#include "canmanager.h"

#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>

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
