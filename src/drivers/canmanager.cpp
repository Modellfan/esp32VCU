// Driver layer: CAN0 (TWAI) and two MCP2515 CAN controllers (CAN1, CAN2) on one SPI bus.
// Pins from board.h.
#include "canmanager.h"

#include <Arduino.h>
#include <ACAN_ESP32.h>
#include <driver/gpio.h>
#include <soc/usb_serial_jtag_reg.h>

#include "board.h"

namespace {
constexpr uint32_t SPI_CLOCK = 10UL * 1000UL * 1000UL;
constexpr uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL;
constexpr uint32_t BITRATE = 500UL * 1000UL;

bool gCan0Active = false;
uint32_t gCan0Error = 0xFFFFFFFF;  // ACAN_ESP32 begin() result
uint16_t gCan1Error = 0xFFFF;
uint16_t gCan2Error = 0xFFFF;

portMUX_TYPE gStatsMux = portMUX_INITIALIZER_UNLOCKED;
CanTaskStats gStats = {0, 0, 0, 0, 0};
CanBusDiag gDiag[3] = {};  // index = bus number, guarded by gStatsMux

constexpr uint8_t EFLG_TXBO = 0x20;  // MCP2515: transmitter bus-off

// CAN0 (TWAI) with ACAN_ESP32: rx buffer 32, tx buffer 8 frames. begin() also leaves a
// bus-off, so the pin test can restart the controller in any state.
bool startCan0() {
    ACAN_ESP32_Settings settings(BITRATE);
    settings.mTxPin = (gpio_num_t)board::GPIO_CAN0_TX;
    settings.mRxPin = (gpio_num_t)board::GPIO_CAN0_RX;
    settings.mRequestedCANMode = ACAN_ESP32_Settings::NormalMode;
    settings.mDriverReceiveBufferSize = 32;
    settings.mDriverTransmitBufferSize = 8;
    gCan0Error = ACAN_ESP32::can.begin(settings);
    return gCan0Error == 0;
}

// CAN0 controller into reset mode with interrupts off (pin test).
void stopCan0() {
    ACAN_ESP32::can.TWAI_INT_ENA_REG() = 0;
    while ((ACAN_ESP32::can.TWAI_MODE_REG() & TWAI_RESET_MODE) == 0) {
        ACAN_ESP32::can.TWAI_MODE_REG() = TWAI_RESET_MODE;
    }
}
}  // namespace

ACAN2515 can1(board::GPIO_CAN1_CS, SPI, board::GPIO_CAN1_INT);
ACAN2515 can2(board::GPIO_CAN2_CS, SPI, board::GPIO_CAN2_INT);

void canManager_setup() {
    SPI.setFrequency(SPI_CLOCK);
    SPI.begin(board::GPIO_SPI_SCK, board::GPIO_SPI_MISO, board::GPIO_SPI_MOSI);

    ACAN2515Settings settings(QUARTZ_FREQUENCY, BITRATE);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    gCan1Error = can1.begin(settings, [] { can1.isr(); });
    VCU_LOG("[CAN] CAN1 (vehicle) init %s (0x%X)\n", gCan1Error == 0 ? "ok" : "FAILED", gCan1Error);

    ACAN2515Settings settings2(QUARTZ_FREQUENCY, BITRATE);
    settings2.mRequestedMode = ACAN2515Settings::NormalMode;
    gCan2Error = can2.begin(settings2, [] { can2.isr(); });
    VCU_LOG("[CAN] CAN2 (drive) init %s (0x%X)\n", gCan2Error == 0 ? "ok" : "FAILED", gCan2Error);

    if (board::CAN0_ENABLED) {
        gCan0Active = startCan0();
        VCU_LOG("[CAN] CAN0 (battery, TWAI/ACAN_ESP32) init %s (0x%lX)\n", gCan0Active ? "ok" : "FAILED",
                (unsigned long)gCan0Error);
    }
}

bool can0Active() {
    return gCan0Active;
}

bool can0Receive(CANMessage &frame) {
    if (!gCan0Active) {
        return false;
    }
    return ACAN_ESP32::can.receive(frame);
}

uint16_t canManager_error(uint8_t bus) {
    return bus == 1 ? gCan1Error : gCan2Error;
}

void canTaskStatsRecord(uint32_t nowMs, uint32_t cycleUs, bool sent) {
    portENTER_CRITICAL(&gStatsMux);
    if (gStats.cycles == 0) {
        gStats.started_ms = nowMs;
    }
    gStats.cycles++;
    if (!sent) {
        gStats.tx_failed++;
    }
    if (cycleUs > gStats.max_cycle_us) {
        gStats.max_cycle_us = cycleUs;
    }
    if (cycleUs > CAN_TASK_PERIOD_MS * 1000U) {
        gStats.overruns++;
    }
    portEXIT_CRITICAL(&gStatsMux);
}

