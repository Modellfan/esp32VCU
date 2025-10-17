/**************************************************************************
 *  Includes and Definitions
 **************************************************************************/
#include <Arduino.h>
#include "USB.h"
#include "USBCDC.h"
#include <TaskScheduler.h>
//#include <ESP32Servo.h>

#include "adsystem_interface.h"
#include "vehicle_control.h"

#define INIT_USB_SERIAL 0

#if(INIT_USB_SERIAL==1)
#define USBSERIAL_PRINTLN(x) USBSerial1.println(x)
#else
#define USBSERIAL_PRINTLN(x)
#endif

#define LOG_MAIN 0
#if (LOG_MAIN==1)
    #define LOG_MSG(x) sendDebugMessage(x)
#else
    #define LOG_MSG(x)
#endif

#define RX_TMP_BUF_SIZE 256
uint8_t rxTmpBuf[RX_TMP_BUF_SIZE] = {};

uint32_t totalBytesReceivedADSystem = 0;

#define ADSYS_PORT Serial // try hardware serial for AD System serial port

#if(INIT_USB_SERIAL==1)
// USB Serial Setup: Use a clear name for the USB CDC object.
USBCDC USBSerial1(0); // First virtual serial port
#endif

#include "canmanager.h"

// RGB LED Config (ESP32-S3 Built-in)
// #define RGB_BUILTIN    48   // Built-in LED pin on ESP32-S3
// #define RGB_BRIGHTNESS 0    // 0 for OFF

// Servo brake_servo;
// const int servoPin = 17;
// int pos = 0;

// ——————————————————————————————————————————————————————————————————————————————
//   Joystick Config (Analog Inputs)
// ——————————————————————————————————————————————————————————————————————————————
#define JOYSTICK_X_PIN 14 //4 // GPIO 4 for X-axis
#define JOYSTICK_Y_PIN 4 //5 // GPIO 5 for Y-axis

// Raw joystick values
int rawJoystickX = 0;
int rawJoystickY = 0;

// Processed joystick values (-100% to 100%)
float processedJoystickX = 0.0;
float processedJoystickY = 0.0;
double joystick_deadzone = 5.0; // percent

// Calibration values (from your provided data)
const int joystickX_ZERO = 1900; // X-axis center value
const int joystickY_ZERO = 1940; // Y-axis center value

const int JOYSTICK_MIN = 0;                  // Minimum raw ADC value
const int JOYSTICK_MAX = 4095;               // Maximum raw ADC value
const int JOYSTICK_RANGE = JOYSTICK_MAX / 2; // Half range for -100% to 100%

// ——————————————————————————————————————————————————————————————————————————————
//   Emergency Button Configuration
// ——————————————————————————————————————————————————————————————————————————————
// The emergency button is connected to pin 15.
// Using INPUT_PULLUP makes it active low (pressed = LOW).
const int EMERGENCY_BUTTON_PIN = 18;
bool emergencyButtonPressed = false;

/**************************************************************************
 *  Global Variables and Objects
 **************************************************************************/
// joystick control signals
bool joystick_control_active = false;
bool joystick_error_flag = false;

const float torque_ramp_accel = 20.0;             // torque acceleration ramp per 10ms
const float torque_ramp_decell = 10.0;            // torque deceleration ramp per 10ms
const float torque_max = 1000.0;                  // it seems like the iMiev allows 1800 here with full battery. Be carefull, this parameter can destroy the battery
const float torque_min = -800.0;                  // max from iMiev with full battery is arround -1000. Be carefull, this parameter can destroy the battery
const float torque_zero_space = 5.0;              // First 5% of joystick are zero zone in both directions
const float torque_regen_cutoff_rpm_high = 300.0; // When the motor rpm is below this , stop  cutoff any negative torque regen -> Hysteresis control
const float torque_regen_cutoff_rpm_low = 200.0;  // When the motor rpm is below this , start cutoff remaining negative torque regen -> Hysteresis control
const float torque_regen_cutoff_rpm_full = 10.0;  // When the motor rpm is below this , cutoff to zero -> Hysteresis control
float torque_theoretical = 0.0;                   // torque request before ramping
float torque_request_internal = 0.0;              // torque request, before putting max on it
float torque_request_calculated = 0.0;            // final torque before putting out to can
double vehicle_speed_limit = 5.0; // km/h
double vehicle_speed_limit_emergency = 15.0;
double vehicle_speed = 0.0; // in km/h, read from iMiev via CAN
double vehicle_speed_target = 0.0;
double vehicle_speed_ramp_up = 0.01;    // 1 km/h per second at 10ms refresh rate
double vehicle_speed_ramp_down = 0.01;  // 1 km/h per second at 10ms refresh rate

int16_t ADSystemPhysicalAccelerationRequest = 0;


uint8_t reverse_bit = 0;
uint32_t last_reverse_toggle_time = 0;
bool steeringControlHoldValue = true;
bool holdingJoystickInDirectionReleased = true;

