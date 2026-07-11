#ifndef X9C103S_H
#define X9C103S_H

#include <Arduino.h>

class X9C103S {
public:
    X9C103S(uint8_t csPin, uint8_t incPin, uint8_t udPin);

    void begin();
    void setPercent(uint8_t percent);
    uint8_t percent() const;
    uint8_t wiper() const;

private:
    static constexpr uint8_t kMaxWiper = 99;

    void pulse(uint8_t count, bool up);
    void select();
    void deselect();
    static uint8_t percentToWiper(uint8_t percent);

    uint8_t _csPin;
    uint8_t _incPin;
    uint8_t _udPin;
    uint8_t _wiper;
};

#endif
