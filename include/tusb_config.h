#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// TinyUSB configuration for 2x CDC device on ESP32-S3.
#define CFG_TUSB_MCU              OPT_MCU_ESP32S3
#define CFG_TUSB_RHPORT0_MODE     (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)
#define CFG_TUSB_OS               OPT_OS_FREERTOS

// Force device stack enabled in case rhport mode isn't picked up.
#define CFG_TUD_ENABLED           1

// EP0 size
#define CFG_TUD_ENDPOINT0_SIZE    64

// Enable device classes
#define CFG_TUD_CDC               2
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0

// CDC buffer sizes
#define CFG_TUD_CDC_RX_BUFSIZE    256
#define CFG_TUD_CDC_TX_BUFSIZE    256

#endif // _TUSB_CONFIG_H_
