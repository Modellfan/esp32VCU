#include "vcu_log.h"

#include <Arduino.h>
#include <stdarg.h>

#if SOC_USB_SERIAL_JTAG_SUPPORTED
#include <HWCDC.h>
#if ARDUINO_USB_MODE && ARDUINO_USB_CDC_ON_BOOT
#define VCU_LOG_PORT HWCDCSerial
#else
namespace {
HWCDC gUsbLog;
}
#define VCU_LOG_PORT gUsbLog
#endif
#endif

void vcuLogBegin() {
#ifdef VCU_LOG_PORT
    VCU_LOG_PORT.begin();
#endif
}

void vcuLogf(const char *format, ...) {
#ifdef VCU_LOG_PORT
    char buffer[192];
    va_list args;
    va_start(args, format);
    const int length = vsnprintf(buffer, sizeof buffer, format, args);
    va_end(args);
    if (length > 0 && VCU_LOG_PORT) {  // only when a host has the port open
        VCU_LOG_PORT.write(reinterpret_cast<const uint8_t *>(buffer),
                           (size_t)(length < (int)sizeof buffer ? length : (int)sizeof buffer - 1));
    }
#else
    (void)format;
#endif
}
