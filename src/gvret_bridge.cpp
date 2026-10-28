#include "gvret_bridge.h"

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define GVRET_PORT Serial
#include "gvret.h"

namespace {

struct ObservedFrame {
    CANMessage frame;
    int bus;
};

QueueHandle_t gQueue = nullptr;
volatile uint32_t gDropped = 0;

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
    ObservedFrame item{frame, bus};
    if (xQueueSend(gQueue, &item, 0) != pdTRUE) {
        gDropped = gDropped + 1;
    }
}

void gvretBridgeService() {
    gvret_loop();
    if (gQueue == nullptr) {
        return;
    }
    ObservedFrame item;
    int budget = 64;
    while (budget-- > 0 && xQueueReceive(gQueue, &item, 0) == pdTRUE) {
        if (gvret_binOutput) {
            sendFrameToUSB(item.frame, item.bus);
        }
    }
}

uint32_t gvretBridgeDropped() {
    return gDropped;
}
