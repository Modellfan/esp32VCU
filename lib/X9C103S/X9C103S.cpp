#include "X9C103S.h"

X9C103S::X9C103S(uint8_t csPin, uint8_t incPin, uint8_t udPin)
    : _csPin(csPin), _incPin(incPin), _udPin(udPin), _wiper(0) {
}

void X9C103S::begin() {
    pinMode(_csPin, OUTPUT);
    pinMode(_incPin, OUTPUT);
    pinMode(_udPin, OUTPUT);

    digitalWrite(_csPin, HIGH);
    digitalWrite(_incPin, HIGH);
    digitalWrite(_udPin, LOW);
    delayMicroseconds(10);

    setPercent(0);
}

void X9C103S::setPercent(uint8_t percent) {
    if (percent > 100U) {
        percent = 100U;
    }

    const uint8_t targetWiper = percentToWiper(percent);

    select();
    pulse(kMaxWiper, false);
    if (targetWiper > 0U) {
        pulse(targetWiper, true);
    }
    deselect();

    _wiper = targetWiper;
}

uint8_t X9C103S::percent() const {
    return (uint8_t)(((uint16_t)_wiper * 100U + (kMaxWiper / 2U)) / kMaxWiper);
}

uint8_t X9C103S::wiper() const {
    return _wiper;
}

void X9C103S::pulse(uint8_t count, bool up) {
    digitalWrite(_udPin, up ? HIGH : LOW);
    delayMicroseconds(10);

    for (uint8_t i = 0; i < count; i++) {
        digitalWrite(_incPin, LOW);
        delayMicroseconds(5);
        digitalWrite(_incPin, HIGH);
        delayMicroseconds(5);
    }
}

void X9C103S::select() {
    digitalWrite(_incPin, HIGH);
    digitalWrite(_csPin, LOW);
    delayMicroseconds(10);
}

void X9C103S::deselect() {
    digitalWrite(_incPin, LOW);
    delayMicroseconds(10);
    digitalWrite(_csPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_incPin, HIGH);
}

uint8_t X9C103S::percentToWiper(uint8_t percent) {
    return (uint8_t)(((uint16_t)percent * kMaxWiper + 50U) / 100U);
}
