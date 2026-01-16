#include <Arduino.h>
#include "tusb.h"

static void cdc_write_str(uint8_t itf, const char *s)
{
    if (!tud_cdc_n_connected(itf))
    {
        return;
    }
    tud_cdc_n_write_str(itf, s);
    tud_cdc_n_write_flush(itf);
}

void setup()
{
    while (!tud_mounted())
    {
        tud_task();
        delay(10);
    }
}

void loop()
{
    tud_task();

    static bool sent0 = false;
    static bool sent1 = false;
    if (!sent0 && tud_cdc_n_connected(0))
    {
        cdc_write_str(0, "CDC0: hello\r\n");
        sent0 = true;
    }
    if (!sent1 && tud_cdc_n_connected(1))
    {
        cdc_write_str(1, "CDC1: hello\r\n");
        sent1 = true;
    }

    // Echo on CDC0 (lowercase)
    if (tud_cdc_n_available(0))
    {
        char buf[64];
        uint32_t n = tud_cdc_n_read(0, buf, sizeof(buf));
        for (uint32_t i = 0; i < n; i++)
        {
            if (buf[i] >= 'A' && buf[i] <= 'Z')
            {
                buf[i] = (char)(buf[i] - 'A' + 'a');
            }
        }
        tud_cdc_n_write(0, buf, n);
        tud_cdc_n_write_flush(0);
    }

    // Echo on CDC1 (uppercase)
    if (tud_cdc_n_available(1))
    {
        char buf[64];
        uint32_t n = tud_cdc_n_read(1, buf, sizeof(buf));
        for (uint32_t i = 0; i < n; i++)
        {
            if (buf[i] >= 'a' && buf[i] <= 'z')
            {
                buf[i] = (char)(buf[i] - 'a' + 'A');
            }
        }
        tud_cdc_n_write(1, buf, n);
        tud_cdc_n_write_flush(1);
    }

    delay(1);
}
