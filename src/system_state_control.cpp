#include "system_state_control.h"
#include "adsystem_interface.h"

SystemStateControl systemState;

SystemStateControl::SystemStateControl() {
    initialize();
}

void SystemStateControl::initialize() {
    adsystem_connected = false;
    last_heartbeat_time = millis();
    operation_mode = Idle;

    // Set up AD System UART message callback
    adsysHandler.setMessageCallback([this](const AdsysMessage& msg) {
        if (msg.type == AdsysMsgType::HEARTBEAT && msg.payload.size() == 1) {
            bool received = (msg.payload[0] == 0x01 || msg.payload[0] == 0x00);
            updateHeartbeat(received);
        }
        // Handle other message types as needed
    });
}

void SystemStateControl::updateHeartbeat(bool received) {
    if (received) {
        last_heartbeat_time = millis();
        adsystem_connected = true;
    }
}

bool SystemStateControl::isAdsystemConnected() const {
    return adsystem_connected;
}

void SystemStateControl::checkHeartbeatTimeout() {
    const uint32_t HEARTBEAT_TIMEOUT_MS = 500; // 500 ms, adjust as needed
    if (millis() - last_heartbeat_time > HEARTBEAT_TIMEOUT_MS) {
        adsystem_connected = false;
        operation_mode = Error;
    }
}

// is called periodically, every 100ms by the scheduler in main.cpp
void SystemStateControl::run() {
    checkHeartbeatTimeout();
    // Additional periodic checks and updates can be added here

    adsysHandler.sendHeartbeat();
}