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
constexpr uint32_t HOST_SILENT_MS = 5000;  // U-07: back to text mode without host traffic

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
    gvret_loop();
    // SavvyCAN sends a keepalive while connected; without host bytes the text log returns.
    if (gvret_binOutput && millis() - gvret_lastHostMs > HOST_SILENT_MS) {
        gvret_binOutput = false;
        VCU_LOG("[GVRET] host silent for %lu ms: text log and commands again\n", (unsigned long)HOST_SILENT_MS);
    }
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
