// Text log on UART0 (COM10), the same port as GVRET. Text is written only while GVRET is
// not in binary mode: SavvyCAN (or tools/gvret_listen.py) switches it with 0xE7, and from
// then on the port carries only GVRET frames until the next reset.
#include "vcu_log.h"

#include <Arduino.h>
#include <stdarg.h>

#include "gvret_bridge.h"

void vcuLogBegin() {
    // UART0 is opened in setup() (Serial.begin) for both GVRET and the log.
}

void vcuLogf(const char *format, ...) {
    if (gvretBridgeBinary()) {
        return;
    }
    char buffer[320];
    va_list args;
    va_start(args, format);
    const int length = vsnprintf(buffer, sizeof buffer, format, args);
    va_end(args);
    if (length > 0) {
        Serial.write(reinterpret_cast<const uint8_t *>(buffer),
                     (size_t)(length < (int)sizeof buffer ? length : (int)sizeof buffer - 1));
    }
}
