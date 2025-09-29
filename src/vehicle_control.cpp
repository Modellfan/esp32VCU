#include "vehicle_control.h"
#include "adsystem_interface.h"
#include <Arduino.h>
#include <ACAN2515_CANMessage.h>
//#include "main.h"

#define LOG_VEHICLE_CONTROL 1
#if (LOG_VEHICLE_CONTROL==1)
    #define LOG_MSG(x) sendDebugMessage(x)
#else
    #define LOG_MSG(x)
#endif

VehicleControl vControl;

// --- ActuatorControl Implementation ---

void ActuatorControl::initialize(uint16_t min_pos_op_range, uint16_t max_pos_op_range, uint16_t emergency_min, uint16_t emergency_max, uint32_t max_move_time, ControlType type) {
    min_position_op_range = min_pos_op_range;
    max_position_op_range = max_pos_op_range;
    op_range = max_position_op_range - min_position_op_range;
    min_position_emergency_trigger = emergency_min;
    max_position_emergency_trigger = emergency_max;
    max_move_time_ms = max_move_time;
    control_type = type;
    position = min_pos_op_range;
    position_time_based = min_pos_op_range;
    position_timestamp = millis();
    target_position = min_pos_op_range;
    position_tracking_in_ms = 0;
    position_tracking_start_time_current_move = 0;
    emergency_triggered = false;
    position_known_time_tracking = false;
    state = Inactive;

    setHysteresisPercentOpRange(2.0, 5.0);

    position_steps_per_ms = 0;

    LOG_MSG("[" + String(name) + "] Initialized: min=" + String(min_position_op_range) + ", max=" + String(max_position_op_range) +
            ", emergency_min=" + String(min_position_emergency_trigger) + ", emergency_max=" + String(max_position_emergency_trigger) +
            ", max_move_time_ms=" + String(max_move_time_ms) + ", mode=" + String(control_type) +
            ", pin_dir_a=" + String(pin_dir_a) + ", pin_dir_b=" + String(pin_dir_b) +
            ", pin_fb_a=" + String(pin_fb_a) + ", pin_fb_b=" + String(pin_fb_b));
}

void ActuatorControl::setPins(uint8_t dir_a, uint8_t dir_b, uint8_t fb_a, uint8_t fb_b)
{
    pin_dir_a = dir_a; pin_dir_b = dir_b; pin_fb_a = fb_a; pin_fb_b = fb_b;
    pinMode(pin_dir_a, OUTPUT);
    pinMode(pin_dir_b, OUTPUT);
    if (pin_fb_a != 255) pinMode(pin_fb_a, INPUT);
    if (pin_fb_b != 255) pinMode(pin_fb_b, INPUT);
    stopActuator();
}

bool ActuatorControl::checkEmergencyBoundariesViolated()
{
    if (( min_position_emergency_trigger != 0xFFFF && position <= min_position_emergency_trigger) || ( max_position_emergency_trigger != 0xFFFF && position >= max_position_emergency_trigger)) {
        emergency_triggered = true;
        state = Error;
        LOG_MSG("[" + String(name) + "][EMERGENCY] Sensor-based position out of range: " + String(position));
        stopActuator();
        return true;
    }
    return false;
}

void ActuatorControl::moveToPercentOpRange(double position_percent) {
    if (position_percent < 0.0) position_percent = 0.0;
    if (position_percent > 100.0) position_percent = 100.0;
    uint16_t target = min_position_op_range + static_cast<uint16_t>(op_range * (position_percent / 100.0));

    setTargetPosition(target);
}

void ActuatorControl::moveTo(uint16_t position_value) {
    if (position_value < min_position_op_range) position_value = min_position_op_range;
    if (position_value > max_position_op_range) position_value = max_position_op_range;
    uint16_t target = position_value;

    setTargetPosition(target);
}

