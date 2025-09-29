#pragma once

#include <stdint.h>
#include <string.h>

// --- Default Actuator I/O pin definitions (can be overridden per instance) ---
#define STEERING_ACTUATOR_DIR_A_PIN   39
#define STEERING_ACTUATOR_DIR_B_PIN   36
#define STEERING_ACTUATOR_FB_A_PIN    255  // Optional
#define STEERING_ACTUATOR_FB_B_PIN    255  // Optional

#define BRAKE_ACTUATOR_DIR_A_PIN   3
#define BRAKE_ACTUATOR_DIR_B_PIN   35
#define BRAKE_ACTUATOR_FB_A_PIN    255  // Optional
#define BRAKE_ACTUATOR_FB_B_PIN    255  // Optional

#define DASHBOARD_LED_ECU_STATUS_PIN 45
#define DASHBOARD_LED_ECU_ERROR_PIN 20 // active low
#define ECU_TRIGGER_EMERGENCY_PIN 46 // active low
#define DASHBOARD_LED_GEAR_PARK_PIN 21
#define DASHBOARD_LED_GEAR_DRIVE_PIN 47
#define DASHBOARD_LED_GEAR_OTHER_PIN 37


struct AdsysMessage;
struct CANMessage;

class ActuatorControl {
public:
    enum ActuatorState : uint8_t {
        Inactive = 0,
        MovingDirA = 1,
        MovingDirB = 2,
        Error = 3
    };
    enum ControlType : uint8_t {
        ActiveControlWithFeedback = 0,
        ActiveControlWithTime = 1,
        MonitoringOnly = 2
    };

    // --- Pin configuration (can be set per instance) ---
    uint8_t pin_dir_a;
    uint8_t pin_dir_b;
    uint8_t pin_fb_a;
    uint8_t pin_fb_b;

private:
    uint16_t position; // position sensor based
    uint32_t position_timestamp; // position timestamp
    uint16_t position_time_based; // position based on time tracking
    uint16_t allowed_tolerance_time_based = 20; // allowed tolerance in time-based position tracking (in position units)
    double position_steps_per_ms; // factor to convert movement duration to steps for time-based tracking
    uint16_t target_position;
    int32_t position_tracking_in_ms; // movement from starting position (0 ms)
    uint32_t position_tracking_start_time_current_move;
    uint32_t max_move_time_ms; // max time allowed for a move command in ms. If this is exceeded, an error state is triggered.
    bool emergency_triggered;
    bool position_known_time_tracking;
    bool allow_clamping_to_known_position = false;
    bool clamping_known_position_DirB_zero = true;

    double hysteresis_percent_start; // hysteresis band start in percent of operation range
    double hysteresis_percent_stop; // hysteresis band stop in percent of operation range 
    uint16_t hysteresis_start; // hysteresis band start in position units
    uint16_t hysteresis_stop; // hysteresis band stop in position units

    uint16_t min_position_op_range;
    uint16_t max_position_op_range;
    uint16_t op_range;
    uint16_t min_position_emergency_trigger;
    uint16_t max_position_emergency_trigger;
    ActuatorState state;
    ControlType control_type;

    char name[16] = {};

    // Option to overwrite external controller by forcing both outputs HIGH
    bool force_both_active = false;

