#include "can3_twai.h"

#include "board.h"

#if __has_include("driver/twai.h")
#include "driver/twai.h"
#define VCU_HAS_TWAI 1
#else
#define VCU_HAS_TWAI 0
#endif

namespace {
bool gActive = false;
}

bool can3Begin() {
#if VCU_HAS_TWAI
    if (!board::CAN3_ENABLED) {
        VCU_LOG("[CAN] CAN3 (battery) disabled: TWAI pins not assigned in board.h\n");
        return false;
    }
    twai_general_config_t general = TWAI_GENERAL_CONFIG_DEFAULT(
        (gpio_num_t)board::GPIO_CAN3_TX, (gpio_num_t)board::GPIO_CAN3_RX, TWAI_MODE_NORMAL);
    general.rx_queue_len = 32;
    general.tx_queue_len = 8;
    twai_timing_config_t timing = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t filter = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    if (twai_driver_install(&general, &timing, &filter) != ESP_OK || twai_start() != ESP_OK) {
        VCU_LOG("[CAN] CAN3 (battery) init FAILED\n");
        return false;
    }
    gActive = true;
    VCU_LOG("[CAN] CAN3 (battery) init ok\n");
    return true;
#else
    return false;
#endif
}

bool can3Active() {
    return gActive;
}

bool can3Receive(CANMessage &frame) {
#if VCU_HAS_TWAI
    if (!gActive) {
        return false;
    }
    twai_message_t message;
    if (twai_receive(&message, 0) != ESP_OK) {
        return false;
    }
    frame.id = message.identifier;
    frame.ext = message.extd != 0;
    frame.rtr = message.rtr != 0;
    frame.len = message.data_length_code > 8 ? 8 : message.data_length_code;
    for (uint8_t i = 0; i < frame.len; i++) {
        frame.data[i] = message.data[i];
    }
    return true;
#else
    (void)frame;
    return false;
#endif
}