void ActuatorControl::setTargetPosition(uint16_t pos) {
    if (pos < min_position_op_range) pos = min_position_op_range;
    if (pos > max_position_op_range) pos = max_position_op_range;

    if(target_position != pos)
    {
        LOG_MSG("[" + String(name) + "] target=" + String(target_position));
    }

    target_position = pos;
}

void ActuatorControl::updatePositionPercentOpRange(double position_percent) {
    if (position_percent < 0.0) position_percent = 0.0;
    if (position_percent > 100.0) position_percent = 100.0;
    uint16_t pos = min_position_op_range + static_cast<uint16_t>(op_range * (position_percent / 100.0));
    position = pos;
    position_timestamp = millis();

    LOG_MSG("[" + String(name) + "] updatePosition: percent=" + String(position_percent, 2) + ", pos=" + String(pos));

    checkEmergencyBoundariesViolated();
}

void ActuatorControl::updatePosition(uint16_t position_value) {
    position = position_value;
    position_timestamp = millis();

    LOG_MSG("[" + String(name) + "] updatePosition: value=" + String(position_value, 2) + ", pos=" + String(position));

    checkEmergencyBoundariesViolated();
}

void ActuatorControl::run() {
    uint32_t now = millis();

    LOG_MSG("[" + String(name) + "] run. State: " + String(state));


    bool dirA_active = digitalRead(pin_dir_a) == HIGH;
    bool dirB_active = digitalRead(pin_dir_b) == HIGH;

    bool dirA_fb_active = feedbackA();
    bool dirB_fb_active = feedbackB();

    if (force_both_active) {
        digitalWrite(pin_dir_a, HIGH);
        digitalWrite(pin_dir_b, HIGH);
        LOG_MSG("[" + String(name) + "] Forcing both outputs HIGH to stop external controller!");
        return;
    }

    static uint32_t both_active_start = 0;
    if (dirA_active && dirB_active) {
        if (both_active_start == 0) both_active_start = now;
        // No error, just track how long both are active if needed
    } else {
        both_active_start = 0;
    }

    if ((state == Error || emergency_triggered)) {
        stopActuator();
        LOG_MSG("[" + String(name) + "][ERROR] Emergency or error state active, outputs stopped.");
        return;
    }

    // --- Time-based Position Tracking ---
    static uint32_t actuator_time_since_movement_start = now;
    static uint8_t actuator_movementDir = 0; // 0 = none, 1 = DirA, 2 = DirB
    static uint16_t actuator_position_since_movement_start = position_time_based;

    // simulate according to actuator controls
    if(state == ActuatorControl::MovingDirA)
    {
        if(actuator_movementDir != 1)
        {
            actuator_movementDir = 1;
            actuator_time_since_movement_start = now;
            actuator_position_since_movement_start = position_time_based;
        }
        position_time_based = actuator_position_since_movement_start + position_steps_per_ms * (now - actuator_time_since_movement_start);
    }
    else if(state == ActuatorControl::MovingDirB)
    {
        if(actuator_movementDir != 2)
        {
            actuator_movementDir = 2;
            actuator_time_since_movement_start = now;
            actuator_position_since_movement_start = position_time_based;
        }
        int32_t iactuator_position = (int32_t) actuator_position_since_movement_start - position_steps_per_ms * (now - actuator_time_since_movement_start);
        position_time_based = iactuator_position < 0 ? 0 : (uint16_t) iactuator_position;
    }
    else
    {
        actuator_movementDir = 0;
    }

    // --- max_move_time_ms limit check ---
    if ((control_type == ActiveControlWithFeedback || control_type == ActiveControlWithTime) &&
        (state == MovingDirA || state == MovingDirB)) {
        if (now - position_tracking_start_time_current_move > max_move_time_ms) {
            stopActuator();
            state = Error;
            emergency_triggered = true;
            LOG_MSG("[" + String(name) + "][ERROR] Move time exceeded max_move_time_ms!");
            return;
        }
    }
    static uint32_t move_start_time = 0;
    if (control_type == MonitoringOnly && (dirA_fb_active || dirB_fb_active)) {
        if (move_start_time == 0) move_start_time = now;
        if (now - move_start_time > max_move_time_ms) {
            state = Error;
            emergency_triggered = true;
            LOG_MSG("[" + String(name) + "][ERROR] Monitoring: Move time exceeded max_move_time_ms!");
        }
    }
    if (control_type == MonitoringOnly && !dirA_fb_active && !dirB_fb_active) {
        move_start_time = 0;
    }

    bool emergency_now_sensor = (( min_position_emergency_trigger != 0xFFFF && position <= min_position_emergency_trigger) || ( max_position_emergency_trigger != 0xFFFF && position >= max_position_emergency_trigger));
    bool emergency_now_time = (( min_position_emergency_trigger != 0xFFFF && position_time_based <= min_position_emergency_trigger) || ( max_position_emergency_trigger != 0xFFFF && position_time_based >= max_position_emergency_trigger));

    if (emergency_now_sensor || emergency_now_time) {
        emergency_triggered = true;
        state = Error;
        LOG_MSG("[" + String(name) + "][EMERGENCY] Position out of range! Sensor: " + String(position) + ", Time-based: " + String(position_time_based));
        if (control_type == MonitoringOnly) {
            if (dirA_fb_active || dirB_fb_active) {
                force_both_active = true;
                LOG_MSG("[" + String(name) + "][EMERGENCY] Forcing both outputs HIGH to stop external controller!");
            }
        } else {
            stopActuator();
        }
        return;
    } else {
        if (emergency_triggered &&
            (position > min_position_emergency_trigger || min_position_emergency_trigger == 0xFFFF) && (position < max_position_emergency_trigger || max_position_emergency_trigger == 0xFFFF) &&
            (position_time_based > min_position_emergency_trigger || min_position_emergency_trigger == 0xFFFF) && (position_time_based < max_position_emergency_trigger || max_position_emergency_trigger == 0xFFFF)) {
            emergency_triggered = false;
            if (state == Error) state = Inactive;
            force_both_active = false;
            LOG_MSG("[" + String(name) + "] Emergency cleared, actuator back in range.");
        }
    }

    // --- Actuator Stop Movement ---
    if (control_type == ActiveControlWithFeedback) {
        if (state == MovingDirA && position >= (target_position - hysteresis_stop) ) {
            stopActuator();
            state = Inactive;
            LOG_MSG("[" + String(name) + "] Target reached (sensor-based), stopped.");
        } else if (state == MovingDirB && position <= (target_position + hysteresis_stop)) {
            stopActuator();
            state = Inactive;
            LOG_MSG("[" + String(name) + "] Target reached (sensor-based), stopped.");
        }
    } else if (control_type == ActiveControlWithTime) {
        if (state == MovingDirA && position_time_based >= (target_position - hysteresis_stop)) {
            stopActuator();
            state = Inactive;
            LOG_MSG("[" + String(name) + "] Target reached (time-based), stopped.");
        } else if (state == MovingDirB && position_time_based <= (target_position + hysteresis_stop)) {
            stopActuator();
            state = Inactive;
            LOG_MSG("[" + String(name) + "] Target reached (time-based), stopped.");
        }
    } else if (control_type == MonitoringOnly) {
        // Do not control outputs in MonitoringOnly mode, except for emergency above!
        return;
    }

    // --- Actuator Start Movement ---
    if(state == Inactive && ((static_cast<int32_t>(position) <= static_cast<int32_t>(target_position) - static_cast<int32_t>(hysteresis_start)) ||
       (static_cast<int32_t>(position) >= static_cast<int32_t>(target_position) + static_cast<int32_t>(hysteresis_start)) ))
    {
        // out of target range, start movement

        position_tracking_start_time_current_move = now;

        LOG_MSG("[" + String(name) + "] out of target range, start movement: target=" + String(target_position) + ", mode=" + String(control_type));

        if (control_type == ActiveControlWithFeedback) {
            if (target_position > position) {
                state = MovingDirA;
                moveDirA();
                LOG_MSG("[" + String(name) + "] Moving DirA (sensor-based)");
            } else if (target_position < position) {
                state = MovingDirB;
                moveDirB();
                LOG_MSG("[" + String(name) + "] Moving DirB (sensor-based)");
            } else {
                state = Inactive;
                stopActuator();
                LOG_MSG("[" + String(name) + "] Already at target (sensor-based)");
            }
        } else if (control_type == ActiveControlWithTime) {
            if (target_position > position_time_based) {
                state = MovingDirA;
                moveDirA();
                position_known_time_tracking = true;
                LOG_MSG("[" + String(name) + "] Moving DirA (time-based)");
            } else if (target_position < position_time_based) {
                state = MovingDirB;
                moveDirB();
                position_known_time_tracking = true;
                LOG_MSG("[" + String(name) + "] Moving DirB (time-based)");
            } else {
                state = Inactive;
                stopActuator();
                position_known_time_tracking = false;
                LOG_MSG("[" + String(name) + "] Already at target (time-based)");
            }
        }
    }
}

