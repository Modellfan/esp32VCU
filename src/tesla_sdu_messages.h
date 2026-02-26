#ifndef TESLA_SDU_MESSAGES_H
#define TESLA_SDU_MESSAGES_H

#include <Arduino.h>
#include <ACAN2515.h>

namespace params {

struct TeslaSduSignals {
    int8_t opmode = 0;
    int8_t lasterr = 0;
    int8_t status = 0;
    int8_t din_ocur = 0;
    int8_t din_ocur51 = 0;
    int8_t din_bms = 0;

    float udc = 0.0f;
    float idc = 0.0f;

    int16_t speed = 0;
    int16_t cruisespeed = 0;
    int16_t pot = 0;
    int16_t pot2 = 0;

    float regenpresent = 0.0f;
    float regenpresent32 = 0.0f;
    int8_t seldir = 0;
    int8_t seldir43 = 0;

    float temperature_heatsink = 0.0f;
    float temperature_heatsink74 = 0.0f;
    float uaux = 0.0f;
};

extern TeslaSduSignals tesla_sdu;

} // namespace params

bool decodeTeslaSdu0x320(const CANMessage &frame);
bool decodeTeslaSdu0x321(const CANMessage &frame);
bool decodeTeslaSdu0x322(const CANMessage &frame);
bool decodeTeslaSdu0x323(const CANMessage &frame);
bool decodeTeslaSdu0x324(const CANMessage &frame);
bool decodeTeslaSdu(const CANMessage &frame);

void encodeTeslaSdu0x320(CANMessage &frame);
void encodeTeslaSdu0x321(CANMessage &frame);
void encodeTeslaSdu0x322(CANMessage &frame);
void encodeTeslaSdu0x323(CANMessage &frame);
void encodeTeslaSdu0x324(CANMessage &frame);

#endif
