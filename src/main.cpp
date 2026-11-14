// eBoxster VCU – the whole sequence in one place (VCU-DEC-015).
//
// loop() (core 1): pollVehicleCAN, pollTeslaCAN, pollBatteryCAN, pollSpareCAN continuously;
//                  updateOutputs (fan, fuel gauge) every 100 ms; CAN error counters every 300 ms.
// ecuTask (core 1, every 10 ms, higher priority): updateCluster -> emulateECUCAN (5 ECU frames).
// Web task (core 0, lowest priority, src/services/webinterface.cpp): WiFi, web API, PWA,
//                  GVRET, log, LED. It starts WiFi itself, so CAN runs before WiFi is up.
// Periods, buffers and priorities: params::can (src/drivers/canmanager_api.h).
//
// Every module owns its data (params::* in its header). The tasks share it under the
// params lock (src/services/params_lock.h).
//
// Folders: app/ (cluster calculation), comms/ (CAN codecs),
// drivers/ (CAN manager with receive monitor, fan, fuel gauge, EEPROM, LED), services/ (web, signal table, lock, GVRET, log).
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "battery_messages.h"
#include "board.h"
#include "canmanager_api.h"
#include "canmanager.h"
#include "cluster_calc.h"
#include "coolant_fan_control.h"
#include "digipot_control.h"
#include "eeprom_at24c.h"
#include "gvret_bridge.h"
#include <esp_task_wdt.h>
#include <esp_system.h>
#include "params_lock.h"
#include "persistence.h"
#include "serial_console.h"
#include "status_led.h"
#include "tesla_sdu_messages.h"
#include "webinterface.h"

//---------------------------------------------------------------------------
// CAN receive: decode, mark the reception time, mirror to GVRET.
//---------------------------------------------------------------------------
// Frames per bus and call: params::can.rx_budget_per_bus; the rest waits for the next call.

// CAN1: Porsche 986 vehicle messages
void pollVehicleCAN() {
    CANMessage frame;
    for (int budget = params::can.rx_budget_per_bus; budget > 0 && can1.available(); budget--) {
        can1.receive(frame);
        canCountRx(1);
        if (decode986Vehicle(frame)) {
            rxMark(BUS_VEHICLE, frame.id);
        }
        gvretBridgeObserve(frame, GVRET_BUS_CAN1);
    }
}

// CAN2: Tesla SDU (OpenInverter)
void pollTeslaCAN() {
    CANMessage frame;
    for (int budget = params::can.rx_budget_per_bus; budget > 0 && can2.available(); budget--) {
        can2.receive(frame);
        canCountRx(2);
        if (decodeTeslaSdu(frame)) {
            rxMark(BUS_DRIVE, frame.id);
        }
        gvretBridgeObserve(frame, GVRET_BUS_CAN2);
    }
}

// CAN0: BMS / charger (TWAI)
void pollBatteryCAN() {
    CANMessage frame;
    for (int budget = params::can.rx_budget_per_bus; budget > 0 && can0Receive(frame); budget--) {
        canCountRx(0);
        if (decodeBattery(frame)) {
            rxMark(BUS_BATTERY, frame.id);
        }
        gvretBridgeObserve(frame, GVRET_BUS_CAN0);
    }
}

// CAN3: spare bus, counted and mirrored to GVRET only
void pollSpareCAN() {
    CANMessage frame;
    for (int budget = params::can.rx_budget_per_bus; budget > 0 && can3.available(); budget--) {
        can3.receive(frame);
        canCountRx(3);
        gvretBridgeObserve(frame, GVRET_BUS_CAN3);
    }
}

//---------------------------------------------------------------------------
// CAN1: Porsche 986 ECU emulation (VCU-DEC-020).
// Like the original DME: all 5 frames back to back every 10 ms, in the original order.
//---------------------------------------------------------------------------
constexpr uint8_t ECU_FRAME_COUNT = 5;

void emulateECUCAN(CANMessage (&frames)[ECU_FRAME_COUNT]) {
    encode986Ecu0x280(frames[0]);
    encode986Ecu0x289(frames[1]);
    encode986Ecu0x298(frames[2]);
    encode986Ecu0x7A0(frames[3]);
    encode986Ecu0x4E0(frames[4]);
}

//---------------------------------------------------------------------------
// Outputs every 100 ms: fan automatic and relays, fuel gauge potentiometer.
//---------------------------------------------------------------------------
constexpr uint32_t OUTPUT_PERIOD_MS = 100;