CanTaskStats canTaskStats() {
    portENTER_CRITICAL(&gStatsMux);
    const CanTaskStats copy = gStats;
    portEXIT_CRITICAL(&gStatsMux);
    return copy;
}

void canCountRx(uint8_t bus) {
    if (bus > 2) {
        return;
    }
    portENTER_CRITICAL(&gStatsMux);
    gDiag[bus].rx_frames++;
    portEXIT_CRITICAL(&gStatsMux);
}

void canRefreshErrors() {
    CanBusDiag d[3];
    portENTER_CRITICAL(&gStatsMux);
    for (int i = 0; i < 3; i++) {
        d[i] = gDiag[i];
    }
    portEXIT_CRITICAL(&gStatsMux);

    // MCP2515 registers over SPI (Arduino SPI transactions are locked).
    if (gCan1Error == 0) {
        d[1].tec = can1.transmitErrorCounter();
        d[1].rec = can1.receiveErrorCounter();
        d[1].eflg = can1.errorFlagRegister();
        d[1].bus_off = (d[1].eflg & EFLG_TXBO) != 0;
    }
    if (gCan2Error == 0) {
        d[2].tec = can2.transmitErrorCounter();
        d[2].rec = can2.receiveErrorCounter();
        d[2].eflg = can2.errorFlagRegister();
        d[2].bus_off = (d[2].eflg & EFLG_TXBO) != 0;
    }
    if (gCan0Active) {
        d[0].tec = (uint8_t)(ACAN_ESP32::can.TWAI_TX_ERR_CNT_REG() & 0xFF);
        d[0].rec = (uint8_t)(ACAN_ESP32::can.TWAI_RX_ERR_CNT_REG() & 0xFF);
        d[0].bus_off = (ACAN_ESP32::can.TWAI_STATUS_REG() & TWAI_BUS_OFF_ST) != 0;
    }

    portENTER_CRITICAL(&gStatsMux);
    for (int i = 0; i < 3; i++) {
        d[i].rx_frames = gDiag[i].rx_frames;  // keep counts that arrived meanwhile
        gDiag[i] = d[i];
    }
    portEXIT_CRITICAL(&gStatsMux);
}

CanBusDiag canBusDiag(uint8_t bus) {
    CanBusDiag copy = {};
    if (bus > 2) {
        return copy;
    }
    portENTER_CRITICAL(&gStatsMux);
    copy = gDiag[bus];
    portEXIT_CRITICAL(&gStatsMux);
    return copy;
}

void canManagerLogCan0Status() {
    if (!gCan0Active) {
        VCU_LOG("[CAN0] not active (init 0x%lX)\n", (unsigned long)gCan0Error);
        return;
    }
    const uint32_t status = ACAN_ESP32::can.TWAI_STATUS_REG();
    const uint32_t tec = ACAN_ESP32::can.TWAI_TX_ERR_CNT_REG() & 0xFF;
    const uint32_t rec = ACAN_ESP32::can.TWAI_RX_ERR_CNT_REG() & 0xFF;
    const char *state = (status & TWAI_BUS_OFF_ST) != 0         ? "bus-off"
                        : (ACAN_ESP32::can.TWAI_MODE_REG() & TWAI_RESET_MODE) != 0 ? "reset"
                        : (tec >= 128 || rec >= 128)             ? "error-passive"
                        : (status & TWAI_ERR_ST) != 0            ? "error-warning"
                                                                 : "running";
    // Sample the RX pin for 50 ms: with traffic on the bus some samples are dominant (0).
    uint32_t samples = 0;
    uint32_t low = 0;
    const uint32_t start = micros();
    while (micros() - start < 50000U) {
        samples++;
        if (gpio_get_level((gpio_num_t)board::GPIO_CAN0_RX) == 0) {
            low++;
        }
    }
    const bool usbPad = (READ_PERI_REG(USB_SERIAL_JTAG_CONF0_REG) & USB_SERIAL_JTAG_USB_PAD_ENABLE) != 0;
    VCU_LOG("[CAN0] state=%s tec=%lu rec=%lu status=0x%02lX flags=0x%lX err_code=0x%02lX rx_buf=%u peak=%u "
            "tx_buf=%u | RX GPIO%d low %lu/%lu samples in 50 ms | usb_pad=%d\n",
            state, (unsigned long)tec, (unsigned long)rec, (unsigned long)(status & 0xFF),
            (unsigned long)ACAN_ESP32::can.statusFlags(), (unsigned long)(ACAN_ESP32::can.TWAI_ERR_CODE_CAP_REG() & 0xFF),
            ACAN_ESP32::can.driverReceiveBufferCount(), ACAN_ESP32::can.driverReceiveBufferPeakCount(),
            ACAN_ESP32::can.driverTransmitBufferCount(), board::GPIO_CAN0_RX, (unsigned long)low,
            (unsigned long)samples, usbPad ? 1 : 0);
}

