// Service: text commands on UART0 (COM10), shared with GVRET (VCU-DEC-010).
// Active only while GVRET is not in binary mode. The GVRET parser (gvret.h) hands every
// byte it does not use itself to serialConsoleByte(); a command ends with CR or LF.
//
//   help         list the commands
//   version      app name, firmware version, build date and time, PlatformIO environment
//   wifi-status  access point and home network (SSID, connected, IP)
//   can0-status  full TWAI status and TX/RX pin levels
//   can0-send    one test frame 0x6C0 on CAN0 (bench only), then can0-status
//   reset        restart the VCU (also: reboot)
#ifndef SERIAL_CONSOLE_H
#define SERIAL_CONSOLE_H

#include <stdint.h>

// Firmware identity, set in platformio.ini (build_flags) like the CANivore firmware.
#ifndef APP_NAME
#define APP_NAME "eBoxster-VCU"
#endif
#ifndef APP_VERSION
#define APP_VERSION "0.0.0"
#endif
#ifndef PIO_ENV_NAME
#define PIO_ENV_NAME "unknown"
#endif

// One line with app, version, build date and environment (start-up and "version").
void serialConsolePrintBuildInfo();
// Web task: one byte from UART0 that GVRET did not consume.
void serialConsoleByte(uint8_t c);

#endif
