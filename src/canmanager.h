#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <SPI.h>
#include <ACAN2515.h>
#include <Arduino.h>

// Pin definitions for CAN1 Motor
inline constexpr byte MCP2515_SCK   = 12;
inline constexpr byte MCP2515_MOSI  = 11;
inline constexpr byte MCP2515_MISO  = 13;
inline constexpr byte MCP2515_CS    = 7; //5; //10;
inline constexpr byte MCP2515_INT   = 8; //6; //9;

// Pin definitions for CAN2 Vehicle
inline constexpr byte MCP2515_CS_CAN2  = 5; //7;
inline constexpr byte MCP2515_INT_CAN2 = 6; //8;

// Other settings
inline constexpr uint32_t SPI_CLOCK        = 10 * 1000 * 1000;
inline constexpr uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;

// CAN bus objects
inline ACAN2515 can_motor(MCP2515_CS, SPI, MCP2515_INT);
inline ACAN2515 can_vehicle(MCP2515_CS_CAN2, SPI, MCP2515_INT_CAN2);

// Setup function for CAN manager, which also initializes SPI.
inline void canManager_setup() {
    // Initialize SPI interface
    SPI.setFrequency(SPI_CLOCK);
    SPI.begin(MCP2515_SCK, MCP2515_MISO, MCP2515_MOSI);

    // Configure ACAN2515 for CAN1.
    MONITOR_PORT.println("Configuring ACAN2515 CAN 1...");
    ACAN2515Settings settings(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode = can_motor.begin(settings, []{ can_motor.isr(); });
    if (errorCode == 0)
        MONITOR_PORT.println("CAN Initialized Successfully!");
    else {
        MONITOR_PORT.print("Configuration error: 0x");
        MONITOR_PORT.println(errorCode, HEX);
    }
    
    // Configure ACAN2515 for can_vehicle.
    MONITOR_PORT.println("Configuring ACAN2515 can_vehicle...");
    ACAN2515Settings settings2(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings2.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode2 = can_vehicle.begin(settings2, []{ can_vehicle.isr(); });
    if (errorCode2 == 0)
        MONITOR_PORT.println("CAN Initialized Successfully!");
    else {
        MONITOR_PORT.print("Configuration error: 0x");
        MONITOR_PORT.println(errorCode2, HEX);
    }
}

#endif // CANMANAGER_H
