#pragma once
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <functional>

// UART handler for AD System Interface

#define ADSYS_UART_START_BYTE 0xF1

// Message type identifiers
enum class AdsysMsgType : uint8_t {
    HEARTBEAT        = 0x01,
    STEERING_ANGLE   = 0x02,
    SET_SIMULATED_DATA_INJECTION = 0xF0,
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
