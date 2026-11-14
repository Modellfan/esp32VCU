# Porsche 986 CAN (CAN1)

> **Gespiegelt aus der DBC, nicht von Hand ändern.** Masterquelle: [porsche_986_996_compiled_working.dbc](porsche_986_996_compiled_working.dbc). Änderungen zuerst in die DBC eintragen, dann `python tools/generate_can_md.py` ausführen. Erkenntnisse aus dem Reverse Engineering: Signale, `CM_`-Kommentare, `OPEN (986 #n)`, `GenMsgCycleTime`. Zuordnung zu VCU-Feldern und UI: [signalliste.md](signalliste.md).

Zykluszeiten aus dem Standmitschnitt `can_logs/986_log_stand.csv` (Originalfahrzeug).

14 Botschaften, 114 Signale.

## Übersicht

| ID | Botschaft | Sender | Rolle | Zyklus | Signale | Offen |
| --- | --- | --- | --- | --- | ---: | --- |
| [0x280](#msg-0x280) | [Engine_1](#msg-0x280) | ECU | von der VCU gesendet (statt DME) | 10 ms | 4 | #6 |
| [0x289](#msg-0x289) | [Motor_2](#msg-0x289) | ECU | von der VCU gesendet (statt DME) | 10 ms | 23 | #12 |
| [0x298](#msg-0x298) | [Engine_2](#msg-0x298) | ECU | von der VCU gesendet (statt DME) | 10 ms | 5 | - |
| [0x4E0](#msg-0x4e0) | [EngineStat](#msg-0x4e0) | ECU | von der VCU gesendet (statt DME) | 10 ms | 17 | - |
| [0x4F8](#msg-0x4f8) | [Unknown_4F8](#msg-0x4f8) | ECU | von der VCU gesendet (statt DME) | - | 0 | #8 |
| [0x7A0](#msg-0x7a0) | [Unknown_7A0](#msg-0x7a0) | ECU | von der VCU gesendet (statt DME) | 10 ms | 0 | - |
| [0x2A8](#msg-0x2a8) | [Vehicle_Speed](#msg-0x2a8) | ABS_PSM | vom Fahrzeug empfangen | 20 ms | 8 | - |
| [0x1A0](#msg-0x1a0) | [ABS_1](#msg-0x1a0) | ABS_PSM | vom Fahrzeug empfangen | 20 ms | 26 | #1, #12 |
| [0x4A1](#msg-0x4a1) | [ABS_2](#msg-0x4a1) | ABS_PSM | vom Fahrzeug empfangen | 100 ms | 11 | #2, #3 |
| [0x510](#msg-0x510) | [InstrCluster_1](#msg-0x510) | INSTR | vom Fahrzeug empfangen | 20 ms | 7 | #4 |
| [0x520](#msg-0x520) | [InstrCluster_2](#msg-0x520) | INSTR | vom Fahrzeug empfangen | 20 ms | 5 | #5 |
| [0x440](#msg-0x440) | [AT_Line1](#msg-0x440) | TCU | vom Fahrzeug empfangen | - | 1 | #9 |
| [0x0C0](#msg-0x0c0) | [SWA](#msg-0x0c0) | STEER | vom Fahrzeug empfangen | 10 ms | 7 | - |
| [0x560](#msg-0x560) | [Unknown_560](#msg-0x560) | INSTR | vom Fahrzeug empfangen | - | 0 | #8 |

## Offene Punkte

Arbeitsliste fürs Reverse Engineering (aus den `OPEN`-Kommentaren der DBC).

- **#1** ([0x1A0](#msg-0x1a0)): byte 6 (bits 48-55) counts up by 1 per frame, so BR1_MSR_Mo (and the VCU value itq_pct) is a counter, not a torque; BR1_Zaehler (56|4) low nibble constant 0xD, bits 57-58 toggle.
- **#2** ([0x4A1](#msg-0x4a1)): bits 13-15 count 0..7 per frame, so ReceiptForACCMessage/DriverBraking/RegulatorError_FDR are a 3-bit counter; find the real driver-braking bit with a braking log.
- **#3** ([0x4A1](#msg-0x4a1)): bits 45-47 and 61-63 change every frame (checksum or multiplexed content?), bit 29 always 1.
- **#4** ([0x510](#msg-0x510)): bits 40 and 43 always 1 (byte 5 = 0x09), meaning unknown.
- **#5** ([0x520](#msg-0x520)): byte 2 bits 16-20 change slowly (0x4E..0x50; temperature or fuel level?); bits 0 and 22 always 1; Counter_Unknown (byte 5) rotates 0x12, 0x03, 0x30, 0x1D every 100 ms - check whether it carries the odometer multiplexed (VW PQ Kombi_3: Kilometerstand 40|20, Standzeit 24|15).
- **#6** ([0x280](#msg-0x280)): bit 4 toggles, byte 1 = 0x79 and byte 6 = 0x54 constant, not in the DBC; VW PQ Motor_1 reads bit 4 Time_Out_Bremsen_Botschaft, byte 1 inner torque, byte 4 inner torque without intervention, byte 6 mechanical loss torque, byte 7 driver torque request. Clarify bytes 1, 4, 6, 7 with the original DME at different pedal positions.
- **#8** ([0x4F8](#msg-0x4f8)): not in the stationary log; check whether it exists on this car.
- **#8** ([0x560](#msg-0x560)): not in the stationary log; check whether it exists on this car.
- **#9** ([0x440](#msg-0x440)): not in the stationary log; check whether the car has a Tiptronic control unit.
- **#10** (allgemein): the original DME sends 0x280/0x289/0x298/0x4E0/0x7A0 every 10 ms, the VCU every 30 ms (5 slots x 6 ms); cluster works on the bench, timeouts in other control units unknown - decision and CAN capture needed.
- **#11** (allgemein): stationary log only (can_logs/986_log_stand.csv); record a driving log with braking, steering, indicators, cruise buttons and lights.
- **#12** ([0x289](#msg-0x289)): bits 0-5 carry four signals selected by MO2_Mp_Code (6|2) without a valid multiplexer declaration; strict DBC tools reject the file.
- **#12** ([0x1A0](#msg-0x1a0)): BR1_ASRMo_fa and BR1_MSR_Mo_inv share byte 5.

## 1. Von der VCU gesendet (statt DME)

<a id="msg-0x280"></a>
### 0x280 Engine_1

Sender: ECU · DLC 8 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x280. Sent by the VCU instead of the DME (original DME 10 ms, VCU 30 ms). RPM scaling matches Teensy (raw = rpm*4).

- **Offen (#6):** bit 4 toggles, byte 1 = 0x79 and byte 6 = 0x54 constant, not in the DBC; VW PQ Motor_1 reads bit 4 Time_Out_Bremsen_Botschaft, byte 1 inner torque, byte 4 inner torque without intervention, byte 6 mechanical loss torque, byte 7 driver torque request. Clarify bytes 1, 4, 6, 7 with the original DME at different pedal positions.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `RPM` | 16 | 16 | Intel | U16 | 0.25 | 0 | 0 … 16383.8 | rpm | INSTR | Engine speed shown by the tachometer; the VCU sends the motor power here (VCU-REQ-005). | - |
| `PedalPosition1` | 32 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | % | UNKNOWN | Not used in Teensy. | - |
| `ThrottlePosition` | 40 | 8 | Intel | U8 | 0.390625 | 0 | 0 … 100 | % | INSTR | - | - |
| `PedalPosition2` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | % | UNKNOWN | Not used in Teensy. | - |

<a id="msg-0x289"></a>
### 0x289 Motor_2

Sender: ECU · DLC 8 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x289. Sent by the VCU instead of the DME. Layout from VW PQ Motor_2. Bit 24 always 1, bit 25 = speed limiter selected (VCU code).

- **Offen (#12):** bits 0-5 carry four signals selected by MO2_Mp_Code (6|2) without a valid multiplexer declaration; strict DBC tools reject the file.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `MO2_Getr_Code` (m2) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | INSTR | Active when MO2_Mp_Code=2. | - |
| `MO2_max_Mo` (m3) | 0 | 6 | Intel | U6 | 10 | 0 | 0 … 630 | Nm | INSTR | Active when MO2_Mp_Code=3. | - |
| `MO2_CAN_Vers` (m0) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | INSTR | Active when MO2_Mp_Code=0. | - |
| `MO2_Motor_Code` (m1) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | INSTR | Active when MO2_Mp_Code=1. | - |
| `MO2_Mp_Code` | 6 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | INSTR | Byte0 multiplex selector. | - |
| `MO2_Kuehlm_T` | 8 | 8 | Intel | U8 | 0.75 | -48 | -47.25 … 142.5 | degC | INSTR | Coolant temperature gauge; the VCU sends the inverter/motor temperature. | - |
| `MO2_BLS` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Brake light switch. | - |
| `MO2_BTS` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Brake test switch. | - |
| `MO2_Sta_Kuehlm` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Coolant temp status. | - |
| `MO2_Sta_Klima` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | AC compressor status. | - |
| `MO2_Sta_No_Bet` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Normal operation status. | - |
| `MO2_Status_TSK` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Drivetrain coordinator status. | - |
| `MO2_Sta_GRA` | 22 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | INSTR | Cruise control status. | - |
| `MO2_Bit24_Const` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | UNKNOWN | Always 1 in the stationary log; meaning unknown. | - |
| `MO2_SpeedLimiter_Selected` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Speed limiter selected (VCU field motor2_speed_limiter_selected); not in the original DBC. | - |
| `MO2_GRAregelt` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Cruise regulating. | - |
| `MO2_Sport_Error` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Sport error flag. | - |
| `MO2_OffRoad` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Offroad mode. | - |
| `MO2_RME_Gehalt` | 29 | 3 | Intel | U3 | 12.5 | 0 | 0 … 75 | % | INSTR | Biodiesel content. | - |
| `MO2_GRA_Soll` | 32 | 8 | Intel | U8 | 1.28 | 0 | 0 … 325.12 | km/h | INSTR | Cruise target speed. | - |
| `MO2_LL_Solldz` | 40 | 8 | Intel | U8 | 10 | 0 | 0 … 2540 | 1/min | INSTR | Idle target speed. | - |
| `MO2_Begr_Mo` | 48 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | INSTR | Limited torque. | - |
| `MO2_Mo_ZWR` | 56 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | INSTR | Min torque with ignition retard. | - |

<a id="msg-0x298"></a>
### 0x298 Engine_2

Sender: ECU · DLC 8 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x298. Sent by the VCU instead of the DME. Several signals marked 'Turbo? Unsure' in the notes; Speed_raw scaling unknown.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `InletAirTemp` | 8 | 8 | Intel | U8 | 0.75 | -48 | -48 … 142.5 | degC | INSTR | - | - |
| `ThrottlePosition1` | 16 | 8 | Intel | U8 | 0.390625 | 0 | 0 … 100 | % | UNKNOWN | Turbo? Unsure. | - |
| `ThrottlePosition2` | 32 | 8 | Intel | U8 | 0.390625 | 0 | 0 … 100 | % | UNKNOWN | Turbo? Unsure. | - |
| `BarometricPressure` | 40 | 8 | Intel | U8 | 5 | 0 | 0 … 1275 | mbar | UNKNOWN | Turbo? Unsure. | - |
| `Speed_raw` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | UNKNOWN | Scaling unknown. | - |

<a id="msg-0x4e0"></a>
### 0x4E0 EngineStat

Sender: ECU · DLC 8 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x4E0. Sent by the VCU instead of the DME. Byte 0 status/lamp interpretation on bits 0..7 plus MO5 lamp/consumption layout on bits 9..31.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CheckEngineLight` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `CheckEngineLight_AlsoBit1` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `EngineStat_Bool2` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `Reduced_Power` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `Fan_Error` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `EngineStat_Bool5` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `EngineStat_Bool6` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `EngineStat_Bool7` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `MO5_Vorgluehen` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Diesel pre-glow lamp. | - |
| `MO5_E_Gas` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | E-Gas system lamp. | - |
| `MO5_OBD_2` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | OBD/MIL lamp control. | - |
| `MO5_Heissl` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Coolant overheat warning lamp. | - |
| `MO5_KlimaKompr` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | AC compressor off request. | - |
| `MO5_Feld_kuehl` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Map cooling installed and OK. | - |
| `MO5_KliKo_Red` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | AC compressor power reduction. | - |
| `MO5_Verbrauch` | 16 | 15 | Intel | U15 | 1 | 0 | 0 … 32767 | uL | INSTR | Fuel consumption counter (15 bit); the VCU simulates consumption (VCU-REQ-006). | - |
| `MO5_UeberlVerb` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Consumption counter overflow. | - |

<a id="msg-0x4f8"></a>
### 0x4F8 Unknown_4F8

Sender: ECU · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

0x4F8. Sent by the VCU (raw bytes). Signals unknown.

- **Offen (#8):** not in the stationary log; check whether it exists on this car.

Keine Signale bekannt (nur Rohbytes).

<a id="msg-0x7a0"></a>
### 0x7A0 Unknown_7A0

Sender: ECU · DLC 8 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x7A0. Sent by the VCU instead of the DME (raw bytes). Signals unknown. Standard payload: 85 0C 00 03 00 00 FF 02 (constant in the log).

Keine Signale bekannt (nur Rohbytes).

## 2. Vom Fahrzeug empfangen

<a id="msg-0x2a8"></a>
### 0x2A8 Vehicle_Speed

Sender: ABS_PSM · DLC 8 · Zyklus 20 ms (Original) · [zur Übersicht](#übersicht)

0x2A8. Wheel speeds, legacy 15-bit variant. Stationary log only: values not verified while driving.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Frei_Bremse_3_1` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | UNKNOWN | - | - |
| `WheelSpeed_LF_15b_legacy` | 1 | 15 | Intel | U15 | 0.01 | 0 | 0 … 327.67 | km/h | UNKNOWN | - | - |
| `Frei_Bremse_3_2` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | UNKNOWN | - | - |
| `WheelSpeed_RF_15b_legacy` | 17 | 15 | Intel | U15 | 0.01 | 0 | 0 … 327.67 | km/h | UNKNOWN | - | - |
| `Frei_Bremse_3_3` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | UNKNOWN | - | - |
| `WheelSpeed_LR_15b_legacy` | 33 | 15 | Intel | U15 | 0.01 | 0 | 0 … 327.67 | km/h | UNKNOWN | - | - |
| `Frei_Bremse_3_4` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | UNKNOWN | - | - |
| `WheelSpeed_RR_15b_legacy` | 49 | 15 | Intel | U15 | 0.01 | 0 | 0 … 327.67 | km/h | UNKNOWN | - | - |

<a id="msg-0x1a0"></a>
### 0x1A0 ABS_1

Sender: ABS_PSM · DLC 8 · Zyklus 20 ms (Original) · [zur Übersicht](#übersicht)

0x1A0. Bremse_1 (BR1) mapping.

- **Offen (#1):** byte 6 (bits 48-55) counts up by 1 per frame, so BR1_MSR_Mo (and the VCU value itq_pct) is a counter, not a torque; BR1_Zaehler (56|4) low nibble constant 0xD, bits 57-58 toggle.
- **Offen (#12):** BR1_ASRMo_fa and BR1_MSR_Mo_inv share byte 5.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `BR1_ASR_Anf` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ASR request. | - |
| `BR1_MSR_Anf` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | MSR request. | - |
| `BR1_ABS_Brems` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ABS control active. | - |
| `BR1_EDS_Ongr` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | EDS intervention. | - |
| `BR1_ESP_Ongr` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ESP intervention. | - |
| `BR1_ASR_Ongr` | 5 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | INSTR | ASR switching influence. | - |
| `BR1_EBV_Ongr` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | EBV intervention. | - |
| `BR1_Lampe_ABS` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ABS warning lamp. | - |
| `BR1_Lampe_ASR` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ASR/ESP lamp. | - |
| `BR1_Lampe_BK` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Brake warning lamp. | - |
| `BR1_Lichtschalt` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Brake switch. | - |
| `BR1_StaDruckschw` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Brake test switch status. | - |
| `BR1_MAD` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Rough road suppression. | - |
| `BR1_Sta_MAD` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Rough road suppression status. | - |
| `BR1_Diagnose` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ABS diagnosis mode. | - |
| `BR1_BKV_active` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Active brake booster status. | - |
| `BR1_Rad_kmh` | 17 | 15 | Intel | U15 | 0.01 | 0 | 0 … 327.67 | km/h | INSTR | Reference vehicle speed (VCU field vref_kmh). | - |
| `BR1_ASRMo_sl` | 32 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | INSTR | ASR torque slow. | - |
| `BR1_ASRMo_fa` | 40 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | INSTR | ASR torque fast (MSR_Anf=0). | - |
| `BR1_MSR_Mo_inv` | 40 | 8 | Intel | U8 | -0.39 | 99.45 | 0 … 99.45 | % | INSTR | Inverse MSR torque (MSR_Anf=1). | - |
| `BR1_MSR_Mo` | 48 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | INSTR | Is a frame counter in the log, not a torque (986 #1). | - |
| `BR1_Zaehler` | 56 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | INSTR | Message counter. | - |
| `BR1_ASR_ESP` | 60 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Control-unit type. | - |
| `BR1_ESPASR_passive` | 61 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ESP/ASR passive. | - |
| `BR1_Sta_ESP` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ESP error status. | - |
| `BR1_Ersatz_Kmh` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Substitute speed flag. | - |

<a id="msg-0x4a1"></a>
### 0x4A1 ABS_2

Sender: ABS_PSM · DLC 8 · Zyklus 100 ms (Original) · [zur Übersicht](#übersicht)

0x4A1. BrakeNormal_or_Offset depends on the multiplex code (bits 30-31).

- **Offen (#2):** bits 13-15 count 0..7 per frame, so ReceiptForACCMessage/DriverBraking/RegulatorError_FDR are a 3-bit counter; find the real driver-braking bit with a braking log.
- **Offen (#3):** bits 45-47 and 61-63 change every frame (checksum or multiplexed content?), bit 29 always 1.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `YawRateSign` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `YawRate` | 1 | 9 | Intel | U9 | 0.0021326 | 0 | 0 … 1.09 | - | INSTR | - | - |
| `NoActivelyBuiltUpBrakePressure` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `ACC_Related_Error` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `ReceiptForACCMessage` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `DriverBraking` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Part of a 3-bit frame counter in the log (986 #2). | - |
| `RegulatorError_FDR` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `Acceleration` | 16 | 9 | Intel | U9 | 0.3125 | 0 | 0 … 159.688 | - | INSTR | - | - |
| `ErrorDriverBrakeDetection` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `Multiplex_Byte4` | 30 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | INSTR | - | - |
| `BrakeNormal_or_Offset` | 32 | 8 | Intel | U8 | 0.1 | 0 | 0 … 25.5 | - | INSTR | - | - |

<a id="msg-0x510"></a>
### 0x510 InstrCluster_1

Sender: INSTR · DLC 8 · Zyklus 20 ms (Original) · [zur Übersicht](#übersicht)

0x510. VehicleSpeed scaling/offset is a working hypothesis. Bits 24-27 are a 4-bit counter (stationary log).

- **Offen (#4):** bits 40 and 43 always 1 (byte 5 = 0x09), meaning unknown.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `VehicleSpeed` | 0 | 16 | Intel | U16 | 0.250264 | -4100.17 | -4100.17 … 16383 | km/h | ECU | - | - |
| `Indicator_Left` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |
| `Indicator_Right` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |
| `Tempomat_Button` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |
| `InstrCluster1_Counter` | 24 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | UNKNOWN | 4-bit frame counter, found in the stationary log 2026-11-11. | - |
| `SteeringwheelButton_Up` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |
| `SteeringwheelButton_Dwn` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |

<a id="msg-0x520"></a>
### 0x520 InstrCluster_2

Sender: INSTR · DLC 8 · Zyklus 20 ms (Original) · [zur Übersicht](#übersicht)

0x520. OilPressure offset assumed 0.

- **Offen (#5):** byte 2 bits 16-20 change slowly (0x4E..0x50; temperature or fuel level?); bits 0 and 22 always 1; Counter_Unknown (byte 5) rotates 0x12, 0x03, 0x30, 0x1D every 100 ms - check whether it carries the odometer multiplexed (VW PQ Kombi_3: Kilometerstand 40|20, Standzeit 24|15).

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `LightDimmer` | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | ECU | - | - |
| `InstrCluster2_Byte2_Unknown` | 16 | 5 | Intel | U5 | 1 | 0 | 0 … 31 | - | UNKNOWN | Changes slowly (0x4E..0x50); meaning unknown (986 #5). | - |
| `AmbientTemp` | 24 | 8 | Intel | U8 | 0.5 | -40 | -40 … 87.5 | degC | ECU | - | - |
| `OilPressure` | 32 | 8 | Intel | U8 | 0.04 | 0 | 0 … 10.2 | bar | ECU | - | - |
| `Counter_Unknown` | 40 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | ECU | Rotates 0x12, 0x03, 0x30, 0x1D every 100 ms; odometer multiplex suspected (986 #5). | - |

<a id="msg-0x440"></a>
### 0x440 AT_Line1

Sender: TCU · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

0x440. PRND mapping from notes: P=128, R=119, N=96, D=81, M=225.

- **Offen (#9):** not in the stationary log; check whether the car has a Tiptronic control unit.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `PRND_Pos` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | INSTR | - | 128 = P, 119 = R, 96 = N, 81 = D, 225 = M |

<a id="msg-0x0c0"></a>
### 0x0C0 SWA

Sender: STEER · DLC 2 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x0C0. RB Lenkwinkel_1 layout: DLC=2, angle on bits 5..14 (factor 2.5, offset -720) plus sign and status/check bits. Not verified while steering.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `COARSE_CHECK__RB_` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `FINE_CHECK__RB_` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `Frei_Lenkwinkel_1_1__RB_` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | UNKNOWN | - | - |
| `LWS_Abgleich__RB_` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Calibration status. | - |
| `LWS_OK__RB_` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Sensor valid flag. | - |
| `Lenkwinkel__RB_` | 5 | 10 | Intel | U10 | 2.5 | -720 | -720 … 1837.5 | - | INSTR | Steering angle raw mapping. | - |
| `Vorzeichen__RB_` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Sign bit. | - |

<a id="msg-0x560"></a>
### 0x560 Unknown_560

Sender: INSTR · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

0x560. Signals unknown, raw bytes only.

- **Offen (#8):** not in the stationary log; check whether it exists on this car.

Keine Signale bekannt (nur Rohbytes).
