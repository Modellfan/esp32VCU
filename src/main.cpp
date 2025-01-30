#include <ACAN2515.h>
#include <ACAN_ESP32.h>
#include <TaskScheduler.h>

// ——————————————————————————————————————————————————————————————————————————————
//   MCP2515 CAN1 (SPI-Based) Config
// ——————————————————————————————————————————————————————————————————————————————
static const byte MCP2515_SCK = 12;
static const byte MCP2515_MOSI = 11;
static const byte MCP2515_MISO = 13;
static const byte MCP2515_CS = 10;
static const byte MCP2515_INT = 9;

static const uint32_t SPI_CLOCK = 10 * 1000 * 1000;
static const uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;

// ——————————————————————————————————————————————————————————————————————————————
//   CAN0 (ESP32 Built-in CAN) Config
// ——————————————————————————————————————————————————————————————————————————————
#define CAN0_RX_PIN GPIO_NUM_20
#define CAN0_TX_PIN GPIO_NUM_21

// ——————————————————————————————————————————————————————————————————————————————
//   CAN Interface Objects
// ——————————————————————————————————————————————————————————————————————————————
#define CAN0 ACAN_ESP32 ::can                     // ESP32 Built-in CAN
ACAN2515 CAN1(MCP2515_CS, SPI, MCP2515_INT); // MCP2515 (SPI) CAN

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

// ——————————————————————————————————————————————————————————————————————————————
//   RGB LED Config (ESP32-S3 Built-in)
// ——————————————————————————————————————————————————————————————————————————————
#define LED_RGB 48
#define BLINK_INTERVAL 500 // Default LED blink interval (can change per mode)

// ——————————————————————————————————————————————————————————————————————————————
//   Define System Modes
// ——————————————————————————————————————————————————————————————————————————————
enum SystemMode
{
    INIT,
    STANDBY,
    ERROR,
    OPERATION
};

SystemMode currentMode = INIT; // Start in INIT mode

// ——————————————————————————————————————————————————————————————————————————————
//   Task Definitions
// ——————————————————————————————————————————————————————————————————————————————

// TaskScheduler object
Scheduler taskManager;

// Function prototypes for task callbacks
void sendCAN0Message();
void sendCAN1Message();
void blinkLED();
void pollCAN0();
void pollCAN1();
void pollJoystick();
void updateLEDColor(SystemMode mode);

// Define tasks
Task taskSendCAN0(2000, TASK_FOREVER, &sendCAN0Message, &taskManager, true); // Send CAN0 message every 2s
// Task taskSendCAN1(2000, TASK_FOREVER, &sendCAN1Message, &taskManager, true); // Send CAN1 message every 2s
Task taskBlinkLED(500, TASK_FOREVER, &blinkLED, &taskManager, true);        // Blink LED every 500ms
Task taskPollJoystick(10, TASK_FOREVER, &pollJoystick, &taskManager, true); // Poll joystick every 10ms

// ——————————————————————————————————————————————————————————————————————————————
//   Setup Function
// ——————————————————————————————————————————————————————————————————————————————
void setup()
{
    Serial.begin(115200);

    // Configure Joystick Pins
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);

    // Configure built-in RGB LED
    pinMode(LED_RGB, OUTPUT);
    updateLEDColor(INIT); // Set LED to INIT mode (Blue)

    bool can0Success = false;
    bool can1Success = false;

    // ——————————————————————————————————————————
    //   Initialize CAN0 (ESP32 Built-in CAN)
    // ——————————————————————————————————————————
    Serial.println("Configuring CAN0 (ESP32 Built-in CAN)...");
    ACAN_ESP32_Settings can0Settings(500UL * 1000UL);                 // 500kbps
    can0Settings.mRequestedCANMode = ACAN_ESP32_Settings::NormalMode; // Normal mode
    can0Settings.mRxPin = GPIO_NUM_20;                                // RX Pin (cast to gpio_num_t)
    can0Settings.mTxPin = GPIO_NUM_21;                                // TX Pin (cast to gpio_num_t)

    const uint32_t can0Error = CAN0.begin(can0Settings);
    if (can0Error == 0)
    {
        Serial.println("CAN0 Initialized Successfully!");
        can0Success = true;
    }
    else
    {
        Serial.print("CAN0 Initialization Error: 0x");
        Serial.println(can0Error, HEX);
    }

    // ——————————————————————————————————————————
    //   Initialize CAN1 (MCP2515 SPI CAN)
    // ——————————————————————————————————————————
    Serial.println("Configuring CAN1 (MCP2515 SPI CAN)...");
    ACAN2515Settings can1Settings(QUARTZ_FREQUENCY, 500UL * 1000UL);
    can1Settings.mRequestedMode = ACAN2515Settings::NormalMode;

    const uint16_t can1Error = CAN1.begin(can1Settings, []
                                          { CAN1.isr(); });
    if (can1Error == 0)
    {
        Serial.println("CAN1 Initialized Successfully!");
        can1Success = true;
    }
    else
    {
        Serial.print("CAN1 Initialization Error: 0x");
        Serial.println(can1Error, HEX);
    }

    // Switch to OPERATION mode if both CAN0 and CAN1 initialized successfully
    if (can0Success && can1Success)
    {
        currentMode = OPERATION;
    }
    else
    {
        currentMode = ERROR;
    }

    updateLEDColor(currentMode); // Update LED based on mode
}

