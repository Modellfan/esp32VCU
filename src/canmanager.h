#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <SPI.h>
#include <ACAN2515.h>
#include <Arduino.h>

// Pin definitions for CAN1
constexpr byte MCP2515_SCK   = 12;
constexpr byte MCP2515_MOSI  = 11;
constexpr byte MCP2515_MISO  = 13;
constexpr byte MCP2515_CS    = 10;
constexpr byte MCP2515_INT   = 9;

// Pin definitions for CAN2
constexpr byte MCP2515_CS_CAN2  = 7;
constexpr byte MCP2515_INT_CAN2 = 8;

// Other settings
constexpr uint32_t SPI_CLOCK        = 10 * 1000 * 1000;
constexpr uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;

// CAN bus objects
ACAN2515 can(MCP2515_CS, SPI, MCP2515_INT);
ACAN2515 can2(MCP2515_CS_CAN2, SPI, MCP2515_INT_CAN2);

// Setup function for CAN manager, which also initializes SPI.
void canManager_setup() {
    // Initialize SPI interface
    SPI.setFrequency(SPI_CLOCK);
    SPI.begin(MCP2515_SCK, MCP2515_MISO, MCP2515_MOSI);

    // Configure ACAN2515 for CAN1.
    GVRET_PORT.println("Configuring ACAN2515 CAN 1...");
    ACAN2515Settings settings(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode = can.begin(settings, []{ can.isr(); });
    if (errorCode == 0)
        GVRET_PORT.println("CAN Initialized Successfully!");
    else {
        GVRET_PORT.print("Configuration error: 0x");
        GVRET_PORT.println(errorCode, HEX);
    }
    
    // Configure ACAN2515 for CAN2.
    GVRET_PORT.println("Configuring ACAN2515 CAN2...");
    ACAN2515Settings settings2(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings2.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode2 = can2.begin(settings2, []{ can2.isr(); });
    if (errorCode2 == 0)
        GVRET_PORT.println("CAN Initialized Successfully!");
    else {
        GVRET_PORT.print("Configuration error: 0x");
        GVRET_PORT.println(errorCode2, HEX);
    }
}

#endif // CANMANAGER_H