void ActuatorControl::stopActuator() {
    digitalWrite(pin_dir_a, LOW);
    digitalWrite(pin_dir_b, LOW);
    state = Inactive;
    LOG_MSG("[" + String(name) + "] stopActuator: Outputs set LOW.");
}
void ActuatorControl::moveDirA() {
    digitalWrite(pin_dir_a, HIGH);
    digitalWrite(pin_dir_b, LOW);
    state = MovingDirA;
    LOG_MSG("[" + String(name) + "] moveDirA: DirA=HIGH, DirB=LOW");
}
void ActuatorControl::moveDirB() {
    digitalWrite(pin_dir_a, LOW);
    digitalWrite(pin_dir_b, HIGH);
    state = MovingDirB;
    LOG_MSG("[" + String(name) + "] moveDirB: DirA=LOW, DirB=HIGH");
}
bool ActuatorControl::feedbackA() const {
    if(pin_fb_a == 255) return false;
    bool fb = digitalRead(pin_fb_a) == HIGH;
    //LOG_MSG("[" + String(name) + "] feedbackA: " + String(fb));
    return fb;
}
bool ActuatorControl::feedbackB() const {
    if(pin_fb_b == 255) return false;
    bool fb = digitalRead(pin_fb_b) == HIGH;
    //LOG_MSG("[" + String(name) + "] feedbackB: " + String(fb));
    return fb;
}

