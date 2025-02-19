/**************************************************************************
 *  Includes and Definitions
 **************************************************************************/
#include <Arduino.h>
#include "USB.h"
#include "USBCDC.h"
#include <TaskScheduler.h>

// Define GVRET_PORT and MONITOR_PORT.
// GVRET communication uses the primary Serial port.
// Monitoring/debug output uses USBSerial1.
#define GVRET_PORT Serial
#define MONITOR_PORT USBSerial1

// USB Serial Setup: Use a clear name for the USB CDC object.
USBCDC USBSerial1(0);  // First virtual serial port

#include "gvret.h"
#include "canmanager.h" 

// RGB LED Config (ESP32-S3 Built-in)
//#define RGB_BUILTIN    48   // Built-in LED pin on ESP32-S3
//#define RGB_BRIGHTNESS 0    // 0 for OFF

/**************************************************************************
 *  Global Variables and Objects
 **************************************************************************/
float brake_pedal_position         = 0.0;  // Scale factor: 0.39216
float accelerator_pedal_percentage = 0.0;  // Scale factor: 0.4
char gear_selection                = ' ';  // Default empty

Scheduler runner;

/**************************************************************************
 *  Function Prototypes
 **************************************************************************/
void pollCAN();
void blinkLED();
void printCANData();

/**************************************************************************
 *  Task Definitions
 **************************************************************************/
Task taskBlinkLED  (500, TASK_FOREVER, &blinkLED, &runner, true);
Task taskPrintStatus(1000, TASK_FOREVER, &printCANData, &runner, true);

//---------------------------------------------------------------------------
// Blacklist Array: Uncomment an ID to block it from being forwarded.
// If the ID is commented out, it is allowed to be forwarded.
//---------------------------------------------------------------------------

static const uint32_t BLACKLISTED_CAN_IDS[] = {
    // 0x101,   // Blacklist this ID
    0x119, // Allow: ID 0x119
    0x149, // Allow: ID 0x149
    0x156, // Allow: ID 0x156
    0x200, // Allow: Wheel front
    0x208, // Allow: Wheel back + brake pedal
    0x210, // Allow: Accelerator pedal
    0x212, // Allow: Relation with voltage/current (traction battery)
    0x215, // Allow: Actual speed and distance travelled
    0x231, // Allow: Brake pedal switch
    0x236, // Allow: ID 0x236
    0x285, // Allow: ID 0x285
    0x286, // Allow: ID 0x286
    // 0x288, // Allow: Motor message
    // 0x298, // Allow: Motor message
    // 0x29A, // Allow: Motor message
    0x2F2, // Allow: ID 0x2F2
    0x300, // Allow: ID 0x300
    0x308, // Allow: ID 0x308
    0x325, // Allow: ID 0x325
    0x346, // Allow: ID 0x346
    0x373, // Allow: ID 0x373
    0x374, // Allow: ID 0x374
    0x375, // Allow: ID 0x375
    0x384, // Allow: ID 0x384
    0x385, // Allow: ID 0x385
    0x3A4, // Allow: ID 0x3A4
    0x408, // Allow: ID 0x408
    0x412, // Allow: ID 0x412
    0x418, // Allow: Gear shift selection
    0x424, // Allow: ID 0x424
    // 0x564, // Allow: Motor message
    // 0x565, // Allow: Motor message
    0x5A1, // Allow: ID 0x5A1
    // 0x695,   // Blacklist: Unknown message
    // 0x696,   // Blacklist: Motor current and regen amps
    0x697, // Allow: ID 0x697
    0x6D0, // Allow: ID 0x6D0
    0x6D1, // Allow: ID 0x6D1
    0x6D2, // Allow: ID 0x6D2
    0x6D3, // Allow: ID 0x6D3
    0x6D4, // Allow: ID 0x6D4
    0x6D5, // Allow: ID 0x6D5
    0x6D6, // Allow: ID 0x6D6
    0x6DA, // Allow: ID 0x6DA
    0x6E1, // Allow: ID 0x6E1
    0x6E2, // Allow: ID 0x6E2
    0x6E3, // Allow: ID 0x6E3
    0x6E4, // Allow: ID 0x6E4
    0x6FA, // Allow: ID 0x6FA
    //0x75A, // Allow: Motor message
    //0x75B  // Allow: Motor message
};

//---------------------------------------------------------------------------
// Helper Function: Returns true if the given CAN ID is blacklisted.
//---------------------------------------------------------------------------

bool isBlacklisted(uint32_t canId) {
    const size_t numIDs = sizeof(BLACKLISTED_CAN_IDS) / sizeof(BLACKLISTED_CAN_IDS[0]);
    for (size_t i = 0; i < numIDs; i++) {
        if (BLACKLISTED_CAN_IDS[i] == canId) {
            return true;
        }
    }
    return false;
}

/**************************************************************************
 *  CAN Bus and Task Functions
 **************************************************************************/
void pollCAN() {
    CANMessage frame;
    
    // -------------------------------------------------------------
    // Handle messages from CAN1
    // -------------------------------------------------------------
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
    
   // -------------------------------------------------------------
    // Handle messages from CAN2
    // -------------------------------------------------------------
    if (can2.available()) {
        can2.receive(frame);
        
        // Always forward to USB (for logging / GVRET).
        sendFrameToUSB(frame, 1);
        
        // Only forward to CAN1 if the ID is NOT blacklisted.
        if (!isBlacklisted(frame.id)) {
            can.tryToSend(frame);
            sendFrameToUSB(frame, 2);
        } else {
            // Optionally, log that this ID was blocked.
            // MONITOR_PORT.println("Dropping blacklisted CAN id: 0x" + String(frame.id, HEX));
        }
    }
}


// Task Function: Toggle the built-in LED.
void blinkLED() {
    digitalWrite(RGB_BUILTIN, !digitalRead(RGB_BUILTIN));
}

// Task Function: Print interpreted CAN data to the monitor port.
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
    // Initialize the primary Serial port for GVRET communication.
    Serial.begin(1000000);
    
    // Initialize USB CDC for monitoring/debug output.
    USBSerial1.begin();
    USB.begin();
    
    // Configure the built-in RGB LED.
    pinMode(RGB_BUILTIN, OUTPUT);
    digitalWrite(RGB_BUILTIN, LOW);
    
    // Initialize the CAN buses using the CAN manager.
    canManager_setup();
    
    // Optionally, print a startup message.
    MONITOR_PORT.println("System Initialized. Starting tasks...");
}

void loop() {
    runner.execute();
    pollCAN();
    gvret_loop();
}
