#ifndef TESLA_SDU_MESSAGES_H
#define TESLA_SDU_MESSAGES_H

#include <Arduino.h>
#include <ACAN2515.h>

namespace params {

// Tesla SDU and OpenInverter stm32-sine are equivalent names in this project.
// The decoded CAN signal layout follows documentation/can_signals.md IDs 0x500-0x508.
struct TeslaSduSignals {
    uint8_t opmode = 0;
    uint8_t lasterr = 0;
    uint16_t status = 0;

    float udc = 0.0f;
    float idc = 0.0f;
    float il1 = 0.0f;
    float il2 = 0.0f;
    float fstat = 0.0f;

    int16_t speed = 0;
    int16_t cruisespeed = 0;
    int32_t turns = 0;
    uint16_t amp = 0;
    float angle = 0.0f;
    uint16_t pot = 0;
    uint16_t pot2 = 0;
    int8_t potnom = 0;
    uint8_t regenpreset = 0;

    // Compatibility aliases used by existing WebUI screens.
    float regenpresent = 0.0f;
    float regenpresent32 = 0.0f;
    int8_t seldir = 0;
    int8_t rotordir = 0;
    int8_t seldir43 = 0;

    uint8_t tmphs = 0;
    uint8_t tmpm = 0;
    float temperature_heatsink = 0.0f;
    float temperature_heatsink74 = 0.0f;
    float uaux = 0.0f;

    uint8_t pwmio = 0;
    uint8_t canio = 0;

    int8_t din_cruise = 2;
    int8_t din_start = 2;
    int8_t din_brake = 2;
    int8_t din_mprot = 2;
    int8_t din_forward = 2;
    int8_t din_reverse = 2;
    int8_t din_emcystop = 2;
    int8_t din_ocur = 2;
    int8_t din_desat = 2;
    int8_t din_bms = 2;
    int8_t din_ocur51 = 2;

    uint32_t uptime = 0;
    float uptime_s = 0.0f;
    float cpuload = 0.0f;

    float ilmax = 0.0f;
    float uac = 0.0f;
    float il1rms = 0.0f;
    float il2rms = 0.0f;
    float boostcalc = 0.0f;
    float fweakcalc = 0.0f;
};

extern TeslaSduSignals tesla_sdu;

} // namespace params

bool decodeTeslaSdu0x500(const CANMessage &frame);
bool decodeTeslaSdu0x501(const CANMessage &frame);
bool decodeTeslaSdu0x502(const CANMessage &frame);
bool decodeTeslaSdu0x503(const CANMessage &frame);
bool decodeTeslaSdu0x504(const CANMessage &frame);
bool decodeTeslaSdu0x505(const CANMessage &frame);
bool decodeTeslaSdu0x506(const CANMessage &frame);
bool decodeTeslaSdu0x507(const CANMessage &frame);
bool decodeTeslaSdu0x508(const CANMessage &frame);
bool decodeTeslaSdu(const CANMessage &frame);

void encodeTeslaSdu0x500(CANMessage &frame);
void encodeTeslaSdu0x501(CANMessage &frame);
void encodeTeslaSdu0x502(CANMessage &frame);
void encodeTeslaSdu0x503(CANMessage &frame);
void encodeTeslaSdu0x504(CANMessage &frame);
void encodeTeslaSdu0x505(CANMessage &frame);
void encodeTeslaSdu0x506(CANMessage &frame);
void encodeTeslaSdu0x507(CANMessage &frame);
void encodeTeslaSdu0x508(CANMessage &frame);

#endif
