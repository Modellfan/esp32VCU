#include "system_state_control.h"
#include "adsystem_interface.h"

#include <Arduino.h>

SystemStateControl systemState;

SystemStateControl::SystemStateControl() {
    initialize();
}

void SystemStateControl::initialize() {
    adsystem_connected = false;
    //adsysConnectionLostAction(); // Start with red LED

    last_heartbeat_time = millis();
    operation_mode = Idle;
}

void SystemStateControl::updateHeartbeat(bool received) {
    if (received) {
        last_heartbeat_time = millis();
        if (!adsystem_connected) {
            // Just regained connection
            adsystem_connected = true;
            operation_mode = Auto; // or appropriate mode
            adsysConnectionOkAction();
        }
    }
}

bool SystemStateControl::isAdsystemConnected() const {
    return adsystem_connected;
}

void SystemStateControl::checkHeartbeatTimeout() {
    const uint32_t HEARTBEAT_TIMEOUT_MS = 500; // 500 ms, adjust as needed
    if (millis() > last_heartbeat_time + HEARTBEAT_TIMEOUT_MS) {
        if(adsystem_connected) {
            // Just lost connection
            adsystem_connected = false;
            operation_mode = Error;
            adsysConnectionLostAction();
        }
    }
}

void SystemStateControl::adsysConnectionLostAction()
{
    neopixelWrite(RGB_BUILTIN, 20, 0, 0); // Red
}
void SystemStateControl::adsysConnectionOkAction()
{
    neopixelWrite(RGB_BUILTIN, 0, 20, 0); // Green
} 

// is called periodically, every 100ms by the scheduler in main.cpp
void SystemStateControl::run() {
    checkHeartbeatTimeout();
    // Additional periodic checks and updates can be added here

    adsysHandler.sendHeartbeat();
}

void SystemStateControl::ADSystemMessagesCb(const AdsysMessage& msg)
{
    switch (msg.type)
    {
    case AdsysMsgType::HEARTBEAT:
        if (msg.payload.size() >= 1) {
            bool heartbeat_received = ((msg.payload[0] == 0) || (msg.payload[0] == 1));
            updateHeartbeat(heartbeat_received);
        }
        break;
    case AdsysMsgType::SET_SIMULATED_DATA_INJECTION:
        if (msg.payload.size() >= 1) {
            bool enable = (msg.payload[0] != 0);
            setInjectSimulatedData(enable);
        }
    }
}

void SystemStateControl::setInjectSimulatedData(bool enable)
{
    inject_simulated_data = enable;
    if (enable)
    {
        enableTaskInjectSimulatedData();
    }
    else
    {
        disableTaskInjectSimulatedData();
    }
}

void SystemStateControl::injectSimulatedData()
{
    static uint16_t angle = 4096; // start at center (0 degrees)
    static int8_t direction = 1;   // 1 = increasing angle, -1 = decreasing angle

    adsysHandler.sendSteeringAngle(angle); // straight


    // update simulated data for next call
    angle += direction * 64;
    if(angle >= 5396) {
        angle = 5396;
        direction = -1;
    } else if(angle <= 2796) {
        angle = 2796;
        direction = 1;
    }
}