uint32_t last_gear_switched_time = 0;
const uint32_t gear_switch_safety_delay_ms = 5000;

// ——————————————————————————————————————————————————————————————————————————————
//   Brake Control
// ——————————————————————————————————————————————————————————————————————————————
// PID Control Constants (Tune these for optimal performance)
const float Kp = 2.0; // Proportional gain
const float Ki = 0.1; // Integral gain
const float Kd = 0.5; // Derivative gain

// Servo position limits
const float SERVO_MIN_POSITION = -180.0; // Minimum servo position
const float SERVO_MAX_POSITION = 180.0;  // Maximum servo position

// PID variables
double brake_pedal_target = 0.0; // Desired brake pedal position (0-100%)
double brake_pedal_target_test = 0.0;   // Test variable for brake pedal position
double steering_angle_target_test = 50.0; // Test variable for steering angle
double torque_request_test = 0.0;      // Test variable for torque request

float servo_position = 0.0;     // Servo output position

// PID state variables
float previous_error = 0.0;
float integral = 0.0;

// === Constants for Braking Logic ===
const float BRAKE_RAMP_UP = 0.05;              // Rate of increase per cycle
const float BRAKE_RAMP_DOWN = 0.1;             // Faster release to avoid brake drag
const float PARK_BRAKE_TIME_THRESHOLD = 800.0; // Time in ms to engage park brake
const float BRAKE_PARKING = 0.8;               // Value of target if in parking brake mode

// === State Variables ===
float brake_calculated = 0.0;      // Final brake application value
unsigned long motor_zero_time = 0; // Time tracking for parking brake

// ——————————————————————————————————————————————————————————————————————————————
//   iMiev original signals
// ——————————————————————————————————————————————————————————————————————————————
float brake_pedal_position = 0.0;         // Scale factor: 0.39216
float accelerator_pedal_percentage = 0.0; // Scale factor: 0.4
char gear_selection = ' ';                // Default empty
int torque_request = 0;
bool brake_pedal_switch = 0;
int motor_rpm = 0;
int steering_angle = 0;

bool ignitionState = 0;

double RPM_fl = 0.;
double RPM_fr = 0.;
double RPM_rl = 0.;
double RPM_rr = 0.;
uint8_t rangeKm = 0;
double SoCValuePercent = 0.;

byte torque_request_byte_0 = 0;
byte torque_request_byte_1 = 0;

// status
bool heartbeat_rx_good = false;
// operation modes (selected by switch):
#define OPERATION_MODE_NORMAL 0 // normal (controlled by AD system)
#define OPERATION_MODE_ERROR  1 // error
#define OPERATION_MODE_TEST   2 // test (controlled by ECU)
uint8_t operation_mode = OPERATION_MODE_NORMAL;




Scheduler runner;

/**************************************************************************
 *  Function Prototypes
 **************************************************************************/
void pollCAN();
void manipulateCAN();
void passthroughCAN();
void printStatus();
void control_dynamics();
void control_acceleration();
void pollJoystick();
void control_brake_pedal();
void interpreteCANframe(const CANMessage &frame);

/**************************************************************************
 *  Task Definitions
 **************************************************************************/
Task taskPrintStatus(500, TASK_FOREVER, &printStatus, &runner, true);
Task taskvControlRun(10, TASK_FOREVER, [](){ vControl.run(); }, &runner, true);
Task taskvControlSendStatus(1000, TASK_FOREVER, [](){ vControl.sendStatus(); }, &runner, true);
Task taskVehicleDynamics(10, TASK_FOREVER, &control_dynamics, &runner, true);

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

    torque_request = physicalAcceleration;

    // 4) Example manipulation: ensure the physical value is never negative
    // if (physicalAcceleration < 0) {
    //     physicalAcceleration = 0;
    // }

    // if (physicalAcceleration < 0)
    // {
    //     if (outFrame.data[7] == 0x10)
    //     {
    //         physicalAcceleration = physicalAcceleration * 3;
    //     }
    // }

    VehicleControl::OperationMode vControlMode = vControl.getOperationMode();

    if (vControlMode == VehicleControl::ECUTestControl && vControl.getTestMode() == VehicleControl::TestMode::Test_BrakeSteeringActuatorJoystick)
    {
        physicalAcceleration = 0;
    }
    else if(vControlMode == VehicleControl::ECUTestControl && vControl.getTestMode() == VehicleControl::TestMode::Test_FullSystem)
    {
        //LOG_MSG("physicalAcceleration = torque_request_calculated: " + String(torque_request_calculated));
        physicalAcceleration = torque_request_calculated;
    }
    else if(vControlMode == VehicleControl::OperationMode::ADSystemControl)
    {
        physicalAcceleration = ADSystemPhysicalAccelerationRequest;
    }

    // safety measure
    if(millis() < last_gear_switched_time + gear_switch_safety_delay_ms) // caution: fails after 49.7 days of continuous run time (will override physicalAcceleration to zero until last_gear_switched_time + gear_switch_safety_delay_ms is reached again or gear was switched again)
    {
        physicalAcceleration = 0;
    }

    // 5) Convert back to raw: raw = physical + 2000
        rawAcceleration = (uint16_t)(physicalAcceleration + 2000);

    // 6) Store the manipulated raw value back in big-endian format
    outFrame.data[0] = (uint8_t)((rawAcceleration >> 8) & 0xFF); // MSB
    outFrame.data[1] = (uint8_t)(rawAcceleration & 0xFF);        // LSB

    if(vControlMode == VehicleControl::OperationMode::ECUTestControl && vControl.getTestMode() == VehicleControl::TestMode::Test_InverterJoystick)
    {
        outFrame.data[7] = reverse_bit;
    }
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

