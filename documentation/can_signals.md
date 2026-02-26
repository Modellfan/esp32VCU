# CAN Signal Reference (User Provided)

## Porsche 986 Coverage (Linked)

| CAN ID | Message | DBC Sender | Description | Target File | Decoder | Encoder | Status |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0x280 | Engine_1 | ECU | [CAN ID 0x280](#can-0x280) | `src/986_ecu_messages.cpp` | [decode986Ecu0x280](../src/986_ecu_messages.cpp#L9) | [encode986Ecu0x280](../src/986_ecu_messages.cpp#L22) | Implemented |
| 0x289 | Motor_2 | ECU | [CAN ID 0x289](#can-0x289) | `src/986_ecu_messages.cpp` | [decode986Ecu0x289](../src/986_ecu_messages.cpp) | [encode986Ecu0x289](../src/986_ecu_messages.cpp) | Implemented |
| 0x298 | Engine_2 | ECU | [CAN ID 0x298](#can-0x298) | `src/986_ecu_messages.cpp` | [decode986Ecu0x298](../src/986_ecu_messages.cpp#L175) | [encode986Ecu0x298](../src/986_ecu_messages.cpp#L189) | Implemented |
| 0x4E0 | EngineStat | ECU | [CAN ID 0x4E0](#can-0x4e0) | `src/986_ecu_messages.cpp` | [decode986Ecu0x4E0](../src/986_ecu_messages.cpp#L213) | [encode986Ecu0x4E0](../src/986_ecu_messages.cpp#L228) | Implemented |
| 0x4F8 | Unknown_4F8 | ECU | [CAN ID 0x4F8](#can-0x4f8) | `src/986_ecu_messages.cpp` | [decode986Ecu0x4F8](../src/986_ecu_messages.cpp#L249) | [encode986Ecu0x4F8](../src/986_ecu_messages.cpp#L259) | Implemented (raw bytes) |
| 0x7A0 | Unknown_7A0 | ECU | [CAN ID 0x7A0](#can-0x7a0) | `src/986_ecu_messages.cpp` | [decode986Ecu0x7A0](../src/986_ecu_messages.cpp#L267) | [encode986Ecu0x7A0](../src/986_ecu_messages.cpp#L277) | Implemented (raw bytes) |
| 0x2A8 | Vehicle_Speed | ABS_PSM | [CAN ID 0x2A8](#can-0x2a8) | `src/986_vehicle_messages.cpp` | [decode986Vehicle0x2A8](../src/986_vehicle_messages.cpp#L9) | [encode986Vehicle0x2A8](../src/986_vehicle_messages.cpp#L34) | Implemented |
| 0x1A0 | ABS_1 | ABS_PSM | [CAN ID 0x1A0](#can-0x1a0) | `src/986_vehicle_messages.cpp` | [decode986Vehicle0x1A0](../src/986_vehicle_messages.cpp#L62) | [encode986Vehicle0x1A0](../src/986_vehicle_messages.cpp#L91) | Implemented |
| 0x4A1 | ABS_2 | ABS_PSM | [CAN ID 0x4A1](#can-0x4a1) | `src/986_vehicle_messages.cpp` | [decode986Vehicle0x4A1](../src/986_vehicle_messages.cpp#L182) | [encode986Vehicle0x4A1](../src/986_vehicle_messages.cpp#L201) | Implemented |
| 0x510 | InstrCluster_1 | INSTR | [CAN ID 0x510](#can-0x510) | `src/986_vehicle_messages.cpp` | [decode986Vehicle0x510](../src/986_vehicle_messages.cpp#L225) | [encode986Vehicle0x510](../src/986_vehicle_messages.cpp#L239) | Implemented |
| 0x520 | InstrCluster_2 | INSTR | [CAN ID 0x520](#can-0x520) | `src/986_vehicle_messages.cpp` | [decode986Vehicle0x520](../src/986_vehicle_messages.cpp#L126) | [encode986Vehicle0x520](../src/986_vehicle_messages.cpp#L138) | Implemented |
| 0x560 | Unknown_560 | INSTR | [CAN ID 0x560](#can-0x560) | `src/986_vehicle_messages.cpp` | [decode986Vehicle0x560](../src/986_vehicle_messages.cpp#L335) | [encode986Vehicle0x560](../src/986_vehicle_messages.cpp#L345) | Implemented (raw bytes) |
| 0x440 | AT_Line1 | TCU | [CAN ID 0x440](#can-0x440) | `src/986_vehicle_messages.cpp` | [decode986Vehicle0x440](../src/986_vehicle_messages.cpp#L255) | [encode986Vehicle0x440](../src/986_vehicle_messages.cpp#L263) | Implemented |
| 0x0C0 | SWA | STEER | [CAN ID 0x00C0](#can-0x00c0) | `src/986_vehicle_messages.cpp` | [decode986Vehicle0x00C0](../src/986_vehicle_messages.cpp#L154) | [encode986Vehicle0x00C0](../src/986_vehicle_messages.cpp#L166) | Implemented |

<a id="can-0x280"></a>
## CAN ID 0x280  Engine_1 (ECU)
Time slice: `30 ms`.

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Engine Speed (RPM) | 16 | 16 | Intel | U16 | 0.25 | 0 | rpm | DBC, More infos, Teensy | All sources agree |
| Throttle Position | 40 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | - |
| Pedal Position 1 | 32 | 8 | Intel | U8 | 1 | - | % | DBC | Not used in Teensy |
| Pedal Position 2 | 56 | 8 | Intel | U8 | 1 | - | % | DBC | Not used in Teensy |

<a id="can-0x289"></a>
## CAN ID 0x289  Motor_2 (ECU)
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
## CAN ID 0x298  Engine_2 (ECU)
Time slice: `30 ms`.

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Inlet Air Temperature | 8 | 8 | Intel | U8 | 0.75 | -48 | degC | DBC, More infos | - |
| Throttle Position 1 | 16 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | Turbo? Unsure |
| Throttle Position 2 | 32 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | Turbo? Unsure |
| Barometric Pressure | 40 | 8 | Intel | U8 | 5 | 0 | mbar | More infos | Turbo? Unsure |
| Vehicle Speed | 56 | 8 | Intel | U8 | ? | 0 | ? | More infos | Scaling unknown |

<a id="can-0x2a8"></a>
## CAN ID 0x2A8  Vehicle Speed (ABS / PSM)

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
## CAN ID 0x1A0  ABS_1 (ABS / PSM)

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

<a id="can-0x4e0"></a>
## CAN ID 0x4E0  EngineStat (ECU)
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

<a id="can-0x520"></a>
## CAN ID 0x520  Instrument Cluster 2

| Signal | Start | Len | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Ambient Temperature | 24 | 8 | U8 | 0.5 | -40 | degC | DBC, More infos | - |
| Oil Pressure | 32 | 8 | U8 | 0.04 | - | bar | DBC, More infos | - |
| Light Dimmer | 8 | 8 | U8 | 1 | 0 | - | DBC | - |
| Counter / Unknown | 40 | 8 | U8 | 1 | 0 | - | More infos | Unknown purpose |

<a id="can-0x00c0"></a>
## CAN ID 0x00C0  Steering Angle Sensor

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| COARSE_CHECK__RB_ | 0 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Check bit |
| FINE_CHECK__RB_ | 1 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Check bit |
| Frei_Lenkwinkel_1_1__RB_ | 2 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Free bit |
| LWS_Abgleich__RB_ | 3 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Calibration status |
| LWS_OK__RB_ | 4 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Sensor valid flag |
| Lenkwinkel__RB_ | 5 | 10 | Intel | U10 | 2.5 | -720 | deg | DBC (RB) | Steering angle raw mapping |
| Vorzeichen__RB_ | 15 | 1 | Intel | Bool | 1 | 0 | - | DBC (RB) | Sign bit |

<a id="can-0x4a1"></a>
## CAN ID 0x4A1 - ABS_2 (ABS / PSM)

Message is implemented in vehicle codec; see linked decoder/encoder in the table above.

<a id="can-0x510"></a>
## CAN ID 0x510 - InstrCluster_1 (INSTR)

Message is implemented in vehicle codec; see linked decoder/encoder in the table above.

<a id="can-0x560"></a>
## CAN ID 0x560 - Unknown_560 (INSTR)

Message is implemented as raw-byte passthrough in vehicle codec.

<a id="can-0x440"></a>
## CAN ID 0x440 - AT_Line1 (TCU)

Message is implemented in vehicle codec; see linked decoder/encoder in the table above.

<a id="can-0x4f8"></a>
## CAN ID 0x4F8 - Unknown_4F8 (ECU)

Message is implemented as raw-byte passthrough in ECU codec.

<a id="can-0x7a0"></a>
## CAN ID 0x7A0 - Unknown_7A0 (ECU)

Message is implemented as raw-byte passthrough in ECU codec.
Time slice: `30 ms`.
Standard payload: `85 0C 00 03 00 00 FF 02`.

Coverage summary:
- Total 986 messages tracked in codec coverage: 14
- ECU sender messages in ECU codec: 6/6
- Non-ECU sender messages in vehicle codec: 8/8
