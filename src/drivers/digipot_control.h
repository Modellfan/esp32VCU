// Fuel gauge: X9C103S digital potentiometer emulating the tank sender (VCU-REQ-009).
#ifndef DIGIPOT_CONTROL_H
#define DIGIPOT_CONTROL_H

#include <Arduino.h>

namespace params {

struct DigipotSignals {
    uint8_t digipot_pct = 0;          // requested position (web service)
    uint8_t digipot_applied_pct = 0;  // position of the potentiometer
    uint8_t digipot_wiper = 0;
};

extern DigipotSignals digipot;

}  // namespace params

// Every CAN cycle, with the params lock held: limits the request to 0..100 %.
inline void digipotClampRequest() {
    if (params::digipot.digipot_pct > 100U) {
        params::digipot.digipot_pct = 100U;
    }
}

void digipotBegin();
// Steps the potentiometer only when the set point changed (a full sequence takes about 2 ms).
void digipotApply(uint8_t percent);
// Copies position and wiper into params::digipot (params lock held).
void digipotReportState();

#endif