void interpreteCANframe(const CANMessage &frame)
{
    // Interpret messages based on their ID.
    if (frame.id == 0x101)
    {
        // Key
        ignitionState = (frame.data[0] == 0x04)?true:false;
    }
    else if (frame.id == 0x200)
    { // Wheel Rotation front
        uint16_t RPM_fl_raw = (frame.data[2] << 8) + frame.data[3]; // front left
        uint16_t RPM_fr_raw = (frame.data[4] << 8) + frame.data[5]; // front right
        RPM_fl = ((double) RPM_fl_raw - 49152.) / 19.;
        RPM_fr = ((double) RPM_fr_raw - 49152.) / 19.;
        
        adsysHandler.sendMessage(AdsysMsgType::RPM_FRONT_LR, &(frame.data[2]), 4);

        //LOG_MSG("0x200: d[2]: 0x" + String(frame.data[2], HEX) + " | d[3]: 0x" + String(frame.data[3], HEX) + " | d[4]: 0x" + String(frame.data[4], HEX) + " | d[5]: 0x" + String(frame.data[5], HEX));
        //LOG_MSG("0x200: uint16_t RPM_fl_raw: " + String(RPM_fl_raw) + " |  uint16_t RPM_fr_raw: " + String(RPM_fr_raw) + " | RPM_fl: " + String(RPM_fl) + " | RPM_fr: " + String(RPM_fr));
    }
    else if (frame.id == 0x208)
    { // Wheel Rotation rear, Brake Position
        uint8_t raw_brake_value = frame.data[3];
        brake_pedal_position = raw_brake_value * 0.25 - 6144.5;

        uint16_t RPM_rr_raw = (frame.data[4] << 8) + frame.data[5]; // front right
        uint16_t RPM_rl_raw = (frame.data[6] << 8) + frame.data[7]; // front left
        RPM_rr = ((double) RPM_rr_raw - 49152.) / 19.;
        RPM_rl = ((double) RPM_rl_raw - 49152.) / 19.;

        //LOG_MSG("0x208: d[4]: 0x" + String(frame.data[4], HEX) + " | d[5]: 0x" + String(frame.data[5], HEX) + " | d[6]: 0x" + String(frame.data[6], HEX) + " | d[7]: 0x" + String(frame.data[7], HEX));
        //LOG_MSG("0x208: uint16_t RPM_rl_raw: " + String(RPM_rl_raw) + " |  uint16_t RPM_rr_raw: " + String(RPM_rr_raw) + " | RPM_rl: " + String(RPM_rl) + " | RPM_rr: " + String(RPM_rr));
  
        adsysHandler.sendMessage(AdsysMsgType::RPM_REAR_RL, &(frame.data[4]), 4);
    }
    else if (frame.id == 0x210)
    { // Accelerator Pedal Percentage
        uint8_t raw_accel_value = frame.data[2];
        accelerator_pedal_percentage = raw_accel_value * 0.4;
    }
    else if (frame.id == 0x215)
    { // Vehicle speed
        uint16_t raw_vehicle_speed_value_uint16 = (frame.data[0] << 8) + frame.data[1];
        int16_t raw_vehicle_speed_value = *(int16_t *) &raw_vehicle_speed_value_uint16; // reinterprete as int16_t
        vehicle_speed = (double) raw_vehicle_speed_value * 0.0078125;
        if(vehicle_speed >= vehicle_speed_limit_emergency || vehicle_speed <= -vehicle_speed_limit_emergency)
        {
            //vControl.setEmergencyMode();
            LOG_MSG("vehicle speed larger than vehicle_speed_emergency. Opened safety circuit.");
        }

        adsysHandler.sendMessage(AdsysMsgType::SPEED, &(frame.data[0]), 2);
    }
    else if (frame.id == 0x236)
    { // Accelerator Pedal Percentage
        uint16_t rawSteering = (uint16_t)((frame.data[0] << 8) | frame.data[1]);
        adsysHandler.sendMessage(AdsysMsgType::STEERING_ANGLE, &(frame.data[0]), 2);

        vControl.updateSteeringAngle(rawSteering);
        //LOG_MSG("Got CAN Msg Steering angle raw: " + String(rawSteering));
        steering_angle = rawSteering - 4096; // div by 30.0 missing? -> According to https://myimiev.com/threads/can-network-reverse-engineering-creating-dbc-imiev.5788/ : Steering = (PID[0] * 256 + PID[1] - 4096) / 30.0;
    }
    else if (frame.id == 0x231)
    { // 0x231 message: 5 bytes message with Brake_Pedal_Switch_Sensor
        // According to the DBC, Brake_Pedal_Switch_Sensor is at bit 32, length 8, big-endian, signed.
        // In a 5-byte message, the 5th byte (index 4) contains bits 32-39.
        brake_pedal_switch = ((int8_t)frame.data[4] > 0);
    }
    else if (frame.id == 0x288)
    {
        // Extract motor_rpm from data[2] (MSB) and data[3] (LSB) in big-endian
        uint16_t rawRpm = (uint16_t)((frame.data[2] << 8) | frame.data[3]);
        // Convert raw value to physical value using scale = 1 and offset = -10000.
        // That is, physical_rpm = rawRpm - 10000.
        motor_rpm = (int16_t)rawRpm - 10000;

        adsysHandler.sendMessage(AdsysMsgType::RPM_MOTOR, &(frame.data[2]), 2);
    }
    else if (frame.id == 0x346)
    {
        // Range
        rangeKm = frame.data[7];
        adsysHandler.sendMessage(AdsysMsgType::BATTERY_RANGE, &(frame.data[7]), 1);
    }
    else if (frame.id == 0x374)
    {        
        uint16_t SoCRaw = (uint16_t)(frame.data[1]);
        int32_t SoCOffset = -10;
        double SoCFactor = 0.5;
        SoCValuePercent = (SoCRaw + SoCOffset) * SoCFactor;
        
        adsysHandler.sendMessage(AdsysMsgType::BATTERY_SOC, &(frame.data[1]), 1);

        //LOG_MSG("SoCRaw: " + String(SoCRaw) + " | SoCValuePercent: " + String(SoCValuePercent));
    }
    else if (frame.id == 0x418)
    { // Gear Shift Selection

        vControl.updateGearSelection(frame.data[0]);

        if(gear_selection != frame.data[0])
        {
            // gear was switched
            last_gear_switched_time = millis();
            setPhysicalAccelerationRequest(0); // safety measure
        }

        switch (frame.data[0])
        {
        case 0x50:
            gear_selection = 'P';
            break;
        case 0x52:
            gear_selection = 'R';
            break;
        case 0x4E:
            gear_selection = 'N';
            break;
        case 0x44:
            gear_selection = 'D';
            break;
        case 0x83:
            gear_selection = 'B';
            break;
        case 0x32:
            gear_selection = 'C';
            break;
        default:
            gear_selection = '?';
            break;
        }
        adsysHandler.sendMessage(AdsysMsgType::GEAR_SELECTION, &(frame.data[0]), 1);
    }
}

