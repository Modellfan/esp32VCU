#include "params_lock.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace {
SemaphoreHandle_t gParamsMutex = nullptr;
}  // namespace

void paramsLockInit() {
    if (gParamsMutex == nullptr) {
        gParamsMutex = xSemaphoreCreateMutex();
    }
}

void paramsLock() {
    if (gParamsMutex != nullptr) {
        xSemaphoreTake(gParamsMutex, portMAX_DELAY);
    }
}

void paramsUnlock() {
    if (gParamsMutex != nullptr) {
        xSemaphoreGive(gParamsMutex);
    }
}