void updateOutputs(uint32_t nowMs) {
    uint8_t fanLevel;
    uint8_t fuelGaugePct;
    CANMessage heaterFrame;
    {
        ParamsGuard guard;
        coolantFanAutoUpdate(nowMs);
        digipotClampRequest();
        fanLevel = params::fan.fan_level;
        fuelGaugePct = params::digipot.digipot_pct;
        encodeHeaterControl(heaterFrame);  // 0x43A to the Zombie-Slave (VCU-REQ-021)
    }
    // Hardware outside the lock (the potentiometer needs about 2 ms per change).
    coolantFanWrite(fanLevel >= 1U, fanLevel >= 2U);
    digipotApply(fuelGaugePct);
    can0Send(heaterFrame);  // every 100 ms; the Zombie-Slave drops the request after 500 ms
    ParamsGuard guard;
    coolantFanReportState();
    digipotReportState();
}

//---------------------------------------------------------------------------
// ECU task (core 1, every 10 ms): Kombi values, then the 5 ECU frames on CAN1.
//---------------------------------------------------------------------------
void ecuTask(void *) {
    esp_task_wdt_add(nullptr);  // review B-05: a hanging ECU task resets the VCU
    TickType_t lastWake = xTaskGetTickCount();
    for (;;) {
        esp_task_wdt_reset();
        const uint32_t startUs = micros();
        CANMessage frames[ECU_FRAME_COUNT];
        {
            ParamsGuard guard;
            updateCluster();  // cluster values, then ECU overrides
            emulateECUCAN(frames);
        }
        bool sent = canBusUp(1);
        if (sent) {
            for (const CANMessage &frame : frames) {
                sent = can1.tryToSend(frame) && sent;  // driver queue holds 16 frames
                gvretBridgeObserve(frame, GVRET_BUS_TX);
            }
        }
        canTaskStatsRecord(millis(), micros() - startUs, sent);
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(params::can.ecu_period_ms));
    }
}

//---------------------------------------------------------------------------
// Setup and Loop
//---------------------------------------------------------------------------
void setup() {
    Serial.begin(board::SERIAL_BAUD);  // UART0: text log, GVRET after 0xE7
#if VCU_LOG_ENABLED
    vcuLogBegin();
#endif
    serialConsolePrintBuildInfo();
    paramsLockInit();
    fastledInit();
    digipotBegin();
    coolantFanBegin();
    canManager_setup();  // CAN0 (TWAI), CAN1..CAN3: receive from here on
    gvretBridgeBegin();

    // Task watchdog (review B-05): ECU task and loop() must report within 3 s, else reset.
    esp_task_wdt_config_t wdt = {3000, 0, true};  // timeout ms, no idle tasks, panic + reset
    if (esp_task_wdt_reconfigure(&wdt) != ESP_OK) {
        esp_task_wdt_init(&wdt);
    }
    enableLoopWDT();
    VCU_LOG("[VCU] reset reason %d\n", (int)esp_reset_reason());

    // ECU emulation before WiFi: frames go out before the web task starts (VCU-REQ-002).
    // Highest application priority on core 1; the web task (WiFi, web server) on core 0 at
    // the lowest priority. setup() returns at once, so loop() polls CAN without delay.
    eepromBegin();       // AT24C (I2C); before the ECU task (I2C failed when started after it, bench 2026-11-14)
    persistenceBegin();  // charging parameters, fan mode, best times (VCU-REQ-020)
    xTaskCreatePinnedToCore(ecuTask, "vcu_ecu", 8192, nullptr, params::can.ecu_task_priority, nullptr, 1);
    webinterfaceBegin(params::can.web_task_priority);
    VCU_LOG("[VCU] started, ECU task period %u ms\n", (unsigned)params::can.ecu_period_ms);
}

// Arduino loop task (core 1, below the ECU task): receive continuously, outputs and
// error counters at a slower rate.
void loop() {
    static uint32_t lastOutputs = 0;
    static uint32_t lastErrors = 0;
    const uint32_t nowMs = millis();
    {
        ParamsGuard guard;
        pollVehicleCAN();
        pollTeslaCAN();
        pollBatteryCAN();
        pollSpareCAN();
    }
    if (nowMs - lastOutputs >= OUTPUT_PERIOD_MS) {
        lastOutputs = nowMs;
        updateOutputs(nowMs);
    }
    if (nowMs - lastErrors >= params::can.error_refresh_ms) {  // error counters (SPI, outside the lock)
        lastErrors = nowMs;
        canRefreshErrors();
        canManagerRecover(nowMs);  // failed init, CAN0 bus-off (review B-06)
    }
    vTaskDelay(1);  // 1 tick: lets the idle task run; receive latency stays about 1 ms
}