/**************************************************************************
 *  CAN Bus and Task Functions
 **************************************************************************/
void manipulateCAN()
{
    CANMessage frame;

    // -------------------------------------------------------------
    // Handle messages from CAN1 - Motor CAN bus
    // -------------------------------------------------------------
    if (can_motor.available()) //This is motor can bus
    {
        can_motor.receive(frame);
        interpreteCANframe(frame);

        // Check if this message needs manipulation.
        if (frame.id == 0x288) // Manipulate motor response to make torque request match
        {
            CANMessage outFrame;
            manipulate_0x288(frame, outFrame);
            can_vehicle.tryToSend(outFrame);
        }
        else
        {
            // For all other forwards as is
            can_vehicle.tryToSend(frame);
        }
    }

    // -------------------------------------------------------------
    // Handle messages from CAN2 - Vehicle CAN bus
    // -------------------------------------------------------------
    if (can_vehicle.available())
    {
        can_vehicle.receive(frame);
        interpreteCANframe(frame);
       
        // Only process messages that are not blacklisted.
        if (!isBlacklisted(frame.id))
        {
            // Check if this message needs manipulation.
            if (frame.id == 0x285) // Motor Control functions
            {
                CANMessage outFrame;
                manipulate_0x285(frame, outFrame);
                can_motor.tryToSend(outFrame);
            }
            else
            {
                // For all other not-blacklisted IDs, forward as is.
                can_motor.tryToSend(frame);
            }
        }
        else
        {
            // Optionally log that the message was blacklisted/dropped.
            // LOG_MSG("Dropping blacklisted CAN id: 0x" + String(frame.id, HEX));
        }
    }
}

void passthroughCAN()
{
    CANMessage frame;

    // -------------------------------------------------------------
    // Handle messages from CAN1 - Motor CAN bus
    // -------------------------------------------------------------
    if (can_motor.available())
    {
        can_motor.receive(frame);
        interpreteCANframe(frame);

        can_vehicle.tryToSend(frame);
    }

    // -------------------------------------------------------------
    // Handle messages from CAN2 - Vehicle CAN bus
    // -------------------------------------------------------------
    if (can_vehicle.available())
    {
        can_vehicle.receive(frame);
        interpreteCANframe(frame);
        
        can_motor.tryToSend(frame);
    }
}

