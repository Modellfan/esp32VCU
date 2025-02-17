#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// Enable multiple CDC (Serial) interfaces
#define CFG_TUD_CDC 2   // Number of virtual serial interfaces
#define CFG_TUD_MSC 0   // Disable mass storage
#define CFG_TUD_HID 0   // Disable HID
#define CFG_TUD_MIDI 0  // Disable MIDI
#define CFG_TUD_VENDOR 0

// Reduce buffer sizes for efficiency
#define CFG_TUD_CDC_RX_BUFSIZE 256
#define CFG_TUD_CDC_TX_BUFSIZE 256

#endif // _TUSB_CONFIG_H_
