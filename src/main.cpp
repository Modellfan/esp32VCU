/**************************************************************************
 *  Includes and Definitions
 **************************************************************************/
#include <Arduino.h>
#include "USB.h"
#include "USBCDC.h"
#include <SPI.h>
#include <ACAN2515.h>
#include <TaskScheduler.h>

// Define GVRET_PORT and MONITOR_PORT.
// For GVRET, we use the primary Serial port.
// For monitoring/debug output, we use USBSerial1.
#define GVRET_PORT Serial //USB crashes as GVRET port
#define MONITOR_PORT USBSerial1

// USB Serial Setup: Use a clear name for the USB CDC object.
USBCDC USBSerial1(0); // First virtual serial port

// Include GVRET-related definitions and functions.
#include "gvret.h"

// MCP2515 SPI and CAN Bus Config
static const byte MCP2515_SCK   = 12;
static const byte MCP2515_MOSI  = 11;
static const byte MCP2515_MISO  = 13;
static const byte MCP2515_CS    = 10;
static const byte MCP2515_INT   = 9;
static const byte MCP2515_CS_CAN2  = 7;
static const byte MCP2515_INT_CAN2 = 8;
static const uint32_t SPI_CLOCK        = 10 * 1000 * 1000;
static const uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;

// RGB LED Config (ESP32-S3 Built-in)
#define RGB_BUILTIN    48   // Built-in LED pin on ESP32-S3
#define RGB_BRIGHTNESS 0    // 0 for OFF

/**************************************************************************
 *  Global Variables and Objects
 **************************************************************************/
// CAN Bus Objects
ACAN2515 can(MCP2515_CS, SPI, MCP2515_INT);
ACAN2515 can2(MCP2515_CS_CAN2, SPI, MCP2515_INT_CAN2);

// Global variables to store interpreted values from CAN messages
float brake_pedal_position         = 0.0;  // Scale factor: 0.39216
float accelerator_pedal_percentage = 0.0;  // Scale factor: 0.4
char gear_selection                = ' ';  // Default empty

// TaskScheduler Setup
Scheduler runner; // Task scheduler

/**************************************************************************
 *  Function Prototypes
 **************************************************************************/
// CAN Bus Functions
void pollCAN();

// Task Functions
void sendCANMessage(); // Placeholder (if needed)
void blinkLED();
void printCANData();

/**************************************************************************
 *  Task Definitions
 **************************************************************************/
Task taskSendCAN   (2000, TASK_FOREVER, &sendCANMessage, &runner, true);   // Every 2 sec
Task taskBlinkLED  (500, TASK_FOREVER, &blinkLED, &runner, true);          // Every 500 ms
Task taskPrintStatus(1000, TASK_FOREVER, &printCANData, &runner, true);      // Every 1 sec

/**************************************************************************
 *  CAN Bus Functions
 **************************************************************************/
void pollCAN() {
    CANMessage frame;
    
    // Check CAN1 for available messages.
    if (can.available()) {
        can.receive(frame);
        
        // Interpret messages based on their ID.
        if (frame.id == 0x208) {  // Wheel Rotation, Brake Position
            uint8_t raw_brake_value = frame.data[3];
            brake_pedal_position = raw_brake_value * 0.39216;
        } else if (frame.id == 0x210) {  // Accelerator Pedal Percentage
            uint8_t raw_accel_value = frame.data[2];
            accelerator_pedal_percentage = raw_accel_value * 0.4;
        } else if (frame.id == 0x418) {  // Gear Shift Selection
            switch (frame.data[0]) {
                case 0x50: gear_selection = 'P'; break;
                case 0x52: gear_selection = 'R'; break;
                case 0x4E: gear_selection = 'N'; break;
                case 0x44: gear_selection = 'D'; break;
                case 0x83: gear_selection = 'B'; break;
                case 0x32: gear_selection = 'C'; break;
                default:   gear_selection = '?'; break;
            }
        }
        
        // Forward the frame to USB via GVRET and re-transmit on CAN2.
        sendFrameToUSB(frame, 0);
        can2.tryToSend(frame);
    }
    
    // Check CAN2 for available messages.
    if (can2.available()) {
        can2.receive(frame);
        sendFrameToUSB(frame, 1);
        can.tryToSend(frame);
    }
}

/**************************************************************************
 *  Task Functions
 **************************************************************************/
void sendCANMessage() {
    // Implement if needed.
}

void blinkLED() {
    digitalWrite(RGB_BUILTIN, !digitalRead(RGB_BUILTIN));
}

void printCANData() {
    MONITOR_PORT.print("Brake Pedal: ");
    MONITOR_PORT.print(brake_pedal_position, 2);
    MONITOR_PORT.print(" | Accelerator: ");
    MONITOR_PORT.print(accelerator_pedal_percentage, 2);
    MONITOR_PORT.print(" | Gear: ");
    MONITOR_PORT.println(gear_selection);
}

/**************************************************************************
 *  Setup and Loop
 **************************************************************************/
void setup() {
    // Initialize Serial Monitor for debugging.
    Serial.begin(1000000);
    
    // Initialize USB CDC for GVRET communication.
    USBSerial1.begin();
    USB.begin();
    
    // Configure built-in LED.
    pinMode(RGB_BUILTIN, OUTPUT);
    digitalWrite(RGB_BUILTIN, LOW);
    
    // Initialize SPI.
    SPI.setFrequency(SPI_CLOCK);
    SPI.begin(MCP2515_SCK, MCP2515_MISO, MCP2515_MOSI);
    
    // Configure ACAN2515 for CAN1.
    MONITOR_PORT.println("Configuring ACAN2515 CAN 1...");
    ACAN2515Settings settings(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode = can.begin(settings, []{ can.isr(); });
    if (errorCode == 0)
        MONITOR_PORT.println("CAN Initialized Successfully!");
    else {
        MONITOR_PORT.print("Configuration error: 0x");
        MONITOR_PORT.println(errorCode, HEX);
    }
    
    // Configure ACAN2515 for CAN2.
    MONITOR_PORT.println("Configuring ACAN2515 CAN2...");
    ACAN2515Settings settings2(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings2.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode2 = can2.begin(settings2, []{ can2.isr(); });
    if (errorCode2 == 0)
        MONITOR_PORT.println("CAN Initialized Successfully!");
    else {
        MONITOR_PORT.print("Configuration error: 0x");
        MONITOR_PORT.println(errorCode2, HEX);
    }
}

void loop() {
    runner.execute(); // Run scheduled tasks.
    pollCAN();        // Process CAN messages and forward them via USB.
    gvret_loop();     // Process GVRET USB commands.
}