void pollCAN()
{
    // always do the manipulation
    manipulateCAN();
}

// ——————————————————————————————————————————————————————————————————————————————
//   Control Vehicle Dynamics (Every 10ms)
// ——————————————————————————————————————————————————————————————————————————————
void control_dynamics()
{
    pollJoystick();
    control_acceleration();
    control_brake_pedal();
}

double Xavg = 0;
double Yavg = 0;

void pollJoystick()
{
    rawJoystickX = analogRead(JOYSTICK_X_PIN); // Read X-axis
    rawJoystickY = analogRead(JOYSTICK_Y_PIN); // Read Y-axis

    /*
    // in the current setup: Xavg = ~1900, Yavg = ~1940
    double cnt = 1000;
    Xavg = (Xavg * (cnt -1) + (double) rawJoystickX) / cnt;
    Yavg = (Yavg * (cnt -1) + (double) rawJoystickY) / cnt;

    LOG_MSG("rawJoystickX: " + String(rawJoystickX) + ", rawJoystickY: " + String(rawJoystickY));
    LOG_MSG("running avg rawJoystickX: " + String(Xavg) + ", running avg rawJoystickY: " + String(Yavg));*/


    // Convert to -100% to 100% range
    processedJoystickX = ((rawJoystickX - joystickX_ZERO) / (float)JOYSTICK_RANGE) * 100.0;
    processedJoystickY = ((rawJoystickY - joystickY_ZERO) / (float)JOYSTICK_RANGE) * 100.0;

    // Constrain values to -100% to 100%
    processedJoystickX = constrain(processedJoystickX, -100, 100);
    processedJoystickY = constrain(processedJoystickY, -100, 100);

    // Read the emergency button (active low, hence pressed = LOW)
    emergencyButtonPressed = false; // (digitalRead(EMERGENCY_BUTTON_PIN) == LOW);
    joystick_control_active = true; //!emergencyButtonPressed;

    if(vControl.getOperationMode() == VehicleControl::OperationMode::ECUTestControl)
    {
        double steeringInputPercent = (processedJoystickX<=-joystick_deadzone || processedJoystickX >= joystick_deadzone)?processedJoystickX/(100.0-joystick_deadzone):0.0;

        if(vControl.getTestMode() == VehicleControl::TestMode::Test_None || vControl.getTestMode() == VehicleControl::TestMode::Test_FullSystem)
        {
            if(steeringControlHoldValue)
            {
                bool holdingJoystickInDirection = (processedJoystickX > joystick_deadzone)?true:((processedJoystickX < -joystick_deadzone)?true:false);
                
                //LOG_MSG("holdingJoystickInDirection is: " + String(holdingJoystickInDirection));
                if(holdingJoystickInDirection)
                {
                    double anglePer = (processedJoystickX>0.0)?0.0:100.0;
                    vControl.setTargetSteeringAngle(anglePer);
                    holdingJoystickInDirectionReleased = false;
                    //LOG_MSG("Holding steering input. Set target steering angle to: " + String(anglePer));
                }
                else if(holdingJoystickInDirectionReleased == false)
                {
                    LOG_MSG("Released steering input. Set target to current steering angle. vControl.getSteeringAnglePercent():" + String(vControl.getSteeringAnglePercent()));
                    vControl.setTargetSteeringAngle(vControl.getSteeringAnglePercent());
                    holdingJoystickInDirectionReleased = true;
                }
            }
            else
            {
                double anglePercent = processedJoystickX > joystick_deadzone?((processedJoystickX-joystick_deadzone)/(100.0-joystick_deadzone)):(processedJoystickX < -joystick_deadzone?((processedJoystickX+joystick_deadzone)/(100.0-joystick_deadzone)):0.0);
                anglePercent = constrain(anglePercent, -100.0, 100.0);
                vControl.setTargetSteeringAngle((100.0 - anglePercent) / 2.0); // Map -100% to 100% joystick to 0% to 100% steering
            }
        }
        else if(vControl.getTestMode() == VehicleControl::TestMode::Test_BrakeSteeringActuatorJoystick)
        {
            int8_t summand = processedJoystickX > joystick_deadzone?1:(processedJoystickX < -joystick_deadzone?-1:0);
            steering_angle_target_test += (double) summand / 2.0;
            steering_angle_target_test = constrain(steering_angle_target_test, -100.0, 100.0);
            vControl.setTargetSteeringAngle((100.0 - steering_angle_target_test) / 2.0); // Map -100% to 100% joystick to 0% to 100% steering

            if(summand != 0)
            {
                LOG_MSG("steering_angle_target_test: " + String(steering_angle_target_test));
            }
        }
        else if(vControl.getTestMode() == VehicleControl::TestMode::Test_InverterJoystick)
        {
            if(steeringInputPercent < 0)
            {
                torque_request_test = 0.0;
            }
            else if(steeringInputPercent > 0)
            {
                if(motor_rpm == 0 && last_reverse_toggle_time + 2000 < millis())
                {
                    last_reverse_toggle_time = millis();
                    reverse_bit = 0x01 - reverse_bit; // toggle bit

                    LOG_MSG("Toggling reverse bit to: " + String(reverse_bit));
                }
                torque_request_test = 0.0;
            }
        }
    }
    //else if ADSystem
    else if(vControl.getOperationMode() == VehicleControl::OperationMode::ADSystemControl)
    {
        int16_t ADSystemJoystickXVal = (int16_t)(processedJoystickX * 16384);
        int16_t ADSystemJoystickYVal = (int16_t)(processedJoystickY * 16384);

        uint16_t ADSystemJoystickXVal_uint16 = *(uint16_t *) &ADSystemJoystickXVal;
        uint16_t ADSystemJoystickYVal_uint16 = *(uint16_t *) &ADSystemJoystickYVal;
        
        uint8_t buf[4] = {};
        buf[0] = (uint8_t) (ADSystemJoystickXVal >> 8);
        buf[1] = (uint8_t) (ADSystemJoystickXVal & 0xFF);
        buf[2] = (uint8_t) (ADSystemJoystickYVal >> 8);
        buf[3] = (uint8_t) (ADSystemJoystickYVal & 0xFF);

        adsysHandler.sendMessage(AdsysMsgType::JOYSTICK_POS_PERCENT, buf, 4);
    }
}

