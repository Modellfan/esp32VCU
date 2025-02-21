/**************************************************************************
 *  Includes and Definitions
 **************************************************************************/
#include <Arduino.h>
#include "USB.h"
#include "USBCDC.h"
#include <TaskScheduler.h>
#include <ESP32Servo.h>

// Define GVRET_PORT and MONITOR_PORT.
// GVRET communication uses the primary Serial port.
// Monitoring/debug output uses USBSerial1.
#define GVRET_PORT Serial
#define MONITOR_PORT USBSerial1

// USB Serial Setup: Use a clear name for the USB CDC object.
USBCDC USBSerial1(0); // First virtual serial port

#include "gvret.h"
#include "canmanager.h"

// RGB LED Config (ESP32-S3 Built-in)
// #define RGB_BUILTIN    48   // Built-in LED pin on ESP32-S3
// #define RGB_BRIGHTNESS 0    // 0 for OFF

Servo brake_servo;
const int servoPin = 17;
int pos = 0;

// ——————————————————————————————————————————————————————————————————————————————
//   Joystick Config (Analog Inputs)
// ——————————————————————————————————————————————————————————————————————————————
#define JOYSTICK_X_PIN 4 // GPIO 4 for X-axis
#define JOYSTICK_Y_PIN 5 // GPIO 5 for Y-axis

// Raw joystick values
int rawJoystickX = 0;
int rawJoystickY = 0;

// Processed joystick values (-100% to 100%)
float processedJoystickX = 0.0;
float processedJoystickY = 0.0;

// Calibration values (from your provided data)
const int joystickX_ZERO = 1993; // X-axis center value
const int joystickY_ZERO = 2005; // Y-axis center value

const int JOYSTICK_MIN = 0;                  // Minimum raw ADC value
const int JOYSTICK_MAX = 4095;               // Maximum raw ADC value
const int JOYSTICK_RANGE = JOYSTICK_MAX / 2; // Half range for -100% to 100%

/**************************************************************************
 *  Global Variables and Objects
 **************************************************************************/
float brake_pedal_position = 0.0;         // Scale factor: 0.39216
float accelerator_pedal_percentage = 0.0; // Scale factor: 0.4
char gear_selection = ' ';                // Default empty
int torque_request = 0;

byte torque_request_byte_0 = 0;
byte torque_request_byte_1 = 0;

Scheduler runner;

/**************************************************************************
 *  Function Prototypes
 **************************************************************************/
void pollCAN();
void blinkLED();
void printCANData();
void control_brake();

/**************************************************************************
 *  Task Definitions
 **************************************************************************/
Task taskBlinkLED(500, TASK_FOREVER, &blinkLED, &runner, true);
Task taskPrintStatus(1000, TASK_FOREVER, &printCANData, &runner, true);
Task taskBrakeControl(10, TASK_FOREVER, &control_brake, &runner, true);

//---------------------------------------------------------------------------
// Blacklist Array: Uncomment an ID to block it from being forwarded.
// If the ID is commented out, it is allowed to be forwarded.
//---------------------------------------------------------------------------
static const uint32_t BLACKLISTED_CAN_IDS[] = {
    0x100, // One Time Startup message - not cyclic
    0x110, // One Time Startup message - not cyclic
    0x111, // One Time Startup message - not cyclic
    0x101, // Blacklist this ID
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
    // 0x285, // Allow: acceleration -> triggers error in instrument cluster propoably reduction of torque by esp ?
    0x286, // Allow: -> only at start and end of the ride? maybe gear selection?
    // 0x288, // Allow: Motor message so not on this bus anyway
    // 0x298, // Allow: Motor message so not on this bus anyway
    // 0x29A, // Allow: Motor message so not on this bus anyway
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
    // 0x564, // Allow: Motor message so not on this bus anyway
    // 0x565, // Allow: Motor message so not on this bus anyway
    0x5A1, // Allow: ID 0x5A1
    0x695, // Blacklist: Unknown message
    0x696, // Blacklist: Motor current and regen amps
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
    // 0x75A, // Allow: Motor message
    // 0x75B  // Allow: Motor message
};

