
#ifndef ARDUINO_ARCH_ESP32
#error "Select an ESP32 board"
#endif

#include <ACAN2515.h>
#include <TaskScheduler.h>

// ——————————————————————————————————————————————————————————————————————————————
//  MCP2515 SPI and CAN Bus Config
// ——————————————————————————————————————————————————————————————————————————————

static const byte MCP2515_SCK = 12;
static const byte MCP2515_MOSI = 11;
static const byte MCP2515_MISO = 13;
static const byte MCP2515_CS = 10;
static const byte MCP2515_INT = 9;

static const byte MCP2515_CS_CAN2 = 7;
static const byte MCP2515_INT_CAN2 = 8;

static const uint32_t SPI_CLOCK = 10 * 1000 * 1000;
static const uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;

ACAN2515 can(MCP2515_CS, SPI, MCP2515_INT);
ACAN2515 can2(MCP2515_CS_CAN2, SPI, MCP2515_INT_CAN2);

// ——————————————————————————————————————————————————————————————————————————————
//  RGB LED Config (ESP32-S3 Built-in)
// ——————————————————————————————————————————————————————————————————————————————
#define RGB_BUILTIN 48   // Built-in RGB LED pin on ESP32-S3
#define RGB_BRIGHTNESS 0 // 0 for OFF, increase for brightness (0-255)

// ——————————————————————————————————————————————————————————————————————————————
//  Global Variables
// ——————————————————————————————————————————————————————————————————————————————

static uint32_t gReceivedFrameCount = 0;
static uint32_t gSentFrameCount = 0;

// ——————————————————————————————————————————————————————————————————————————————
//  TaskScheduler Setup
// ——————————————————————————————————————————————————————————————————————————————
Scheduler runner; // Create the task scheduler

// Function prototypes
void sendCANMessage();
void receiveCANMessage();
void blinkLED();
void printStatus(); // New function to print CAN data

// Define tasks
Task taskSendCAN(2000, TASK_FOREVER, &sendCANMessage, &runner, true); // Runs every 2s
Task taskBlinkLED(500, TASK_FOREVER, &blinkLED, &runner, true);       // Runs every 500ms
Task taskPrintStatus(1000, TASK_FOREVER, &printStatus, &runner, true); // Runs every 1s

// Global variables to store interpreted values
float brake_pedal_position = 0.0;  // Applying scale 0.39216
float accelerator_pedal_percentage = 0.0; // Applying scale 0.4
char gear_selection = ' '; // Default empty

// ——————————————————————————————————————————————————————————————————————————————
//  Task: Receive CAN Message
// ——————————————————————————————————————————————————————————————————————————————
void pollCAN()
{
    CANMessage frame;

    // Check if a CAN message is available
    if (can.available())
    {
        can.receive(frame);

        // Interpret messages based on their ID
        if (frame.id == 0x208) // Wheel Rotation, Brake Position
        {
            uint8_t raw_brake_value = frame.data[3];  // Read raw value
            brake_pedal_position = raw_brake_value * 0.39216;  // Apply scale factor
        }
        else if (frame.id == 0x210) // Accelerator Pedal Percentage
        {
            uint8_t raw_accel_value = frame.data[2];  // Read raw value
            accelerator_pedal_percentage = raw_accel_value * 0.4;  // Apply scale factor
        }
        else if (frame.id == 0x418) // Gear Shift Selection
        {
            // Interpret Gear Selection based on predefined value table
            switch (frame.data[0])
            {
                case 0x50: gear_selection = 'P'; break; // Park
                case 0x52: gear_selection = 'R'; break; // Reverse
                case 0x4E: gear_selection = 'N'; break; // Neutral
                case 0x44: gear_selection = 'D'; break; // Drive
                case 0x83: gear_selection = 'B'; break; // Brake/Low Mode
                case 0x32: gear_selection = 'C'; break; // Creep Mode
                default:   gear_selection = '?'; break; // Unknown
            }
        }

        //Man in the midle part. Inject manipulation here
        can2.tryToSend(frame);

    }

    if (can2.available())
    {
        //Man in the midle part. Inject manipulation here
        can.tryToSend(frame);

    }
}