void control_brake_pedal()
{
    // Compute error between target and actual position
    // float error = brake_pedal_target - (brake_pedal_position / 100);

    // // PID calculations
    // integral += error; // Accumulate integral term
    // float derivative = error - previous_error;
    // previous_error = error;

    // // Compute PID output
    // float pid_output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // // Apply the PID output to the servo position
    // servo_position += pid_output;

    // // Constrain servo position within limits
    // servo_position = constrain(servo_position, SERVO_MIN_POSITION, SERVO_MAX_POSITION);

    // // Write the servo position
    // brake_servo.write(servo_position);

    VehicleControl::OperationMode vControlMode = vControl.getOperationMode();

    if(vControlMode == VehicleControl::OperationMode::ECUTestControl)
    {
        VehicleControl::TestMode vControlTestMode = vControl.getTestMode();

        if(vControlTestMode == VehicleControl::TestMode::Test_None || vControlTestMode == VehicleControl::TestMode::Test_FullSystem)
        {
            vControl.setTargetBrakePosition(brake_pedal_target * 100.0); // Map 0.0 to 1.0 to 0 to 100%
        }
        else if(vControlTestMode == VehicleControl::TestMode::Test_BrakeSteeringActuatorJoystick)
        {
            int8_t summand = processedJoystickY > torque_zero_space?1:(processedJoystickY < -torque_zero_space?-1:0);
            brake_pedal_target_test += (double) summand / 2.0; // increase or decrease target by joystick position
            brake_pedal_target_test = constrain(brake_pedal_target_test, 0.0, 100.0);
            vControl.setTargetBrakePosition(brake_pedal_target_test); // Map 0 to 1 -> 0 to 100%

            if(summand != 0)
            {
                LOG_MSG("brake_pedal_target_test: " + String(brake_pedal_target_test) + "%");
            }
        }
    }
    else
    {
        // don't control brake in ADSystemControl mode
    }
}

