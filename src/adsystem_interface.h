#pragma once
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <functional>
#include <WString.h>

// UART handler for AD System Interface

#define ADSYS_UART_START_BYTE 0xF1

// Message type identifiers
enum class AdsysMsgType : uint8_t {
    HEARTBEAT           = 0x01,
    STEERING_ANGLE      = 0x02,
    PHYSICAL_ACCELERATION_REQUEST = 0x04, // int16 physical acceleration request, to be injected to motor control CAN message -800 to 1000
    JOYSTICK_POS_PERCENT  = 0x10, // 2x int16, X and Y values in 100%, factor 1/16384
    BATTERY_SOC         = 0x11, // uint16_t, offset of -1056, factor of 0.25
    BATTERY_RANGE       = 0x12, // range in km uint8_t, offset 0, factor 1 (0-255)
    GEAR_SELECTION      = 0x13, // uint8_t ASCII character: raw forwarding: P R N D B C ?
    RPM_FRONT_CAN_FRAME = 0x14, // whole CAN frame of message 0x200 (8 bytes), since the CAN matrix has some faulty information
    RPM_REAR_CAN_FRAME  = 0x15, // whole CAN frame of message 0x208 (8 bytes), since the CAN matrix has some faulty information, note that there is also the signal brake pedal position (of iMiev) that can be ignored
    RPM_MOTOR           = 0x16, // uint16_t offset -10000, factor 1
    SPEED               = 0x17, // int16_t, speed in km/h, offset 0, factor 0.0078125
    SET_SIMULATED_DATA_INJECTION = 0xF0, // payload 0x00 for disable, 0x01 for enable
    // Add more as needed...
};

// Heartbeat payload
struct AdsysHeartbeat {
    uint8_t state; // 0x00 or 0x01
};

// Steering angle payload
struct AdsysSteeringAngle {
    uint16_t angle_raw; // signed 16-bit
};

// Add more payload structs as needed...

// Generic message structure
struct AdsysMessage {
    AdsysMsgType type;
    std::vector<uint8_t> payload;
};

// UART handler class
class AdsysUartHandler {
public:
    using MessageCallback = std::function<void(const AdsysMessage&)>;

    AdsysUartHandler();

    // Call this with each received byte
    void onByteReceived(uint8_t byte);

    // Send a message (payload is raw bytes)
    void sendMessage(AdsysMsgType type, const uint8_t* payload, size_t length);

    // Helper to send heartbeat
    void sendHeartbeat();

    // Helper to send steering angle
    void sendSteeringAngle(uint16_t angle);

    // Set callback for received messages
    void setMessageCallback(MessageCallback cb);

    // Platform-specific: implement this to send a byte
    void uartSendByte(uint8_t byte);
    void uartSendBytes(std::vector<uint8_t> &bytes);

private:
    std::vector<uint8_t> rxBuffer;
    MessageCallback messageCallback;

    uint8_t heartbeat_state;

    void processBuffer();
    uint8_t calcChecksum(const uint8_t* data, size_t length);
};

extern AdsysUartHandler adsysHandler;

void sendDebugMessage(const char* msg);
void sendDebugMessage(const String& msg);
void sendDebugMessage(StringSumHelper& msg);