// --- VehicleControl Implementation ---

VehicleControl::VehicleControl() {
    //initialize(); // Moved to main.cpp after CAN and ADSystem initialization
}

void VehicleControl::test()
{
    brake_actuator.moveDirA();
    sleep(2);
    brake_actuator.moveDirB();
    sleep(2);
    brake_actuator.stopActuator();

    sleep(2);

    steering_actuator.moveDirA();
    sleep(2);
    steering_actuator.moveDirB();
    sleep(4);
    steering_actuator.moveDirA();
    sleep(2);
    steering_actuator.stopActuator();
}

void VehicleControl::initialize() {
    adsystem_connected = false;
    last_heartbeat_time = millis();
    operation_mode = Idle;
    target_operation_mode = ECUTestControl;
    initialization_state_operation_mode = Idle;
    gear_selection = Gear_Other;
    inject_simulated_data = true;

    brake_actuator.setPins(BRAKE_ACTUATOR_DIR_A_PIN, BRAKE_ACTUATOR_DIR_B_PIN, BRAKE_ACTUATOR_FB_A_PIN, BRAKE_ACTUATOR_FB_B_PIN);
    brake_actuator.initialize(0, 120, 0xFFFF, 120+12, 20000, ActuatorControl::ActiveControlWithTime); // actual max position is 1800 steps; just use a low value for now; 120 steps = ~10mm
    //brake_actuator.initialize(0, 120, 0xFFFF, 120+12, 20000, ActuatorControl::MonitoringOnly); // actual max position is 1800 steps; just use a low value for now
    brake_actuator.setPositionStepsPerMs(0.052008); // 11.82 position units per mm; 4.4mm/s; 150mm length (i.e., 34091 ms for full range)
    brake_actuator.setName("Brake");
    brake_actuator.enableClampingToKnownPosition(true, true);
    brake_actuator.setHysteresisPercentOpRange(2.0, 5.0);
    steering_actuator.setPins(STEERING_ACTUATOR_DIR_A_PIN, STEERING_ACTUATOR_DIR_B_PIN, STEERING_ACTUATOR_FB_A_PIN, STEERING_ACTUATOR_FB_B_PIN);
    steering_actuator.initialize(2796+96, 5396-96, 2796-96, 5396+96, 20000, ActuatorControl::ActiveControlWithFeedback);
    //steering_actuator.initialize(2796+96, 5396-96, 2796-96, 5396+96, 20000, ActuatorControl::MonitoringOnly);
    steering_actuator.setPositionStepsPerMs(0.16); // 0.16 position units per ms; 6.0mm/s; 165mm length (i.e., 1031 ms for full range)
    steering_actuator.setName("Steering");
    steering_actuator.setHysteresisPercentOpRange(2.5, 3.5);

    pin_led_gear_park = DASHBOARD_LED_GEAR_PARK_PIN;
    pin_led_gear_drive = DASHBOARD_LED_GEAR_DRIVE_PIN;
    pin_led_gear_other = DASHBOARD_LED_GEAR_OTHER_PIN;
    pin_led_ecu_status = DASHBOARD_LED_ECU_STATUS_PIN;
    pin_led_ecu_error = DASHBOARD_LED_ECU_ERROR_PIN;
    pin_ecu_trigger_emergency = ECU_TRIGGER_EMERGENCY_PIN;

    pinMode(pin_led_gear_park, OUTPUT);
    pinMode(pin_led_gear_drive, OUTPUT);
    pinMode(pin_led_gear_other, OUTPUT);
    pinMode(pin_led_ecu_status, OUTPUT);
    pinMode(pin_led_ecu_error, OUTPUT);
    pinMode(pin_ecu_trigger_emergency, OUTPUT);

    digitalWrite(pin_ecu_trigger_emergency, HIGH); // not triggering emergency

    // LED test pattern at initialization
    digitalWrite(pin_led_ecu_error, LOW);
    digitalWrite(pin_led_ecu_status, HIGH);
    digitalWrite(pin_led_gear_park, HIGH);
    digitalWrite(pin_led_gear_drive, HIGH);
    digitalWrite(pin_led_gear_other, HIGH);
    delay(500);
    digitalWrite(pin_led_ecu_error, HIGH);
    digitalWrite(pin_led_ecu_status, LOW);
    digitalWrite(pin_led_gear_park, LOW);
    digitalWrite(pin_led_gear_drive, LOW);
    digitalWrite(pin_led_gear_other, LOW);
    delay(500);
    digitalWrite(pin_led_ecu_error, LOW);
    digitalWrite(pin_led_ecu_status, HIGH);
    digitalWrite(pin_led_gear_park, HIGH);
    digitalWrite(pin_led_gear_drive, HIGH);
    digitalWrite(pin_led_gear_other, HIGH);
    delay(500);

    digitalWrite(pin_led_ecu_error, HIGH);

    //test();

    LOG_MSG("[VehicleControl] Move brake actuator to zero position to track position.");
    brake_actuator.moveDirB();
    delay(5000);
    brake_actuator.stopActuator();

    LOG_MSG("[VehicleControl] Initialized.");
}