//---------------------------------------------------------------------------
// Helper Function: Returns true if the given CAN ID is blacklisted.
//---------------------------------------------------------------------------
bool isBlacklisted(uint32_t canId)
{
    const size_t numIDs = sizeof(BLACKLISTED_CAN_IDS) / sizeof(BLACKLISTED_CAN_IDS[0]);
    for (size_t i = 0; i < numIDs; i++)
    {
        if (BLACKLISTED_CAN_IDS[i] == canId)
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
// Manipulate messages here.
//---------------------------------------------------------------------------
void manipulate_0x285(const CANMessage &inFrame, CANMessage &outFrame)
{
    // 1) Copy all message metadata and bytes initially
    outFrame.id = inFrame.id;
    outFrame.len = inFrame.len;
    outFrame.data[0] = inFrame.data[0];
    outFrame.data[1] = inFrame.data[1];
    outFrame.data[2] = inFrame.data[2];
    outFrame.data[3] = inFrame.data[3];
    outFrame.data[4] = inFrame.data[4];
    outFrame.data[5] = inFrame.data[5];
    outFrame.data[6] = inFrame.data[6];
    outFrame.data[7] = inFrame.data[7];

    torque_request_byte_0 = inFrame.data[0];
    torque_request_byte_1 = inFrame.data[1];

    // 2) Interpret the first two bytes as big-endian unsigned 16-bit
    //    data[0] is MSB, data[1] is LSB
    uint16_t rawAcceleration = (uint16_t)((outFrame.data[0] << 8) | outFrame.data[1]);

    // 3) Convert to physical value using scale=1, bias=-2000
    //    physical = raw - 2000
    int16_t physicalAcceleration = (int16_t)rawAcceleration - 2000;

    // 4) Example manipulation: ensure the physical value is never negative
    // if (physicalAcceleration < 0) {
    //     physicalAcceleration = 0;
    // }
    physicalAcceleration = physicalAcceleration * 1.0;

    // 5) Convert back to raw: raw = physical + 2000
    rawAcceleration = (uint16_t)(physicalAcceleration + 2000);

    // 6) Store the manipulated raw value back in big-endian format
    outFrame.data[0] = (uint8_t)((rawAcceleration >> 8) & 0xFF); // MSB
    outFrame.data[1] = (uint8_t)(rawAcceleration & 0xFF);        // LSB
}

void manipulate_0x288(const CANMessage &inFrame, CANMessage &outFrame)
{
    // 1) Copy all message metadata and bytes initially
    outFrame.id = inFrame.id;
    outFrame.len = inFrame.len;
    outFrame.data[0] = torque_request_byte_0; // Maybe the ecu notices the manipulation of troque request, because those two bytes dows not reflect the actual expected torque
    outFrame.data[1] = torque_request_byte_1;
    outFrame.data[2] = inFrame.data[2];
    outFrame.data[3] = inFrame.data[3];
    outFrame.data[4] = inFrame.data[4];
    outFrame.data[5] = inFrame.data[5];
    outFrame.data[6] = inFrame.data[6];
    outFrame.data[7] = inFrame.data[7];
}

/**************************************************************************
 *  CAN Bus and Task Functions
 **************************************************************************/
void pollCAN()
{
    CANMessage frame;

    // -------------------------------------------------------------
    // Handle messages from CAN1
    // -------------------------------------------------------------
    if (can.available()) // CAN 0 in savvycan is motor can
    {
        can.receive(frame);
        // Check if this message needs manipulation.
        if (frame.id == 0x288) // Motor Control functions
        {
            CANMessage outFrame;
            manipulate_0x288(frame, outFrame);
            can2.tryToSend(outFrame);
            sendFrameToUSB(outFrame, 0);
        }
        else
        {
            // For all other not-blacklisted IDs, forward as is.
            can2.tryToSend(frame);
            sendFrameToUSB(frame, 0);
        }
    }

    // -------------------------------------------------------------
    // Handle messages from CAN2
    // -------------------------------------------------------------
    if (can2.available())
    {
        can2.receive(frame);

        // Always forward to USB (for logging / GVRET).
        sendFrameToUSB(frame, 1);

        // Only process messages that are not blacklisted.
        if (!isBlacklisted(frame.id))
        {
            // Check if this message needs manipulation.
            if (frame.id == 0x285) // Motor Control functions
            {
                CANMessage outFrame;
                manipulate_0x285(frame, outFrame);
                can.tryToSend(outFrame);
                sendFrameToUSB(outFrame, 2);
            }
            else
            {
                // For all other not-blacklisted IDs, forward as is.
                can.tryToSend(frame);
                sendFrameToUSB(frame, 2);
            }
        }
        else
        {
            // Optionally log that the message was blacklisted/dropped.
            // MONITOR_PORT.println("Dropping blacklisted CAN id: 0x" + String(frame.id, HEX));
        }
    }
}

// Task Function: Toggle the built-in LED.
void blinkLED()
{
    digitalWrite(RGB_BUILTIN, !digitalRead(RGB_BUILTIN));
}

// ——————————————————————————————————————————————————————————————————————————————
//   Task: Poll Joystick (Every 10ms)
// ——————————————————————————————————————————————————————————————————————————————
void pollJoystick()
{
    rawJoystickX = analogRead(JOYSTICK_X_PIN); // Read X-axis
    rawJoystickY = analogRead(JOYSTICK_Y_PIN); // Read Y-axis

    // Convert to -100% to 100% range
    processedJoystickX = ((rawJoystickX - joystickX_ZERO) / (float)JOYSTICK_RANGE) * 100.0;
    processedJoystickY = ((rawJoystickY - joystickY_ZERO) / (float)JOYSTICK_RANGE) * 100.0;

    // Constrain values to -100% to 100%
    processedJoystickX = constrain(processedJoystickX, -100, 100);
    processedJoystickY = constrain(processedJoystickY, -100, 100);
}

// Task Function: Print interpreted CAN data to the monitor port.
void printCANData()
{
    MONITOR_PORT.print("Brake Pedal: ");
    MONITOR_PORT.print(brake_pedal_position, 2);
    MONITOR_PORT.print("Torque Request: ");
    MONITOR_PORT.print(torque_request);
    MONITOR_PORT.print(" | Accelerator: ");
    MONITOR_PORT.print(accelerator_pedal_percentage, 2);
    MONITOR_PORT.print(" | Gear: ");
    MONITOR_PORT.println(gear_selection);
}

void control_brake()
{
    pollJoystick();
    brake_servo.write(processedJoystickY * 1.8);
}

/**************************************************************************
 *  Setup and Loop
 **************************************************************************/
void setup()
{
    // Initialize the primary Serial port for GVRET communication.
    Serial.begin(1000000);

    // Initialize USB CDC for monitoring/debug output.
    USBSerial1.begin();
    USB.begin();

    // Initialize Servo
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    brake_servo.setPeriodHertz(50);           // standard 50 hz servo
    brake_servo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object

    // Configure the built-in RGB LED.
    pinMode(RGB_BUILTIN, OUTPUT);
    digitalWrite(RGB_BUILTIN, LOW);

    // Initialize the CAN buses using the CAN manager.
    canManager_setup();

    // Optionally, print a startup message.
    MONITOR_PORT.println("System Initialized. Starting tasks...");
}

void loop()
{
    runner.execute();
    pollCAN();
    gvret_loop();
}
