# Porsche 986 CAN (CAN1)

> **Gespiegelt aus der DBC, nicht von Hand ändern.** Masterquelle: [porsche_986_996_compiled_working.dbc](porsche_986_996_compiled_working.dbc). Änderungen zuerst in die DBC eintragen, dann `python tools/generate_can_md.py` ausführen. Erkenntnisse aus dem Reverse Engineering: Signale, `CM_`-Kommentare, `OPEN (986 #n)`, `GenMsgCycleTime`. Zuordnung zu VCU-Feldern und UI: [signalliste.md](signalliste.md).

Zykluszeiten aus dem Standmitschnitt `can_logs/986_log_stand.csv` (Originalfahrzeug).

14 Botschaften, 112 Signale.

## Übersicht

| ID | Botschaft | Sender | Rolle | Zyklus | Signale | Offen |
| --- | --- | --- | --- | --- | ---: | --- |
| [0x280](#msg-0x280) | [Engine_1](#msg-0x280) | ECU | von der VCU gesendet (statt DME) | 10 ms | 4 | #6 |
| [0x289](#msg-0x289) | [Motor_2](#msg-0x289) | ECU | von der VCU gesendet (statt DME) | 10 ms | 23 | #14 |
| [0x298](#msg-0x298) | [Engine_2](#msg-0x298) | ECU | von der VCU gesendet (statt DME) | 10 ms | 5 | - |
| [0x4E0](#msg-0x4e0) | [EngineStat](#msg-0x4e0) | ECU | von der VCU gesendet (statt DME) | 10 ms | 17 | - |
| [0x4F8](#msg-0x4f8) | [Unknown_4F8](#msg-0x4f8) | ECU | von der VCU gesendet (statt DME) | - | 0 | - |
| [0x7A0](#msg-0x7a0) | [Unknown_7A0](#msg-0x7a0) | ECU | von der VCU gesendet (statt DME) | 10 ms | 0 | - |
| [0x2A8](#msg-0x2a8) | [Vehicle_Speed](#msg-0x2a8) | ABS_PSM | vom Fahrzeug empfangen | 20 ms | 8 | - |
| [0x1A0](#msg-0x1a0) | [ABS_1](#msg-0x1a0) | ABS_PSM | vom Fahrzeug empfangen | 20 ms | 21 | - |
| [0x4A1](#msg-0x4a1) | [ABS_2](#msg-0x4a1) | ABS_PSM | vom Fahrzeug empfangen | 100 ms | 9 | #3 |
| [0x510](#msg-0x510) | [InstrCluster_1](#msg-0x510) | INSTR | vom Fahrzeug empfangen | 20 ms | 9 | #4 |
| [0x520](#msg-0x520) | [InstrCluster_2](#msg-0x520) | INSTR | vom Fahrzeug empfangen | 20 ms | 9 | #5 |
| [0x440](#msg-0x440) | [AT_Line1](#msg-0x440) | TCU | vom Fahrzeug empfangen | - | 1 | - |
| [0x0C0](#msg-0x0c0) | [SWA](#msg-0x0c0) | STEER | vom Fahrzeug empfangen | 10 ms | 6 | #15 |
| [0x560](#msg-0x560) | [Unknown_560](#msg-0x560) | INSTR | vom Fahrzeug empfangen | - | 0 | - |

## Offene Punkte

Arbeitsliste fürs Reverse Engineering (aus den `OPEN`-Kommentaren der DBC).

- **#3** ([0x4A1](#msg-0x4a1)): front/rear assignment of the pulse counters (left/right clear) and meaning of bit 29.
- **#4** ([0x510](#msg-0x510)): bits 40 and 43 always 1 in all logs incl. start-up; bit 45 set only in 986start and the first 4 s of 20261119_stand (before an ignition cycle); Tempomat and steering-wheel buttons never seen set.
- **#5** ([0x520](#msg-0x520)): weights of slots 1-3 (64/4096/262144 assumed: 20 952 km at the end of fahrt2) and total vs trip odometer - compare with an odometer photo; meaning of byte 2.
- **#6** ([0x280](#msg-0x280)): meaning of bytes 1, 4, 6, 7 (VW PQ Motor_1: inner torque, inner torque without intervention, mechanical loss torque, driver torque request) needs the original DME at different pedal positions with the engine running; whether the VCU should copy byte 1 bit 12 and byte 5 = 0 is open.
- **#11** (allgemein): no log yet with cruise control/steering-wheel buttons, lights/dimmer operated, hazard lights or an ABS stop.
- **#13** (allgemein): 20261119_stand shows two VCU send gaps (90.94-91.03 s with other frames still logged, 256.48-256.56 s with nothing logged); cause unknown.
- **#14** ([0x289](#msg-0x289)): align the VCU with the original.
- **#15** ([0x0C0](#msg-0x0c0)): scale 2.5 deg/bit (+-545 deg) and sign (+ = right turn assumed) not verified at full lock.

## 1. Von der VCU gesendet (statt DME)

<a id="msg-0x280"></a>
### 0x280 Engine_1

Sender: ECU · DLC 8 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x280. Sent by the VCU instead of the DME every 10 ms. RPM scaling matches Teensy (raw = rpm*4). Original DME (engine off) sends only 00 79 00 00 61 00 54 1C; bit 4 does not toggle (set once together with bit 23 and RPM 4000 in a later segment of the log, probably a tester mode). VCU sends 00 69 .. .. 61 60 54 1C: byte 1 lacks bit 12 and byte 5 is 0x60 (ThrottlePosition 37.5 %) instead of 0x00.

- **Offen (#6):** meaning of bytes 1, 4, 6, 7 (VW PQ Motor_1: inner torque, inner torque without intervention, mechanical loss torque, driver torque request) needs the original DME at different pedal positions with the engine running; whether the VCU should copy byte 1 bit 12 and byte 5 = 0 is open.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `RPM` | 16 | 16 | Intel | U16 | 0.25 | 0 | 0 … 16383.8 | rpm | INSTR | Engine speed shown by the tachometer; the VCU sends the motor power here (VCU-REQ-005). | - |
| `PedalPosition1` | 32 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | % | UNKNOWN | Not used in Teensy. | - |
| `ThrottlePosition` | 40 | 8 | Intel | U8 | 0.390625 | 0 | 0 … 100 | % | INSTR | - | - |
| `PedalPosition2` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | % | UNKNOWN | Not used in Teensy. | - |

<a id="msg-0x289"></a>
### 0x289 Motor_2

Sender: ECU · DLC 8 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x289. Sent by the VCU instead of the DME. Layout from VW PQ Motor_2. Bit 24 always 1, bit 25 = speed limiter selected (VCU code). Byte 0 multiplexed by MO2_Mp_Code (resolved #12): original DME and VCU send 0x00, 0x40, 0x80, 0xF3. Original holds the mux values 4/4/4/5 frames (170 ms), the VCU 4/4/4/4; original byte 3 = 0x7D, VCU 0x7C (bit 24 not sent; the VCU sets bit 24 in 0x298 instead, original 0x298 byte 3 = 0x00) -

- **Offen (#14):** align the VCU with the original.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `MO2_Getr_Code` (m2) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | INSTR | Active when MO2_Mp_Code=2. | - |
| `MO2_max_Mo` (m3) | 0 | 6 | Intel | U6 | 10 | 0 | 0 … 630 | Nm | INSTR | Active when MO2_Mp_Code=3; original DME sends raw 51 (510 Nm). | - |
| `MO2_CAN_Vers` (m0) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | INSTR | Active when MO2_Mp_Code=0. | - |
| `MO2_Motor_Code` (m1) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | INSTR | Active when MO2_Mp_Code=1. | - |
| `MO2_Mp_Code` (Mux) | 6 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | INSTR | Byte 0 multiplexer: 0x00, 0x40, 0x80, 0xF3 for values 0-3. | - |
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

0x4F8. Not present in any log, original DME included (resolved #8); the VCU does not send it either.

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

0x1A0. Bremse_1 (BR1) mapping (resolved #1, #12, logs 2026-11-19): byte 6 is an 8-bit message counter (former BR1_MSR_Mo), byte 7 lateral acceleration (former BR1_Zaehler, ASR_ESP, ESPASR_passive, Sta_ESP, Ersatz_Kmh). BR1_MSR_Anf is 0 in all logs, so the VW PQ multiplexed BR1_MSR_Mo_inv on byte 5 is dropped. Bytes 0, 4, 5 and bit 13 change only during ASR events (fahrt2 502.4 s, 835.4 s; fahrt1 67.8, 106.9, 131.2 s); no ABS stop logged yet. Power-on self test sets the lamp bits 8-10 and byte 7 = 0x00 for about 1 s.

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
| `BR1_Lichtschalt` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Brake light switch / driver braking: agrees with the inverter input SDU_din_brake in 99.9 % of frames, all 30 edges of fahrt2. | - |
| `BR1_StaDruckschw` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Brake pressure above threshold: rises 20-320 ms before BR1_Lichtschalt, with hysteresis. | - |
| `BR1_MAD` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Single 20 ms pulses and on during ASR events; wheel slip detected suspected. | - |
| `BR1_Sta_MAD` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Rough road suppression status. | - |
| `BR1_Diagnose` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | ABS diagnosis mode. | - |
| `BR1_BKV_active` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Active brake booster status. | - |
| `BR1_Rad_kmh` | 17 | 15 | Intel | U15 | 0.01 | 0 | 0 … 327.67 | km/h | INSTR | Reference vehicle speed (VCU field vref_kmh). | - |
| `BR1_ASRMo_sl` | 32 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | INSTR | ASR torque limit request (slow); 0xFE = no limit, down to 34 % during ASR events. The VCU does not act on it. | - |
| `BR1_ASRMo_fa` | 40 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | INSTR | ASR torque limit request (fast), equal to or below BR1_ASRMo_sl. | - |
| `BR1_Zaehler` | 48 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | INSTR | 8-bit message counter, +1 per frame (all logs). | - |
| `BR1_Querbeschl` | 56 | 8 | Intel | U8 | 0.1 | -12.8 | -12.8 … 12.7 | m/s2 | INSTR | Lateral acceleration (PSM sensor), + = left turn; r = 0.88 with the lateral acceleration from the 0x2A8 wheel speeds (fahrt2), none with longitudinal. About 0x7D at standstill, 0x00 while initialising. Scale/offset estimated - OPEN (986 #1): verify in a steady corner. | - |

<a id="msg-0x4a1"></a>
### 0x4A1 ABS_2

Sender: ABS_PSM · DLC 8 · Zyklus 100 ms (Original) · [zur Übersicht](#übersicht)

0x4A1. Four 16-bit words with an 11-bit wheel pulse counter each (resolved #2, #3, logs 2026-11-19): +1 per about 41.8 mm, wraps at 2048, all 0 at standstill; increment vs vehicle speed r = 0.9997. Bits 11, 12, 27, 28, 43, 44, 59, 60 always 0. Bits 13-15 frame counter; bits 30-31/45-47/61-63 an 8-bit millisecond time stamp (hi*64 + mid*8 + lo, +100/101 per frame, also at standstill). Driver braking is not in this message (see 0x1A0 BR1_Lichtschalt).

- **Offen (#3):** front/rear assignment of the pulse counters (left/right clear) and meaning of bit 29.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Radimpulse_VL` | 0 | 11 | Intel | U11 | 1 | 0 | 0 … 2047 | - | INSTR | Cumulative wheel pulse counter, ~41.8 mm per count, wraps at 2048 (front/rear OPEN, 986 #3). | - |
| `ABS2_Zaehler` | 13 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | INSTR | Frame counter, +1 per 100 ms frame. | - |
| `Radimpulse_VR` | 16 | 11 | Intel | U11 | 1 | 0 | 0 … 2047 | - | INSTR | - | - |
| `ABS2_Init_OK` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | 0 only in the first frame after ABS power-up (986start 14.85 s); meaning assumed. | - |
| `ABS2_Zeit_hi` | 30 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | INSTR | Time stamp t = hi*64 + mid*8 + lo in ms, +100/101 per frame. | - |
| `Radimpulse_HL` | 32 | 11 | Intel | U11 | 1 | 0 | 0 … 2047 | - | INSTR | - | - |
| `ABS2_Zeit_mid` | 45 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | INSTR | - | - |
| `Radimpulse_HR` | 48 | 11 | Intel | U11 | 1 | 0 | 0 … 2047 | - | INSTR | - | - |
| `ABS2_Zeit_lo` | 61 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | INSTR | - | - |

<a id="msg-0x510"></a>
### 0x510 InstrCluster_1

Sender: INSTR · DLC 8 · Zyklus 20 ms (Original) · [zur Übersicht](#übersicht)

0x510. Bits 24-27 4-bit counter, bits 28-31, byte 4 and byte 7 always 0. Indicator bits blink with 0.66 s period (0.34 s on).

- **Offen (#4):** bits 40 and 43 always 1 in all logs incl. start-up; bit 45 set only in 986start and the first 4 s of 20261119_stand (before an ignition cycle); Tempomat and steering-wheel buttons never seen set.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `VehicleSpeed` | 0 | 14 | Intel | U14 | 0.25 | 0 | 0 … 4095.75 | km/h | ECU | Displayed speed: about BR1_Rad_kmh + 4 km/h, lag about 0.4 s (fahrt1/fahrt2); max raw 517 (129 km/h). | - |
| `InstrCluster1_Bit14_Valid` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | 0 for about 1 s after cluster start, then 1; speed-valid flag suspected (was decoded as part of VehicleSpeed with offset -4100). | - |
| `Indicator_Left` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | Left/right supported by the wheel speed difference in turns (fahrt2). | - |
| `Indicator_Right` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |
| `Tempomat_Button` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |
| `InstrCluster1_Counter` | 24 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | UNKNOWN | 4-bit frame counter, found in the stationary log 2026-11-11. | - |
| `InstrCluster1_Bit45` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | UNKNOWN | See OPEN (986 #4). | - |
| `SteeringwheelButton_Up` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |
| `SteeringwheelButton_Dwn` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ECU | - | - |

<a id="msg-0x520"></a>
### 0x520 InstrCluster_2

Sender: INSTR · DLC 8 · Zyklus 20 ms (Original) · [zur Übersicht](#übersicht)

0x520. Byte 0 always 0x01; bytes 6-7 always 0 (VW PQ Kombi_3 odometer layout 40|20 refuted). Byte 5 is a 4-slot multiplex (Kombi_Mux bits 40-41, 6 data bits, 100 ms per slot, 400 ms cycle); slot 0 counts +1 per km (14 steps in fahrt1/fahrt2, 0.989 km per step against BR1_Rad_kmh), values carry on across logs. OilPressure 0xFF in all logs (not available).

- **Offen (#5):** weights of slots 1-3 (64/4096/262144 assumed: 20 952 km at the end of fahrt2) and total vs trip odometer - compare with an odometer photo; meaning of byte 2.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `LightDimmer` | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | ECU | Only 0 and 100 seen: 0 for about 20 s after wake-up and at shutdown. | - |
| `InstrCluster2_Byte2_Unknown` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | UNKNOWN | Whole byte: 78..80 changing by +-1 in 986_log_stand, 0xFF in all VCU logs (sensor removed?); fuel level or oil temperature suspected (986 #5). | 255 = not available |
| `AmbientTemp` | 24 | 8 | Intel | U8 | 0.5 | -40 | -40 … 87.5 | degC | ECU | Plausible against the BMS cell temperatures (8-9 degC, 20261119_stand); not checked with a thermometer. | - |
| `OilPressure` | 32 | 8 | Intel | U8 | 0.04 | 0 | 0 … 10.2 | bar | ECU | - | 255 = not available |
| `Kombi_Mux` (Mux) | 40 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | ECU | - | - |
| `Odometer_km_0` (m0) | 42 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | km | ECU | Odometer slot 0, +1 per km (verified fahrt1/fahrt2). | - |
| `Odometer_km_1` (m1) | 42 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | ECU | - | - |
| `Odometer_km_2` (m2) | 42 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | ECU | - | - |
| `Odometer_km_3` (m3) | 42 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | ECU | - | - |

<a id="msg-0x440"></a>
### 0x440 AT_Line1

Sender: TCU · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

0x440. PRND mapping from notes: P=128, R=119, N=96, D=81, M=225. Not present in any log incl. the drives 2026-11-19 (resolved #9): the car has no Tiptronic control unit on CAN.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `PRND_Pos` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | INSTR | - | 128 = P, 119 = R, 96 = N, 81 = D, 225 = M |

<a id="msg-0x0c0"></a>
### 0x0C0 SWA

Sender: STEER · DLC 2 · Zyklus 10 ms (Original) · [zur Übersicht](#übersicht)

0x0C0. RB Lenkwinkel_1 layout: DLC=2, angle bits 5-15 as signed 11-bit value (two's complement, the former sign bit 15 is its MSB) plus status/check bits. Fahrt2: raw -218..216, continuous through zero, r = -0.94 with the rear wheel speed difference of 0x2A8.

- **Offen (#15):** scale 2.5 deg/bit (+-545 deg) and sign (+ = right turn assumed) not verified at full lock.

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `COARSE_CHECK__RB_` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `FINE_CHECK__RB_` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | - | - |
| `Frei_Lenkwinkel_1_1__RB_` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | UNKNOWN | - | - |
| `LWS_Abgleich__RB_` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Calibration status. | - |
| `LWS_OK__RB_` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | INSTR | Sensor valid flag. | - |
| `Lenkwinkel__RB_` | 5 | 11 | Intel | S11 | 2.5 | 0 | -2560 … 2557.5 | deg | INSTR | Signed 11 bit; scale and sign see OPEN (986 #15). | - |

<a id="msg-0x560"></a>
### 0x560 Unknown_560

Sender: INSTR · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

0x560. Not present in any log incl. the drives 2026-11-19 (resolved #8).

Keine Signale bekannt (nur Rohbytes).
