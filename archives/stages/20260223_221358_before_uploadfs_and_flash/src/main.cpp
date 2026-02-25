//---------------------------------------------------------------------------
// Includes and Definitions
//---------------------------------------------------------------------------
#include <Arduino.h>
#include "USB.h"
#include "USBCDC.h"
#include <TaskScheduler.h>
#include <ctype.h>

// Define GVRET_PORT and MONITOR_PORT.
// GVRET communication uses the primary Serial port.
// Monitoring/debug output uses USBSerial1.
#define GVRET_PORT Serial
#define MONITOR_PORT USBSerial1

// USB Serial Setup: Use a clear name for the USB CDC object.
USBCDC USBSerial1(0); // First virtual serial port

#include "gvret.h"
#include "canmanager.h"
#include "webinterface.h"
#include "tesla_sdu_messages.h"

// RGB LED Config (ESP32-S3 Built-in)
// #define RGB_BUILTIN    48   // Built-in LED pin on ESP32-S3
// #define RGB_BRIGHTNESS 0    // 0 for OFF

//---------------------------------------------------------------------------
// Global Variables and Objects
//---------------------------------------------------------------------------
Scheduler runner;

//---------------------------------------------------------------------------
// Porsche 986 original signals
//---------------------------------------------------------------------------
float engine_speed_rpm = 0.0f;
float throttle_position_280 = 0.0f;
float pedal_position_1 = 0.0f;
float pedal_position_2 = 0.0f;

float coolant_temperature = 0.0f;
bool coolant_level_switch = false;
bool cruise_control_active = false;

float inlet_air_temperature = 0.0f;
float throttle_position_1_298 = 0.0f;
float throttle_position_2_298 = 0.0f;
float barometric_pressure_mbar = 0.0f;
uint8_t vehicle_speed_298_raw = 0;

float wheel_speed_fl_legacy = 0.0f;
float wheel_speed_fr_legacy = 0.0f;
float wheel_speed_rl_legacy = 0.0f;
float wheel_speed_rr_legacy = 0.0f;
float wheel_speed_fl = 0.0f;
float wheel_speed_fr = 0.0f;
float wheel_speed_rl = 0.0f;
float wheel_speed_rr = 0.0f;

bool tcs_intervention = false;
bool msr_request = false;
bool abs_intervention = false;
bool abd_intervention = false;
bool fdr_intervention = false;
uint8_t asr_control_mode = 0;
bool abs_warning_lamp = false;
bool brake_warning_lamp = false;
bool brake_switch = false;
bool brake_switch_inverted = false;
bool handbrake_switch = false;
float vehicle_reference_speed = 0.0f;
float tcs_intervention_slow = 0.0f;
float tcs_intervention_fast = 0.0f;
float intervention_torque = 0.0f;
float lateral_acceleration = 0.0f;

bool check_engine_light = false;
bool reduced_power = false;
bool fan_error = false;
uint16_t fuel_used_raw = 0;
float boost_pressure = 0.0f;
float oil_temperature = 0.0f;

float ambient_temperature = 0.0f;
float oil_pressure = 0.0f;
uint8_t light_dimmer = 0;
uint8_t cluster_counter = 0;

float steering_angle_old_deg = 0.0f;
float steering_angle_deg = 0.0f;



//---------------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------------
void pollCAN();
void blinkLED();
void printStatus();
void interpreteCANframe(const CANMessage &frame);

//---------------------------------------------------------------------------
// Task Definitions
//---------------------------------------------------------------------------
Task taskBlinkLED(500, TASK_FOREVER, &blinkLED, &runner, true);
Task taskPrintStatus(500, TASK_FOREVER, &printStatus, &runner, false);


