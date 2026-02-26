#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <FastLED.h>

// RGB LED Config (ESP32-S3 Built-in)
#ifndef RGB_BUILTIN
#define RGB_BUILTIN 48
#endif

#ifndef RGB_BRIGHTNESS
#define RGB_BRIGHTNESS 32
#endif

#ifndef FASTLED_DATA_PIN
#define FASTLED_DATA_PIN 48
#endif

enum LedColor : uint8_t {
    LED_RED = 0,
    LED_GREEN,
    LED_BLUE,
    LED_YELLOW,
    LED_WHITE
};

inline CRGB *fastledBuffer() {
    static CRGB leds[1];
    return leds;
}

inline void fastledInit() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    CRGB *leds = fastledBuffer();
    FastLED.addLeds<WS2812, FASTLED_DATA_PIN, GRB>(leds, 1);
    FastLED.setBrightness(RGB_BRIGHTNESS);
    leds[0] = CRGB::Black;
    FastLED.show();
    initialized = true;
}

inline void fastled(LedColor color) {
    CRGB *leds = fastledBuffer();
    CRGB rgb = CRGB::Black;

    switch (color) {
        case LED_RED:    rgb = CRGB::Red; break;
        case LED_GREEN:  rgb = CRGB::Green; break;
        case LED_BLUE:   rgb = CRGB::Blue; break;
        case LED_YELLOW: rgb = CRGB::Yellow; break;
        case LED_WHITE:  rgb = CRGB::White; break;
        default:         rgb = CRGB::Black; break;
    }

    leds[0] = rgb;
    FastLED.show();
}

inline void fastledOff() {
    CRGB *leds = fastledBuffer();
    leds[0] = CRGB::Black;
    FastLED.show();
}

#endif
