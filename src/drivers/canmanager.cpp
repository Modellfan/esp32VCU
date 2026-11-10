// Driver layer: CAN0 (TWAI) and three MCP2515 CAN controllers (CAN1..CAN3) on one SPI bus.
// Pins from board.h, settings and status in params::can (canmanager_api.h).
#include "canmanager.h"

#include <Arduino.h>
#include <ACAN_ESP32.h>
#include <driver/gpio.h>
#include <soc/usb_serial_jtag_reg.h>

#include "board.h"
#include "params_lock.h"

namespace {
bool gCan0Active = false;
uint32_t gError[CAN_BUS_COUNT] = {0xFFFFFFFF, 0xFFFF, 0xFFFF, 0xFFFF};  // init results

portMUX_TYPE gStatsMux = portMUX_INITIALIZER_UNLOCKED;
CanTaskStats gStats = {0, 0, 0, 0, 0};
CanBusDiag gDiag[CAN_BUS_COUNT] = {};  // index = bus number, guarded by gStatsMux

constexpr uint8_t EFLG_TXBO = 0x20;  // MCP2515: transmitter bus-off

// CAN0 (TWAI) with ACAN_ESP32. begin() also leaves a bus-off, so the pin test can restart
// the controller in any state.
bool startCan0() {
    ACAN_ESP32_Settings settings(params::can.bitrate);
    settings.mTxPin = (gpio_num_t)board::GPIO_CAN0_TX;
    settings.mRxPin = (gpio_num_t)board::GPIO_CAN0_RX;
    settings.mRequestedCANMode = ACAN_ESP32_Settings::NormalMode;
    settings.mDriverReceiveBufferSize = params::can.can0_rx_buffer;
    settings.mDriverTransmitBufferSize = params::can.can0_tx_buffer;
    gError[0] = ACAN_ESP32::can.begin(settings);
    return gError[0] == 0;
}

// CAN0 controller into reset mode with interrupts off (pin test).
void stopCan0() {
    ACAN_ESP32::can.TWAI_INT_ENA_REG() = 0;
    while ((ACAN_ESP32::can.TWAI_MODE_REG() & TWAI_RESET_MODE) == 0) {
        ACAN_ESP32::can.TWAI_MODE_REG() = TWAI_RESET_MODE;
    }
}

uint16_t startMcp2515(ACAN2515 &controller, void (*isr)()) {
    ACAN2515Settings settings(params::can.mcp2515_quartz_hz, params::can.bitrate);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    settings.mReceiveBufferSize = params::can.mcp2515_rx_buffer;
    settings.mTransmitBuffer0Size = params::can.mcp2515_tx_buffer;
    return controller.begin(settings, isr);
}

void readMcp2515Errors(ACAN2515 &controller, CanBusDiag &d) {
    d.tec = controller.transmitErrorCounter();
    d.rec = controller.receiveErrorCounter();
    d.eflg = controller.errorFlagRegister();
    d.bus_off = (d.eflg & EFLG_TXBO) != 0;
}

void copyStatus(params::CanBusStatus &out, uint8_t bus, const CanBusDiag &d) {
    out.up = bus == 0 ? gCan0Active : gError[bus] == 0;
    out.error = gError[bus];
    out.rx_frames = d.rx_frames;
    out.tec = d.tec;
    out.rec = d.rec;
    out.eflg = d.eflg;
    out.bus_off = d.bus_off;
}

// Caller holds the params lock.
void publishStatus(const CanBusDiag *d) {
    copyStatus(params::can.can0, 0, d[0]);
    copyStatus(params::can.can1, 1, d[1]);
    copyStatus(params::can.can2, 2, d[2]);
    copyStatus(params::can.can3, 3, d[3]);
}
}  // namespace

ACAN2515 can1(board::GPIO_CAN1_CS, SPI, board::GPIO_CAN1_INT);
ACAN2515 can2(board::GPIO_CAN2_CS, SPI, board::GPIO_CAN2_INT);
ACAN2515 can3(board::GPIO_CAN3_CS, SPI, board::GPIO_CAN3_INT);

