// eBoxster VCU – the whole sequence in one place (VCU-DEC-015).
//
// CAN task, core 1, every 6 ms (VCU-REQ-001/002):
//   pollVehicleCAN, pollTeslaCAN, pollBatteryCAN -> updateCluster -> coolantFanAutoUpdate
//   -> emulateECUCAN (one ECU frame per cycle) -> outputs (fan relays, fuel gauge)
// Web task, core 0 (src/services/webinterface.cpp): WiFi, web API, PWA, GVRET, LED.
//
// Every module owns its data (params::* in its header). The two tasks share it under
// the params lock (src/services/params_lock.h).
//
// Folders: app/ (cluster calculation), comms/ (CAN codecs, receive monitor),
// drivers/ (CAN controllers, fan, fuel gauge, LED), services/ (web, signal table, lock, GVRET, log).
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "battery_messages.h"
#include "board.h"
#include "can_rx_monitor.h"
#include "canmanager.h"
#include "cluster_calc.h"
#include "coolant_fan_control.h"
#include "digipot_control.h"
#include "gvret_bridge.h"
#include "params_lock.h"
#include "status_led.h"
#include "tesla_sdu_messages.h"
#include "webinterface.h"

//---------------------------------------------------------------------------
// CAN receive: decode, mark the reception time, mirror to GVRET.
//---------------------------------------------------------------------------
constexpr int RX_BUDGET_PER_BUS = 32;  // frames per bus and cycle; the rest waits for the next cycle

// CAN1: Porsche 986 vehicle messages
void pollVehicleCAN() {
    CANMessage frame;
    for (int budget = RX_BUDGET_PER_BUS; budget > 0 && can1.available(); budget--) {
        can1.receive(frame);
        canCountRx(1);
        if (decode986Vehicle(frame)) {
            rxMark(BUS_VEHICLE, frame.id);
        }
        gvretBridgeObserve(frame, BUS_VEHICLE);
    }
}

// CAN2: Tesla SDU (OpenInverter)
void pollTeslaCAN() {
    CANMessage frame;
    for (int budget = RX_BUDGET_PER_BUS; budget > 0 && can2.available(); budget--) {
        can2.receive(frame);
        canCountRx(2);
        if (decodeTeslaSdu(frame)) {
            rxMark(BUS_DRIVE, frame.id);
        }
        gvretBridgeObserve(frame, BUS_DRIVE);
    }
}

// CAN0: BMS / charger (TWAI)
void pollBatteryCAN() {
    CANMessage frame;
    for (int budget = RX_BUDGET_PER_BUS; budget > 0 && can0Receive(frame); budget--) {
        canCountRx(0);
        if (decodeBattery(frame)) {
            rxMark(BUS_BATTERY, frame.id);
        }
        gvretBridgeObserve(frame, BUS_BATTERY);
    }
}

//---------------------------------------------------------------------------
// CAN1: Porsche 986 ECU emulation.
// 5 ECU frames x 6 ms slot = 30 ms full cycle, evenly distributed.
//---------------------------------------------------------------------------
void emulateECUCAN(CANMessage &outFrame) {
    static uint8_t slot = 0;
    switch (slot) {
        case 0: encode986Ecu0x280(outFrame); break;
        case 1: encode986Ecu0x289(outFrame); break;
        case 2: encode986Ecu0x298(outFrame); break;
        case 3: encode986Ecu0x4E0(outFrame); break;
        default: encode986Ecu0x7A0(outFrame); break;
    }
    slot = (uint8_t)((slot + 1U) % 5U);
}

//---------------------------------------------------------------------------
// CAN task (core 1)
//---------------------------------------------------------------------------
void canTask(void *) {
    TickType_t lastWake = xTaskGetTickCount();
    uint8_t diagDivider = 0;
    for (;;) {
        const uint32_t startUs = micros();
        const uint32_t nowMs = millis();
        CANMessage outFrame;
        uint8_t fanLevel;
        uint8_t fuelGaugePct;

        {
            ParamsGuard guard;
            pollVehicleCAN();
            pollTeslaCAN();
            pollBatteryCAN();
            updateCluster();               // cluster values, then ECU overrides
            coolantFanAutoUpdate(nowMs);
            digipotClampRequest();
            emulateECUCAN(outFrame);
            fanLevel = params::fan.fan_level;
            fuelGaugePct = params::digipot.digipot_pct;
        }

        const bool sent = can1.tryToSend(outFrame);
        gvretBridgeObserve(outFrame, GVRET_BUS_TX);

        // Hardware outputs outside the lock (the potentiometer needs about 2 ms per change).
        coolantFanWrite(fanLevel >= 1U, fanLevel >= 2U);
        digipotApply(fuelGaugePct);
        {
            ParamsGuard guard;
            coolantFanReportState();
            digipotReportState();
        }

        if (++diagDivider >= 50U) {  // error counters every 300 ms (SPI, outside the lock)
            diagDivider = 0;
            canRefreshErrors();
        }

        canTaskStatsRecord(nowMs, micros() - startUs, sent);
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(CAN_TASK_PERIOD_MS));
    }
}

//---------------------------------------------------------------------------
// Setup and Loop
//---------------------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // UART0: GVRET only
#if VCU_LOG_ENABLED
    vcuLogBegin();
#endif
    paramsLockInit();
    fastledInit();
    digipotBegin();
    coolantFanBegin();
    canManager_setup();  // CAN0 (TWAI), CAN1, CAN2
    gvretBridgeBegin();

    // ECU emulation first: frames go out before WiFi is even started (VCU-REQ-002).
    // Highest application priority on core 1; WiFi and the web server run on core 0.
    xTaskCreatePinnedToCore(canTask, "vcu_can", 8192, nullptr, configMAX_PRIORITIES - 2, nullptr, 1);
    webinterfaceBegin();
    VCU_LOG("[VCU] started, CAN task period %u ms\n", (unsigned)CAN_TASK_PERIOD_MS);
}

void loop() {
    // All work runs in the CAN task (core 1) and the web task (core 0).
    vTaskDelay(pdMS_TO_TICKS(1000));
}
