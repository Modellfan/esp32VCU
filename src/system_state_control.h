#pragma once

#include <stdint.h>

struct AdsysMessage;

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
    void adsysConnectionLostAction();
    void adsysConnectionOkAction();

    void injectSimulatedData();
    void setInjectSimulatedData(bool enable);
    bool getInjectSimulatedData() const { return inject_simulated_data; }

    void ADSystemMessagesCb(const AdsysMessage& msg);

private:
    bool adsystem_connected;
    uint32_t last_heartbeat_time; // in milliseconds
    OperationMode operation_mode;
    bool inject_simulated_data = false;
};

extern SystemStateControl systemState;

void enableTaskInjectSimulatedData();
void disableTaskInjectSimulatedData();