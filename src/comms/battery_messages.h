// Communication layer: CAN0 (TWAI) on the bus of TeensyBMS, Zombie-Slave and Tesla DC/DC
// (VCU-REQ-018, VCU-DEC-018). Layout: documentation/ZombieSlave_teensybms_dcdc_mvcu.dbc.
// The VCU listens, and as "mVCU" sends the heater request 0x43A (VCU-REQ-021).
//
// TeensyBMS and Zombie-Slave frames carry a 4-bit counter (byte 6) and a CRC (byte 7: low
// byte of the STM32 CRC-32 over bytes 0..7 with byte 7 = 0). Frames with a wrong CRC are
// dropped. Sign convention of the BMS: current and power positive = discharge.
#ifndef BATTERY_MESSAGES_H
#define BATTERY_MESSAGES_H

#include <ACAN2515.h>
#include <Arduino.h>

namespace params {

struct BatterySignals {
    // TeensyBMS 0x41A
    float pack_voltage_v = 0.0f;
    float pack_current_a = 0.0f;       // + discharge, - charge
    float cell_v_min = 0.0f;
    float cell_v_max = 0.0f;
    // TeensyBMS 0x41B
    int16_t cell_t_min_c = 0;
    int16_t cell_t_max_c = 0;
    float balancing_v = 0.0f;
    uint16_t cell_delta_mv = 0;
    float pack_power_kw = 0.0f;        // + discharge, - charge
    // TeensyBMS 0x41C
    float max_discharge_a = 0.0f;
    float max_charge_a = 0.0f;
    uint8_t contactor_state = 0;       // 0 INIT, 1 OPEN, 2/3 CLOSING, 4 CLOSED, 5/6 OPENING, 7 FAULT
    uint8_t dtc_flags = 0;             // bit 0 CAN send, 1 CAN init, 2 pack, 3 contactor, 4 shunt
    // TeensyBMS 0x41D
    float soc_pct = 0.0f;
    float soh_pct = 0.0f;
    uint8_t balancing_status = 0;      // 0 idle, 1 balancing, 2 finished
    uint8_t bms_state = 0;             // 0 INIT, 1 OPERATING, 2 FAULT
    // TeensyBMS 0x41E
    float avg_power_kw = 0.0f;         // average energy per hour
    uint16_t remaining_time_s = 0;     // to empty (discharge) or to full (charge)
    float remaining_energy_kwh = 0.0f;
    // Zombie-Slave 0x437
    uint8_t vehicle_state = 0;         // 0 SLEEP .. 4 READY, 6 DRIVE, 7 CHARGE, 8 ERROR, 9 LIMP_HOME
    bool force_shutdown = false;
    uint8_t hv_request = 0;
    bool charging_active = false;      // vehicle_state == CHARGE
    // Zombie-Slave 0x438
    uint32_t charge_power_w = 0;       // AC input, estimated
    uint32_t charge_power_max_w = 0;
    // Zombie-Slave 0x439
    bool heater_active = false;
    bool heater_contactor_closed = false;
    bool heater_fault = false;
    bool heater_thermal_ok = false;
    bool heater_contactor_out = false;
    bool heater_can_request = false;   // request from 0x43A as seen by the Zombie-Slave
    // Tesla DC/DC 0x210
    float dcdc_output_v = 0.0f;
    uint8_t dcdc_output_a = 0;
    uint16_t dcdc_input_w = 0;
    float dcdc_coolant_c = 0.0f;
    uint16_t dcdc_fault_bits = 0;      // bits 0..11 of the frame (12 status/fault flags)
    bool dcdc_coolant_request = false;
    // Not on CAN today (Zombie-Slave internal, see documentation/signalliste_ui_tcu.md, section 9)
    bool plug_connected = false;
    // Statistics
    uint32_t crc_errors = 0;
};

// Charging parameters set through /api/set; no message to the charger defined yet.
struct ChargingSignals {
    uint8_t target_charge_pct = 80;   // 50..100
    uint8_t ac_current_limit_a = 16;  // 6..32
};

// Heater request of the VCU (mVCU), sent in 0x43A every 100 ms. Not stored.
struct HeaterSignals {
    bool heater_request = false;
};

extern BatterySignals battery;
extern ChargingSignals charging;
extern HeaterSignals heater;

}  // namespace params

// CAN0 receive (params lock held): true when the frame was decoded.
bool decodeBattery(const CANMessage &frame);
// 0x43A Heater_CloseRequest with counter and CRC (params lock held), every 100 ms.
void encodeHeaterControl(CANMessage &frame);

#endif