void control_acceleration()
{
    //  Overall control principle
    //  -5% to 5% joystick -> zero acceleration -> ramp up regen to torque min, when motor rpm is below torque_regen_cutoff_rpm put torque to zero
    //  > 5% joystick -> acceleration -> ramp up to torque_theoretical which is depending on the joystick position

    VehicleControl::OperationMode vControlMode = vControl.getOperationMode();

    if(vControlMode == VehicleControl::OperationMode::ECUTestControl)
    {
        VehicleControl::TestMode vControlTestMode = vControl.getTestMode();
        if (vControlTestMode == VehicleControl::TestMode::Test_BrakeSteeringActuatorJoystick)
            return;
        else if (vControlTestMode == VehicleControl::TestMode::Test_InverterJoystick)
        {
            int8_t summand = processedJoystickY > torque_zero_space?5:(processedJoystickY < -torque_zero_space?-5:0);
            torque_request_test += summand;
            torque_request_calculated = constrain(torque_request_test, torque_min, torque_max);

            if(summand != 0)
            {
                LOG_MSG("torque_request_test: " + String(torque_request_test) + "%");
            }
        }
    }
    else if(vControlMode == VehicleControl::OperationMode::ADSystemControl)
    {
        // TODO: use AD System inputs
    }

    // we need motor speed additional
    if ((joystick_error_flag == false) && (joystick_control_active == true))
    {
        if (processedJoystickY > torque_zero_space) // Acceleration or hold max speed
        {
            double vehicle_speed_input = (processedJoystickY - joystick_deadzone) * vehicle_speed_limit;

            if(vehicle_speed_target < vehicle_speed_input - 0.1)
            {
                vehicle_speed_target += vehicle_speed_ramp_up; 
            }
            else if (vehicle_speed_target > vehicle_speed_input + 0.1)
            {
                vehicle_speed_target -= vehicle_speed_ramp_down;
            }
            vehicle_speed_target = constrain(vehicle_speed_target, 0.0, vehicle_speed_limit);

            double save_prev_value = torque_request_internal;

            double vdiff_m_per_s = (vehicle_speed_target - vehicle_speed) / 3.6; // km/h to m/s -> factor 1000/3600
            double mass = 1000;
            double sign = vdiff_m_per_s/abs(vdiff_m_per_s);

            torque_theoretical = sign * 200.0 * vdiff_m_per_s * vdiff_m_per_s * mass / 100.0 / 5.0; // 100: 10ms steps, 5: torque to reach target in 5 seconds

            // now ramping consideration
            if (torque_request_internal < torque_theoretical)
            {
                torque_request_internal = torque_request_internal + torque_ramp_accel;
            }
            else
            {
                torque_request_internal = torque_request_internal - torque_ramp_accel;
            }

            // debug speed limiter
            // LOG_MSG("v in:" + String(vehicle_speed_input) + " | v target: " + String(vdiff_m_per_s) + " | v target: " + String(vdiff_m_per_s) + " | torque_theoretical: " + String(torque_theoretical) + " | torque req internal: " + String(torque_request_internal));

            // limit speed
            if(vehicle_speed > vehicle_speed_limit)
            {
                torque_request_internal -= torque_ramp_accel;
            }

            // Clamp torque within safe limits
            torque_request_internal = constrain(torque_request_internal, torque_min, torque_max);
        }
        else // Regen
        {
            vehicle_speed_target -= vehicle_speed_ramp_down;
            vehicle_speed_target = constrain(vehicle_speed_target, 0.0, vehicle_speed_limit);

            torque_theoretical = 0;
            if (motor_rpm > torque_regen_cutoff_rpm_high) // Normal regen behavior above high threshold
            {
                if (torque_request_internal > 0)
                {
                    torque_request_internal -= torque_ramp_accel;
                }
                else
                {
                    torque_request_internal -= torque_ramp_decell;
                }
            }
            else if (motor_rpm < torque_regen_cutoff_rpm_low) // Below low threshold, turn torque off smoothly
            {
                torque_request_internal *= 0.9; // Gradual decay instead of instant cutoff

                if (abs(torque_request_internal) < torque_regen_cutoff_rpm_full)
                {
                    torque_request_internal = 0.0; // Fully off only when close to zero
                }
            }
            // If within hysteresis band, maintain current torque
            else
            {
                // Do nothing (hold previous torque to prevent oscillation)
            }

            // Clamp torque within safe limits
            torque_request_internal = constrain(torque_request_internal, torque_min, torque_max);
        }
    }
    else
    {
        torque_request_calculated = 0.0;
    }
    // clamp to max values again for extra safety
    torque_request_calculated = constrain(torque_request_internal, torque_min, torque_max);

    if (processedJoystickY < (torque_zero_space * -1))
    {
        // Compute braking force based on joystick input
        brake_calculated = BRAKE_PARKING + ((1-BRAKE_PARKING) * (-processedJoystickY / (100.0 - torque_zero_space)));
    }
    else if (processedJoystickY > torque_zero_space)
    {
        brake_calculated = 0;
    }
    else
    {
        // Check if motor is at zero RPM and hold it for a set time
        if (motor_rpm == 0)
        {
            if (motor_zero_time == 0)
            {
                motor_zero_time = millis(); // Start timing
            }
            else if ((millis() - motor_zero_time) > (PARK_BRAKE_TIME_THRESHOLD))
            {
                brake_calculated = BRAKE_PARKING; // Engage parking brake
            }
        }
        else
        {
            motor_zero_time = 0; // Reset timer when RPM is nonzero
        }
    }

    // Apply smooth ramping to reach target brake position
    if (brake_pedal_target < brake_calculated)
    {
        brake_pedal_target += BRAKE_RAMP_UP;
    }
    else if (brake_pedal_target > brake_calculated)
    {
        brake_pedal_target -= BRAKE_RAMP_DOWN;
    }

    // Ensure brake target remains within valid limits
    brake_pedal_target = constrain(brake_calculated, 0.0, 1.0);

    //LOG_MSG("Brake Pedal Target: " + String(brake_pedal_target * 100.0) + "%");
}

// ——————————————————————————————————————————————————————————————————————————————
//   Print Status (Every 500ms)
// ——————————————————————————————————————————————————————————————————————————————