//---------------------------------------------------------------------------
// Blacklist Array: Uncomment an ID to block it from being forwarded.
// If the ID is commented out, it is allowed to be forwarded.
//---------------------------------------------------------------------------
static const uint32_t BLACKLISTED_CAN_IDS[] = {
    // 0x280, // Engine_1 (ECU)
    // 0x289, // EngineTemp (ECU)
    // 0x298, // Engine_2 (ECU)
    // 0x2A8, // Vehicle Speed (ABS / PSM)
    // 0x1A0, // ABS_1 (ABS / PSM)
    // 0x4E0, // EngineStat (ECU)
    // 0x520, // Instrument Cluster 2
    // 0x00C0 // Steering Angle Sensor
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
// Helper functions for Intel (little-endian) bit fields.
//---------------------------------------------------------------------------
static inline uint32_t readBitsLE(const uint8_t *data, uint16_t startBit, uint8_t bitLen)
{
    uint32_t value = 0;
    for (uint8_t i = 0; i < bitLen; i++)
    {
        uint16_t bitIndex = startBit + i;
        uint8_t byteIndex = bitIndex / 8;
        uint8_t bitInByte = bitIndex % 8;
        uint8_t bit = (data[byteIndex] >> bitInByte) & 0x01;
        value |= (uint32_t)bit << i;
    }
    return value;
}

static inline int32_t signExtend(uint32_t value, uint8_t bitLen)
{
    if (bitLen == 0 || bitLen >= 32)
    {
        return (int32_t)value;
    }
    uint32_t signBit = 1UL << (bitLen - 1);
    if (value & signBit)
    {
        value |= (~0UL << bitLen);
    }
    return (int32_t)value;
}

static inline void writeBitsLE(uint8_t *data, uint16_t startBit, uint8_t bitLen, uint32_t value)
{
    for (uint8_t i = 0; i < bitLen; i++)
    {
        uint16_t bitIndex = startBit + i;
        uint8_t byteIndex = bitIndex / 8;
        uint8_t bitInByte = bitIndex % 8;
        uint8_t bit = (value >> i) & 0x01;
        data[byteIndex] &= ~(1U << bitInByte);
        data[byteIndex] |= (bit << bitInByte);
    }
}

static inline void copyFrame(const CANMessage &inFrame, CANMessage &outFrame)
{
    outFrame.id = inFrame.id;
    outFrame.len = inFrame.len;
    for (uint8_t i = 0; i < 8; i++)
    {
        outFrame.data[i] = inFrame.data[i];
    }
}

//---------------------------------------------------------------------------
// Engine speed manipulation (CAN ID 0x280)
//---------------------------------------------------------------------------
bool engine_speed_override_active = true;
float engine_speed_override_rpm = 8000.0f;
float engine_speed_rpm_offset = 0.0f;

void manipulate_0x280(const CANMessage &inFrame, CANMessage &outFrame)
{
    copyFrame(inFrame, outFrame);

    uint16_t rawEngineSpeed = (uint16_t)readBitsLE(outFrame.data, 16, 16);
    float rpm = rawEngineSpeed * 0.25f;

    if (engine_speed_override_active)
    {
        rpm = engine_speed_override_rpm;
    }
    rpm += engine_speed_rpm_offset;
    if (rpm < 0.0f)
    {
        rpm = 0.0f;
    }

    uint16_t newRaw = (uint16_t)(rpm / 0.25f);
    writeBitsLE(outFrame.data, 16, 16, newRaw);
}

//---------------------------------------------------------------------------
// EngineTemp manipulation (CAN ID 0x289)
//---------------------------------------------------------------------------
bool engine_temp_override_active = false;
float coolant_temperature_override_c = 90.0f;
bool coolant_level_switch_override = false;
bool cruise_control_active_override = false;
bool engine_stat_override_active = false;
bool check_engine_light_override = false;
bool reduced_power_override = false;
bool fan_error_override = false;
uint16_t fuel_used_override_raw = 0;
float boost_pressure_override_mbar = 0.0f;
float oil_temperature_override_c = 90.0f;

static inline uint8_t clampU8(int32_t value)
{
    if (value < 0)
    {
        return 0;
    }
    if (value > 255)
    {
        return 255;
    }
    return (uint8_t)value;
}

static inline uint16_t clampU16(int32_t value)
{
    if (value < 0)
    {
        return 0;
    }
    if (value > 65535)
    {
        return 65535;
    }
    return (uint16_t)value;
}

void manipulate_0x289(const CANMessage &inFrame, CANMessage &outFrame)
{
    copyFrame(inFrame, outFrame);

    if (!engine_temp_override_active)
    {
        return;
    }

    int32_t rawCoolantTemp = (int32_t)((coolant_temperature_override_c + 48.0f) / 0.75f + 0.5f);
    uint8_t rawTemp = clampU8(rawCoolantTemp);
    writeBitsLE(outFrame.data, 8, 8, rawTemp);
    writeBitsLE(outFrame.data, 16, 1, coolant_level_switch_override ? 1U : 0U);
    writeBitsLE(outFrame.data, 22, 1, cruise_control_active_override ? 1U : 0U);
}

//---------------------------------------------------------------------------
// EngineStat manipulation (CAN ID 0x4E0)
//---------------------------------------------------------------------------
void manipulate_0x4E0(const CANMessage &inFrame, CANMessage &outFrame)
{
    copyFrame(inFrame, outFrame);

    if (!engine_stat_override_active)
    {
        return;
    }

    writeBitsLE(outFrame.data, 0, 1, check_engine_light_override ? 1U : 0U);
    writeBitsLE(outFrame.data, 3, 1, reduced_power_override ? 1U : 0U);
    writeBitsLE(outFrame.data, 4, 1, fan_error_override ? 1U : 0U);

    writeBitsLE(outFrame.data, 16, 16, fuel_used_override_raw);

    int32_t rawBoost = (int32_t)(boost_pressure_override_mbar / 10.0f + 0.5f);
    writeBitsLE(outFrame.data, 32, 8, clampU8(rawBoost));

    int32_t rawOilTemp = (int32_t)((oil_temperature_override_c + 48.0f) / 0.75f + 0.5f);
    writeBitsLE(outFrame.data, 40, 8, clampU8(rawOilTemp));
}

//---------------------------------------------------------------------------
// Monitor Port Command Handling
//---------------------------------------------------------------------------
void processMonitorCommand(char *line)
{
    for (char *p = line; *p != '\0'; p++)
    {
        *p = (char)toupper((unsigned char)*p);
    }

    if (strncmp(line, "HELP", 4) == 0 || (line[0] == '?' && line[1] == '\0'))
    {
        MONITOR_PORT.println("Commands:");
        MONITOR_PORT.println("  RPM <value>");
        MONITOR_PORT.println("  289 <coolant_c> <level 0/1> <cruise 0/1>");
        return;
    }

    float rpmValue = 0.0f;
    if (sscanf(line, "RPM %f", &rpmValue) == 1)
    {
        engine_speed_override_rpm = rpmValue;
        engine_speed_override_active = true;
        return;
    }

    float coolC = 0.0f;
    unsigned int level = 0;
    unsigned int cruise = 0;
    if (sscanf(line, "289 %f %u %u", &coolC, &level, &cruise) == 3)
    {
        coolant_temperature_override_c = coolC;
        coolant_level_switch_override = (level != 0);
        cruise_control_active_override = (cruise != 0);
        engine_temp_override_active = true;
        return;
    }
}

void handleMonitorInput()
{
    static char line[96];
    static size_t linePos = 0;

    while (MONITOR_PORT.available())
    {
        int c = MONITOR_PORT.read();
        if (c < 0)
        {
            break;
        }
        if (c == '\r')
        {
            continue;
        }
        if (c == '\n')
        {
            line[linePos] = '\0';
            if (linePos > 0)
            {
                processMonitorCommand(line);
            }
            linePos = 0;
            continue;
        }
        if (linePos < (sizeof(line) - 1))
        {
            line[linePos++] = (char)c;
        }
    }
}



//---------------------------------------------------------------------------
// Interprete incoming messages
//---------------------------------------------------------------------------
void interpreteCANframe(const CANMessage &frame)
{
    // Interpret messages based on their ID.
    switch (frame.id)
    {
    case 0x280: // Engine_1 (ECU)
    {
        uint16_t rawEngineSpeed = (uint16_t)readBitsLE(frame.data, 16, 16);
        engine_speed_rpm = rawEngineSpeed * 0.25f;
        throttle_position_280 = readBitsLE(frame.data, 40, 8) * 0.390625f;
        pedal_position_1 = readBitsLE(frame.data, 32, 8) * 1.0f;
        pedal_position_2 = readBitsLE(frame.data, 56, 8) * 1.0f;
        break;
    }
    case 0x289: // EngineTemp (ECU)
    {
        uint8_t rawCoolantTemp = (uint8_t)readBitsLE(frame.data, 8, 8);
        coolant_temperature = rawCoolantTemp * 0.75f - 48.0f;
        coolant_level_switch = readBitsLE(frame.data, 16, 1) != 0;
        cruise_control_active = readBitsLE(frame.data, 22, 1) != 0;
        break;
    }
    case 0x298: // Engine_2 (ECU)
    {
        uint8_t rawIat = (uint8_t)readBitsLE(frame.data, 8, 8);
        inlet_air_temperature = rawIat * 0.75f - 48.0f;
        throttle_position_1_298 = readBitsLE(frame.data, 16, 8) * 0.390625f;
        throttle_position_2_298 = readBitsLE(frame.data, 32, 8) * 0.390625f;
        barometric_pressure_mbar = readBitsLE(frame.data, 40, 8) * 5.0f;
        vehicle_speed_298_raw = (uint8_t)readBitsLE(frame.data, 56, 8);
        break;
    }
    case 0x2A8: // Vehicle Speed (ABS / PSM)
    {
        uint16_t rawLegacyFL = (uint16_t)readBitsLE(frame.data, 1, 15);
        uint16_t rawLegacyFR = (uint16_t)readBitsLE(frame.data, 17, 15);
        uint16_t rawLegacyRL = (uint16_t)readBitsLE(frame.data, 33, 15);
        uint16_t rawLegacyRR = (uint16_t)readBitsLE(frame.data, 49, 15);
        wheel_speed_fl_legacy = rawLegacyFL * 0.01f;
        wheel_speed_fr_legacy = rawLegacyFR * 0.01f;
        wheel_speed_rl_legacy = rawLegacyRL * 0.01f;
        wheel_speed_rr_legacy = rawLegacyRR * 0.01f;

        uint16_t rawNewFL = (uint16_t)readBitsLE(frame.data, 0, 16);
        uint16_t rawNewFR = (uint16_t)readBitsLE(frame.data, 16, 16);
        uint16_t rawNewRL = (uint16_t)readBitsLE(frame.data, 32, 16);
        uint16_t rawNewRR = (uint16_t)readBitsLE(frame.data, 48, 16);
        wheel_speed_fl = rawNewFL * 0.005218f - 1.6338f;
        wheel_speed_fr = rawNewFR * 0.005218f - 1.6338f;
        wheel_speed_rl = rawNewRL * 0.005218f - 1.6338f;
        wheel_speed_rr = rawNewRR * 0.005218f - 1.6338f;
        break;
    }
    case 0x1A0: // ABS_1 (ABS / PSM)
    {
        tcs_intervention = readBitsLE(frame.data, 0, 1) != 0;
        msr_request = readBitsLE(frame.data, 1, 1) != 0;
        abs_intervention = readBitsLE(frame.data, 2, 1) != 0;
        abd_intervention = readBitsLE(frame.data, 3, 1) != 0;
        fdr_intervention = readBitsLE(frame.data, 4, 1) != 0;
        asr_control_mode = (uint8_t)readBitsLE(frame.data, 5, 2);
        abs_warning_lamp = readBitsLE(frame.data, 8, 1) != 0;
        brake_warning_lamp = readBitsLE(frame.data, 10, 1) != 0;
        brake_switch = readBitsLE(frame.data, 11, 1) != 0;
        brake_switch_inverted = readBitsLE(frame.data, 12, 1) != 0;
        handbrake_switch = readBitsLE(frame.data, 15, 1) != 0;
        vehicle_reference_speed = readBitsLE(frame.data, 17, 16) * 0.01f;
        tcs_intervention_slow = readBitsLE(frame.data, 32, 8) * 0.39f;
        tcs_intervention_fast = readBitsLE(frame.data, 40, 8) * 0.39f;
        intervention_torque = readBitsLE(frame.data, 48, 8) * 0.39f;
        lateral_acceleration = readBitsLE(frame.data, 48, 8) * 0.01f - 1.27f;
        break;
    }
    case 0x4E0: // EngineStat (ECU)
    {
        check_engine_light = readBitsLE(frame.data, 0, 1) != 0;
        reduced_power = readBitsLE(frame.data, 3, 1) != 0;
        fan_error = readBitsLE(frame.data, 4, 1) != 0;
        fuel_used_raw = (uint16_t)readBitsLE(frame.data, 16, 16);
        boost_pressure = readBitsLE(frame.data, 32, 8) * 10.0f;
        oil_temperature = readBitsLE(frame.data, 40, 8) * 0.75f - 48.0f;
        break;
    }
    case 0x520: // Instrument Cluster 2
    {
        ambient_temperature = readBitsLE(frame.data, 24, 8) * 0.5f - 40.0f;
        oil_pressure = readBitsLE(frame.data, 32, 8) * 0.04f;
        light_dimmer = (uint8_t)readBitsLE(frame.data, 8, 8);
        cluster_counter = (uint8_t)readBitsLE(frame.data, 40, 8);
        break;
    }
    case 0x00C0: // Steering Angle Sensor
    {
        int32_t rawOld = signExtend(readBitsLE(frame.data, 4, 12), 12);
        steering_angle_old_deg = rawOld * 0.390625f;
        int32_t rawNew = signExtend(readBitsLE(frame.data, 0, 16), 16);
        steering_angle_deg = rawNew * 0.0773f - 1.1668f;
        break;
    }
    case 0x320: // Tesla SDU Motor_Status
    {
        decode0x320(frame);
        break;
    }
    case 0x321: // Tesla SDU Voltage_Current
    {
        decode0x321(frame);
        break;
    }
    case 0x322: // Tesla SDU Speed_Input
    {
        decode0x322(frame);
        break;
    }
    case 0x323: // Tesla SDU Steering
    {
        decode0x323(frame);
        break;
    }
    case 0x324: // Tesla SDU Temperatur_LowVoltage
    {
        decode0x324(frame);
        break;
    }
    default:
        break;
    }
}

//---------------------------------------------------------------------------
// CAN Bus and Task Functions
//---------------------------------------------------------------------------
void pollCAN()
{
    CANMessage frame;

    // -------------------------------------------------------------
    // Handle messages from CAN1 - Motor CAN bus
    // -------------------------------------------------------------
    if (can.available()) // This is motor CAN bus
    {
        CANMessage outFrame;
        can.receive(frame);
        interpreteCANframe(frame);
        outFrame = frame;

        // Only process messages that are not blacklisted.
        if (!isBlacklisted(frame.id))
        {
            // Check if this message needs manipulation.
            if (frame.id == 0x280) // Engine_1 (ECU)
            {
                manipulate_0x280(frame, outFrame);
            }
            else if (frame.id == 0x289) // EngineTemp (ECU)
            {
                manipulate_0x289(frame, outFrame);
            }
            else if (frame.id == 0x4E0) // EngineStat (ECU)
            {
                manipulate_0x4E0(frame, outFrame);
            }
            // For all other not-blacklisted IDs, forward as is.
            can.tryToSend(outFrame);
            sendFrameToUSB(outFrame, 2);

            // For all other forwards as is
            can2.tryToSend(outFrame);
            sendFrameToUSB(outFrame, 0);
        }
        else
        {
            // Optionally log that the message was blacklisted/dropped.
            // MONITOR_PORT.println("Dropping blacklisted CAN id: 0x" + String(frame.id, HEX));
        }
    }

    // -------------------------------------------------------------
    // Handle messages from CAN2 - Vehicle CAN bus
    // -------------------------------------------------------------
    if (can2.available())
    {
        CANMessage outFrame;
        can2.receive(frame);
        sendFrameToUSB(frame, 1);
        interpreteCANframe(frame);
        outFrame = frame;

        // Only process messages that are not blacklisted.
        if (!isBlacklisted(frame.id))
        {

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


//---------------------------------------------------------------------------
// Print Status (Every 500ms)
//---------------------------------------------------------------------------

void printStatus()
{
    MONITOR_PORT.printf(
        "ENG{rpm=%.1f thr=%.1f ped1=%.1f ped2=%.1f} "
        "TEMP{cool=%.1f clvl=%u crs=%u iat=%.1f amb=%.1f oil=%.1f} "
        "AIR{bar=%.0f boost=%.0f} "
        "THR{t1=%.1f t2=%.1f} "
        "SPD{vref=%.1f vraw=%u wfl=%.2f wfr=%.2f wrl=%.2f wrr=%.2f lfl=%.2f lfr=%.2f lrl=%.2f lrr=%.2f} "
        "ABS{tcs=%u msr=%u abs=%u abd=%u fdr=%u asr=%u} "
        "BRK{aw=%u bw=%u sw=%u swi=%u hb=%u} "
        "TRQ{tcss=%.2f tcsf=%.2f itq=%.2f lat=%.2f} "
        "STAT{cel=%u red=%u fan=%u fuel=%u} "
        "OIL{pres=%.2f} "
        "CLUS{dim=%u cnt=%u} "
        "STR{old=%.1f new=%.1f}\n",
        engine_speed_rpm,
        throttle_position_280,
        pedal_position_1,
        pedal_position_2,
        coolant_temperature,
        coolant_level_switch ? 1U : 0U,
        cruise_control_active ? 1U : 0U,
        inlet_air_temperature,
        ambient_temperature,
        oil_temperature,
        barometric_pressure_mbar,
        boost_pressure,
        throttle_position_1_298,
        throttle_position_2_298,
        vehicle_reference_speed,
        (unsigned int)vehicle_speed_298_raw,
        wheel_speed_fl,
        wheel_speed_fr,
        wheel_speed_rl,
        wheel_speed_rr,
        wheel_speed_fl_legacy,
        wheel_speed_fr_legacy,
        wheel_speed_rl_legacy,
        wheel_speed_rr_legacy,
        tcs_intervention ? 1U : 0U,
        msr_request ? 1U : 0U,
        abs_intervention ? 1U : 0U,
        abd_intervention ? 1U : 0U,
        fdr_intervention ? 1U : 0U,
        (unsigned int)asr_control_mode,
        abs_warning_lamp ? 1U : 0U,
        brake_warning_lamp ? 1U : 0U,
        brake_switch ? 1U : 0U,
        brake_switch_inverted ? 1U : 0U,
        handbrake_switch ? 1U : 0U,
        tcs_intervention_slow,
        tcs_intervention_fast,
        intervention_torque,
        lateral_acceleration,
        check_engine_light ? 1U : 0U,
        reduced_power ? 1U : 0U,
        fan_error ? 1U : 0U,
        (unsigned int)fuel_used_raw,
        oil_pressure,
        (unsigned int)light_dimmer,
        (unsigned int)cluster_counter,
        steering_angle_old_deg,
        steering_angle_deg);
}

//---------------------------------------------------------------------------
// Setup and Loop
//---------------------------------------------------------------------------
void setup()
{
    // Initialize the primary Serial port for GVRET communication.
    Serial.begin(115200);

    // Initialize USB CDC for monitoring/debug output.
    USBSerial1.begin();
    USB.begin();

    // Configure the built-in RGB LED.
    pinMode(RGB_BUILTIN, OUTPUT);
    digitalWrite(RGB_BUILTIN, LOW);

    // Initialize the CAN buses using the CAN manager.
    canManager_setup();

    // Start AP web interface.
    webinterfaceBegin();

    // Optionally, print a startup message.
    MONITOR_PORT.println("System Initialized. Starting tasks...");
    MONITOR_PORT.println("Monitor port: USBSerial1 (CDC0). Send '?' + LF for help.");
}

void loop()
{
    handleMonitorInput();
    webinterfaceHandle();
    runner.execute();
    pollCAN();
    gvret_loop();
}
