# CAN Signal Reference

## Table of Contents

- [Porsche 986 ECU](#porsche-986-ecu)
  - [CAN ID 0x280 - Engine_1 (ECU)](#can-0x280)
  - [CAN ID 0x289 - Motor_2 (ECU)](#can-0x289)
  - [CAN ID 0x298 - Engine_2 (ECU)](#can-0x298)
  - [CAN ID 0x4E0 - EngineStat (ECU)](#can-0x4e0)
  - [CAN ID 0x4F8 - Unknown_4F8 (ECU)](#can-0x4f8)
  - [CAN ID 0x7A0 - Unknown_7A0 (ECU)](#can-0x7a0)
- [Porsche 986 Vehicle CAN](#porsche-986-vehicle-can)
  - [CAN ID 0x2A8 - Vehicle Speed (ABS / PSM)](#can-0x2a8)
  - [CAN ID 0x1A0 - ABS_1 (ABS / PSM)](#can-0x1a0)
  - [CAN ID 0x4A1 - ABS_2 (ABS / PSM)](#can-0x4a1)
  - [CAN ID 0x510 - InstrCluster_1 (INSTR)](#can-0x510)
  - [CAN ID 0x520 - InstrCluster_2 (INSTR)](#can-0x520)
  - [CAN ID 0x560 - Unknown_560 (INSTR)](#can-0x560)
  - [CAN ID 0x440 - AT_Line1 (TCU)](#can-0x440)
  - [CAN ID 0x00C0 - Steering Angle Sensor](#can-0x00c0)
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

## Porsche 986 ECU

| CAN ID | Message | DBC Sender | Description | Target File | Decoder | Encoder | Status |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0x280 | Engine_1 | ECU | [CAN ID 0x280](#can-0x280) | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x280](../src/comms/986_ecu_messages.cpp#L9) | [encode986Ecu0x280](../src/comms/986_ecu_messages.cpp#L22) | Implemented |
| 0x289 | Motor_2 | ECU | [CAN ID 0x289](#can-0x289) | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x289](../src/comms/986_ecu_messages.cpp) | [encode986Ecu0x289](../src/comms/986_ecu_messages.cpp) | Implemented |
| 0x298 | Engine_2 | ECU | [CAN ID 0x298](#can-0x298) | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x298](../src/comms/986_ecu_messages.cpp#L175) | [encode986Ecu0x298](../src/comms/986_ecu_messages.cpp#L189) | Implemented |
| 0x4E0 | EngineStat | ECU | [CAN ID 0x4E0](#can-0x4e0) | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x4E0](../src/comms/986_ecu_messages.cpp#L213) | [encode986Ecu0x4E0](../src/comms/986_ecu_messages.cpp#L228) | Implemented |
| 0x4F8 | Unknown_4F8 | ECU | [CAN ID 0x4F8](#can-0x4f8) | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x4F8](../src/comms/986_ecu_messages.cpp#L249) | [encode986Ecu0x4F8](../src/comms/986_ecu_messages.cpp#L259) | Implemented (raw bytes) |
| 0x7A0 | Unknown_7A0 | ECU | [CAN ID 0x7A0](#can-0x7a0) | `src/comms/986_ecu_messages.cpp` | [decode986Ecu0x7A0](../src/comms/986_ecu_messages.cpp#L267) | [encode986Ecu0x7A0](../src/comms/986_ecu_messages.cpp#L277) | Implemented (raw bytes) |

<a id="can-0x280"></a>
### CAN ID 0x280 - Engine_1 (ECU)
Time slice: `30 ms`.

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Engine Speed (RPM) | 16 | 16 | Intel | U16 | 0.25 | 0 | rpm | DBC, More infos, Teensy | All sources agree |
| Throttle Position | 40 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | - |
| Pedal Position 1 | 32 | 8 | Intel | U8 | 1 | - | % | DBC | Not used in Teensy |
| Pedal Position 2 | 56 | 8 | Intel | U8 | 1 | - | % | DBC | Not used in Teensy |

<a id="can-0x289"></a>
### CAN ID 0x289 - Motor_2 (ECU)
Time slice: `30 ms`.

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| MO2_Mp_Code (Multiplex selector) | 6 | 2 | Intel | U2 | 1 | 0 | - | VW PQ DBC | Byte0 multiplex selector |
| MO2_CAN_Vers (m0) | 0 | 6 | Intel | U6 | 1 | 0 | - | VW PQ DBC | Active when `MO2_Mp_Code=0` |
| MO2_Motor_Code (m1) | 0 | 6 | Intel | U6 | 1 | 0 | - | VW PQ DBC | Active when `MO2_Mp_Code=1` |
| MO2_Getr_Code (m2) | 0 | 6 | Intel | U6 | 1 | 0 | - | VW PQ DBC | Active when `MO2_Mp_Code=2` |
| MO2_max_Mo (m3) | 0 | 6 | Intel | U6 | 10 | 0 | Nm | VW PQ DBC | Active when `MO2_Mp_Code=3` |
| MO2_Kuehlm_T | 8 | 8 | Intel | U8 | 0.75 | -48 | degC | VW PQ DBC | Coolant temperature |
| MO2_BLS | 16 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | Brake light switch |
| MO2_BTS | 17 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | Brake test switch |
| MO2_Sta_Kuehlm | 18 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | Coolant temp status |
| MO2_Sta_Klima | 19 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | AC compressor status |
| MO2_Sta_No_Bet | 20 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | Normal operation status |
| MO2_Status_TSK | 21 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | Drivetrain coordinator status |
| MO2_Sta_GRA | 22 | 2 | Intel | U2 | 1 | 0 | - | VW PQ DBC | Cruise status |
| MO2_GRAregelt | 26 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | Cruise regulating |
| MO2_Sport_Error | 27 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | Sport error flag |
| MO2_OffRoad | 28 | 1 | Intel | Bool | 1 | 0 | - | VW PQ DBC | Offroad mode |
| MO2_RME_Gehalt | 29 | 3 | Intel | U3 | 12.5 | 0 | % | VW PQ DBC | Biodiesel content |
| MO2_GRA_Soll | 32 | 8 | Intel | U8 | 1.28 | 0 | km/h | VW PQ DBC | Cruise target speed |
| MO2_LL_Solldz | 40 | 8 | Intel | U8 | 10 | 0 | rpm | VW PQ DBC | Idle target speed |
| MO2_Begr_Mo | 48 | 8 | Intel | U8 | 0.39 | 0 | % | VW PQ DBC | Limited torque |
| MO2_Mo_ZWR | 56 | 8 | Intel | U8 | 0.39 | 0 | % | VW PQ DBC | Min torque with ignition retard |

<a id="can-0x298"></a>
### CAN ID 0x298 - Engine_2 (ECU)
Time slice: `30 ms`.

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Inlet Air Temperature | 8 | 8 | Intel | U8 | 0.75 | -48 | degC | DBC, More infos | - |
| Throttle Position 1 | 16 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | Turbo? Unsure |
| Throttle Position 2 | 32 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | Turbo? Unsure |
| Barometric Pressure | 40 | 8 | Intel | U8 | 5 | 0 | mbar | More infos | Turbo? Unsure |
| Vehicle Speed | 56 | 8 | Intel | U8 | ? | 0 | ? | More infos | Scaling unknown |

<a id="can-0x4e0"></a>
### CAN ID 0x4E0 - EngineStat (ECU)
Time slice: `30 ms`.

| Signal | Start | Len | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Check Engine Light | 0 | 1 | Bool | 1 | 0 | - | Legacy/byte0 | Byte-0 interpretation retained |
| Check Engine Light Bit1 | 1 | 1 | Bool | 1 | 0 | - | Legacy/byte0 | Byte-0 interpretation retained |
| EngineStat Bool2 | 2 | 1 | Bool | 1 | 0 | - | Legacy/byte0 | Byte-0 interpretation retained |
| Reduced Power | 3 | 1 | Bool | 1 | 0 | - | Legacy/byte0 | Byte-0 interpretation retained |
| Fan Error | 4 | 1 | Bool | 1 | 0 | - | Legacy/byte0 | Byte-0 interpretation retained |
| EngineStat Bool5 | 5 | 1 | Bool | 1 | 0 | - | Legacy/byte0 | Byte-0 interpretation retained |
| EngineStat Bool6 | 6 | 1 | Bool | 1 | 0 | - | Legacy/byte0 | Byte-0 interpretation retained |
| EngineStat Bool7 | 7 | 1 | Bool | 1 | 0 | - | Legacy/byte0 | Byte-0 interpretation retained |
| MO5_Vorgluehen | 9 | 1 | Bool | 1 | 0 | - | User definition | Diesel pre-glow lamp |
| MO5_E_Gas | 10 | 1 | Bool | 1 | 0 | - | User definition | E-Gas system lamp |
| MO5_OBD_2 | 11 | 1 | Bool | 1 | 0 | - | User definition | OBD/MIL lamp control |
| MO5_Heissl | 12 | 1 | Bool | 1 | 0 | - | User definition | Coolant overheat warning lamp |
| MO5_KlimaKompr | 13 | 1 | Bool | 1 | 0 | - | User definition | AC compressor off request |
| MO5_Feld_kuehl | 14 | 1 | Bool | 1 | 0 | - | User definition | Map cooling installed and OK |
| MO5_KliKo_Red | 15 | 1 | Bool | 1 | 0 | - | User definition | AC compressor power reduction |
| MO5_Verbrauch | 16 | 15 | U15 | 1 | 0 | uL | User definition | Fuel consumption counter (15-bit) |
| MO5_UeberlVerb | 31 | 1 | Bool | 1 | 0 | - | User definition | Consumption counter overflow |

<a id="can-0x4f8"></a>
### CAN ID 0x4F8 - Unknown_4F8 (ECU)

Message is implemented as raw-byte passthrough in ECU codec.

<a id="can-0x7a0"></a>
### CAN ID 0x7A0 - Unknown_7A0 (ECU)

Message is implemented as raw-byte passthrough in ECU codec.
Time slice: `30 ms`.
Standard payload: `85 0C 00 03 00 00 FF 02`.

ECU coverage summary:
- ECU sender messages in ECU codec: 6/6

## Porsche 986 Vehicle CAN

| CAN ID | Message | DBC Sender | Description | Target File | Decoder | Encoder | Status |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0x2A8 | Vehicle_Speed | ABS_PSM | [CAN ID 0x2A8](#can-0x2a8) | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x2A8](../src/comms/986_vehicle_messages.cpp#L9) | [encode986Vehicle0x2A8](../src/comms/986_vehicle_messages.cpp#L34) | Implemented |
| 0x1A0 | ABS_1 | ABS_PSM | [CAN ID 0x1A0](#can-0x1a0) | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x1A0](../src/comms/986_vehicle_messages.cpp#L62) | [encode986Vehicle0x1A0](../src/comms/986_vehicle_messages.cpp#L91) | Implemented |
| 0x4A1 | ABS_2 | ABS_PSM | [CAN ID 0x4A1](#can-0x4a1) | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x4A1](../src/comms/986_vehicle_messages.cpp#L182) | [encode986Vehicle0x4A1](../src/comms/986_vehicle_messages.cpp#L201) | Implemented |
| 0x510 | InstrCluster_1 | INSTR | [CAN ID 0x510](#can-0x510) | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x510](../src/comms/986_vehicle_messages.cpp#L225) | [encode986Vehicle0x510](../src/comms/986_vehicle_messages.cpp#L239) | Implemented |
| 0x520 | InstrCluster_2 | INSTR | [CAN ID 0x520](#can-0x520) | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x520](../src/comms/986_vehicle_messages.cpp#L126) | [encode986Vehicle0x520](../src/comms/986_vehicle_messages.cpp#L138) | Implemented |
| 0x560 | Unknown_560 | INSTR | [CAN ID 0x560](#can-0x560) | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x560](../src/comms/986_vehicle_messages.cpp#L335) | [encode986Vehicle0x560](../src/comms/986_vehicle_messages.cpp#L345) | Implemented (raw bytes) |
| 0x440 | AT_Line1 | TCU | [CAN ID 0x440](#can-0x440) | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x440](../src/comms/986_vehicle_messages.cpp#L255) | [encode986Vehicle0x440](../src/comms/986_vehicle_messages.cpp#L263) | Implemented |
| 0x0C0 | SWA | STEER | [CAN ID 0x00C0](#can-0x00c0) | `src/comms/986_vehicle_messages.cpp` | [decode986Vehicle0x00C0](../src/comms/986_vehicle_messages.cpp#L154) | [encode986Vehicle0x00C0](../src/comms/986_vehicle_messages.cpp#L166) | Implemented |

<a id="can-0x2a8"></a>
### CAN ID 0x2A8 - Vehicle Speed (ABS / PSM)

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Frei_Bremse_3_1 | 0 | 1 | Intel | Bool | 1 | 0 | - | DBC (986) | Free/status bit |
| Wheel Speed FL | 1 | 15 | Intel | U15 | 0.01 | 0 | km/h | More infos | Legacy definition |
| Frei_Bremse_3_2 | 16 | 1 | Intel | Bool | 1 | 0 | - | DBC (986) | Free/status bit |
| Wheel Speed FR | 17 | 15 | Intel | U15 | 0.01 | 0 | km/h | More infos | Legacy definition |
| Frei_Bremse_3_3 | 32 | 1 | Intel | Bool | 1 | 0 | - | DBC (986) | Free/status bit |
| Wheel Speed RL | 33 | 15 | Intel | U15 | 0.01 | 0 | km/h | More infos | Legacy definition |
| Frei_Bremse_3_4 | 48 | 1 | Intel | Bool | 1 | 0 | - | DBC (986) | Free/status bit |
| Wheel Speed RR | 49 | 15 | Intel | U15 | 0.01 | 0 | km/h | More infos | Legacy definition |

<a id="can-0x1a0"></a>
### CAN ID 0x1A0 - ABS_1 (ABS / PSM)

| Signal | Start | Len | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| BR1_ASR_Anf | 0 | 1 | Bool | 1 | 0 | - | User definition | ASR request |
| BR1_MSR_Anf | 1 | 1 | Bool | 1 | 0 | - | User definition | MSR request |
| BR1_ABS_Brems | 2 | 1 | Bool | 1 | 0 | - | User definition | ABS control active |
| BR1_EDS_Ongr | 3 | 1 | Bool | 1 | 0 | - | User definition | EDS intervention |
| BR1_ESP_Ongr | 4 | 1 | Bool | 1 | 0 | - | User definition | ESP intervention |
| BR1_ASR_Ongr | 5 | 2 | U2 | 1 | 0 | - | User definition | ASR switching influence |
| BR1_EBV_Ongr | 7 | 1 | Bool | 1 | 0 | - | User definition | EBV intervention |
| BR1_Lampe_ABS | 8 | 1 | Bool | 1 | 0 | - | User definition | ABS warning lamp |
| BR1_Lampe_ASR | 9 | 1 | Bool | 1 | 0 | - | User definition | ASR/ESP lamp |
| BR1_Lampe_BK | 10 | 1 | Bool | 1 | 0 | - | User definition | Brake warning lamp |
| BR1_Lichtschalt | 11 | 1 | Bool | 1 | 0 | - | User definition | Brake switch |
| BR1_StaDruckschw | 12 | 1 | Bool | 1 | 0 | - | User definition | Brake test switch status |
| BR1_MAD | 13 | 1 | Bool | 1 | 0 | - | User definition | Rough road suppression |
| BR1_Sta_MAD | 14 | 1 | Bool | 1 | 0 | - | User definition | Rough road suppression status |
| BR1_Diagnose | 15 | 1 | Bool | 1 | 0 | - | User definition | ABS diagnosis mode |
| BR1_BKV_active | 16 | 1 | Bool | 1 | 0 | - | User definition | Active brake booster status |
| BR1_Rad_kmh | 17 | 15 | U15 | 0.01 | 0 | km/h | User definition | Vehicle speed |
| BR1_ASRMo_sl | 32 | 8 | U8 | 0.39 | 0 | % | User definition | ASR torque slow |
| BR1_ASRMo_fa | 40 | 8 | U8 | 0.39 | 0 | % | User definition | ASR torque fast (`MSR_Anf=0`) |
| BR1_MSR_Mo_inv | 40 | 8 | U8 | -0.39 | 99.45 | % | User definition | Inverse MSR torque (`MSR_Anf=1`) |
| BR1_MSR_Mo | 48 | 8 | U8 | 0.39 | 0 | % | User definition | MSR torque |
| BR1_Zaehler | 56 | 4 | U4 | 1 | 0 | - | User definition | Message counter |
| BR1_ASR_ESP | 60 | 1 | Bool | 1 | 0 | - | User definition | Control-unit type |
| BR1_ESPASR_passive | 61 | 1 | Bool | 1 | 0 | - | User definition | ESP/ASR passive |
| BR1_Sta_ESP | 62 | 1 | Bool | 1 | 0 | - | User definition | ESP error status |
| BR1_Ersatz_Kmh | 63 | 1 | Bool | 1 | 0 | - | User definition | Substitute speed flag |

<a id="can-0x4a1"></a>
### CAN ID 0x4A1 - ABS_2 (ABS / PSM)

Message is implemented in vehicle codec; see linked decoder/encoder in the table above.

<a id="can-0x510"></a>
### CAN ID 0x510 - InstrCluster_1 (INSTR)

Message is implemented in vehicle codec; see linked decoder/encoder in the table above.

<a id="can-0x520"></a>
### CAN ID 0x520 - Instrument Cluster 2

| Signal | Start | Len | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Ambient Temperature | 24 | 8 | U8 | 0.5 | -40 | degC | DBC, More infos | - |
| Oil Pressure | 32 | 8 | U8 | 0.04 | - | bar | DBC, More infos | - |
| Light Dimmer | 8 | 8 | U8 | 1 | 0 | - | DBC | - |
| Counter / Unknown | 40 | 8 | U8 | 1 | 0 | - | More infos | Unknown purpose |

<a id="can-0x560"></a>
### CAN ID 0x560 - Unknown_560 (INSTR)

Message is implemented as raw-byte passthrough in vehicle codec.

<a id="can-0x440"></a>
### CAN ID 0x440 - AT_Line1 (TCU)

Message is implemented in vehicle codec; see linked decoder/encoder in the table above.

<a id="can-0x00c0"></a>
### CAN ID 0x00C0 - Steering Angle Sensor

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| COARSE_CHECK__RB_ | 0 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Check bit |
| FINE_CHECK__RB_ | 1 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Check bit |
| Frei_Lenkwinkel_1_1__RB_ | 2 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Free bit |
| LWS_Abgleich__RB_ | 3 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Calibration status |
| LWS_OK__RB_ | 4 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Sensor valid flag |
| Lenkwinkel__RB_ | 5 | 10 | Intel | U10 | 2.5 | -720 | deg | DBC (RB) | Steering angle raw mapping |
| Vorzeichen__RB_ | 15 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Sign bit |

Vehicle CAN coverage summary:
- Non-ECU sender messages in vehicle codec: 8/8

Porsche 986 total coverage summary:
- Total 986 messages tracked in codec coverage: 14

## OpenInverter stm32-sine

All OpenInverter `stm32-sine` telemetry messages use:

- Standard 11-bit CAN identifiers
- Intel/little-endian signal byte order
- DBC byte order notation `@1`
- Physical value calculation: `physicalValue = rawValue * factor + offset`
- Signed values decoded as two's-complement with the configured bit length

Signed extraction must not treat negative values as unsigned. Messages shorter than their configured DLC are invalid/incomplete.

| CAN ID | Decimal | Message | CAN Device | Description | DLC | Signal Count | Decoder | WebUI Rendering | Status |
| --- | ---: | --- | --- | --- | ---: | ---: | --- | --- | --- |
| 0x500 | 1280 | DC electrical | OpenInverter `stm32-sine` | [CAN ID 0x500 / 1280](#openinverter-0x500) | 8 | 4 | Required | Required | Documented |
| 0x501 | 1281 | Motor output | OpenInverter `stm32-sine` | [CAN ID 0x501 / 1281](#openinverter-0x501) | 8 | 4 | Required | Required | Documented |
| 0x502 | 1282 | RMS and derived SINE values | OpenInverter `stm32-sine` | [CAN ID 0x502 / 1282](#openinverter-0x502) | 8 | 4 | Required | Required | Documented |
| 0x503 | 1283 | Driver demand | OpenInverter `stm32-sine` | [CAN ID 0x503 / 1283](#openinverter-0x503) | 8 | 5 | Required | Required | Documented |
| 0x504 | 1284 | Direction, regen and thermal state | OpenInverter `stm32-sine` | [CAN ID 0x504 / 1284](#openinverter-0x504) | 8 | 6 | Required | Required | Documented |
| 0x505 | 1285 | Mode, fault and status | OpenInverter `stm32-sine` | [CAN ID 0x505 / 1285](#openinverter-0x505) | 7 | 6 | Required | Required | Documented |
| 0x506 | 1286 | Digital inputs A | OpenInverter `stm32-sine` | [CAN ID 0x506 / 1286](#openinverter-0x506) | 2 | 8 | Required | Required | Documented |
| 0x507 | 1287 | Digital inputs B and cruise speed | OpenInverter `stm32-sine` | [CAN ID 0x507 / 1287](#openinverter-0x507) | 4 | 3 | Required | Required | Documented |
| 0x508 | 1288 | Turns and uptime | OpenInverter `stm32-sine` | [CAN ID 0x508 / 1288](#openinverter-0x508) | 8 | 2 | Required | Required | Documented |

OpenInverter coverage summary:
- Total OpenInverter `stm32-sine` messages documented: 9
- Total OpenInverter `stm32-sine` signals documented: 42

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
