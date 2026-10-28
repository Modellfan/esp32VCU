#include "tesla_sdu_messages.h"
#include "can_utils.h"

namespace params {
TeslaSduSignals tesla_sdu;
}

static void clearFrame(CANMessage &frame, uint16_t id, uint8_t len) {
    frame.id = id;
    frame.len = len;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }
}

static int32_t roundToInt(float value) {
    return (int32_t)(value >= 0.0f ? value + 0.5f : value - 0.5f);
}

static uint32_t clampUnsigned(int32_t value, uint32_t maxValue) {
    if (value < 0) {
        return 0;
    }
    const uint32_t unsignedValue = (uint32_t)value;
    return unsignedValue > maxValue ? maxValue : unsignedValue;
}

static int32_t clampSigned(int32_t value, int32_t minValue, int32_t maxValue) {
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

static int32_t readSigned(const uint8_t *data, uint16_t startBit, uint8_t bitLen) {
    return signExtend(readBitsLE(data, startBit, bitLen), bitLen);
}

// OpenInverter stm32-sine 0x500 - DC electrical
bool decodeTeslaSdu0x500(const CANMessage &frame) {
    if (frame.id != 0x500 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.udc = (float)readBitsLE(frame.data, 0, 16) * 0.1f;
    params::tesla_sdu.idc = (float)(int16_t)readSigned(frame.data, 16, 16) * 0.1f;
    params::tesla_sdu.il1 = (float)(int16_t)readSigned(frame.data, 32, 16) * 0.1f;
    params::tesla_sdu.il2 = (float)(int16_t)readSigned(frame.data, 48, 16) * 0.1f;
    return true;
}

void encodeTeslaSdu0x500(CANMessage &frame) {
    clearFrame(frame, 0x500, 8);
    writeBitsLE(frame.data, 0, 16, clampUnsigned(roundToInt(params::tesla_sdu.udc / 0.1f), 0xFFFFU));
    writeBitsLE(frame.data, 16, 16, (uint16_t)clampSigned(roundToInt(params::tesla_sdu.idc / 0.1f), -32768, 32767));
    writeBitsLE(frame.data, 32, 16, (uint16_t)clampSigned(roundToInt(params::tesla_sdu.il1 / 0.1f), -32768, 32767));
    writeBitsLE(frame.data, 48, 16, (uint16_t)clampSigned(roundToInt(params::tesla_sdu.il2 / 0.1f), -32768, 32767));
}

// OpenInverter stm32-sine 0x501 - Motor output
bool decodeTeslaSdu0x501(const CANMessage &frame) {
    if (frame.id != 0x501 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.fstat = (float)(int16_t)readSigned(frame.data, 0, 16) * 0.1f;
    params::tesla_sdu.speed = (int16_t)readSigned(frame.data, 16, 16);
    params::tesla_sdu.uac = (float)readBitsLE(frame.data, 32, 16) * 0.1f;
    params::tesla_sdu.ilmax = (float)readBitsLE(frame.data, 48, 16) * 0.1f;
    return true;
}

void encodeTeslaSdu0x501(CANMessage &frame) {
    clearFrame(frame, 0x501, 8);
    writeBitsLE(frame.data, 0, 16, (uint16_t)clampSigned(roundToInt(params::tesla_sdu.fstat / 0.1f), -32768, 32767));
    writeBitsLE(frame.data, 16, 16, (uint16_t)params::tesla_sdu.speed);
    writeBitsLE(frame.data, 32, 16, clampUnsigned(roundToInt(params::tesla_sdu.uac / 0.1f), 0xFFFFU));
    writeBitsLE(frame.data, 48, 16, clampUnsigned(roundToInt(params::tesla_sdu.ilmax / 0.1f), 0xFFFFU));
}

// OpenInverter stm32-sine 0x502 - RMS and derived SINE values
bool decodeTeslaSdu0x502(const CANMessage &frame) {
    if (frame.id != 0x502 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.il1rms = (float)readBitsLE(frame.data, 0, 16) * 0.1f;
    params::tesla_sdu.il2rms = (float)readBitsLE(frame.data, 16, 16) * 0.1f;
    params::tesla_sdu.boostcalc = (float)readBitsLE(frame.data, 32, 16);
    params::tesla_sdu.fweakcalc = (float)readBitsLE(frame.data, 48, 16) * 0.1f;
    return true;
}

void encodeTeslaSdu0x502(CANMessage &frame) {
    clearFrame(frame, 0x502, 8);
    writeBitsLE(frame.data, 0, 16, clampUnsigned(roundToInt(params::tesla_sdu.il1rms / 0.1f), 0xFFFFU));
    writeBitsLE(frame.data, 16, 16, clampUnsigned(roundToInt(params::tesla_sdu.il2rms / 0.1f), 0xFFFFU));
    writeBitsLE(frame.data, 32, 16, clampUnsigned(roundToInt(params::tesla_sdu.boostcalc), 0xFFFFU));
    writeBitsLE(frame.data, 48, 16, clampUnsigned(roundToInt(params::tesla_sdu.fweakcalc / 0.1f), 0xFFFFU));
}

// OpenInverter stm32-sine 0x503 - Driver demand
bool decodeTeslaSdu0x503(const CANMessage &frame) {
    if (frame.id != 0x503 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.pot = (uint16_t)readBitsLE(frame.data, 0, 12);
    params::tesla_sdu.pot2 = (uint16_t)readBitsLE(frame.data, 12, 12);
    params::tesla_sdu.potnom = (int8_t)readSigned(frame.data, 24, 8);
    params::tesla_sdu.amp = (uint16_t)readBitsLE(frame.data, 32, 16);
    params::tesla_sdu.angle = (float)readBitsLE(frame.data, 48, 16) * 0.1f;
    return true;
}

void encodeTeslaSdu0x503(CANMessage &frame) {
    clearFrame(frame, 0x503, 8);
    writeBitsLE(frame.data, 0, 12, clampUnsigned(params::tesla_sdu.pot, 0x0FFFU));
    writeBitsLE(frame.data, 12, 12, clampUnsigned(params::tesla_sdu.pot2, 0x0FFFU));
    writeBitsLE(frame.data, 24, 8, (uint8_t)params::tesla_sdu.potnom);
    writeBitsLE(frame.data, 32, 16, params::tesla_sdu.amp);
    writeBitsLE(frame.data, 48, 16, clampUnsigned(roundToInt(params::tesla_sdu.angle / 0.1f), 0xFFFFU));
}

// OpenInverter stm32-sine 0x504 - Direction, regen and thermal state
bool decodeTeslaSdu0x504(const CANMessage &frame) {
    if (frame.id != 0x504 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.regenpreset = (uint8_t)readBitsLE(frame.data, 0, 8);
    params::tesla_sdu.regenpresent = params::tesla_sdu.regenpreset;
    params::tesla_sdu.regenpresent32 = params::tesla_sdu.regenpreset;
    params::tesla_sdu.seldir = (int8_t)readSigned(frame.data, 8, 8);
    params::tesla_sdu.rotordir = (int8_t)readSigned(frame.data, 16, 8);
    params::tesla_sdu.seldir43 = params::tesla_sdu.rotordir;
    params::tesla_sdu.tmphs = (uint8_t)readBitsLE(frame.data, 32, 8);
    params::tesla_sdu.tmpm = (uint8_t)readBitsLE(frame.data, 40, 8);
    params::tesla_sdu.temperature_heatsink = params::tesla_sdu.tmphs;
    params::tesla_sdu.temperature_heatsink74 = params::tesla_sdu.tmpm;
    params::tesla_sdu.uaux = (float)readBitsLE(frame.data, 48, 16) * 0.1f;
    return true;
}

void encodeTeslaSdu0x504(CANMessage &frame) {
    clearFrame(frame, 0x504, 8);
    writeBitsLE(frame.data, 0, 8, params::tesla_sdu.regenpreset);
    writeBitsLE(frame.data, 8, 8, (uint8_t)params::tesla_sdu.seldir);
    writeBitsLE(frame.data, 16, 8, (uint8_t)params::tesla_sdu.rotordir);
    writeBitsLE(frame.data, 32, 8, clampUnsigned(roundToInt(params::tesla_sdu.tmphs), 0xFFU));
    writeBitsLE(frame.data, 40, 8, clampUnsigned(roundToInt(params::tesla_sdu.tmpm), 0xFFU));
    writeBitsLE(frame.data, 48, 16, clampUnsigned(roundToInt(params::tesla_sdu.uaux / 0.1f), 0xFFFFU));
}

// OpenInverter stm32-sine 0x505 - Mode, fault and status
bool decodeTeslaSdu0x505(const CANMessage &frame) {
    if (frame.id != 0x505 || frame.len < 7) {
        return false;
    }

    params::tesla_sdu.opmode = (uint8_t)readBitsLE(frame.data, 0, 8);
    params::tesla_sdu.lasterr = (uint8_t)readBitsLE(frame.data, 8, 8);
    params::tesla_sdu.pwmio = (uint8_t)readBitsLE(frame.data, 16, 8);
    params::tesla_sdu.canio = (uint8_t)readBitsLE(frame.data, 24, 8);
    params::tesla_sdu.status = (uint16_t)readBitsLE(frame.data, 32, 16);
    params::tesla_sdu.cpuload = (float)readBitsLE(frame.data, 48, 8);
    return true;
}

void encodeTeslaSdu0x505(CANMessage &frame) {
    clearFrame(frame, 0x505, 7);
    writeBitsLE(frame.data, 0, 8, (uint8_t)params::tesla_sdu.opmode);
    writeBitsLE(frame.data, 8, 8, (uint8_t)params::tesla_sdu.lasterr);
    writeBitsLE(frame.data, 16, 8, (uint8_t)params::tesla_sdu.pwmio);
    writeBitsLE(frame.data, 24, 8, params::tesla_sdu.canio);
    writeBitsLE(frame.data, 32, 16, params::tesla_sdu.status);
    writeBitsLE(frame.data, 48, 8, clampUnsigned(roundToInt(params::tesla_sdu.cpuload), 0xFFU));
}

// OpenInverter stm32-sine 0x506 - Digital inputs A
bool decodeTeslaSdu0x506(const CANMessage &frame) {
    if (frame.id != 0x506 || frame.len < 2) {
        return false;
    }

    params::tesla_sdu.din_cruise = (int8_t)readBitsLE(frame.data, 0, 2);
    params::tesla_sdu.din_start = (int8_t)readBitsLE(frame.data, 2, 2);
    params::tesla_sdu.din_brake = (int8_t)readBitsLE(frame.data, 4, 2);
    params::tesla_sdu.din_mprot = (int8_t)readBitsLE(frame.data, 6, 2);
    params::tesla_sdu.din_forward = (int8_t)readBitsLE(frame.data, 8, 2);
    params::tesla_sdu.din_reverse = (int8_t)readBitsLE(frame.data, 10, 2);
    params::tesla_sdu.din_emcystop = (int8_t)readBitsLE(frame.data, 12, 2);
    params::tesla_sdu.din_ocur = (int8_t)readBitsLE(frame.data, 14, 2);
    params::tesla_sdu.din_ocur51 = params::tesla_sdu.din_ocur;
    return true;
}

void encodeTeslaSdu0x506(CANMessage &frame) {
    clearFrame(frame, 0x506, 2);
    writeBitsLE(frame.data, 0, 2, (uint8_t)params::tesla_sdu.din_cruise & 0x03U);
    writeBitsLE(frame.data, 2, 2, (uint8_t)params::tesla_sdu.din_start & 0x03U);
    writeBitsLE(frame.data, 4, 2, (uint8_t)params::tesla_sdu.din_brake & 0x03U);
    writeBitsLE(frame.data, 6, 2, (uint8_t)params::tesla_sdu.din_mprot & 0x03U);
    writeBitsLE(frame.data, 8, 2, (uint8_t)params::tesla_sdu.din_forward & 0x03U);
    writeBitsLE(frame.data, 10, 2, (uint8_t)params::tesla_sdu.din_reverse & 0x03U);
    writeBitsLE(frame.data, 12, 2, (uint8_t)params::tesla_sdu.din_emcystop & 0x03U);
    writeBitsLE(frame.data, 14, 2, (uint8_t)params::tesla_sdu.din_ocur & 0x03U);
}

// OpenInverter stm32-sine 0x507 - Digital inputs B and cruise speed
bool decodeTeslaSdu0x507(const CANMessage &frame) {
    if (frame.id != 0x507 || frame.len < 4) {
        return false;
    }

    params::tesla_sdu.din_desat = (int8_t)readBitsLE(frame.data, 0, 2);
    params::tesla_sdu.din_bms = (int8_t)readBitsLE(frame.data, 2, 2);
    params::tesla_sdu.cruisespeed = (int16_t)readSigned(frame.data, 16, 16);
    return true;
}

void encodeTeslaSdu0x507(CANMessage &frame) {
    clearFrame(frame, 0x507, 4);
    writeBitsLE(frame.data, 0, 2, (uint8_t)params::tesla_sdu.din_desat & 0x03U);
    writeBitsLE(frame.data, 2, 2, (uint8_t)params::tesla_sdu.din_bms & 0x03U);
    writeBitsLE(frame.data, 16, 16, (uint16_t)params::tesla_sdu.cruisespeed);
}

// OpenInverter stm32-sine 0x508 - Turns and uptime
bool decodeTeslaSdu0x508(const CANMessage &frame) {
    if (frame.id != 0x508 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.turns = readSigned(frame.data, 0, 32);
    params::tesla_sdu.uptime = readBitsLE(frame.data, 32, 32);
    params::tesla_sdu.uptime_s = (float)params::tesla_sdu.uptime * 0.01f;
    return true;
}

void encodeTeslaSdu0x508(CANMessage &frame) {
    clearFrame(frame, 0x508, 8);
    writeBitsLE(frame.data, 0, 32, (uint32_t)params::tesla_sdu.turns);
    writeBitsLE(frame.data, 32, 32, params::tesla_sdu.uptime);
}

bool decodeTeslaSdu(const CANMessage &frame) {
    switch (frame.id) {
        case 0x500: return decodeTeslaSdu0x500(frame);
        case 0x501: return decodeTeslaSdu0x501(frame);
        case 0x502: return decodeTeslaSdu0x502(frame);
        case 0x503: return decodeTeslaSdu0x503(frame);
        case 0x504: return decodeTeslaSdu0x504(frame);
        case 0x505: return decodeTeslaSdu0x505(frame);
        case 0x506: return decodeTeslaSdu0x506(frame);
        case 0x507: return decodeTeslaSdu0x507(frame);
        case 0x508: return decodeTeslaSdu0x508(frame);
        default: return false;
    }
}
