#include "tesla_sdu_messages.h"
#include "986_can_codec.h"
#include "can_utils.h"
namespace params {
TeslaSduSignals tesla_sdu;
}

bool decodeTeslaSdu0x320(const CANMessage &frame) {
    if (frame.id != 0x320 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.opmode = (int8_t)can986::signExtend(readBitsLE(frame.data, 0, 8), 8);
    params::tesla_sdu.lasterr = (int8_t)can986::signExtend(readBitsLE(frame.data, 8, 8), 8);
    params::tesla_sdu.status = (int8_t)can986::signExtend(readBitsLE(frame.data, 16, 8), 8);
    params::tesla_sdu.din_ocur = (int8_t)can986::signExtend(readBitsLE(frame.data, 24, 8), 8);
    params::tesla_sdu.din_ocur51 = (int8_t)can986::signExtend(readBitsLE(frame.data, 32, 8), 8);
    params::tesla_sdu.din_bms = (int8_t)can986::signExtend(readBitsLE(frame.data, 40, 8), 8);
    return true;
}

void encodeTeslaSdu0x320(CANMessage &frame) {
    frame.id = 0x320;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 8, (uint8_t)params::tesla_sdu.opmode);
    writeBitsLE(frame.data, 8, 8, (uint8_t)params::tesla_sdu.lasterr);
    writeBitsLE(frame.data, 16, 8, (uint8_t)params::tesla_sdu.status);
    writeBitsLE(frame.data, 24, 8, (uint8_t)params::tesla_sdu.din_ocur);
    writeBitsLE(frame.data, 32, 8, (uint8_t)params::tesla_sdu.din_ocur51);
    writeBitsLE(frame.data, 40, 8, (uint8_t)params::tesla_sdu.din_bms);
}

bool decodeTeslaSdu0x321(const CANMessage &frame) {
    if (frame.id != 0x321 || frame.len < 8) {
        return false;
    }

    const int32_t rawUdc = can986::signExtend(readBitsLE(frame.data, 0, 32), 32);
    const int32_t rawIdc = can986::signExtend(readBitsLE(frame.data, 32, 32), 32);
    params::tesla_sdu.udc = rawUdc * 0.001f;
    params::tesla_sdu.idc = rawIdc * 0.001f;
    return true;
}

void encodeTeslaSdu0x321(CANMessage &frame) {
    frame.id = 0x321;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const int32_t rawUdc = (int32_t)(params::tesla_sdu.udc / 0.001f);
    const int32_t rawIdc = (int32_t)(params::tesla_sdu.idc / 0.001f);
    writeBitsLE(frame.data, 0, 32, (uint32_t)rawUdc);
    writeBitsLE(frame.data, 32, 32, (uint32_t)rawIdc);
}

bool decodeTeslaSdu0x322(const CANMessage &frame) {
    if (frame.id != 0x322 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.speed = (int16_t)can986::signExtend(readBitsLE(frame.data, 0, 16), 16);
    params::tesla_sdu.cruisespeed = (int16_t)can986::signExtend(readBitsLE(frame.data, 16, 16), 16);
    params::tesla_sdu.pot = (int16_t)can986::signExtend(readBitsLE(frame.data, 32, 16), 16);
    params::tesla_sdu.pot2 = (int16_t)can986::signExtend(readBitsLE(frame.data, 48, 16), 16);
    return true;
}

void encodeTeslaSdu0x322(CANMessage &frame) {
    frame.id = 0x322;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    writeBitsLE(frame.data, 0, 16, (uint16_t)params::tesla_sdu.speed);
    writeBitsLE(frame.data, 16, 16, (uint16_t)params::tesla_sdu.cruisespeed);
    writeBitsLE(frame.data, 32, 16, (uint16_t)params::tesla_sdu.pot);
    writeBitsLE(frame.data, 48, 16, (uint16_t)params::tesla_sdu.pot2);
}

bool decodeTeslaSdu0x323(const CANMessage &frame) {
    if (frame.id != 0x323 || frame.len < 8) {
        return false;
    }

    const int16_t rawRegenPresent = (int16_t)can986::signExtend(readBitsLE(frame.data, 0, 16), 16);
    const int16_t rawRegenPresent32 = (int16_t)can986::signExtend(readBitsLE(frame.data, 16, 16), 16);
    params::tesla_sdu.regenpresent = rawRegenPresent * 0.01f;
    params::tesla_sdu.regenpresent32 = rawRegenPresent32 * 0.01f;
    params::tesla_sdu.seldir = (int8_t)can986::signExtend(readBitsLE(frame.data, 32, 8), 8);
    params::tesla_sdu.seldir43 = (int8_t)can986::signExtend(readBitsLE(frame.data, 40, 8), 8);
    return true;
}

void encodeTeslaSdu0x323(CANMessage &frame) {
    frame.id = 0x323;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const int16_t rawRegenPresent = (int16_t)(params::tesla_sdu.regenpresent / 0.01f);
    const int16_t rawRegenPresent32 = (int16_t)(params::tesla_sdu.regenpresent32 / 0.01f);
    writeBitsLE(frame.data, 0, 16, (uint16_t)rawRegenPresent);
    writeBitsLE(frame.data, 16, 16, (uint16_t)rawRegenPresent32);
    writeBitsLE(frame.data, 32, 8, (uint8_t)params::tesla_sdu.seldir);
    writeBitsLE(frame.data, 40, 8, (uint8_t)params::tesla_sdu.seldir43);
}

bool decodeTeslaSdu0x324(const CANMessage &frame) {
    if (frame.id != 0x324 || frame.len < 8) {
        return false;
    }

    const int16_t rawTempHeatsink = (int16_t)can986::signExtend(readBitsLE(frame.data, 0, 16), 16);
    const int16_t rawTempHeatsink74 = (int16_t)can986::signExtend(readBitsLE(frame.data, 16, 16), 16);
    const int16_t rawUaux = (int16_t)can986::signExtend(readBitsLE(frame.data, 32, 16), 16);
    params::tesla_sdu.tmphs = rawTempHeatsink * 0.01f;
    params::tesla_sdu.tmpm = rawTempHeatsink74 * 0.01f;
    params::tesla_sdu.temperature_heatsink = params::tesla_sdu.tmphs;
    params::tesla_sdu.temperature_heatsink74 = params::tesla_sdu.tmpm;
    params::tesla_sdu.uaux = rawUaux * 0.01f;
    return true;
}

void encodeTeslaSdu0x324(CANMessage &frame) {
    frame.id = 0x324;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const int16_t rawTempHeatsink = (int16_t)(params::tesla_sdu.tmphs / 0.01f);
    const int16_t rawTempHeatsink74 = (int16_t)(params::tesla_sdu.tmpm / 0.01f);
    const int16_t rawUaux = (int16_t)(params::tesla_sdu.uaux / 0.01f);
    writeBitsLE(frame.data, 0, 16, (uint16_t)rawTempHeatsink);
    writeBitsLE(frame.data, 16, 16, (uint16_t)rawTempHeatsink74);
    writeBitsLE(frame.data, 32, 16, (uint16_t)rawUaux);
}

bool decodeTeslaSdu(const CANMessage &frame) {
    switch (frame.id) {
        case 0x320: return decodeTeslaSdu0x320(frame);
        case 0x321: return decodeTeslaSdu0x321(frame);
        case 0x322: return decodeTeslaSdu0x322(frame);
        case 0x323: return decodeTeslaSdu0x323(frame);
        case 0x324: return decodeTeslaSdu0x324(frame);
        default: return false;
    }
}