// ——————————————————————————————————————————————————————————————————————————————
//   Loop Function - Polls CAN0 & CAN1 for Messages
// ——————————————————————————————————————————————————————————————————————————————
void loop()
{
    taskManager.execute(); // Runs scheduled tasks for sending messages
    pollCAN0();            // Manually poll CAN0 (ESP32)
    pollCAN1();            // Manually poll CAN1 (MCP2515)
}

// ——————————————————————————————————————————————————————————————————————————————
//  Poll CAN0 (ESP32 Built-in CAN)
// ——————————————————————————————————————————————————————————————————————————————
void pollCAN0()
{
    CANMessage frame;
    while (CAN0.receive(frame))
    {
        Serial.print("[CAN0] Received: ID=0x");
        Serial.print(frame.id, HEX);
        Serial.print(" Length=");
        Serial.print(frame.len);
        Serial.print(" Data=");

        for (uint8_t i = 0; i < frame.len; i++)
        {
            Serial.print(frame.data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}

// ——————————————————————————————————————————————————————————————————————————————
//   Poll CAN1 (MCP2515 CAN)
// ——————————————————————————————————————————————————————————————————————————————
void pollCAN1()
{
    CANMessage frame;
    if (CAN1.available())
    {
        CAN1.receive(frame);
        Serial.print("[CAN1] Received: ID=0x");
        Serial.print(frame.id, HEX);
        Serial.print(" Length=");
        Serial.print(frame.len);
        Serial.print(" Data=");

        for (uint8_t i = 0; i < frame.len; i++)
        {
            Serial.print(frame.data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}

// ——————————————————————————————————————————————————————————————————————————————
//   Task: Send CAN0 Message (ESP32 Built-in CAN)
// ——————————————————————————————————————————————————————————————————————————————
void sendCAN0Message()
{
    CANMessage frame;
    frame.id = 0x100;
    frame.len = 8;
    frame.data[0] = 0x11;
    frame.data[1] = 0x22;
    frame.data[2] = 0x33;
    frame.data[3] = 0x44;
    frame.data[4] = 0x55;
    frame.data[5] = 0x66;
    frame.data[6] = 0x77;
    frame.data[7] = 0x88;

    const bool ok = CAN0.tryToSend(frame);
    if (ok)
    {
        Serial.println("[CAN0] Message Sent Successfully!");
    }
    else
    {
        Serial.println("[CAN0] ERROR: Failed to send message!");

        // Get and print detailed error status using statusFlags()
        uint32_t canStatus = ACAN_ESP32::can.statusFlags();

        Serial.print("[CAN0] Status Flags: ");
        Serial.println(canStatus, BIN);

        if (canStatus & 0x01)
            Serial.println("[CAN0] ERROR: Hardware RX Buffer Overflow!");

        if (canStatus & 0x02)
            Serial.println("[CAN0] ERROR: Driver RX Buffer Overflow!");

        if (canStatus & 0x04)
            Serial.println("[CAN0] ERROR: CAN Controller is Bus-Off!");

        if (canStatus & 0x08)
            Serial.println("[CAN0] ERROR: CAN Controller is in Reset Mode!");

        // Switch to ERROR mode if any status flag is set
        if (canStatus != 0)
        {
            currentMode = ERROR;
            updateLEDColor(currentMode);
        }
    }
}

// ——————————————————————————————————————————————————————————————————————————————
//   Task: Send CAN1 Message (MCP2515 CAN)
// ——————————————————————————————————————————————————————————————————————————————
void sendCAN1Message()
{
    CANMessage frame;
    frame.id = 0x200;
    frame.len = 8;
    frame.data[0] = 0xAA;
    frame.data[1] = 0xBB;
    frame.data[2] = 0xCC;
    frame.data[3] = 0xDD;
    frame.data[4] = 0xEE;
    frame.data[5] = 0xFF;
    frame.data[6] = 0x00;
    frame.data[7] = 0x11;

    const bool ok = CAN1.tryToSend(frame);
    if (ok)
    {
        Serial.println("[CAN1] Message Sent Successfully!");
    }
    else
    {
        Serial.println("[CAN1] ERROR: Failed to send message!");

        // Get and print detailed error status
        Serial.print("[CAN1] Transmit Error Counter: ");
        Serial.println(CAN1.transmitErrorCounter());

        Serial.print("[CAN1] Receive Error Counter: ");
        Serial.println(CAN1.receiveErrorCounter());

        Serial.print("[CAN1] Error Flags: ");
        Serial.println(CAN1.errorFlagRegister(), BIN);

        // Switch to ERROR mode
        currentMode = ERROR;
        updateLEDColor(currentMode);
    }
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

    if (processedJoystickY > 80)
    {
        currentMode = ERROR;
        updateLEDColor(ERROR);
    }

    if (processedJoystickY < -80)
    {
        currentMode = STANDBY;
        updateLEDColor(STANDBY);
    }

    // Serial.print("Raw X: ");
    // Serial.print(rawJoystickX);
    // Serial.print(" | Raw Y: ");
    // Serial.print(rawJoystickY);
    // Serial.print(" || Processed X: ");
    // Serial.print(processedJoystickX, 2);
    // Serial.print("% | Processed Y: ");
    // Serial.print(processedJoystickY, 2);
    // Serial.println("%");
}

// ——————————————————————————————————————————————————————————————————————————————
//   Task: Blink LED Based on Mode
// ——————————————————————————————————————————————————————————————————————————————
void blinkLED()
{
    static bool ledState = false;
    ledState = !ledState;

    switch (currentMode)
    {
    case INIT:
        neopixelWrite(LED_RGB, ledState ? 0 : 0, ledState ? 0 : 0, ledState ? 255 : 0); // Blue
        break;
    case STANDBY:
        neopixelWrite(LED_RGB, ledState ? 255 : 0, ledState ? 255 : 0, ledState ? 0 : 0); // Yellow
        break;
    case ERROR:
        neopixelWrite(LED_RGB, ledState ? 255 : 0, ledState ? 0 : 0, ledState ? 0 : 0); // Red
        break;
    case OPERATION:
        neopixelWrite(LED_RGB, ledState ? 0 : 0, ledState ? 255 : 0, ledState ? 0 : 0); // Green
        break;
    }
}

// ——————————————————————————————————————————————————————————————————————————————
//   Function: Update LED Color Based on System Mode
// ——————————————————————————————————————————————————————————————————————————————
void updateLEDColor(SystemMode mode)
{
    switch (mode)
    {
    case INIT:
        neopixelWrite(LED_RGB, 0, 0, RGB_BRIGHTNESS); // Blue
        taskBlinkLED.setInterval(500);                // Blink every 500ms
        break;
    case STANDBY:
        neopixelWrite(LED_RGB, RGB_BRIGHTNESS, RGB_BRIGHTNESS, 0); // Yellow
        taskBlinkLED.setInterval(1000);                            // Blink every 1s
        break;
    case ERROR:
        neopixelWrite(LED_RGB, RGB_BRIGHTNESS, 0, 0); // Red
        taskBlinkLED.setInterval(250);                // Blink faster in error mode
        break;
    case OPERATION:
        neopixelWrite(LED_RGB, 0, RGB_BRIGHTNESS, 0); // Green
        taskBlinkLED.setInterval(1000);               // Blink every 1s
        break;
    }

    taskBlinkLED.enable(); // Ensure the LED task is running
}