void canManager_setup() {
    SPI.setFrequency(params::can.spi_clock_hz);
    SPI.begin(board::GPIO_SPI_SCK, board::GPIO_SPI_MISO, board::GPIO_SPI_MOSI);

    gError[1] = startMcp2515(can1, [] { can1.isr(); });
    VCU_LOG("[CAN] CAN1 (vehicle) init %s (0x%lX)\n", gError[1] == 0 ? "ok" : "FAILED", (unsigned long)gError[1]);
    gError[2] = startMcp2515(can2, [] { can2.isr(); });
    VCU_LOG("[CAN] CAN2 (drive) init %s (0x%lX)\n", gError[2] == 0 ? "ok" : "FAILED", (unsigned long)gError[2]);
    gError[3] = startMcp2515(can3, [] { can3.isr(); });
    VCU_LOG("[CAN] CAN3 (spare) init %s (0x%lX)\n", gError[3] == 0 ? "ok" : "FAILED", (unsigned long)gError[3]);

    if (board::CAN0_ENABLED) {
        gCan0Active = startCan0();
        VCU_LOG("[CAN] CAN0 (battery, TWAI/ACAN_ESP32) init %s (0x%lX)\n", gCan0Active ? "ok" : "FAILED",
                (unsigned long)gError[0]);
    }
    ParamsGuard guard;
    publishStatus(gDiag);
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

bool can0Send(const CANMessage &frame) {
    return gCan0Active && ACAN_ESP32::can.tryToSend(frame);
}

uint32_t canManager_error(uint8_t bus) {
    return bus < CAN_BUS_COUNT ? gError[bus] : 0xFFFFFFFF;
}

void canTaskStatsRecord(uint32_t nowMs, uint32_t cycleUs, bool sent) {
    const uint32_t periodUs = (uint32_t)params::can.ecu_period_ms * 1000U;  // set once at start-up
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
    if (cycleUs > periodUs) {
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
    if (bus >= CAN_BUS_COUNT) {
        return;
    }
    portENTER_CRITICAL(&gStatsMux);
    gDiag[bus].rx_frames++;
    portEXIT_CRITICAL(&gStatsMux);
}

void canRefreshErrors() {
    CanBusDiag d[CAN_BUS_COUNT];
    portENTER_CRITICAL(&gStatsMux);
    for (int i = 0; i < CAN_BUS_COUNT; i++) {
        d[i] = gDiag[i];
    }
    portEXIT_CRITICAL(&gStatsMux);

    // MCP2515 registers over SPI (Arduino SPI transactions are locked).
    if (gError[1] == 0) {
        readMcp2515Errors(can1, d[1]);
    }
    if (gError[2] == 0) {
        readMcp2515Errors(can2, d[2]);
    }
    if (gError[3] == 0) {
        readMcp2515Errors(can3, d[3]);
    }
    if (gCan0Active) {
        const uint32_t status = ACAN_ESP32::can.TWAI_STATUS_REG();
        d[0].tec = (uint8_t)(ACAN_ESP32::can.TWAI_TX_ERR_CNT_REG() & 0xFF);
        d[0].rec = (uint8_t)(ACAN_ESP32::can.TWAI_RX_ERR_CNT_REG() & 0xFF);
        d[0].eflg = (uint8_t)(status & 0xFF);
        d[0].bus_off = (status & TWAI_BUS_OFF_ST) != 0;
    }

    CanTaskStats t;
    portENTER_CRITICAL(&gStatsMux);
    for (int i = 0; i < CAN_BUS_COUNT; i++) {
        d[i].rx_frames = gDiag[i].rx_frames;  // keep counts that arrived meanwhile
        gDiag[i] = d[i];
    }
    t = gStats;
    portEXIT_CRITICAL(&gStatsMux);

    const uint16_t peak = gCan0Active ? ACAN_ESP32::can.driverReceiveBufferPeakCount() : 0;
    const bool overflow = gCan0Active && (ACAN_ESP32::can.statusFlags() & 0x2U) != 0;
    ParamsGuard guard;
    publishStatus(d);
    params::can.ecu_cycles = t.cycles;
    params::can.ecu_overruns = t.overruns;
    params::can.ecu_max_cycle_us = t.max_cycle_us;
    params::can.ecu_tx_failed = t.tx_failed;
    params::can.ecu_started_ms = t.started_ms;
    params::can.can0_rx_peak = peak;
    params::can.can0_rx_overflow = overflow;
}

CanBusDiag canBusDiag(uint8_t bus) {
    CanBusDiag copy = {};
    if (bus >= CAN_BUS_COUNT) {
        return copy;
    }
    portENTER_CRITICAL(&gStatsMux);
    copy = gDiag[bus];
    portEXIT_CRITICAL(&gStatsMux);
    return copy;
}

void canManagerLogCan0Status() {
    if (!gCan0Active) {
        VCU_LOG("[CAN0] not active (init 0x%lX)\n", (unsigned long)gError[0]);
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
        VCU_LOG("[PINTEST] CAN0 restarted %s (0x%lX)\n", gCan0Active ? "ok" : "FAILED", (unsigned long)gError[0]);
    }
}

void canManagerLogStatus() {
    const CanTaskStats t = canTaskStats();
    char line[400];
    int n = snprintf(line, sizeof line, "[DIAG] t=%lu ms cyc=%lu ovr=%lu max=%lu us txfail=%lu",
                     (unsigned long)millis(), (unsigned long)t.cycles, (unsigned long)t.overruns,
                     (unsigned long)t.max_cycle_us, (unsigned long)t.tx_failed);
    for (uint8_t bus = 0; bus < CAN_BUS_COUNT && n > 0 && n < (int)sizeof line; bus++) {
        const CanBusDiag d = canBusDiag(bus);
        const bool up = bus == 0 ? gCan0Active : canManager_error(bus) == 0;
        n += snprintf(line + n, sizeof line - n, " | CAN%u %s rx=%lu tec=%u rec=%u eflg=0x%02X%s", bus,
                      up ? "up" : "DOWN", (unsigned long)d.rx_frames, d.tec, d.rec, d.eflg,
                      d.bus_off ? " BUS-OFF" : "");
    }
    VCU_LOG("%s\n", line);
}
