#include "tusb.h"

// You can use Espressif's VID (0x303A) for experiments; for products, use your own VID/PID.
#define USB_VID   0x303A
#define USB_PID   0x4002
#define USB_BCD   0x0100

enum {
  ITF_NUM_CDC0 = 0,
  ITF_NUM_CDC0_DATA,
  ITF_NUM_CDC1,
  ITF_NUM_CDC1_DATA,
  ITF_NUM_TOTAL
};

#define EPNUM_CDC0_NOTIF  0x81
#define EPNUM_CDC0_OUT    0x02
#define EPNUM_CDC0_IN     0x82

#define EPNUM_CDC1_NOTIF  0x83
#define EPNUM_CDC1_OUT    0x04
#define EPNUM_CDC1_IN     0x84

tusb_desc_device_t const desc_device = {
  .bLength            = sizeof(tusb_desc_device_t),
  .bDescriptorType    = TUSB_DESC_DEVICE,
  .bcdUSB             = 0x0200,

  .bDeviceClass       = TUSB_CLASS_MISC,
  .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
  .bDeviceProtocol    = MISC_PROTOCOL_IAD,

  .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
  .idVendor           = USB_VID,
  .idProduct          = USB_PID,
  .bcdDevice          = USB_BCD,

  .iManufacturer      = 0x01,
  .iProduct           = 0x02,
  .iSerialNumber      = 0x03,

  .bNumConfigurations = 0x01
};

uint8_t const desc_configuration[] = {
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, (TUD_CONFIG_DESC_LEN + 2 * TUD_CDC_DESC_LEN),
                        TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

  // CDC0
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC0, 4,
                     EPNUM_CDC0_NOTIF, 8,
                     EPNUM_CDC0_OUT, EPNUM_CDC0_IN, 64),

  // CDC1
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC1, 5,
                     EPNUM_CDC1_NOTIF, 8,
                     EPNUM_CDC1_OUT, EPNUM_CDC1_IN, 64),
};

uint8_t const *tud_descriptor_device_cb(void)
{
  return (uint8_t const *)&desc_device;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index;
  return desc_configuration;
}

// String descriptors (UTF-16LE)
static char const *string_desc_arr[] = {
  (const char[]) { 0x09, 0x04 }, // 0: English (0x0409)
  "Espressif",                   // 1
  "ESP32-S3 Dual CDC",           // 2
  "123456",                      // 3
  "GVRET CDC",                    // 4 (CDC0 interface string index)
  "Monitor CDC",                 // 5 (CDC1 interface string index)
};

static uint16_t _desc_str[32];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if (index == 0) {
    _desc_str[1] = 0x0409;
    chr_count = 1;
  } else {
    if (index >= (sizeof(string_desc_arr) / sizeof(string_desc_arr[0]))) {
      return NULL;
    }

    const char *str = string_desc_arr[index];
    chr_count = 0;

    while (str[chr_count] && chr_count < 31) {
      _desc_str[1 + chr_count] = (uint16_t) str[chr_count];
      chr_count++;
    }
  }

  _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));
  return _desc_str;
}
