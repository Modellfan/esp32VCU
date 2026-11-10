#include "battery_messages.h"

#include "can_utils.h"

namespace params {
BatterySignals battery;
ChargingSignals charging;
HeaterSignals heater;
}  // namespace params

namespace {

// STM32 hardware CRC-32 (poly 0x04C11DB7, init 0xFFFFFFFF, 32-bit words MSB first, no
// reflection) over bytes 0..7 as two little-endian words with byte 7 = 0; low byte.
// Same as TeensyVCU src/utils/can_crc.h and Zombie-Slave mVCUIntegration::CheckCrc().
uint8_t stm32Crc8(const uint8_t *data) {
    uint32_t words[2];
    words[0] = (uint32_t)data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
    words[1] = (uint32_t)data[4] | ((uint32_t)data[5] << 8) | ((uint32_t)data[6] << 16);
    uint32_t crc = 0xFFFFFFFFUL;
    for (uint32_t word : words) {
        crc ^= word;
        for (int bit = 0; bit < 32; bit++) {
            crc = (crc & 0x80000000UL) ? (crc << 1) ^ 0x04C11DB7UL : crc << 1;
        }
    }
    return (uint8_t)(crc & 0xFFU);
}

bool crcOk(const CANMessage &frame) {
    if (frame.len < 8 || stm32Crc8(frame.data) != frame.data[7]) {
        params::battery.crc_errors++;
        return false;
    }
    return true;
}

uint16_t u16(const uint8_t *d, uint8_t i) {
    return (uint16_t)(d[i] | (d[i + 1] << 8));
}

uint32_t u32(const uint8_t *d, uint8_t i) {
    return (uint32_t)d[i] | ((uint32_t)d[i + 1] << 8) | ((uint32_t)d[i + 2] << 16) | ((uint32_t)d[i + 3] << 24);
}

uint8_t gHeaterCounter = 0;

}  // namespace

bool decodeBattery(const CANMessage &frame) {
    if (frame.ext || frame.rtr) {
        return false;
    }
    params::BatterySignals &b = params::battery;
    const uint8_t *d = frame.data;
    switch (frame.id) {
        case 0x41A:  // TeensyBMS_VoltageCurrent
            if (!crcOk(frame)) return false;
            b.pack_voltage_v = u16(d, 0) * 0.1f;
            b.pack_current_a = u16(d, 2) * 0.1f - 500.0f;
            b.cell_v_min = d[4] * 0.02f;
            b.cell_v_max = d[5] * 0.02f;
            return true;
        case 0x41B:  // TeensyBMS_CellTempPower
            if (!crcOk(frame)) return false;
            b.cell_t_min_c = (int16_t)d[0] - 40;
            b.cell_t_max_c = (int16_t)d[1] - 40;
            b.balancing_v = d[2] * 0.02f;
            b.cell_delta_mv = (uint16_t)(d[3] * 2U);
            b.pack_power_kw = u16(d, 4) * 0.01f - 300.0f;
            return true;
        case 0x41C:  // TeensyBMS_LimitsFault
            if (!crcOk(frame)) return false;
            b.max_discharge_a = u16(d, 0) * 0.1f;
            b.max_charge_a = u16(d, 2) * 0.1f;
            b.contactor_state = d[4];
            b.dtc_flags = d[5];
            return true;
        case 0x41D:  // TeensyBMS_SocSoh
            if (!crcOk(frame)) return false;
            b.soc_pct = u16(d, 0) * 0.01f;
            b.soh_pct = u16(d, 2) * 0.01f;
            b.balancing_status = d[4];
            b.bms_state = d[5];
            return true;
        case 0x41E:  // TeensyBMS_Hmi
            if (!crcOk(frame)) return false;
            b.avg_power_kw = (int16_t)u16(d, 0) * 0.01f;
            b.remaining_time_s = u16(d, 2);
            b.remaining_energy_kwh = u16(d, 4) * 0.001f;
            return true;
        case 0x437:  // ZombieSlave_VcuStatus
            if (!crcOk(frame)) return false;
            b.vehicle_state = d[0];
            b.force_shutdown = d[1] != 0;
            b.hv_request = d[2];
            b.charging_active = d[0] == 7U;
            return true;
        case 0x438:  // mVCU_ChargePowerStatus (no counter, no CRC)
            if (frame.len < 8) return false;
            b.charge_power_w = u32(d, 0);
            b.charge_power_max_w = u32(d, 4);
            return true;
        case 0x439:  // mVCU_HeaterStatus
            if (!crcOk(frame)) return false;
            b.heater_active = d[0] != 0;
            b.heater_contactor_closed = d[1] != 0;
            b.heater_fault = d[2] != 0;
            b.heater_thermal_ok = d[3] != 0;
            b.heater_contactor_out = d[4] != 0;
            b.heater_can_request = d[5] != 0;
            return true;
        case 0x210:  // TeslaDCDC_Status (no counter, no CRC)
            if (frame.len < 6) return false;
            b.dcdc_fault_bits = (uint16_t)(u16(d, 0) & 0x0FFFU);
            b.dcdc_coolant_request = (d[1] & 0x01U) != 0;
            b.dcdc_coolant_c = (int8_t)d[2] * 0.5f + 40.0f;
            b.dcdc_input_w = (uint16_t)(d[3] * 16U);
            b.dcdc_output_a = d[4];
            b.dcdc_output_v = d[5] * 0.1f;
            return true;
        default:
            return false;
    }
}

void encodeHeaterControl(CANMessage &frame) {
    frame.id = 0x43A;
    frame.ext = false;
    frame.rtr = false;
    frame.len = 8;
    for (uint8_t i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }
    frame.data[0] = params::heater.heater_request ? 0x01U : 0x00U;
    frame.data[6] = gHeaterCounter;
    frame.data[7] = stm32Crc8(frame.data);
    gHeaterCounter = (uint8_t)((gHeaterCounter + 1U) & 0x0FU);
}
