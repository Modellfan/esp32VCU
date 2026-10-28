// Driver layer: two MCP2515 CAN controllers on one SPI bus. Pins from board.h.
#include "canmanager.h"

#include <Arduino.h>

#include "board.h"

namespace {
constexpr uint32_t SPI_CLOCK = 10UL * 1000UL * 1000UL;
constexpr uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;
constexpr uint32_t BITRATE = 500UL * 1000UL;

uint16_t gCan1Error = 0xFFFF;
uint16_t gCan2Error = 0xFFFF;
}  // namespace

ACAN2515 can(board::GPIO_CAN1_CS, SPI, board::GPIO_CAN1_INT);
ACAN2515 can2(board::GPIO_CAN2_CS, SPI, board::GPIO_CAN2_INT);

void canManager_setup() {
    SPI.setFrequency(SPI_CLOCK);
    SPI.begin(board::GPIO_SPI_SCK, board::GPIO_SPI_MISO, board::GPIO_SPI_MOSI);

    ACAN2515Settings settings(QUARTZ_FREQUENCY, BITRATE);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    gCan1Error = can.begin(settings, [] { can.isr(); });
    VCU_LOG("[CAN] CAN1 (vehicle) init %s (0x%X)\n", gCan1Error == 0 ? "ok" : "FAILED", gCan1Error);

    ACAN2515Settings settings2(QUARTZ_FREQUENCY, BITRATE);
    settings2.mRequestedMode = ACAN2515Settings::NormalMode;
    gCan2Error = can2.begin(settings2, [] { can2.isr(); });
    VCU_LOG("[CAN] CAN2 (drive) init %s (0x%X)\n", gCan2Error == 0 ? "ok" : "FAILED", gCan2Error);
}

uint16_t canManager_error(uint8_t bus) {
    return bus == 1 ? gCan1Error : gCan2Error;
}