    bool checkEmergencyBoundariesViolated();

public:
    // --- Actuator control and monitoring methods ---
    void initialize(uint16_t min_pos_op_range, uint16_t max_pos_op_range, uint16_t emergency_min, uint16_t emergency_max, uint32_t max_move_time, ControlType type);
    void setPins(uint8_t dir_a, uint8_t dir_b, uint8_t fb_a = 255, uint8_t fb_b = 255);
    void setName(const char* actuator_name) {
        strncpy(name, actuator_name, sizeof(name)-1);
        name[sizeof(name)-1] = '\0';
    }
    void setHysteresisPercentOpRange(double per_start, double per_stop) {hysteresis_percent_start = per_start; hysteresis_percent_stop = per_stop; 
        if (hysteresis_percent_start < 0.0) hysteresis_percent_start = 0.0;
        if (hysteresis_percent_start > 100.0) hysteresis_percent_start = 100.0;
        if (hysteresis_percent_stop < 0.0) hysteresis_percent_stop = 0.0;
        if (hysteresis_percent_stop > 100.0) hysteresis_percent_stop = 100.0;
        if (hysteresis_percent_stop < hysteresis_percent_start) hysteresis_percent_stop = hysteresis_percent_start;
        hysteresis_start = static_cast<uint16_t>(op_range * (hysteresis_percent_start / 100.0));
        hysteresis_stop = static_cast<uint16_t>(op_range * (hysteresis_percent_stop / 100.0));
    };
    ActuatorState getState() const { return state; }
    bool getEmergency() { return emergency_triggered;};
    void setTargetPosition(uint16_t position);
    void setControlType(ControlType type) { control_type = type; }
    void calibratePosition(uint16_t known_position); // set current position to known value (e.g., 0 or 100%)
    void calibratePositionPercent(double known_position_percent); // set current position to known percent (0.0 to 100.0)
    void moveToPercentOpRange(double position_percent); // move actuator to position in percent (0.0 to 100.0)
    void moveTo(uint16_t position); // move actuator to absolute position in operation range
    void TimePositionTrackingZeroPosition() { position_tracking_in_ms = 0; }
    void enableClampingToKnownPosition(bool enable, bool DirA_zero = true) { allow_clamping_to_known_position = enable; clamping_known_position_DirB_zero = DirA_zero; }
    void run();
    void updatePositionPercentOpRange(double position_percent); // position in percent (0.0 to 100.0) 0 = min_position, 100 = max_position
    void updatePosition(uint16_t position_value);
    uint16_t getMaxPosition() { return max_position_op_range; }
    uint16_t getMinPosition() { return min_position_op_range; }
    uint16_t getOpRange() { return op_range; }
    uint16_t getPosition() { return position; }
    double getPositionStepsPerMs() { return position_steps_per_ms; }
    void setPositionStepsPerMs(double steps_per_ms) { position_steps_per_ms = steps_per_ms; }

    // --- I/O control helpers ---
    void stopActuator();
    void moveDirA();
    void moveDirB();
    bool feedbackA() const;
    bool feedbackB() const;
};

class VehicleControl {
public:
    enum OperationMode : uint8_t {
        Idle = 0,
        ADSystemControl = 1,
        ECUTestControl = 2,
        Error = 3,
        Emergency = 4,
        Test = 5
    };

    enum GearSelection : uint8_t {
        Gear_Park = 0x50,
        Gear_Drive = 0x44,
        Gear_Other = 0xFF
    };

    VehicleControl();

    void updateHeartbeat(bool received);
    bool isAdsystemConnected() const;

    void checkHeartbeatTimeout();
    void initialize();
    void run();

    OperationMode getOperationMode() const { return operation_mode; }
    
    void adsysConnectionLostAction();
    void adsysConnectionOkAction();

    void injectSimulatedData();
    void setInjectSimulatedData(bool enable);
    bool getInjectSimulatedData() const { return inject_simulated_data; }

    void ADSystemMessagesCb(const AdsysMessage& msg);

    void updateSteeringAngle(uint16_t rawangle) { steering_actuator.updatePosition(rawangle); }
    void setTargetSteeringAngle(double position_percent) {
        if(operation_mode == ECUTestControl)
        {
            steering_actuator.moveToPercentOpRange(position_percent);
        }
    }

    void setTargetBrakePosition(double position_percent) { brake_actuator.moveToPercentOpRange(position_percent); }
    void updateGearSelection(uint8_t gear);
    void updateJoystickSteering(double steering);
    void updateJoystickThrottle(double throttle);

    void test();

    void requestOperationMode(OperationMode mode) { target_operation_mode = mode; }
    void setEmergencyMode() { operation_mode = Emergency; }

private:
    void setOperationMode(OperationMode mode) { operation_mode = mode; }

    bool adsystem_connected;
    uint32_t last_heartbeat_time; // in milliseconds
    OperationMode operation_mode; // current operation mode
    OperationMode target_operation_mode; // requested operation mode
    OperationMode initialization_state_operation_mode; // keep initialization state of selected operation mode
    GearSelection gear_selection;
    bool inject_simulated_data = false;
    ActuatorControl brake_actuator;
    ActuatorControl steering_actuator;
    uint8_t pin_led_gear_park;
    uint8_t pin_led_gear_drive;
    uint8_t pin_led_gear_other;
    uint8_t pin_led_ecu_status;
    uint8_t pin_led_ecu_error;
    uint8_t pin_ecu_trigger_emergency;

    double JoystickSteering = 0.0;
    double JoystickThrottle = 0.0;
    uint32_t JoystickSteeringTimestamp = 0;
    uint32_t JoystickThrottleTimestamp = 0;
};

extern VehicleControl vControl;

void interpreteCANframe(const CANMessage &frame); // forward declaration