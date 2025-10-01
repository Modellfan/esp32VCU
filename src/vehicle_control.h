#pragma once

#include <stdint.h>
#include <string.h>

// --- Default Actuator I/O pin definitions (can be overridden per instance) ---
#define STEERING_ACTUATOR_DIR_A_PIN   39
#define STEERING_ACTUATOR_DIR_B_PIN   36
#define STEERING_ACTUATOR_FB_A_PIN    255  // Optional
#define STEERING_ACTUATOR_FB_B_PIN    255  // Optional
#define STEERING_ACTUATOR_MIN_POSITION 2796+(192*2)
#define STEERING_ACTUATOR_MAX_POSITION 5396-(192*2)
#define STEERING_ACTUATOR_MIN_POSITION_EMERGENCY 2796
#define STEERING_ACTUATOR_MAX_POSITION_EMERGENCY 5396

#define BRAKE_ACTUATOR_DIR_A_PIN   3
#define BRAKE_ACTUATOR_DIR_B_PIN   35
#define BRAKE_ACTUATOR_FB_A_PIN    255  // Optional
#define BRAKE_ACTUATOR_FB_B_PIN    255  // Optional

#define ACTUATOR_EMERGENCY_TIMEOUT_NO_CHECK 0xFFFFFFFF
#define ACTUATOR_EMERGENCY_POSITION_LIMIT_NO_CHECK 0xFFFF

#define BRAKE_ACTUATOR_MM_TO_STEPS_FACTOR 34596.0/150.0
#define BRAKE_ACTUATOR_MIN_POSITION 0 // has to be zero, is used for driving further back (DirB) when 0 position is reached to recalibrate 0 position
#define BRAKE_ACTUATOR_MAX_POSITION_MM 32.0 // represents 100% braking
#define BRAKE_ACTUATOR_MAX_POSITION BRAKE_ACTUATOR_MAX_POSITION_MM*BRAKE_ACTUATOR_MM_TO_STEPS_FACTOR // ~34596 is full scale 150mm
#define BRAKE_ACTUATOR_MIN_POSITION_EMERGENCY 0xFFFF // no check for running over zero position; motor has stop switches. Use driving longer to recalibrate during run time when setting to zero position
#define BRAKE_ACTUATOR_MAX_POSITION_EMERGENCY BRAKE_ACTUATOR_MAX_POSITION+100
//#define BRAKE_ACTUATOR_POSITION_STEPS_PER_MS 0.052008 (estimation for sensor steps: 11.82 steps per mm; 4.4 mm/s under nominal load -> best estimation: ~0.052008 steps per ms)
#define BRAKE_ACTUATOR_POSITION_STEPS_PER_MS 1 // USING 1 HERE TO MINIMIZE ROUNDING ERRORS IN TIME-BASED APPROACH -> this scales a step by factor 19.22781 compared to hall sensor steps; 1 step then represents a propagation of 4.4 um. The full scale would be 1800*19.22=~34596 steps

#define DASHBOARD_LED_ECU_STATUS_PIN 45
#define DASHBOARD_LED_ECU_ERROR_PIN 20 // active low
#define ECU_TRIGGER_EMERGENCY_PIN 46 // active low
#define DASHBOARD_LED_GEAR_PARK_PIN 21
#define DASHBOARD_LED_GEAR_DRIVE_PIN 47
#define DASHBOARD_LED_GEAR_OTHER_PIN 37

#define EXTEND_DRIVE_TO_ZERO_TIME_MS 1000
#define BRAKE_ACTUATOR_INITIAL_DRIVE_BACK_TIME_MS 7000


struct AdsysMessage;
struct CANMessage;

class ActuatorControl {
public:
    enum ActuatorState : uint8_t {
        Inactive = 0,
        MovingDirA = 1,
        MovingDirB = 2,
        Error = 3,
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

    bool extendDriveToZeroActive = false;
    bool extendDriveToZeroStarted = false;
    uint32_t extendDriveToZeroStartTime;