void VehicleControl::updateHeartbeat(bool received) {
    if (received) {
        last_heartbeat_time = millis();
        if (!adsystem_connected) {
            adsystem_connected = true;
            adsysConnectionOkAction();
        }
    }
}

bool VehicleControl::isAdsystemConnected() const {
    return adsystem_connected;
}

void VehicleControl::checkHeartbeatTimeout() {
    const uint32_t HEARTBEAT_TIMEOUT_MS = 500;
    uint32_t current_time = millis();
    if (current_time > last_heartbeat_time + HEARTBEAT_TIMEOUT_MS) {
        if(adsystem_connected) {
            adsystem_connected = false;
            if(operation_mode != ADSystemControl)
            {
                operation_mode = Emergency; // go directly to emergency mode, since ECU can't control normal brake in mode ADSystemControl
            }
            adsysConnectionLostAction();
        }
    }
    if (current_time < last_heartbeat_time) {
        last_heartbeat_time = current_time;
    }
}

void VehicleControl::adsysConnectionLostAction() {
    neopixelWrite(RGB_BUILTIN, 20, 0, 0); // Red
}
void VehicleControl::adsysConnectionOkAction()
{
    neopixelWrite(RGB_BUILTIN, 0, 20, 0); // Green
}

// is called periodically, every 10ms by the scheduler in main.cpp
void VehicleControl::run() {
    checkHeartbeatTimeout();

    if(inject_simulated_data)
    {
        injectSimulatedData();
    }

    const uint32_t HEARTBEAT_INTERVAL_MS = 100;
    static uint32_t last_heartbeat_sent_time = 0;
    uint32_t current_time = millis();
    if (current_time >= last_heartbeat_sent_time + HEARTBEAT_INTERVAL_MS || current_time < last_heartbeat_sent_time /* handle millis() overflow */) {
        adsysHandler.sendHeartbeat();
        last_heartbeat_sent_time = current_time;
    }

    brake_actuator.run();
    steering_actuator.run();

    if((steering_actuator.getEmergency() || brake_actuator.getEmergency()))
    {
        digitalWrite(pin_ecu_trigger_emergency, LOW); // trigger emergency
        digitalWrite(pin_led_ecu_error, LOW); // turn on error LED
    }
}

