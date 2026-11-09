// Text commands on UART0 (see serial_console.h). Runs in the web task (core 0).
#include "serial_console.h"

#include <Arduino.h>
#include <string.h>

#include "board.h"
#include "canmanager.h"
#include "webinterface.h"

namespace {

char gLine[64];
size_t gLength = 0;
bool gOverflow = false;

void execute(const char *command) {
    if (strcmp(command, "help") == 0) {
        VCU_LOG("[serial] Commands: help, version, wifi-status, can0-status, can0-send, can0-pintest, reset\n");
    } else if (strcmp(command, "version") == 0) {
        serialConsolePrintBuildInfo();
    } else if (strcmp(command, "wifi-status") == 0) {
        webinterfaceLogWifiStatus();
    } else if (strcmp(command, "can0-status") == 0) {
        canManagerLogCan0Status();
    } else if (strcmp(command, "can0-send") == 0) {
        canManagerCan0SendTest();
    } else if (strcmp(command, "can0-pintest") == 0) {
        canManagerCan0PinTest();
    } else if (strcmp(command, "reset") == 0 || strcmp(command, "reboot") == 0) {
        VCU_LOG("[serial] Reset requested\n");
        Serial.flush();
        delay(50);
        ESP.restart();
    } else {
        VCU_LOG("[serial] Unknown command: %s (help)\n", command);
    }
}

}  // namespace

void serialConsolePrintBuildInfo() {
    VCU_LOG("[VERSION] app=%s version=%s build=%s %s env=%s\n", APP_NAME, APP_VERSION, __DATE__,
            __TIME__, PIO_ENV_NAME);
}

void serialConsoleByte(uint8_t c) {
    if (c == '\r' || c == '\n') {
        if (gOverflow) {
            VCU_LOG("[serial] Command too long\n");
        } else if (gLength > 0) {
            gLine[gLength] = '\0';
            execute(gLine);
        }
        gLength = 0;
        gOverflow = false;
        return;
    }
    if (c < 0x20 || c > 0x7E) {  // not text (GVRET bytes, noise): start over
        gLength = 0;
        gOverflow = false;
        return;
    }
    if (gLength + 1 < sizeof gLine) {
        gLine[gLength++] = (char)c;
    } else {
        gOverflow = true;
    }
}
