// Central hardware assignment of the VCU board (ESP32-S3-DevKitC-1, 8 MB flash).
// Every GPIO used anywhere in the firmware is defined here and nowhere else.
// A duplicate assignment fails the build (static_assert below).
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

namespace board {

constexpr int GPIO_UNASSIGNED = -1;

// SPI bus shared by both MCP2515 CAN controllers (8 MHz crystal, 500 kbit/s).
constexpr int GPIO_SPI_SCK = 12;
constexpr int GPIO_SPI_MOSI = 11;
constexpr int GPIO_SPI_MISO = 13;
constexpr int GPIO_CAN1_CS = 10;   // vehicle CAN (Porsche 986)
constexpr int GPIO_CAN1_INT = 9;
constexpr int GPIO_CAN2_CS = 7;    // drive CAN (Tesla SDU / OpenInverter), confirmed 2026-10-28
constexpr int GPIO_CAN2_INT = 8;

// CAN0 for BMS and charger via the ESP32-S3 TWAI controller (transceiver only).
// Pins as on the main branch (user decision 2026-11-09). GPIO20 is USB D+: the native USB
// port (text log) cannot be used, so the log is compiled out (platformio.ini).
constexpr int GPIO_CAN0_TX = 21;
constexpr int GPIO_CAN0_RX = 20;

// X9C103S digital potentiometer (fuel gauge sender emulation).
constexpr int GPIO_DIGIPOT_CS = 4;
constexpr int GPIO_DIGIPOT_INC = 5;
constexpr int GPIO_DIGIPOT_UD = 6;

// Coolant fan relay stages. Stage 1 was configured on GPIO7, which is CAN2 chip select.
// The real pin is unknown (open question 2026-10-28): fan outputs stay disabled until set.
constexpr int GPIO_FAN_STAGE1 = GPIO_UNASSIGNED;
constexpr int GPIO_FAN_STAGE2 = 15;

// On-board WS2812 RGB LED.
constexpr int GPIO_RGB_LED = 48;

constexpr bool CAN0_ENABLED = GPIO_CAN0_TX != GPIO_UNASSIGNED && GPIO_CAN0_RX != GPIO_UNASSIGNED;
constexpr bool FAN_OUTPUTS_ENABLED = GPIO_FAN_STAGE1 != GPIO_UNASSIGNED && GPIO_FAN_STAGE2 != GPIO_UNASSIGNED;

constexpr int kAssignedPins[] = {
    GPIO_SPI_SCK, GPIO_SPI_MOSI, GPIO_SPI_MISO,
    GPIO_CAN1_CS, GPIO_CAN1_INT, GPIO_CAN2_CS, GPIO_CAN2_INT,
    GPIO_CAN0_TX, GPIO_CAN0_RX,
    GPIO_DIGIPOT_CS, GPIO_DIGIPOT_INC, GPIO_DIGIPOT_UD,
    GPIO_FAN_STAGE1, GPIO_FAN_STAGE2,
    GPIO_RGB_LED,
};

constexpr bool pinsUnique(const int *pins, int count) {
    for (int i = 0; i < count; i++) {
        if (pins[i] == GPIO_UNASSIGNED) {
            continue;
        }
        for (int j = i + 1; j < count; j++) {
            if (pins[i] == pins[j]) {
                return false;
            }
        }
    }
    return true;
}

static_assert(pinsUnique(kAssignedPins, sizeof(kAssignedPins) / sizeof(kAssignedPins[0])),
              "board.h: a GPIO is assigned twice");

}  // namespace board

// Diagnostic log on the native USB port; UART0 (Serial) carries only GVRET binary data.
#ifndef VCU_LOG_ENABLED
#define VCU_LOG_ENABLED 1
#endif
#if VCU_LOG_ENABLED
#include "vcu_log.h"
#define VCU_LOG(...) vcuLogf(__VA_ARGS__)
#else
#define VCU_LOG(...) do { } while (0)
#endif

#endif  // BOARD_H