    uint32_t actuator_time_since_movement_start = 0;
    uint8_t actuator_movementDir = 0; // 0 = none, 1 = DirA, 2 = DirB
    uint16_t actuator_position_since_movement_start;

    uint32_t move_start_time = 0;

    uint16_t allowed_tolerance_time_based = 20; // allowed tolerance in time-based position tracking (in position units)
    double position_steps_per_ms; // factor to convert movement duration to steps for time-based tracking
    uint16_t target_position;
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
    void setMinPosition(uint16_t pos) {min_position_op_range = pos; op_range = max_position_op_range - min_position_op_range;}
    ActuatorState getState() const { return state; }
    bool getEmergency() { return emergency_triggered;};
    void setTargetPosition(uint16_t position);
    void setControlType(ControlType type) { control_type = type; }
    void calibratePosition(uint16_t known_position); // set current position to known value (e.g., 0 or 100%)
    void calibratePositionPercent(double known_position_percent); // set current position to known percent (0.0 to 100.0)
    void moveToPercentOpRange(double position_percent); // move actuator to position in percent (0.0 to 100.0)
    void moveTo(uint16_t position); // move actuator to absolute position in operation range
    void TimePositionTrackingZeroPosition() { position_time_based = 0; }
    void enableClampingToKnownPosition(bool enable, bool DirA_zero = true) { allow_clamping_to_known_position = enable; clamping_known_position_DirB_zero = DirA_zero; }
    void run();
    void resetTimeBasedPosition(double percent) {position_time_based = min_position_op_range + op_range * percent / 100.0;}
    void updatePositionPercentOpRange(double position_percent); // position in percent (0.0 to 100.0) 0 = min_position, 100 = max_position
    void updatePosition(uint16_t position_value);
    uint16_t getMaxPosition() { return max_position_op_range; }
    uint16_t getMinPosition() { return min_position_op_range; }
    uint16_t getOpRange() { return op_range; }
    uint16_t getPosition() { return position; }
    uint16_t getPositionTimeBased() { return position_time_based;}
    double getPositionStepsPerMs() { return position_steps_per_ms; }
    void setPositionStepsPerMs(double steps_per_ms) { position_steps_per_ms = steps_per_ms; }
    void activateExtendDriveToZero();

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
        Idle = 0,               // no control, but monitoring, vehicle should be stationary, emergency actions enabled
        ADSystemControl = 1,    // normal operation, controlled by AD system. Steering and brake actuator controlled directly by AD System, steering angle and other vehicle data is forwarded from vehicle CAN message. ECU controls inverter according to inputs from AD System. ECU monitors.
        ECUTestControl = 2,     // test mode, controlled by ECU. Steering and brake actuator and motor controlled by ECU via joystick input
        Error = 3,              // error state, try to stabilize vehicle
        Emergency = 4,          // emergency state, trigger emergency actions (open safety circuit, switch off HV and trigger emergency brake)
        Test = 5,               // test mode, individual test routines
    };

    enum TestMode : uint8_t {
        Test_None = 0,
        Test_BrakeSteeringActuatorJoystick = 1,
        Test_InverterJoystick = 2,
        Test_FullSystem = 3
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

    void requestOperationMode(OperationMode mode) { target_operation_mode = mode; }
    void setEmergencyMode();
    OperationMode getOperationMode() const { return operation_mode; }

    TestMode getTestMode() const { return test_mode; }
    
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
    double getSteeringAnglePercent() {return (((double) steering_actuator.getPosition() - steering_actuator.getMinPosition()) / (double) steering_actuator.getOpRange() * 100.0);}

    void setTargetBrakePosition(double position_percent);
    void updateGearSelection(uint8_t gear);
    void updateJoystickSteering(double steering);
    void updateJoystickThrottle(double throttle);

    void test();

private:
    void setOperationMode(OperationMode mode) { operation_mode = mode; }

    bool adsystem_connected;
    uint32_t last_heartbeat_time; // in milliseconds
    OperationMode operation_mode; // current operation mode
    OperationMode target_operation_mode; // requested operation mode
    OperationMode initialization_state_operation_mode; // keep initialization state of selected operation mode
    TestMode test_mode;
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