#include "tesla_sdu_messages.h"
#include "986_can_codec.h"
#include "can_utils.h"
namespace params {
TeslaSduSignals tesla_sdu;
}

bool decode0x320(const CANMessage &frame) {
    if (frame.id != 0x320 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.opmode = (int8_t)can986::signExtend(canutils::readBitsLE(frame.data, 0, 8), 8);
    params::tesla_sdu.lasterr = (int8_t)can986::signExtend(canutils::readBitsLE(frame.data, 8, 8), 8);
    params::tesla_sdu.status = (int8_t)can986::signExtend(canutils::readBitsLE(frame.data, 16, 8), 8);
    params::tesla_sdu.din_ocur = (int8_t)can986::signExtend(canutils::readBitsLE(frame.data, 24, 8), 8);
    params::tesla_sdu.din_ocur51 = (int8_t)can986::signExtend(canutils::readBitsLE(frame.data, 32, 8), 8);
    params::tesla_sdu.din_bms = (int8_t)can986::signExtend(canutils::readBitsLE(frame.data, 40, 8), 8);
    return true;
}

void encode0x320(CANMessage &frame) {
    frame.id = 0x320;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    canutils::writeBitsLE(frame.data, 0, 8, (uint8_t)params::tesla_sdu.opmode);
    canutils::writeBitsLE(frame.data, 8, 8, (uint8_t)params::tesla_sdu.lasterr);
    canutils::writeBitsLE(frame.data, 16, 8, (uint8_t)params::tesla_sdu.status);
    canutils::writeBitsLE(frame.data, 24, 8, (uint8_t)params::tesla_sdu.din_ocur);
    canutils::writeBitsLE(frame.data, 32, 8, (uint8_t)params::tesla_sdu.din_ocur51);
    canutils::writeBitsLE(frame.data, 40, 8, (uint8_t)params::tesla_sdu.din_bms);
}

bool decode0x321(const CANMessage &frame) {
    if (frame.id != 0x321 || frame.len < 8) {
        return false;
    }

    const int32_t rawUdc = can986::signExtend(canutils::readBitsLE(frame.data, 0, 32), 32);
    const int32_t rawIdc = can986::signExtend(canutils::readBitsLE(frame.data, 32, 32), 32);
    params::tesla_sdu.udc = rawUdc * 0.001f;
    params::tesla_sdu.idc = rawIdc * 0.001f;
    return true;
}

void encode0x321(CANMessage &frame) {
    frame.id = 0x321;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const int32_t rawUdc = (int32_t)(params::tesla_sdu.udc / 0.001f);
    const int32_t rawIdc = (int32_t)(params::tesla_sdu.idc / 0.001f);
    canutils::writeBitsLE(frame.data, 0, 32, (uint32_t)rawUdc);
    canutils::writeBitsLE(frame.data, 32, 32, (uint32_t)rawIdc);
}

bool decode0x322(const CANMessage &frame) {
    if (frame.id != 0x322 || frame.len < 8) {
        return false;
    }

    params::tesla_sdu.speed = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 0, 16), 16);
    params::tesla_sdu.cruisespeed = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 16, 16), 16);
    params::tesla_sdu.pot = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 32, 16), 16);
    params::tesla_sdu.pot2 = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 48, 16), 16);
    return true;
}

void encode0x322(CANMessage &frame) {
    frame.id = 0x322;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    canutils::writeBitsLE(frame.data, 0, 16, (uint16_t)params::tesla_sdu.speed);
    canutils::writeBitsLE(frame.data, 16, 16, (uint16_t)params::tesla_sdu.cruisespeed);
    canutils::writeBitsLE(frame.data, 32, 16, (uint16_t)params::tesla_sdu.pot);
    canutils::writeBitsLE(frame.data, 48, 16, (uint16_t)params::tesla_sdu.pot2);
}

bool decode0x323(const CANMessage &frame) {
    if (frame.id != 0x323 || frame.len < 8) {
        return false;
    }

    const int16_t rawRegenPresent = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 0, 16), 16);
    const int16_t rawRegenPresent32 = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 16, 16), 16);
    params::tesla_sdu.regenpresent = rawRegenPresent * 0.01f;
    params::tesla_sdu.regenpresent32 = rawRegenPresent32 * 0.01f;
    params::tesla_sdu.seldir = (int8_t)can986::signExtend(canutils::readBitsLE(frame.data, 32, 8), 8);
    params::tesla_sdu.seldir43 = (int8_t)can986::signExtend(canutils::readBitsLE(frame.data, 40, 8), 8);
    return true;
}

void encode0x323(CANMessage &frame) {
    frame.id = 0x323;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const int16_t rawRegenPresent = (int16_t)(params::tesla_sdu.regenpresent / 0.01f);
    const int16_t rawRegenPresent32 = (int16_t)(params::tesla_sdu.regenpresent32 / 0.01f);
    canutils::writeBitsLE(frame.data, 0, 16, (uint16_t)rawRegenPresent);
    canutils::writeBitsLE(frame.data, 16, 16, (uint16_t)rawRegenPresent32);
    canutils::writeBitsLE(frame.data, 32, 8, (uint8_t)params::tesla_sdu.seldir);
    canutils::writeBitsLE(frame.data, 40, 8, (uint8_t)params::tesla_sdu.seldir43);
}

bool decode0x324(const CANMessage &frame) {
    if (frame.id != 0x324 || frame.len < 8) {
        return false;
    }

    const int16_t rawTempHeatsink = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 0, 16), 16);
    const int16_t rawTempHeatsink74 = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 16, 16), 16);
    const int16_t rawUaux = (int16_t)can986::signExtend(canutils::readBitsLE(frame.data, 32, 16), 16);
    params::tesla_sdu.temperature_heatsink = rawTempHeatsink * 0.01f;
    params::tesla_sdu.temperature_heatsink74 = rawTempHeatsink74 * 0.01f;
    params::tesla_sdu.uaux = rawUaux * 0.01f;
    return true;
}

void encode0x324(CANMessage &frame) {
    frame.id = 0x324;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    const int16_t rawTempHeatsink = (int16_t)(params::tesla_sdu.temperature_heatsink / 0.01f);
    const int16_t rawTempHeatsink74 = (int16_t)(params::tesla_sdu.temperature_heatsink74 / 0.01f);
    const int16_t rawUaux = (int16_t)(params::tesla_sdu.uaux / 0.01f);
    canutils::writeBitsLE(frame.data, 0, 16, (uint16_t)rawTempHeatsink);
    canutils::writeBitsLE(frame.data, 16, 16, (uint16_t)rawTempHeatsink74);
    canutils::writeBitsLE(frame.data, 32, 16, (uint16_t)rawUaux);
}



