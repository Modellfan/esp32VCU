// SIL stub: the parts of the Arduino core used by the firmware modules under test.
// Time is driven by the simulation (sil_api.cpp), not by a real clock.
#ifndef SIL_ARDUINO_H
#define SIL_ARDUINO_H

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <string>

uint32_t millis();
uint32_t micros();

// Minimal String: only what signal_table.cpp uses (parse helpers).
class String {
public:
    String() = default;
    String(const char *text) : value_(text ? text : "") {}
    String(const std::string &text) : value_(text) {}
    const char *c_str() const { return value_.c_str(); }
    unsigned int length() const { return (unsigned int)value_.size(); }
    bool operator==(const char *other) const { return value_ == (other ? other : ""); }
    bool operator==(const String &other) const { return value_ == other.value_; }
    bool equalsIgnoreCase(const char *other) const { return strcasecmp(value_.c_str(), other) == 0; }

private:
    std::string value_;
};

#endif
