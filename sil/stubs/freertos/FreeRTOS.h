// SIL stub: single-threaded simulation, so FreeRTOS primitives are no-ops.
#ifndef SIL_FREERTOS_H
#define SIL_FREERTOS_H

#include <stdint.h>

typedef uint32_t TickType_t;
#define portMAX_DELAY 0xFFFFFFFFu
#define pdTRUE 1

#endif