void VehicleControl::ADSystemMessagesCb(const AdsysMessage& msg)
{
    switch (msg.type)
    {
    case AdsysMsgType::HEARTBEAT:
        if (msg.payload.size() >= 1) {
            bool heartbeat_received = ((msg.payload[0] == 0) || (msg.payload[0] == 1));
            updateHeartbeat(heartbeat_received);
        }
        break;
    case AdsysMsgType::TORQUE_REQUEST:
        if (msg.payload.size() >= 2) {
            int16_t torque_request = (static_cast<int16_t>(msg.payload[0]) << 8) | static_cast<int16_t>(msg.payload[1]);
            // Process torque_request as needed
            LOG_MSG("[VehicleControl] Received Torque Request: " + String(torque_request));

            double Y_inject = static_cast<double>(torque_request) / 0xFFFF * 100.0; // Convert to percentage (-100% to +100%)
        }
        break;
    case AdsysMsgType::SET_SIMULATED_DATA_INJECTION:
        if (msg.payload.size() >= 1) {
            bool enable = (msg.payload[0] != 0);
            setInjectSimulatedData(enable);
        }
    }
}

void VehicleControl::updateJoystickSteering(double steering)
{
    JoystickSteering = steering; JoystickSteeringTimestamp = millis();
}
void VehicleControl::updateJoystickThrottle(double throttle)
{
    JoystickThrottle = throttle; JoystickThrottleTimestamp = millis();
}

void VehicleControl::setInjectSimulatedData(bool enable)
{
    inject_simulated_data = enable;
}