void canManagerCan0SendTest() {
    if (!gCan0Active) {
        VCU_LOG("[CAN0] not active\n");
        return;
    }
    CANMessage frame;
    frame.id = 0x6C0;  // bench test ID, not used on the vehicle buses
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = (uint8_t)(0xC0 + i);
    }
    const bool queued = ACAN_ESP32::can.tryToSend(frame);
    delay(20);
    VCU_LOG("[CAN0] send 0x6C0 %s\n", queued ? "queued" : "FAILED");
    canManagerLogCan0Status();
}

namespace {
// Count low samples of a pin over a time window (busy loop, diagnostics only).
uint32_t countLow(gpio_num_t pin, uint32_t windowUs, uint32_t &samples) {
    uint32_t low = 0;
    samples = 0;
    const uint32_t start = micros();
    while (micros() - start < windowUs) {
        samples++;
        if (gpio_get_level(pin) == 0) {
            low++;
        }
    }
    return low;
}
}  // namespace

void canManagerCan0PinTest() {
    const gpio_num_t rx = (gpio_num_t)board::GPIO_CAN0_RX;
    const gpio_num_t tx = (gpio_num_t)board::GPIO_CAN0_TX;
    const bool wasActive = gCan0Active;
    gCan0Active = false;  // loop() stops reading CAN0
    delay(5);             // let a running can0Receive() finish
    if (wasActive) {
        stopCan0();
    }
    gpio_reset_pin(rx);
    gpio_reset_pin(tx);
    gpio_set_direction(rx, GPIO_MODE_INPUT);
    // TX recessive (high) while RX is sampled with the three pull settings.
    gpio_set_direction(tx, GPIO_MODE_OUTPUT);
    gpio_set_level(tx, 1);

    uint32_t n = 0;
    gpio_set_pull_mode(rx, GPIO_FLOATING);
    delay(2);
    const uint32_t lowFloat = countLow(rx, 20000, n);
    gpio_set_pull_mode(rx, GPIO_PULLUP_ONLY);
    delay(2);
    const uint32_t lowUp = countLow(rx, 20000, n);
    gpio_set_pull_mode(rx, GPIO_PULLDOWN_ONLY);
    delay(2);
    const uint32_t lowDown = countLow(rx, 20000, n);
    VCU_LOG("[PINTEST] TX high: RX GPIO%d low samples float %lu, pull-up %lu, pull-down %lu (of %lu each)\n",
            board::GPIO_CAN0_RX, (unsigned long)lowFloat, (unsigned long)lowUp, (unsigned long)lowDown,
            (unsigned long)n);

    // TX dominant for 20 ms: the transceiver echoes it on RXD, the other nodes see errors.
    gpio_set_pull_mode(rx, GPIO_FLOATING);
    const uint8_t rec1Before = can1.receiveErrorCounter();
    const uint8_t rec2Before = can2.receiveErrorCounter();
    gpio_set_level(tx, 0);
    delayMicroseconds(200);
    const uint32_t lowEcho = countLow(rx, 20000, n);
    gpio_set_level(tx, 1);
    const uint8_t rec1After = can1.receiveErrorCounter();
    const uint8_t rec2After = can2.receiveErrorCounter();
    VCU_LOG("[PINTEST] TX low 20 ms: RX low %lu/%lu (echo) | CAN1 REC %u->%u, CAN2 REC %u->%u\n",
            (unsigned long)lowEcho, (unsigned long)n, rec1Before, rec1After, rec2Before, rec2After);

    gpio_reset_pin(rx);
    gpio_reset_pin(tx);
    if (wasActive) {
        gCan0Active = startCan0();
        VCU_LOG("[PINTEST] CAN0 restarted %s (0x%lX)\n", gCan0Active ? "ok" : "FAILED", (unsigned long)gCan0Error);
    }
}

void canManagerLogStatus() {
    const CanTaskStats t = canTaskStats();
    char line[320];
    int n = snprintf(line, sizeof line, "[DIAG] t=%lu ms cyc=%lu ovr=%lu max=%lu us txfail=%lu",
                     (unsigned long)millis(), (unsigned long)t.cycles, (unsigned long)t.overruns,
                     (unsigned long)t.max_cycle_us, (unsigned long)t.tx_failed);
    for (uint8_t bus = 0; bus < 3 && n > 0 && n < (int)sizeof line; bus++) {
        const CanBusDiag d = canBusDiag(bus);
        const bool up = bus == 0 ? gCan0Active : canManager_error(bus) == 0;
        n += snprintf(line + n, sizeof line - n, " | CAN%u %s rx=%lu tec=%u rec=%u eflg=0x%02X%s", bus,
                      up ? "up" : "DOWN", (unsigned long)d.rx_frames, d.tec, d.rec, d.eflg,
                      d.bus_off ? " BUS-OFF" : "");
    }
    VCU_LOG("%s\n", line);
}
