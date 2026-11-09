// eBoxster VCU – startup only. Layers = folders in src/ (documentation/architecture.svg):
//   app/       application: vcu_app.cpp, cluster_calc.cpp
//   data/      data (RTE): vcu_params.cpp, signal_table.cpp; params::* also in the can/ codecs
//   can/       communication: 986_*_messages.cpp, tesla_sdu_messages.cpp, battery_messages.cpp
//   drivers/   canmanager.cpp, can3_twai.cpp, coolant_fan_control.cpp, digipot_control.cpp, status_led.h
//   services/  vcu_rt.cpp (6 ms real-time task, core 1), webinterface.cpp (core 0), gvret_bridge.cpp, vcu_log.cpp
#include <Arduino.h>

#include "board.h"
#include "can3_twai.h"
#include "canmanager.h"
#include "coolant_fan_control.h"
#include "digipot_control.h"
#include "gvret_bridge.h"
#include "status_led.h"
#include "vcu_params.h"
#include "vcu_rt.h"
#include "webinterface.h"

void setup() {
    Serial.begin(115200);  // UART0: GVRET only
#if VCU_LOG_ENABLED
    vcuLogBegin();
#endif
    paramsLockInit();
    fastledInit();
    digipotBegin();
    coolantFanBegin();
    canManager_setup();
    can3Begin();
    gvretBridgeBegin();

    // ECU emulation first: frames go out before WiFi is even started (VCU-REQ-002).
    rtBegin();
    webinterfaceBegin();
    VCU_LOG("[VCU] started, rt period %u ms\n", (unsigned)RT_PERIOD_MS);
}

void loop() {
    // All work runs in the real-time task (core 1) and the service task (core 0).
    vTaskDelay(pdMS_TO_TICKS(1000));
}
