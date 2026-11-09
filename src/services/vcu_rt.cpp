#include "vcu_rt.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "battery_messages.h"
#include "board.h"
#include "can3_twai.h"
#include "canmanager.h"
#include "coolant_fan_control.h"
#include "digipot_control.h"
#include "gvret_bridge.h"
#include "tesla_sdu_messages.h"
#include "vcu_app.h"
#include "vcu_params.h"

namespace {

constexpr int RX_BUDGET_PER_BUS = 32;  // frames per bus and cycle; the rest waits for the next cycle

portMUX_TYPE gStatsMux = portMUX_INITIALIZER_UNLOCKED;
RtStats gStats = {0, 0, 0, 0, 0};

template <typename Decoder>
void pollMcp(ACAN2515 &controller, CanBus bus, Decoder decode) {
    CANMessage frame;
    int budget = RX_BUDGET_PER_BUS;
    while (budget-- > 0 && controller.available()) {
        controller.receive(frame);
        if (decode(frame)) {
            rxMark(bus, frame.id);
        }
        gvretBridgeObserve(frame, bus);
    }
}

void pollCan3() {
    CANMessage frame;
    int budget = RX_BUDGET_PER_BUS;
    while (budget-- > 0 && can3Receive(frame)) {
        if (decodeBattery(frame)) {
            rxMark(BUS_BATTERY, frame.id);
        }
        gvretBridgeObserve(frame, BUS_BATTERY);
    }
}

void encodeSlot(uint8_t slot, CANMessage &frame) {
    switch (slot) {
        case 0: encode986Ecu0x280(frame); break;
        case 1: encode986Ecu0x289(frame); break;
        case 2: encode986Ecu0x298(frame); break;
        case 3: encode986Ecu0x4E0(frame); break;
        default: encode986Ecu0x7A0(frame); break;
    }
}

// Applies params::outputs to the hardware. The digital potentiometer is only stepped
// when its set point changed (a full step sequence takes about 2 ms).
void applyOutputs(uint8_t fanLevel, uint8_t digipotPct, uint8_t &digipotApplied) {
    coolantFanWrite(fanLevel >= 1U, fanLevel >= 2U);
    if (digipotPct != digipotApplied) {
        digipotSetPercent(digipotPct);
        digipotApplied = digipotPct;
    }
}

void rtTask(void *) {
    TickType_t lastWake = xTaskGetTickCount();
    uint8_t slot = 0;
    uint8_t digipotApplied = 255;  // force the first write
    bool first = true;

    for (;;) {
        const uint32_t startUs = micros();
        const uint32_t nowMs = millis();
        CANMessage out;
        uint8_t fanLevel;
        uint8_t digipotPct;

        {
            ParamsGuard guard;
            pollMcp(can, BUS_VEHICLE, decode986Vehicle);
            pollMcp(can2, BUS_DRIVE, decodeTeslaSdu);
            pollCan3();
            appStep(nowMs);
            encodeSlot(slot, out);
            fanLevel = params::outputs.fan_level;
            digipotPct = params::outputs.digipot_pct;
        }

        const bool sent = can.tryToSend(out);
        gvretBridgeObserve(out, GVRET_BUS_TX);
        slot = (uint8_t)((slot + 1U) % 5U);

        applyOutputs(fanLevel, digipotPct, digipotApplied);
        {
            ParamsGuard guard;
            params::outputs.fan_stage1 = coolantFanStage1();
            params::outputs.fan_stage2 = coolantFanStage2();
            params::outputs.fan_outputs_enabled = coolantFanOutputsEnabled();
            params::outputs.digipot_applied_pct = digipotPercent();
            params::outputs.digipot_wiper = digipotWiper();
        }

        const uint32_t cycleUs = micros() - startUs;
        portENTER_CRITICAL(&gStatsMux);
        if (first) {
            gStats.started_ms = nowMs;
            first = false;
        }
        gStats.cycles++;
        if (!sent) {
            gStats.tx_failed++;
        }
        if (cycleUs > gStats.max_cycle_us) {
            gStats.max_cycle_us = cycleUs;
        }
        if (cycleUs > RT_PERIOD_MS * 1000U) {
            gStats.overruns++;
        }
        portEXIT_CRITICAL(&gStatsMux);

        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(RT_PERIOD_MS));
    }
}

}  // namespace

void rtBegin() {
    // Highest application priority on core 1; WiFi and the web server run on core 0.
    xTaskCreatePinnedToCore(rtTask, "vcu_rt", 8192, nullptr, configMAX_PRIORITIES - 2, nullptr, 1);
}

RtStats rtStats() {
    portENTER_CRITICAL(&gStatsMux);
    const RtStats copy = gStats;
    portEXIT_CRITICAL(&gStatsMux);
    return copy;
}