void VehicleControl::injectSimulatedData()
{
    static uint16_t angle = 4096; // start at center (0 degrees)
    static int8_t angle_direction = 1;   // 1 = increasing angle, -1 = decreasing angle
    static uint16_t brake_position = 0;
    static int8_t brake_direction = 1;

    if(operation_mode == ADSystemControl)
    {
        // just oscillate between full left and full right
        angle += angle_direction * 64;
        if(angle >= 5396) {
            angle = 5396;
            angle_direction = -1;
        } else if(angle <= 2796) {
            angle = 2796;
            angle_direction = 1;
        }

        // just oscillate between full pressed and full released
        int32_t ibrake_position = brake_position + (brake_direction * 64);
        if(ibrake_position >= 1800) {
            ibrake_position = 1800;
            brake_direction = -1;
        } else if(ibrake_position <= 0) {
            ibrake_position = 0;
            brake_direction = 1;
        }
        brake_position = (uint16_t) ibrake_position;
    }
    else if(operation_mode == ECUTestControl)
    {
        static uint32_t angle_time_since_movement_start = millis();
        static uint8_t angle_movementDir = 0; // 0 = none, 1 = DirA, 2 = DirB
        static uint16_t angle_position_since_movement_start = angle;

        // simulate according to actuator controls
        if(steering_actuator.getState() == ActuatorControl::MovingDirA)
        {
            if(angle_movementDir != 1)
            {
                angle_movementDir = 1;
                angle_time_since_movement_start = millis();
                angle_position_since_movement_start = angle;
            }
            angle = angle_position_since_movement_start + steering_actuator.getPositionStepsPerMs() * (millis() - angle_time_since_movement_start);
        }
        else if(steering_actuator.getState() == ActuatorControl::MovingDirB)
        {
            if(angle_movementDir != 2)
            {
                angle_movementDir = 2;
                angle_time_since_movement_start = millis();
                angle_position_since_movement_start = angle;
            }
            angle = angle_position_since_movement_start - steering_actuator.getPositionStepsPerMs() * (millis() - angle_time_since_movement_start);
        }
        else
        {
            angle_movementDir = 0;
        }

        static uint32_t brake_time_since_movement_start = millis();
        static uint8_t brake_movementDir = 0; // 0 = none, 1 = DirA, 2 = DirB
        static uint16_t brake_position_since_movement_start = brake_position;

        // simulate according to actuator controls
        if(brake_actuator.getState() == ActuatorControl::MovingDirA)
        {
            if(brake_movementDir != 1)
            {
                brake_movementDir = 1;
                brake_time_since_movement_start = millis();
                brake_position_since_movement_start = brake_position;
            }
            brake_position = brake_position_since_movement_start + brake_actuator.getPositionStepsPerMs() * (millis() - brake_time_since_movement_start);
        }
        else if(brake_actuator.getState() == ActuatorControl::MovingDirB)
        {
            if(brake_movementDir != 2)
            {
                brake_movementDir = 2;
                brake_time_since_movement_start = millis();
                brake_position_since_movement_start = brake_position;
            }
            int32_t ibrake_position = (int32_t) brake_position_since_movement_start - brake_actuator.getPositionStepsPerMs() * (millis() - brake_time_since_movement_start);
            brake_position = ibrake_position < 0 ? 0 : (uint16_t) ibrake_position;
        }
        else
        {
            brake_movementDir = 0;
        }
    }

    CANMessage steeringFrame = {};
    steeringFrame.id = 0x236; // Steering angle ID
    steeringFrame.data[0] = (angle >> 8) & 0xFF;
    steeringFrame.data[1] = angle & 0xFF;
    
    interpreteCANframe(steeringFrame);
}

void VehicleControl::updateGearSelection(uint8_t gear) {
    switch (gear) {
        case 0x50:
            gear_selection = Gear_Park;
            digitalWrite(pin_led_gear_park, HIGH);
            digitalWrite(pin_led_gear_drive, LOW);
            digitalWrite(pin_led_gear_other, LOW);
            break;
        case 0x44:
            gear_selection = Gear_Drive;
            digitalWrite(pin_led_gear_park, LOW);
            digitalWrite(pin_led_gear_drive, HIGH);
            digitalWrite(pin_led_gear_other, LOW);
            break;
        default:
            gear_selection = Gear_Other;
            digitalWrite(pin_led_gear_park, LOW);
            digitalWrite(pin_led_gear_drive, LOW);
            digitalWrite(pin_led_gear_other, HIGH);
            break;
    }
}