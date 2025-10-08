#include "adsystem_interface.h"
#include <Arduino.h>

AdsysUartHandler adsysHandler;

// Constructor
AdsysUartHandler::AdsysUartHandler() : messageCallback(nullptr) {}

// Calculate checksum so that sum of all bytes including checksum is 0
uint8_t AdsysUartHandler::calcChecksum(const uint8_t* data, size_t length) {
    uint8_t sum = 0;
    for (size_t i = 0; i < length; i++) {
        sum += data[i];
    }
    return (uint8_t)(0 - sum);
}

// Call this for each received byte
void AdsysUartHandler::onByteReceived(uint8_t byte) {
    rxBuffer.push_back(byte);

    // Try to process buffer if at least 4 bytes (start, type, payload, checksum)
    if (rxBuffer.size() >= 4) {
        processBuffer();
    }
}

void AdsysUartHandler::onBytesReceived(uint8_t *bytes, size_t numBytes) {
    std::vector<uint8_t> bytesV(bytes, bytes+numBytes);
    rxBuffer.insert(rxBuffer.end(), bytesV.begin(), bytesV.end());

    // Try to process buffer if at least 4 bytes (start, type, payload, checksum)
    if (rxBuffer.size() >= 4) {
        processBuffer();
    }
}

// Process rxBuffer for complete messages
void AdsysUartHandler::processBuffer() {
    while (rxBuffer.size() >= 4) {
        // Look for start byte
        if (rxBuffer[0] != ADSYS_UART_START_BYTE) {
            rxBuffer.erase(rxBuffer.begin());
            continue;
        }

        // At least start (1 byte) + type (1 byte) + length of payload (1 byte) + payload (min 1) + checksum (1 byte)
        if (rxBuffer.size() < 5) return;

        // Determine payload length by type
        uint8_t type = rxBuffer[1];
        size_t payloadLen = rxBuffer[2];

        size_t msgLen = 1 + 1 + 1 + payloadLen + 1; // start + type + length of payload + payload + checksum

        if (rxBuffer.size() < msgLen) return; // Wait for more bytes

        // Checksum validation
        uint8_t sum = 0;
        for (size_t i = 0; i < msgLen; ++i) sum += rxBuffer[i];
        if (sum != 0) {
            // Invalid, drop start byte and retry
            sendDebugMessage("Dropped message: ID: 0x" +String(type, HEX));
            rxBuffer.erase(rxBuffer.begin());
            continue;
        }

        // Extract payload
        AdsysMessage msg;
        msg.type = (AdsysMsgType)type;
        msg.payload.assign(rxBuffer.begin() + 3, rxBuffer.begin() + 3 + payloadLen);

        // Callback
        if (messageCallback)
        {
            messageCallback(msg);
        }

        // Remove processed message
        rxBuffer.erase(rxBuffer.begin(), rxBuffer.begin() + msgLen);

        // Optional: skip trailing '\n'
        if (!rxBuffer.empty() && rxBuffer[0] == '\n') {
            rxBuffer.erase(rxBuffer.begin());
        }
    }
}

// Send a message (payload is raw bytes)
void AdsysUartHandler::sendMessage(AdsysMsgType type, const uint8_t* payload, size_t length) {
    std::vector<uint8_t> msg;
    msg.reserve(16);
    msg.push_back(ADSYS_UART_START_BYTE);
    msg.push_back(static_cast<uint8_t>(type));
    msg.push_back(static_cast<uint8_t>(length));
    for (size_t i = 0; i < length; i++) msg.push_back(payload[i]);
    uint8_t checksum = calcChecksum(msg.data(), msg.size());

    msg.push_back(checksum);

    // Optional: append '\n' for readability
    msg.push_back('\n');

    //for (uint8_t b : msg) uartSendByte(b);

    uartSendBytes(msg);
}

// Helper to send heartbeat
void AdsysUartHandler::sendHeartbeat() {
    heartbeat_state = (heartbeat_state == 0) ? 1 : 0; // toggle state
    sendMessage(AdsysMsgType::HEARTBEAT, &heartbeat_state, 1);
}

// Helper to send steering angle
void AdsysUartHandler::sendSteeringAngle(uint16_t angle) {
    uint8_t payload[2];
    payload[0] = (uint8_t)(angle & 0xFF);
    payload[1] = (uint8_t)((angle >> 8) & 0xFF);
    sendMessage(AdsysMsgType::STEERING_ANGLE, payload, 2);
}

// Set callback for received messages
void AdsysUartHandler::setMessageCallback(MessageCallback cb) {
    messageCallback = cb;
}