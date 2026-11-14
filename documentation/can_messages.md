# CAN Signal Reference

## Table of Contents

- [Porsche 986 (CAN1)](#porsche-986-can1)
- [OpenInverter stm32-sine](#openinverter-stm32-sine)
  - [CAN ID 0x500 / 1280 - DC electrical](#openinverter-0x500)
  - [CAN ID 0x501 / 1281 - Motor output](#openinverter-0x501)
  - [CAN ID 0x502 / 1282 - RMS and derived SINE values](#openinverter-0x502)
  - [CAN ID 0x503 / 1283 - Driver demand](#openinverter-0x503)
  - [CAN ID 0x504 / 1284 - Direction, regen and thermal state](#openinverter-0x504)
  - [CAN ID 0x505 / 1285 - Mode, fault and status](#openinverter-0x505)
  - [CAN ID 0x506 / 1286 - Digital inputs A](#openinverter-0x506)
  - [CAN ID 0x507 / 1287 - Digital inputs B and cruise speed](#openinverter-0x507)
  - [CAN ID 0x508 / 1288 - Turns and uptime](#openinverter-0x508)
- [CAN0: TeensyBMS, Zombie-Slave, Tesla DC/DC](#can0-bms-zombie)

## Porsche 986 (CAN1)

Bitlage, Skalierung, Zykluszeiten und offene Punkte des Reverse Engineering: [porsche_986_messages.md](porsche_986_messages.md) (Spiegel der DBC [porsche_986_996_compiled_working.dbc](porsche_986_996_compiled_working.dbc), Masterquelle). Hier nur die Zuordnung zum Code.

### Von der VCU gesendet (DME-Emulation)

| CAN ID | Message | DBC Sender | Target File | Decoder | Encoder | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 0x280 | Engine_1 | ECU | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x280](../src/comms/986_ecu_messages.cpp#L9) | [encode986Ecu0x280](../src/comms/986_ecu_messages.cpp#L22) | Implemented |
| 0x289 | Motor_2 | ECU | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x289](../src/comms/986_ecu_messages.cpp) | [encode986Ecu0x289](../src/comms/986_ecu_messages.cpp) | Implemented |
| 0x298 | Engine_2 | ECU | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x298](../src/comms/986_ecu_messages.cpp#L175) | [encode986Ecu0x298](../src/comms/986_ecu_messages.cpp#L189) | Implemented |
| 0x4E0 | EngineStat | ECU | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x4E0](../src/comms/986_ecu_messages.cpp#L213) | [encode986Ecu0x4E0](../src/comms/986_ecu_messages.cpp#L228) | Implemented |
| 0x4F8 | Unknown_4F8 | ECU | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x4F8](../src/comms/986_ecu_messages.cpp#L249) | [encode986Ecu0x4F8](../src/comms/986_ecu_messages.cpp#L259) | Implemented (raw bytes) |
| 0x7A0 | Unknown_7A0 | ECU | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x7A0](../src/comms/986_ecu_messages.cpp#L267) | [encode986Ecu0x7A0](../src/comms/986_ecu_messages.cpp#L277) | Implemented (raw bytes) |

### Vom Fahrzeug empfangen

| CAN ID | Message | DBC Sender | Target File | Decoder | Encoder | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 0x2A8 | Vehicle_Speed | ABS_PSM | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x2A8](../src/comms/986_vehicle_messages.cpp#L9) | [encode986Vehicle0x2A8](../src/comms/986_vehicle_messages.cpp#L34) | Implemented |
| 0x1A0 | ABS_1 | ABS_PSM | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x1A0](../src/comms/986_vehicle_messages.cpp#L62) | [encode986Vehicle0x1A0](../src/comms/986_vehicle_messages.cpp#L91) | Implemented |
| 0x4A1 | ABS_2 | ABS_PSM | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x4A1](../src/comms/986_vehicle_messages.cpp#L182) | [encode986Vehicle0x4A1](../src/comms/986_vehicle_messages.cpp#L201) | Implemented |
| 0x510 | InstrCluster_1 | INSTR | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x510](../src/comms/986_vehicle_messages.cpp#L225) | [encode986Vehicle0x510](../src/comms/986_vehicle_messages.cpp#L239) | Implemented |
| 0x520 | InstrCluster_2 | INSTR | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x520](../src/comms/986_vehicle_messages.cpp#L126) | [encode986Vehicle0x520](../src/comms/986_vehicle_messages.cpp#L138) | Implemented |
| 0x560 | Unknown_560 | INSTR | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x560](../src/comms/986_vehicle_messages.cpp#L335) | [encode986Vehicle0x560](../src/comms/986_vehicle_messages.cpp#L345) | Implemented (raw bytes) |
| 0x440 | AT_Line1 | TCU | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x440](../src/comms/986_vehicle_messages.cpp#L255) | [encode986Vehicle0x440](../src/comms/986_vehicle_messages.cpp#L263) | Implemented |
| 0x0C0 | SWA | STEER | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x00C0](../src/comms/986_vehicle_messages.cpp#L154) | [encode986Vehicle0x00C0](../src/comms/986_vehicle_messages.cpp#L166) | Implemented |

<a id="openinverter-0x500"></a>
### CAN ID 0x500 / 1280 - DC electrical

DLC: `8`

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| udc | 0 | 16 | Intel | U16 | 0.1 | 0 | V | DC bus voltage |
| idc | 16 | 16 | Intel | S16 | 0.1 | 0 | A | DC bus current |
| il1 | 32 | 16 | Intel | S16 | 0.1 | 0 | A | Phase/current channel 1 |
| il2 | 48 | 16 | Intel | S16 | 0.1 | 0 | A | Phase/current channel 2 |

<a id="openinverter-0x501"></a>
### CAN ID 0x501 / 1281 - Motor output

DLC: `8`

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| fstat | 0 | 16 | Intel | S16 | 0.1 | 0 | Hz | Electrical frequency/state frequency |
| speed | 16 | 16 | Intel | S16 | 1 | 0 | rpm | Motor speed |
| uac | 32 | 16 | Intel | U16 | 0.1 | 0 | V | AC output voltage |
| ilmax | 48 | 16 | Intel | U16 | 0.1 | 0 | A | Current limit / max phase current |

<a id="openinverter-0x502"></a>
### CAN ID 0x502 / 1282 - RMS and derived SINE values

DLC: `8`

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| il1rms | 0 | 16 | Intel | U16 | 0.1 | 0 | A | RMS current channel 1 |
| il2rms | 16 | 16 | Intel | U16 | 0.1 | 0 | A | RMS current channel 2 |
| boostcalc | 32 | 16 | Intel | U16 | 1 | 0 | dig | Boost calculation value |
| fweakcalc | 48 | 16 | Intel | U16 | 0.1 | 0 | Hz | Field weakening calculation value |

<a id="openinverter-0x503"></a>
### CAN ID 0x503 / 1283 - Driver demand

DLC: `8`

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| pot | 0 | 12 | Intel | U12 | 1 | 0 | dig | Pedal/potentiometer input 1, bits 0-11 |
| pot2 | 12 | 12 | Intel | U12 | 1 | 0 | dig | Pedal/potentiometer input 2, bits 12-23 |
| potnom | 24 | 8 | Intel | S8 | 1 | 0 | % | Nominal pedal value, bits 24-31 |
| amp | 32 | 16 | Intel | U16 | 1 | 0 | dig | Requested/derived current value, bits 32-47 |
| angle | 48 | 16 | Intel | U16 | 0.1 | 0 | deg | Rotor/electrical angle, bits 48-63 |

<a id="openinverter-0x504"></a>
### CAN ID 0x504 / 1284 - Direction, regen and thermal state

DLC: `8`

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit / enum | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| regenpreset | 0 | 8 | Intel | U8 | 1 | 0 | % | Regen preset |
| seldir | 8 | 8 | Intel | S8 | 1 | 0 | DIRS | Selected direction |
| rotordir | 16 | 8 | Intel | S8 | 1 | 0 | DIRS | Rotor direction |
| tmphs | 32 | 8 | Intel | U8 | 1 | 0 | degC | Heatsink temperature |
| tmpm | 40 | 8 | Intel | U8 | 1 | 0 | degC | Motor temperature |
| uaux | 48 | 16 | Intel | U16 | 0.1 | 0 | V | Auxiliary voltage |

`DIRS` enum:

| Raw | Label |
| ---: | --- |
| -1 | Reverse |
| 0 | Neutral |
| 1 | Forward |

<a id="openinverter-0x505"></a>
### CAN ID 0x505 / 1285 - Mode, fault and status

DLC: `7`

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit / enum | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| opmode | 0 | 8 | Intel | U8 | 1 | 0 | OPMODES | Operating mode enum |
| lasterr | 8 | 8 | Intel | U8 | 1 | 0 | ERRORS | Last error enum |
| pwmio | 16 | 8 | Intel | U8 | 1 | 0 | - | PWM I/O numeric value |
| canio | 24 | 8 | Intel | U8 | 1 | 0 | CANIOS | Bitfield, decode all active flags |
| status | 32 | 16 | Intel | U16 | 1 | 0 | STATUS | Bitfield, decode all active flags |
| cpuload | 48 | 8 | Intel | U8 | 1 | 0 | % | CPU load |

`OPMODES` enum:

| Raw | Label |
| ---: | --- |
| 0 | Off |
| 1 | Run |
| 2 | ManualRun |
| 3 | Boost |
| 4 | Buck |
| 5 | Sine |
| 6 | AcHeat |

`ERRORS` enum:

| Raw | Label |
| ---: | --- |
| 0 | OVERCURRENT |
| 1 | THROTTLE1 |
| 2 | THROTTLE2 |
| 3 | CANTIMEOUT |
| 4 | EMCYSTOP |
| 5 | MPROT |
| 6 | DESAT |
| 7 | OVERVOLTAGE |
| 8 | ENCODER |
| 9 | PRECHARGE |
| 10 | TMPHSMAX |
| 11 | CURRENTLIMIT |
| 12 | PWMSTUCK |
| 13 | HICUROFS1 |
| 14 | HICUROFS2 |
| 15 | HIRESOFS |
| 16 | LORESAMP |
| 17 | TMPMMAX |
| 18 | CANCRC |
| 19 | CANCOUNTER |

`CANIOS` bitfield:

| Bit value | Label |
| ---: | --- |
| 1 | Cruise |
| 2 | Start |
| 4 | Brake |
| 8 | Forward |
| 16 | Reverse |
| 32 | BMS |

`STATUS` bitfield:

| Bit value | Label |
| ---: | --- |
| 1 | UdcLow |
| 2 | UdcHigh |
| 4 | UdcBelowUdcSw |
| 8 | UdcLim |
| 16 | EmcyStop |
| 32 | MotorProtection |
| 64 | PotPressed |
| 128 | TmpHs |
| 256 | WaitStart |
| 512 | BrakeCheck |

Decode all active `CANIOS` and `STATUS` flags. If a bitfield raw value is zero, render `None`.

<a id="openinverter-0x506"></a>
### CAN ID 0x506 / 1286 - Digital inputs A

DLC: `2`

| Signal | Start | Len | Endian | Type | Factor | Offset | Enum | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| din_cruise | 0 | 2 | Intel | U2 | 1 | 0 | ONOFF | Cruise input |
| din_start | 2 | 2 | Intel | U2 | 1 | 0 | ONOFF | Start input |
| din_brake | 4 | 2 | Intel | U2 | 1 | 0 | ONOFF | Brake input |
| din_mprot | 6 | 2 | Intel | U2 | 1 | 0 | OKERR | Motor protection input |
| din_forward | 8 | 2 | Intel | U2 | 1 | 0 | ONOFF | Forward input |
| din_reverse | 10 | 2 | Intel | U2 | 1 | 0 | ONOFF | Reverse input |
| din_emcystop | 12 | 2 | Intel | U2 | 1 | 0 | OKERR | Emergency stop input |
| din_ocur | 14 | 2 | Intel | U2 | 1 | 0 | OKERR | Overcurrent input |

`ONOFF` enum:

| Raw | Label |
| ---: | --- |
| 0 | Off |
| 1 | On |
| 2 | N/A |

`OKERR` enum:

| Raw | Label |
| ---: | --- |
| 0 | Error |
| 1 | Ok |
| 2 | N/A |

<a id="openinverter-0x507"></a>
### CAN ID 0x507 / 1287 - Digital inputs B and cruise speed

DLC: `4`

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit / enum | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| din_desat | 0 | 2 | Intel | U2 | 1 | 0 | OKERR | Desaturation input |
| din_bms | 2 | 2 | Intel | U2 | 1 | 0 | ONOFF | BMS input |
| cruisespeed | 16 | 16 | Intel | S16 | 1 | 0 | rpm | Cruise speed |

Bits 4-15 are unused/reserved and must be ignored.

<a id="openinverter-0x508"></a>
### CAN ID 0x508 / 1288 - Turns and uptime

DLC: `8`

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Notes |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- |
| turns | 0 | 32 | Intel | S32 | 1 | 0 | turns | Motor turns |
| uptime | 32 | 32 | Intel | U32 | 0.01 | 0 | s | Original raw unit is 10 ms; factor converts to seconds |

<a id="can0-bms-zombie"></a>
## CAN0: TeensyBMS, Zombie-Slave, Tesla DC/DC

Bit layout, scaling and value tables: [ZombieSlave_teensybms_dcdc_mvcu.dbc](ZombieSlave_teensybms_dcdc_mvcu.dbc) (copy of `Modellfan/Zombie-Slave` 205cc7c, 2026-07-11). Decoder: `src/comms/battery_messages.cpp`. VCU-specific points only:

| ID | Name | Sender | Cycle | Counter/CRC | VCU |
| --- | --- | --- | --- | --- | --- |
| 0x41A | TeensyBMS_VoltageCurrent | TeensyBMS | 100 ms | yes | `battery_pack_voltage_v`, `battery_pack_current_a` (+ discharge), cell min/max |
| 0x41B | TeensyBMS_CellTempPower | TeensyBMS | 100 ms | yes | cell temperatures, delta, `battery_pack_power_kw` |
| 0x41C | TeensyBMS_LimitsFault | TeensyBMS | 100 ms | yes | current limits, contactor state, DTC flags |
| 0x41D | TeensyBMS_SocSoh | TeensyBMS | 1 s | yes | `battery_soc_pct`, SoH, BMS state (stale after 2.5 s) |
| 0x41E | TeensyBMS_Hmi | TeensyBMS | 1 s | yes | average power, remaining time and energy (stale after 2.5 s) |
| 0x437 | ZombieSlave_VcuStatus | Zombie-Slave | 100 ms | yes | `battery_vehicle_state`; `battery_charging_active` = state 7 (CHARGE) |
| 0x438 | mVCU_ChargePowerStatus | Zombie-Slave | 100 ms | no | charge power actual / max |
| 0x439 | mVCU_HeaterStatus | Zombie-Slave | 100 ms | yes | `heater_*` |
| 0x210 | TeslaDCDC_Status | Tesla DC/DC | ? | no | `dcdc_*` (whether the DC/DC is on this bus is open) |
| 0x43A | mVCU_HeaterControl | **VCU** | 100 ms | yes | byte 0 bit 0 = `heater_request`; the Zombie-Slave needs consecutive counters and drops the request after 500 ms |

Counter: byte 6, bits 0..3. CRC: byte 7 = low byte of the STM32 CRC-32 (poly 0x04C11DB7, init 0xFFFFFFFF, two little-endian 32-bit words of bytes 0..7 with byte 7 = 0, no reflection, no final XOR), as in TeensyVCU `src/utils/can_crc.h`. The VCU drops received frames with a wrong CRC (`battery_crc_errors`) and does not check the counter.
