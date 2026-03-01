#include "canmanager.h"

#include <Arduino.h>

namespace {
constexpr byte MCP2515_SCK = 12;
constexpr byte MCP2515_MOSI = 11;
constexpr byte MCP2515_MISO = 13;
constexpr byte MCP2515_CS = 10;
constexpr byte MCP2515_INT = 9;

constexpr byte MCP2515_CS_CAN2 = 7;
constexpr byte MCP2515_INT_CAN2 = 8;

constexpr uint32_t SPI_CLOCK = 10UL * 1000UL * 1000UL;
constexpr uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;
} // namespace

ACAN2515 can(MCP2515_CS, SPI, MCP2515_INT);
ACAN2515 can2(MCP2515_CS_CAN2, SPI, MCP2515_INT_CAN2);

void canManager_setup() {
    SPI.setFrequency(SPI_CLOCK);
    SPI.begin(MCP2515_SCK, MCP2515_MISO, MCP2515_MOSI);

    Serial.println("Configuring ACAN2515 CAN 1...");
    ACAN2515Settings settings(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode = can.begin(settings, [] { can.isr(); });
    if (errorCode == 0) {
        Serial.println("CAN Initialized Successfully!");
    } else {
        Serial.print("Configuration error: 0x");
        Serial.println(errorCode, HEX);
    }

    Serial.println("Configuring ACAN2515 CAN2...");
    ACAN2515Settings settings2(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings2.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode2 = can2.begin(settings2, [] { can2.isr(); });
    if (errorCode2 == 0) {
        Serial.println("CAN Initialized Successfully!");
    } else {
        Serial.print("Configuration error: 0x");
        Serial.println(errorCode2, HEX);
    }
}
