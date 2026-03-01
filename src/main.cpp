//---------------------------------------------------------------------------
// Includes and Definitions
//---------------------------------------------------------------------------
#include <Arduino.h>
#include <TaskScheduler.h>
#include <ctype.h>

#define GVRET_PORT Serial

#include "gvret.h"
#include "canmanager.h"
#include "webinterface.h"
#include "tesla_sdu_messages.h"
#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "cluster_calc.h"
#include "utils.h"
#include "can_utils.h"

// RGB LED Config is handled in src/utils.h via RGB_BUILTIN / RGB_BRIGHTNESS.

//---------------------------------------------------------------------------
// Global Variables and Objects
//---------------------------------------------------------------------------
Scheduler runner;

//---------------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------------
void pollVehicleCAN();
void pollTeslaCAN();
void emulateECUCAN();
void blinkLED();

//---------------------------------------------------------------------------
// Task Definitions
//---------------------------------------------------------------------------
Task taskBlinkLED(500, TASK_FOREVER, &blinkLED, &runner, true);
Task taskEmulateECUCAN(6, TASK_FOREVER, &emulateECUCAN, &runner, true);

//---------------------------------------------------------------------------
// CAN 1 Handle Porsche 986 Vehicle CAN Messages
//---------------------------------------------------------------------------
void pollVehicleCAN()
{
    CANMessage frame;
    while (can.available())
    {
        can.receive(frame);
        //sendFrameToUSB(frame, 1);

        (void)decode986Vehicle(frame);
    }
}

//---------------------------------------------------------------------------
// CAN 2 Handle Tesla SDU CAN Messages
//---------------------------------------------------------------------------
void pollTeslaCAN()
{
    CANMessage frame;
    while (can2.available())
    {
        can2.receive(frame);
        //sendFrameToUSB(frame, 2);
        (void)decodeTeslaSdu(frame);
    }
}

//---------------------------------------------------------------------------
// CAN 1 Porsche 986 Emulation
//---------------------------------------------------------------------------
void emulateECUCAN()
{
    static uint8_t slot = 0;
    CANMessage outFrame;

    // Keep cluster calculations time-aligned with ECU CAN emulation.
    updateCluster();

    // 5 ECU frames x 6 ms slot = 30 ms full cycle, evenly distributed.
    switch (slot)
    {
    case 0:
        encode986Ecu0x280(outFrame);
        break;
    case 1:
        encode986Ecu0x289(outFrame);
        break;
    case 2:
        encode986Ecu0x298(outFrame);
        break;
    case 3:
        encode986Ecu0x4E0(outFrame);
        break;
    case 4:
    default:
        encode986Ecu0x7A0(outFrame);
        break;
    }

    can.tryToSend(outFrame);
    //sendFrameToUSB(outFrame, 0); // 0 to differentiate between received and sent frames in SavvyCAN

    slot++;
    if (slot >= 5U)
    {
        slot = 0;
    }
}

// Task Function: Toggle the built-in LED.
void blinkLED()
{
    static bool ledOn = false;
    FastLED.setBrightness((uint8_t)((255U * 10U) / 100U));
    if (ledOn)
    {
        fastledOff();
    }
    else
    {
        fastled(LED_GREEN);
    }
    ledOn = !ledOn;
}

//---------------------------------------------------------------------------
// Setup and Loop
//---------------------------------------------------------------------------
void setup()
{
    // Initialize the primary Serial port for GVRET communication.
    Serial.begin(250000);

    // Initialize built-in RGB LED utility.
    fastledInit();

    // Initialize the CAN buses using the CAN manager.
    canManager_setup();

    // Start AP web interface.
    webinterfaceBegin();
}

void loop()
{
    webinterfaceHandle();
    runner.execute();
    pollVehicleCAN();
    pollTeslaCAN();
    gvret_loop();
}
