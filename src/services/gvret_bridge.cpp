#include "gvret_bridge.h"
#include "board.h"  // VCU_LOG

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define GVRET_PORT Serial
#include "gvret.h"

namespace {

struct ObservedFrame {
    CANMessage frame;
    int bus;
    uint32_t micros;  // time stamp at reception, not when the frame reaches the PC (B-12)
};

QueueHandle_t gQueue = nullptr;
volatile uint32_t gDropped = 0;
portMUX_TYPE gDroppedMux = portMUX_INITIALIZER_UNLOCKED;

}  // namespace

void gvretBridgeBegin() {
    if (gQueue == nullptr) {
        gQueue = xQueueCreate(128, sizeof(ObservedFrame));
    }
}

void gvretBridgeObserve(const CANMessage &frame, int bus) {
    // Forward only while SavvyCAN has switched GVRET to binary mode.
    if (gQueue == nullptr || !gvret_binOutput) {
        return;
    }
    ObservedFrame item{frame, bus, micros()};
    if (xQueueSend(gQueue, &item, 0) != pdTRUE) {
        portENTER_CRITICAL(&gDroppedMux);  // called from two tasks
        gDropped = gDropped + 1;
        portEXIT_CRITICAL(&gDroppedMux);
    }
}

void gvretBridgeService() {
    // Binary mode stays on until the next reset: SavvyCAN sends nothing while it only listens
    // (2026-11-16: a 5 s host timeout cut every SavvyCAN recording after 5 s).
    gvret_loop();
    if (gQueue == nullptr) {
        return;
    }
    ObservedFrame item;
    int budget = 64;
    while (budget-- > 0 && xQueueReceive(gQueue, &item, 0) == pdTRUE) {
        if (gvret_binOutput) {
            sendFrameToUSB(item.frame, item.bus, item.micros);
        }
    }
}

bool gvretBridgeBinary() {
    return gvret_binOutput;
}

uint32_t gvretBridgeDropped() {
    return gDropped;
}