void printStatus()
{
    LOG_MSG("Joystick X: " + String(processedJoystickX) + " | Joystick Y: " + String(processedJoystickY));
    LOG_MSG("Brake Pedal Target: " + String(brake_pedal_target) + "% | Brake Pedal Position: " + String(brake_pedal_position));
    LOG_MSG("Torque Request (iMiev): " + String(torque_request) + " | Gear: " + String(gear_selection) + " | Motor RPM: " + String(motor_rpm) + " | Vehicle Speed: " + String(vehicle_speed) + " km/h");
    LOG_MSG("Torque Theoretical: " + String(torque_theoretical) + " | Torque Calculated: " + String(torque_request_calculated));
    LOG_MSG("Vehicle speed: " + String(vehicle_speed) + " | SoC: " + String(SoCValuePercent) + "% | Range: " + String(rangeKm) + " km | RPMs: FL: " + String(RPM_fl) + " | FR: " + String(RPM_fr) + " | RL: " + String(RPM_rl) + " | RR: " + String(RPM_rr) + " | ignitionState: " + String(ignitionState));
    LOG_MSG("Total bytes received from AD System: " + String(totalBytesReceivedADSystem));
}

/**************************************************************************
 *  Setup and Loop
 **************************************************************************/
void setup()
{
    Serial.begin(115200);

#if(INIT_USB_SERIAL==1)
    // Initialize USB CDC for monitoring/debug output.
    USBSerial1.begin();
    //USBSerial1.setRxBufferSize(1024);
    USB.begin();
#endif

    ignitionState = false;

    //delay(10000);

    // Initialize Servo
    // ESP32PWM::allocateTimer(0);
    // ESP32PWM::allocateTimer(1);
    // ESP32PWM::allocateTimer(2);
    // ESP32PWM::allocateTimer(3);
    // brake_servo.setPeriodHertz(50);           // standard 50 hz servo
    // brake_servo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object

    // Configure the built-in RGB LED.
    pinMode(RGB_BUILTIN, OUTPUT);
    //digitalWrite(RGB_BUILTIN, LOW);

    // Configure the emergency button pin (active low).
    pinMode(EMERGENCY_BUTTON_PIN, INPUT_PULLDOWN);

    // Initialize the CAN buses using the CAN manager.
    canManager_setup();

    vControl.initialize();

    // Set up AD System UART message callback
    adsysHandler.setMessageCallback([&](const AdsysMessage &msg){ vControl.ADSystemMessagesCb(msg); });
    vControl.adsysConnectionLostAction(); // set initial connection state
   
    // Optionally, print a startup message.
    LOG_MSG("System Initialized. Starting tasks...");
}

// careful, this was behaving buggy -> individual bytes were not put out
void AdsysUartHandler::uartSendByte(uint8_t byte)
{
    ADSYS_PORT.write(byte);
}

void AdsysUartHandler::uartSendBytes(std::vector<uint8_t> &bytes)
{
    ADSYS_PORT.write(bytes.data(), bytes.size());
}


void sendDebugMessage(const char* msg)
{
    USBSERIAL_PRINTLN(msg);
}

void sendDebugMessage(const String& msg)
{
    USBSERIAL_PRINTLN(msg);
}
void sendDebugMessage(StringSumHelper& msg)
{
    USBSERIAL_PRINTLN(msg);
}

void receive_from_adsystem()
{
    int numBytes = ADSYS_PORT.available();
    //LOG_MSG("Bytes in Rx buffer: " + String(numBytes));

    if(numBytes <= 0)
    {
        return;
    }

    do
    {
        size_t readNum = (numBytes > RX_TMP_BUF_SIZE)? RX_TMP_BUF_SIZE : numBytes;
        ADSYS_PORT.readBytes(rxTmpBuf, readNum);
        adsysHandler.onBytesReceived(rxTmpBuf, readNum);

        //LOG_MSG("Rx " + String(readNum) + " byte(s)");

        numBytes -= readNum;
        totalBytesReceivedADSystem += readNum;
    } while(numBytes >= RX_TMP_BUF_SIZE);
}

void loop()
{
    runner.execute();
    pollCAN();

    receive_from_adsystem();
}

void setVehicleSpeedLimit(double speed)
{
    if(speed <= vehicle_speed_limit_emergency)
    {
        vehicle_speed_limit = speed;
    }
}

void setVehicleSpeedLimitEmergency(double speed)
{
    vehicle_speed_limit_emergency = speed;
}

void setTargetVehicleSpeed(double speed)
{
    if(speed <= vehicle_speed_limit && speed < vehicle_speed_limit_emergency)
    {
        vehicle_speed_target = speed;
    }
}

double readVehicleSpeedLimit()
{
    return vehicle_speed_limit;
}

double readVehicleSpeedLimitEmergency()
{
    return vehicle_speed_limit_emergency;
}

double readTargetVehicleSpeed()
{
    return vehicle_speed_target;
}

void setPhysicalAccelerationRequest(int16_t ADSystemPhysicalAccerealation)
{
    //LOG_MSG("Received physical acceleration request: " + String(ADSystemPhysicalAccerealation) + "; Value would have been set but for testing it is hard coded to zero.");
    //ADSystemPhysicalAccerealation = 0;
    ADSystemPhysicalAccelerationRequest = constrain(ADSystemPhysicalAccerealation, torque_min, torque_max);;
}

bool getIgnitionState()
{
    return ignitionState;
}

bool getVehicleMoving()
{
    return (vehicle_speed>1.0)?true:false;
}