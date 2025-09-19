#pragma once

#include <Arduino.h>

class SystemStateControl {
public:
    enum OperationMode : uint8_t {
        Idle = 0,
        Auto = 1,
        Manual = 2,
        Error = 3,
        Emergency = 4,
        Test = 5
    };

    SystemStateControl();

    void updateHeartbeat(bool received);
    bool isAdsystemConnected() const;

    void checkHeartbeatTimeout();
    void initialize();
    void run();

    // Optionally, provide accessors if needed
    OperationMode getOperationMode() const { return operation_mode; }
    void setOperationMode(OperationMode mode) { operation_mode = mode; }
    void adsysConnectionLostAction() { neopixelWrite(RGB_BUILTIN, 20, 0, 0); } // Red
    void adsysConnectionOkAction()   { neopixelWrite(RGB_BUILTIN, 0, 20, 0); } // Green

private:
    bool adsystem_connected;
    uint32_t last_heartbeat_time; // in milliseconds
    OperationMode operation_mode;
};

extern SystemStateControl systemState;