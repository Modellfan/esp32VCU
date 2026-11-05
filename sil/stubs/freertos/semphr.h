// SIL stub: mutex that always succeeds (the simulation runs in one thread).
#ifndef SIL_SEMPHR_H
#define SIL_SEMPHR_H

#include "FreeRTOS.h"

typedef void *SemaphoreHandle_t;

static inline SemaphoreHandle_t xSemaphoreCreateMutex() {
    static int token = 0;
    return &token;
}
static inline int xSemaphoreTake(SemaphoreHandle_t, TickType_t) { return pdTRUE; }
static inline int xSemaphoreGive(SemaphoreHandle_t) { return pdTRUE; }

#endif