// ——————————————————————————————————————————————————————————————————————————————
//  Task: Print CAN Data
// ——————————————————————————————————————————————————————————————————————————————
void printCANData()
{
    Serial.print("Brake Pedal: ");
    Serial.print(brake_pedal_position, 2); // Print with 2 decimal places
    Serial.print(" | Accelerator: ");
    Serial.print(accelerator_pedal_percentage, 2); // Print with 2 decimal places
    Serial.print(" | Gear: ");
    Serial.println(gear_selection);
}


// ——————————————————————————————————————————————————————————————————————————————
//  Setup Function
// ——————————————————————————————————————————————————————————————————————————————
void setup()
{
    Serial.begin(115200);

    // Configure built-in LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Start SPI
    SPI.setFrequency(SPI_CLOCK);
    SPI.begin(MCP2515_SCK, MCP2515_MISO, MCP2515_MOSI);

    // Configure ACAN2515
    Serial.println("Configuring ACAN2515 CAN 1...");
    ACAN2515Settings settings(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;

    const uint16_t errorCode = can.begin(settings, []
                                         { can.isr(); });

    if (errorCode == 0)
    {
        Serial.println("CAN Initialized Successfully!");
    }
    else
    {
        Serial.print("Configuration error: 0x");
        Serial.println(errorCode, HEX);
    }

    // Configure ACAN2515
    Serial.println("Configuring ACAN2515 CAN2...");
    ACAN2515Settings settings2(QUARTZ_FREQUENCY, 500UL * 1000UL);
    settings2.mRequestedMode = ACAN2515Settings::NormalMode;

    const uint16_t errorCode2 = can2.begin(settings2, []
                                           { can2.isr(); });

    if (errorCode2 == 0)
    {
        Serial.println("CAN Initialized Successfully!");
    }
    else
    {
        Serial.print("Configuration error: 0x");
        Serial.println(errorCode2, HEX);
    }
}

// ——————————————————————————————————————————————————————————————————————————————
//  Loop Function - Runs TaskScheduler
// ——————————————————————————————————————————————————————————————————————————————
void loop()
{
    runner.execute(); // Runs scheduled tasks
    pollCAN();
}

// ——————————————————————————————————————————————————————————————————————————————
//  Task: Send CAN Message
// ——————————————————————————————————————————————————————————————————————————————
void sendCANMessage()
{
    // can.poll(); // No interrupt, call this function as often as possible
    CANMessage frame;

    // Create a proper CAN message
    frame.id = 0x123; // Standard 11-bit ID (e.g., 0x123)
    frame.len = 8;    // 8-byte data length
    frame.data[0] = 0x11;
    frame.data[1] = 0x22;
    frame.data[2] = 0x33;
    frame.data[3] = 0x44;
    frame.data[4] = 0x55;
    frame.data[5] = 0x66;
    frame.data[6] = 0x77;
    frame.data[7] = 0x88;

    // Serial.print("CAN 1: ");
    // const bool ok = can.tryToSend(frame);
    // if (ok)
    // {
    //     Serial.print("Sent! - Buffer: ");
    //     Serial.println(can.transmitBufferCount(0));
    // }
    // else
    // {      
    //     Serial.print("Transmit error: ");
    //     Serial.println(can.errorFlagRegister(), BIN);
    // }

    // Serial.print("CAN 2: ");
    // const bool ok2 = can2.tryToSend(frame);
    // if (ok2)
    // {
    //     Serial.print("Sent! - Buffer: ");
    //     Serial.println(can2.transmitBufferCount(0));
    // }
    // else
    // {      
    //     Serial.print("Transmit error: ");
    //     Serial.println(can2.errorFlagRegister(), BIN);
    // }
}

// ——————————————————————————————————————————————————————————————————————————————
//  Task: Blink LED
// ——————————————————————————————————————————————————————————————————————————————
void blinkLED()
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

// ——————————————————————————————————————————————————————————————————————————————
//  Task: Print STatus
// ——————————————————————————————————————————————————————————————————————————————
void printStatus()
{
    Serial.print("Brake Pedal: ");
    Serial.print(brake_pedal_position);
    Serial.print(" | Accelerator: ");
    Serial.print(accelerator_pedal_percentage);
    Serial.print(" | Gear: ");
    Serial.println(gear_selection);
}