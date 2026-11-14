# VW PQ CAN (Referenz)

> **Gespiegelt aus der DBC, nicht von Hand ändern.** Masterquelle: [vw_pq.dbc](vw_pq.dbc). Änderungen zuerst in die DBC eintragen, dann `python tools/generate_can_md.py` ausführen. Nur Hintergrund zum Abgleich mit den 986-Botschaften (VCU-DEC-013); die VCU nutzt diese Botschaften nicht.

86 Botschaften, 1331 Signale.

## Übersicht

| ID | Botschaft | Sender | Rolle | Zyklus | Signale | Offen |
| --- | --- | --- | --- | --- | ---: | --- |
| [0x050](#msg-0x050) | [Airbag_1](#msg-0x050) | XXX | - | - | 20 | - |
| [0x0C0](#msg-0x0c0) | [Lenkwinkel_1__RB_](#msg-0x0c0) | XXX | - | - | 7 | - |
| [0x0C2](#msg-0x0c2) | [Lenkwinkel_1](#msg-0x0c2) | XXX | - | - | 11 | - |
| [0x0C4](#msg-0x0c4) | [Lenkwinkel_1__ITT_](#msg-0x0c4) | XXX | - | - | 7 | - |
| [0x0D0](#msg-0x0d0) | [Lenkhilfe_3](#msg-0x0d0) | XXX | - | - | 11 | - |
| [0x0D2](#msg-0x0d2) | [HCA_1](#msg-0x0d2) | XXX | - | - | 7 | - |
| [0x100](#msg-0x100) | [MSG_1](#msg-0x100) | XXX | - | - | 5 | - |
| [0x102](#msg-0x102) | [Master_1](#msg-0x102) | XXX | - | - | 9 | - |
| [0x104](#msg-0x104) | [Master_2](#msg-0x104) | XXX | - | - | 9 | - |
| [0x106](#msg-0x106) | [Master_3](#msg-0x106) | XXX | - | - | 5 | - |
| [0x10C](#msg-0x10c) | [Slave_1](#msg-0x10c) | XXX | - | - | 13 | - |
| [0x112](#msg-0x112) | [PSG_1](#msg-0x112) | XXX | - | - | 6 | - |
| [0x1A0](#msg-0x1a0) | [Bremse_1](#msg-0x1a0) | ABS | - | - | 26 | - |
| [0x1A8](#msg-0x1a8) | [Bremse_6](#msg-0x1a8) | XXX | - | - | 5 | - |
| [0x1AC](#msg-0x1ac) | [Bremse_8](#msg-0x1ac) | XXX | - | - | 22 | - |
| [0x260](#msg-0x260) | [ADR_2](#msg-0x260) | XXX | - | - | 7 | - |
| [0x280](#msg-0x280) | [Motor_1](#msg-0x280) | XXX | - | - | 14 | - |
| [0x284](#msg-0x284) | [Motor_Bremse](#msg-0x284) | XXX | - | - | 9 | - |
| [0x288](#msg-0x288) | [Motor_2](#msg-0x288) | Motor | - | - | 22 | - |
| [0x2A0](#msg-0x2a0) | [Bremse_4](#msg-0x2a0) | XXX | - | - | 4 | - |
| [0x2A8](#msg-0x2a8) | [Bremsbooster_1](#msg-0x2a8) | XXX | - | - | 11 | - |
| [0x2C0](#msg-0x2c0) | [Allrad_1](#msg-0x2c0) | XXX | - | - | 17 | - |
| [0x320](#msg-0x320) | [Kombi_1](#msg-0x320) | XXX | - | - | 24 | - |
| [0x343](#msg-0x343) | [RDK_Status](#msg-0x343) | XXX | - | - | 20 | - |
| [0x360](#msg-0x360) | [ADR_System](#msg-0x360) | XXX | - | - | 27 | - |
| [0x366](#msg-0x366) | [AWV](#msg-0x366) | XXX | - | - | 31 | - |
| [0x368](#msg-0x368) | [ACC_System](#msg-0x368) | XXX | - | - | 14 | - |
| [0x380](#msg-0x380) | [Motor_3](#msg-0x380) | Motor | - | - | 18 | - |
| [0x384](#msg-0x384) | [Motor_Momente](#msg-0x384) | XXX | - | - | 1 | - |
| [0x388](#msg-0x388) | [GRA](#msg-0x388) | XXX | - | - | 10 | - |
| [0x38A](#msg-0x38a) | [GRA_Neu](#msg-0x38a) | XXX | - | - | 20 | - |
| [0x390](#msg-0x390) | [Gate_Komf_1](#msg-0x390) | XXX | - | - | 55 | - |
| [0x392](#msg-0x392) | [Gate_Komf_2](#msg-0x392) | XXX | - | - | 34 | - |
| [0x3A0](#msg-0x3a0) | [Bremse_10](#msg-0x3a0) | XXX | - | - | 18 | - |
| [0x3BA](#msg-0x3ba) | [SWA_1](#msg-0x3ba) | SWA | - | - | 9 | - |
| [0x3D0](#msg-0x3d0) | [Lenkhilfe_1](#msg-0x3d0) | XXX | - | - | 22 | - |
| [0x3D2](#msg-0x3d2) | [Lenkhilfe_2](#msg-0x3d2) | XXX | - | - | 11 | - |
| [0x3D4](#msg-0x3d4) | [PLA_1](#msg-0x3d4) | XXX | - | - | 8 | - |
| [0x420](#msg-0x420) | [Kombi_2](#msg-0x420) | XXX | - | - | 15 | - |
| [0x440](#msg-0x440) | [Getriebe_1](#msg-0x440) | Transmission | - | - | 20 | - |
| [0x448](#msg-0x448) | [Waehlhebel_1](#msg-0x448) | XXX | - | - | 8 | - |
| [0x470](#msg-0x470) | [BSG_Kombi](#msg-0x470) | XXX | - | - | 25 | - |
| [0x480](#msg-0x480) | [Motor_5](#msg-0x480) | Motor | - | - | 37 | - |
| [0x488](#msg-0x488) | [Motor_6](#msg-0x488) | XXX | - | - | 11 | - |
| [0x497](#msg-0x497) | [Parkhilfe_01](#msg-0x497) | XXX | - | - | 23 | - |
| [0x4A0](#msg-0x4a0) | [Bremse_3](#msg-0x4a0) | XXX | - | - | 8 | - |
| [0x4A8](#msg-0x4a8) | [Bremse_5](#msg-0x4a8) | XXX | - | - | 27 | - |
| [0x500](#msg-0x500) | [MSG_2](#msg-0x500) | XXX | - | - | 4 | - |
| [0x512](#msg-0x512) | [PSG_2](#msg-0x512) | XXX | - | - | 4 | - |
| [0x514](#msg-0x514) | [NOX_1](#msg-0x514) | XXX | - | - | 13 | - |
| [0x520](#msg-0x520) | [Kombi_3](#msg-0x520) | XXX | - | - | 25 | - |
| [0x52C](#msg-0x52c) | [ADR_1](#msg-0x52c) | XXX | - | - | 22 | - |
| [0x52E](#msg-0x52e) | [Diag_Lenkhilfe](#msg-0x52e) | XXX | - | - | 5 | - |
| [0x530](#msg-0x530) | [Navigation_1](#msg-0x530) | XXX | - | - | 12 | - |
| [0x534](#msg-0x534) | [Sitz_info](#msg-0x534) | XXX | - | - | 6 | - |
| [0x538](#msg-0x538) | [Wischer_1](#msg-0x538) | XXX | - | - | 14 | - |
| [0x53C](#msg-0x53c) | [Fahrwerk_1](#msg-0x53c) | XXX | - | - | 4 | - |
| [0x540](#msg-0x540) | [Getriebe_2](#msg-0x540) | XXX | - | - | 20 | - |
| [0x548](#msg-0x548) | [Getriebe_4](#msg-0x548) | XXX | - | - | 7 | - |
| [0x550](#msg-0x550) | [Airbag_2](#msg-0x550) | XXX | - | - | 7 | - |
| [0x56A](#msg-0x56a) | [ACC_GRA_Anzeige](#msg-0x56a) | XXX | - | - | 19 | - |
| [0x570](#msg-0x570) | [BSG_Last](#msg-0x570) | XXX | - | - | 21 | - |
| [0x572](#msg-0x572) | [ZAS_1](#msg-0x572) | XXX | - | - | 10 | - |
| [0x578](#msg-0x578) | [BatMan_1](#msg-0x578) | XXX | - | - | 7 | - |
| [0x580](#msg-0x580) | [Motor_Flexia](#msg-0x580) | XXX | - | - | 21 | - |
| [0x588](#msg-0x588) | [Motor_7](#msg-0x588) | XXX | - | - | 13 | - |
| [0x590](#msg-0x590) | [Niveau_1](#msg-0x590) | XXX | - | - | 25 | - |
| [0x598](#msg-0x598) | [Daempfer_1](#msg-0x598) | XXX | - | - | 8 | - |
| [0x5A0](#msg-0x5a0) | [Bremse_2](#msg-0x5a0) | XXX | - | - | 14 | - |
| [0x5B7](#msg-0x5b7) | [Bremse_11](#msg-0x5b7) | XXX | - | - | 10 | - |
| [0x5B8](#msg-0x5b8) | [WFS_1](#msg-0x5b8) | XXX | - | - | 3 | - |
| [0x5BE](#msg-0x5be) | [LDW_Status](#msg-0x5be) | XXX | - | - | 15 | - |
| [0x5C0](#msg-0x5c0) | [EPB_1](#msg-0x5c0) | XXX | - | - | 30 | - |
| [0x5D0](#msg-0x5d0) | [Systeminfo_1](#msg-0x5d0) | XXX | - | - | 20 | - |
| [0x5D2](#msg-0x5d2) | [Ident](#msg-0x5d2) | XXX | - | - | 22 | - |
| [0x5D8](#msg-0x5d8) | [Verbauliste_1](#msg-0x5d8) | XXX | - | - | 59 | - |
| [0x5DC](#msg-0x5dc) | [Soll_Verbauliste_neu](#msg-0x5dc) | XXX | - | - | 64 | - |
| [0x5DE](#msg-0x5de) | [Lenkhilfe_Fehler](#msg-0x5de) | XXX | - | - | 39 | - |
| [0x5E0](#msg-0x5e0) | [Klima_1](#msg-0x5e0) | XXX | - | - | 18 | - |
| [0x60E](#msg-0x60e) | [Einheiten_1](#msg-0x60e) | XXX | - | - | 1 | - |
| [0x700](#msg-0x700) | [MSG_3](#msg-0x700) | XXX | - | - | 2 | - |
| [0x704](#msg-0x704) | [Motor_NOX](#msg-0x704) | XXX | - | - | 7 | - |
| [0x712](#msg-0x712) | [PSG_3](#msg-0x712) | XXX | - | - | 1 | - |
| [0x7C0](#msg-0x7c0) | [LWS_Kalibrierung](#msg-0x7c0) | XXX | - | - | 4 | - |
| [0x7C2](#msg-0x7c2) | [Lenkwinkel_Init](#msg-0x7c2) | XXX | - | - | 2 | - |
| [0x7D0](#msg-0x7d0) | [Diagnose_1](#msg-0x7d0) | XXX | - | - | 10 | - |

## Botschaften

<a id="msg-0x050"></a>
### 0x050 Airbag_1

Sender: XXX · DLC 4 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Front_Crash` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heck_Crash` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Seiten_Crash_Fahrer` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Seiten_Crash_Beifahrer` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Rollover` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Crash_Intensitaet` | 5 | 3 | Intel | U3 | 1 | 0 | 0 … 111 | B | XXX | - | - |
| `Airbag_Lampe` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Airbag_deaktiviert` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kindersitzerkennung` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Airbag_Systemfehler` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Gurtschalter_Fahrer` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Gurtwarnung_Fahrer` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Gurtschalter_Beifahrer` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Gurtwarnung_Beifahrer` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Airbag_in_Diagnose` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Airbag_im_Stellgliedtest` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Airbag_1_2` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `COUNTER` | 20 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `CHECKSUM` | 24 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x0c0"></a>
### 0x0C0 Lenkwinkel_1__RB_

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `COARSE_CHECK__RB_` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `FINE_CHECK__RB_` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Lenkwinkel_1_1__RB_` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `LWS_Abgleich__RB_` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `LWS_OK__RB_` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkwinkel__RB_` | 5 | 10 | Intel | U10 | 2.5 | -720 | -720 … 720 | - | XXX | - | - |
| `Vorzeichen__RB_` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x0c2"></a>
### 0x0C2 Lenkwinkel_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

Steering wheel angle message 1 - Contains steering angle, speed, and status information

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `LW1_LRW` | 0 | 15 | Intel | U15 | 0.04375 | 0 | 0 … 1433.55625 | °Grad | Gateway, ESP, ABS | Steering wheel angle (0 = straight ahead, 7FFFh = stop) | - |
| `LW1_LRW_Sign` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway, ESP, ABS | Steering angle sign: 0=positive (left), 1=negative (right) | 0 = positives_Vorzeichen, 1 = negatives_Vorzeichen |
| `LW1_Lenk_Gesch` | 16 | 15 | Intel | U15 | 0.04375 | 0 | 0 … 1433.55625 | °Grad/s | Gateway, ESP, ABS | Steering wheel angular velocity (0 = no movement, 7FFFh = max speed) | - |
| `LW1_Gesch_Sign` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway, ESP, ABS | Angular velocity sign: 0=positive, 1=negative | 0 = positives_Vorzeichen, 1 = negatives_Vorzeichen |
| `LW1_ID` | 32 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | Gateway, ESP, ABS | Calibration ID - 0=not calibrated, 128=calibrated (PQ35/46/VW32x/VW411) | 0 = noch_not_kalibriert, 128 = kalibriert_nur_bei_PQ35_46_VW32x_VW411 |
| `LW1_Initquelle` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway, ESP, ABS | Initialization source | 0 = Bremse_3, 1 = EPS_Bit |
| `LW1_Status` | 41 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Gateway, ESP, ABS | Sensor status | 0 = OK, 1 = no_initial, 2 = sporadic_failure, 3 = permanent_failure |
| `LW1_Sta_KL30` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway, ESP, ABS | Terminal 30 status (Kostal sensor) | 0 = ok, 1 = no_init_s |
| `LW1_Zaehler` | 44 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | Gateway, ESP, ABS | Free running message counter | - |
| `LW1_Kodier` | 48 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | Gateway | Coding data (multiplex output depends on counter) | - |
| `LW1_CRC` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | Gateway, ESP, ABS | Checksum - One's complement of 8-bit sum of bytes 1,2,3,4,6 | - |

<a id="msg-0x0c4"></a>
### 0x0C4 Lenkwinkel_1__ITT_

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `COARSE_CHECK__ITT_` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `FINE_CHECK__ITT_` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `LWS_Initialisierung__ITT_` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `LWS_Abgleich__ITT_` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `LWS_OK` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkwinkel__ITT_` | 5 | 10 | Intel | U10 | 1.5 | -768 | -768 … 766.5 | - | XXX | - | - |
| `Vorzeichen__ITT_` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x0d0"></a>
### 0x0D0 Lenkhilfe_3

Sender: XXX · DLC 6 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `LH3_BS_Spiegel` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `COUNTER` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `LH3_LM` | 16 | 10 | Intel | U10 | 1 | 0 | 0 … 1023 | - | XXX | - | - |
| `LH3_LMSign` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH3_LMValid` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH3_Sta_DSR` | 28 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `LH3_BLW` | 32 | 12 | Intel | U12 | 0.15 | 0 | 0 … 615 | - | XXX | - | - |
| `LH3_BLWSign` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH3_BLWValid` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH3_Lenkungstyp` | 46 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |

<a id="msg-0x0d2"></a>
### 0x0D2 HCA_1

Sender: XXX · DLC 5 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `COUNTER` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `HCA_Status` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `LM_Offset` | 16 | 15 | Intel | U15 | 0.03125 | 0 | 0 … 300 | cNm | XXX | centiNewton-meters for ease of calculation without FP math in Panda | - |
| `LM_OffSign` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `Vib_Freq` | 32 | 4 | Intel | U4 | 4 | 0 | 0 … 60 | Hz | XXX | Steering wheel haptic, frequency | - |
| `Vib_Amp` | 36 | 4 | Intel | U4 | 0.5 | 0 | 0 … 7.5 | Nm | XXX | Steering wheel haptic, amplitude | - |

<a id="msg-0x100"></a>
### 0x100 MSG_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Soll_Einspritzmenge` | 0 | 16 | Intel | U16 | 0.03125 | 0 | 0 … 2047 | mg/H | XXX | - | - |
| `Soll_Voreinspritzung` | 16 | 12 | Intel | U12 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Soll_Foerderbeginn_NW__3_2_2_` | 28 | 12 | Intel | U12 | 0.01172 | 0 | 0 … 768 | degNW | XXX | - | - |
| `Soll_Foerderbeginn_KW__3_2_2_` | 40 | 16 | Intel | U16 | 0.01172 | -384 | -384 … 384 | KW | XXX | - | - |
| `Kurbelwellendrehzahl__3_2_2_` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x102"></a>
### 0x102 Master_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `LLR_Freigabe_nach_Start` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `PD_Anteil_der_LLR_aktiv` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `I_Anteil_der_LLR_aktiv` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `LLR_ist_aktiv` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Master_1_1` | 4 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Drehmoment_LLR__Luftpfad_` | 8 | 16 | Intel | U16 | 0.003052 | -100 | -100 … 100 | % | XXX | - | - |
| `Drehmoment_LLR__PD_Anteil_` | 24 | 16 | Intel | U16 | 0.003052 | -100 | -100 … 100 | % | XXX | - | - |
| `Drehmoment_LLR__I_Anteil_` | 40 | 16 | Intel | U16 | 0.003052 | -100 | -100 … 100 | % | XXX | - | - |
| `Stationaere_Solldrehzahl` | 56 | 8 | Intel | U8 | 10 | 0 | 0 … 2550 | Umin | XXX | - | - |

<a id="msg-0x104"></a>
### 0x104 Master_2

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Normierter_Fahrpedalwinkel` | 0 | 16 | Intel | U16 | 0.001526 | 0 | 0 … 100 | % | XXX | - | - |
| `Zaehler__Master_2_` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `Pedalwert_Komplement` | 24 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `SA_Verbot_von_FGR` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Vmax_Begrenzung_aktiv` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Master_erkannt` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Einspritzverbot_DV_E_lernen` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `DK_Sollwertbegrenzung` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ubat_Freigabe_DVE` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x106"></a>
### 0x106 Master_3

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Delta_Motormoment_aus_Verlustmo` | 0 | 16 | Intel | U16 | 0.003052 | -100 | -100 … 100 | % | XXX | - | - |
| `Relative_Momentenanforderung_de` | 16 | 16 | Intel | U16 | 0.003052 | 0 | 0 … 200 | % | XXX | - | - |
| `Indiziertes_Sollmoment_f_r_Vmax` | 32 | 16 | Intel | U16 | 0.0015259 | 0 | 0 … 100 | % | XXX | - | - |
| `Motortemperatur_linearisiert` | 48 | 8 | Intel | U8 | 0.75 | -48 | -48 … 143.25 | - | XXX | - | - |
| `Frei_Master_3_1` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x10c"></a>
### 0x10C Slave_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Indiziertes_Istmoment__Slave_` | 0 | 16 | Intel | U16 | 0.0015259 | 0 | 0 … 99.998 | % | XXX | - | - |
| `Timeout_Bremsenbotschaft` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehler_Bremsenbotschaft` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehler_Getriebe_Moment` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehler_Momentenausgabe` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Master_erkannt__Slave_` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Einspritzverbot_lernen__Slave_` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Drosselklappe_Sollwertbegrenzun` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ubat_Freigabe_DVE__Slave_` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Delta_Zuendwinkelausgabe` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Slave_1_1` | 25 | 7 | Intel | U7 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Delta_Drosselklappenwinkel` | 32 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Delta_reduziertes_Sollmoment` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x112"></a>
### 0x112 PSG_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Pumpen_Statuswort__3_2_2_` | 0 | 20 | Intel | U20 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Nockenwellendrehzahl__3_2_2_` | 20 | 12 | Intel | U12 | 4 | 0 | 0 … 16380 | upm | XXX | - | - |
| `Ansteuerdauer__3_2_2_` | 32 | 11 | Intel | U11 | 0.0469 | 0 | 0 … 96 | NW | XXX | - | - |
| `Zylinderzaehler__3_2_2_` | 43 | 3 | Intel | U3 | 1 | 0 | 1 … 8 | Zaehler | XXX | - | - |
| `Pumpentemperatur__3_2_2_` | 44 | 12 | Intel | U12 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Pumpentemperatur__2_1_` | 48 | 16 | Intel | U16 | 0.0625 | 0 | 0 … 4096 | K | XXX | - | - |

<a id="msg-0x1a0"></a>
### 0x1A0 Bremse_1

Sender: ABS · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `BR1_ASR_Anf` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Motor | ASR request to engine - requests in bytes 5 and 6 to be executed | 0 = no_requirement, 1 = ASR_requirement |
| `BR1_MSR_Anf` (Mux) | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Motor | MSR request - when set, byte 6 contains inverse MSR torque | 0 = no_requirement, 1 = MSR_requirement |
| `BR1_ABS_Brems` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | ABS control active (including pressure reduction) | 0 = no_ABS_control, 1 = ABS_control |
| `BR1_EDS_Ongr` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Electronic differential lock intervention | 0 = no_EDS_intervention, 1 = EDS_intervention |
| `BR1_ESP_Ongr` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | ESP driving dynamics control intervention | 0 = no_ESP_intervention, 1 = ESP_intervention |
| `BR1_ASR_Ongr` | 5 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Transmission | ASR transmission switching influence | 0 = no_requirement, 1 = ASR_shift_map, 2 = Downshift, 3 = Switching_ban |
| `BR1_EBV_Ongr` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Electronic brake force distribution intervention | 0 = no_EBV_intervention, 1 = EBV_intervention |
| `BR1_Lampe_ABS` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | ABS safety warning lamp | 0 = Lamp_off, 1 = Lamp_on |
| `BR1_Lampe_ASR` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | ASR/ESP lamp (including flashing info) | 0 = Lamp_off, 1 = Lamp_on |
| `BR1_Lampe_BK` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Brake control lamp (red) | 0 = Lamp_off, 1 = Lamp_on |
| `BR1_Lichtschalt` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Brake light switch / brake pressure threshold exceeded | 0 = no_Bremsen_Schw_unterschr, 1 = Bremse_betaetigt_Schw_ueberschr |
| `BR1_StaDruckschw` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Brake test switch / pressure threshold status | 0 = Sta_guel_BKV_n_angest_k_Br, 1 = Sta_n_verf_BKV_angest_Br |
| `BR1_MAD` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | OBD rough road suppression | 0 = no_Offblendung, 1 = Offblendung |
| `BR1_Sta_MAD` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Rough road suppression status | 0 = valid, 1 = invalid |
| `BR1_Diagnose` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | ABS diagnosis mode active | 0 = no, 1 = in_Diagnose |
| `BR1_BKV_active` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Active brake booster installed and not faulty | 0 = BKV_not_installed_or_fault, 1 = BKV_installed_and_not_fault |
| `BR1_Rad_kmh` | 17 | 15 | Intel | U15 | 0.01 | 0 | 0 … 326.39 | km/h | XXX | Vehicle speed (mean of driven wheels or ABS reference) | - |
| `BR1_ASRMo_sl` | 32 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | Motor | ASR engagement torque slow (0-99.06%) | - |
| `BR1_ASRMo_fa` (m0) | 40 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | Motor | ASR engagement torque fast when MSR_Anf=0 (0-99.06%) | - |
| `BR1_MSR_Mo_inv` (m1) | 40 | 8 | Intel | U8 | -0.39 | 99.45 | 0 … 99.06 | % | Motor | Inverse MSR torque when MSR_Anf=1 (99.06-0%) | - |
| `BR1_MSR_Mo` | 48 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | Motor | MSR engagement torque (0-99.06%) | - |
| `BR1_Zaehler` | 56 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | Message counter to mark intervention moment | - |
| `BR1_ASR_ESP` | 60 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Control unit type: 0=ABS/EDS, 1=ASR/ESP/MABS | 0 = ABS_ABS_EDS, 1 = ASR_ESP_MABS |
| `BR1_ESPASR_passive` | 61 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | ESP/ASR passive or deactivated by driver | 0 = ESP_ASR_activated, 1 = ESP_oder_ASR_passive_get_oder_Fktumsch_oder_Rollenmodus |
| `BR1_Sta_ESP` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | ESP error status (fault memory entry) | 0 = ok, 1 = Failure |
| `BR1_Ersatz_Kmh` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Speed is substitute value (sensor defect) | 0 = OK, 1 = Substitute_value |

<a id="msg-0x1a8"></a>
### 0x1A8 Bremse_6

Sender: XXX · DLC 3 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Bremsdruck__Bremse_6_` | 0 | 10 | Intel | U10 | 0.3255 | -40 | -40 … 293 | bar | XXX | - | - |
| `Frei_Bremse_6_1` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Status_Bremsdruck__Bremse_6__du` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zaehler_Bremse_6` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Checksumme_Bremse_6` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x1ac"></a>
### 0x1AC Bremse_8

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `BR8_Checksumme` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `BR8_Zaehler` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `BR8_Sta_ACC_Anf` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Verz_EPB_akt` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Sta_Br_temp` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Sta_Br_Druck` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_TolAbgl_HL` | 16 | 8 | Intel | U8 | 0.048828125 | -6.201171875 | -6.201171875 … 6.15234375 | Unit_PerCent | XXX | - | - |
| `BR8_TolAbgl_HR` | 24 | 8 | Intel | U8 | 0.048828125 | -6.201171875 | -6.201171875 … 6.15234375 | Unit_PerCent | XXX | - | - |
| `BR8_Istbeschl` | 32 | 9 | Intel | U9 | 0.02 | -7.22 | -7.22 … 2.98 | Unit_MeterPerSeconSquar | XXX | - | - |
| `BR8_Sta_HW_BLS` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_QB_LBeschl` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_ESC_Mode` | 43 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `BR8_aktBrSyst` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Fa_bremst` | 46 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_StaBrSyst` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Laengsbeschl` | 48 | 10 | Intel | U10 | 0.03125 | -16 | -15.96875 … 15.9375 | Unit_MeterPerSeconSquar | XXX | - | - |
| `BR8_Sta_ADR_BR` | 58 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Quattro` | 59 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Sta_VerzReg` | 60 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Sta_BLS` | 61 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Verz_EPB` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR8_Check_EPB` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x260"></a>
### 0x260 ADR_2

Sender: XXX · DLC 4 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Checksumme_ADR_2` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zaehler_ADR_2` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Standby` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Relais_Test_Fehler` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_ADR_2_1` | 14 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Anforderung_Bremsdruck` | 16 | 11 | Intel | U11 | 0.0625 | 0 | 0 … 127.9375 | bar | XXX | - | - |
| `Frei_ADR_2_2` | 27 | 5 | Intel | U5 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x280"></a>
### 0x280 Motor_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Leergasinformation` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fahrpedalwert_ungenau__Motor_1_` (Mux) | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kickdownschalter` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kupplungsschalter` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Time_Out_Bremsen_Botschaft` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerstatus_Brems_Momenteneing` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerstatus_Getriebe_Momentene` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Momentenangaben_ungenau` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | Approximate Torque Values | - |
| `inneres_Motor_Moment` | 8 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99 | MDI | XXX | Engine Indicated Torque | - |
| `Motordrehzahl` | 16 | 16 | Intel | U16 | 0.25 | 0 | 0 … 16256 | U/min | XXX | Engine Speed | - |
| `inneres_Motor_Moment_ohne_exter` | 32 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99 | MDI | XXX | Inner torque without external | - |
| `Fahrpedalwert_oder_Drosselklapp` | 40 | 8 | Intel | U8 | 0.4 | 0 | 0 … 101.6 | % | XXX | Accelerator Pedal or Throttle Position | - |
| `mechanisches_Motor_Verlustmomen` | 48 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99 | MDI | XXX | Mechanical Torque Loss | - |
| `Fahrerwunschmoment` | 56 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99 | MDI | XXX | Driver Requested Torque | - |

<a id="msg-0x284"></a>
### 0x284 Motor_Bremse

Sender: XXX · DLC 6 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `MOB_CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | Bremse_MK25AESP | Checksum MOB | - |
| `MOB_COUNTER` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | Bremse_MK25AESP | Counter MOB | - |
| `MOB_Standby` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `MOB_Anhaltewunsch` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Bremse_MK25AESP, Getriebe_DQ | - | - |
| `MOB_Freigabe` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | BCM_Gateway, Bremse_MK25AESP, Gateway_separat | - | - |
| `TSK_v_Begrenzung_aktiv` | 15 | 1 | Motorola | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `MOB_Bremsstgr` | 16 | 11 | Intel | U11 | 0.048852 | 0 | 0 … 100 | Unit_PerCent | Vector__XXX | - | - |
| `MOB_Bremsmom` | 27 | 13 | Intel | U13 | 4 | 0 | 0 … 32760 | Unit_NewtoMeter | Bremse_MK25AESP | - | - |
| `TSK_ax_Getriebe_01` | 40 | 8 | Intel | U8 | 0.048 | 0 | 0 … 255 | m/s2 | XXX | - | - |

<a id="msg-0x288"></a>
### 0x288 Motor_2

Sender: Motor · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

Motor message 2 - Contains engine status, temperatures, and torque information

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `MO2_Getr_Code` (m2) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | Gateway | Transmission code (multiplex value when Mp_Code=2) | 0 = _5HP19, 1 = _5HP24, 2 = AG4, 3 = VL300, 4 = VQ250, 5 = VQ35, 6 = AG4_yestco, 7 = AG5_yestco, 8 = autom_Kupplung, 9 = autom_Kupplung_mit_ASG, 10 = AG6_yestco_ZF_6HP_AISIN, 11 = DQ500_DQ250_DQ200, 12 = SQ100_SQ200, 14 = AL1000_AL551_AL951_ZF8HPx, 15 = Handschalter_konv, 16 = DL800, 17 = E_Fahrzeug__Single_Gear |
| `MO2_max_Mo` (m3) | 0 | 6 | Intel | U6 | 10 | 0 | 0 … 630 | Nm | Gateway | Maximum torque MDI (multiplex value when Mp_Code=3) | - |
| `MO2_CAN_Vers` (m0) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | Gateway | CAN version (multiplex value when Mp_Code=0) | - |
| `MO2_Motor_Code` (m1) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | Gateway | Motor code (multiplex value when Mp_Code=1) | - |
| `MO2_Mp_Code` | 6 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Gateway | Multiplex code - switches after 4 transmissions | 0 = CAN_Stand, 1 = Motor_Kod, 2 = Getriebe_Kod, 3 = MDI_Max |
| `MO2_Kuehlm_T` | 8 | 8 | Intel | U8 | 0.75 | -48 | -47.25 … 142.5 | °C | Gateway | Engine coolant temperature or substitute value | - |
| `MO2_BLS` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Brake light switch - unfiltered raw signal | 0 = no_Bremsen, 1 = Bremse_betaetigt |
| `MO2_BTS` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Brake test switch | 0 = no_braking, 1 = Brake_activated |
| `MO2_Sta_Kuehlm` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Coolant temperature status - 0=OK, 1=Not OK | 0 = Temperature_ok, 1 = Temperature_not_ok |
| `MO2_Sta_Klima` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Air conditioning compressor status | 0 = Climate_off, 1 = Climate_on |
| `MO2_Sta_No_Bet` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Normal operation status (Terminal 15 on, init complete) | 0 = no_Normalbetrieb, 1 = Normalbetrieb |
| `MO2_Status_TSK` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Drive train coordinator status | 0 = TSK_not_available, 1 = TSK_available |
| `MO2_Sta_GRA` | 22 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | ACC, GRA | GRA/ACC status | 0 = ADR_GRA_off, 1 = gra_activated_lamp_on, 2 = gra_overdriven_lamp_on, 3 = ADR_gra_faulted |
| `TSK_Limiter_ausgewaehlt` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ACC, GRA | Speed limiter selected | 0 = no_limiter_selected, 1 = Limiter_Selected |
| `MO2_GRAregelt` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | GRA regulating to displayed speed | 0 = no, 1 = yes |
| `MO2_Sport_Error` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Sport mode error (Porsche Cayenne) | 0 = no_Failure, 1 = Failure |
| `MO2_OffRoad` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | OffRoad mode active | 0 = not_active, 1 = active |
| `MO2_RME_Gehalt` | 29 | 3 | Intel | U3 | 12.5 | 0 | 0 … 75 | % | Gateway | Rapeseed oil methyl ester content (diesel) | - |
| `MO2_GRA_Soll` | 32 | 8 | Intel | U8 | 1.28 | 0 | 0 … 325.12 | km/h | ACC, GRA | GRA target speed setpoint | - |
| `MO2_LL_Solldz` | 40 | 8 | Intel | U8 | 10 | 0 | 0 … 2540 | 1/min | Gateway | Idle target speed | - |
| `MO2_Begr_Mo` | 48 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | Gateway | Limiting torque - max possible at speed | - |
| `MO2_Mo_ZWR` | 56 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | Gateway | Minimum engine torque with ignition angle retardation | - |

<a id="msg-0x2a0"></a>
### 0x2A0 Bremse_4

Sender: XXX · DLC 3 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `ABS_Vorgabewert_mitte_Kupplungs` | 0 | 8 | Intel | U8 | 3 | -381 | -381 … 378 | Nm/min | XXX | - | - |
| `ABS_Vorgabewert_hinten_Kupplung` | 8 | 8 | Intel | U8 | 0.7874 | 0 | 0 … 100 | % | XXX | - | - |
| `Einheit_Kupplungssteifigkeit` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Bremse_4_1` | 17 | 7 | Intel | U7 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x2a8"></a>
### 0x2A8 Bremsbooster_1

Sender: XXX · DLC 3 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Checksumme_Booster_1` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zaehler_Booster_1` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Frei_Bremsbooster_1_1` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Bremseingriff_Fahrer` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Eingriff_Bremsbooster` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Bremsbooster_verf_gbar` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Status_Bremsbooster_Steuerung` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ADR_Relais_ge_ffnet` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Position_Standby` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Loseschalter_unplausibel_Boost` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag_Booster` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x2c0"></a>
### 0x2C0 Allrad_1

Sender: XXX · DLC 5 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Fehler_Allrad_Kupplung` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ubertemperaturschutz__Allrad_1_` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerstatus_Kupplungssteifigke` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kupplung_komplett_offen` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Notlauf` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Allrad_Warnlampe` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Geschwindigkeitsbegrenzung` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Einheit_der_Kupplungssteifigkei` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kupplungssteifigkeit_Mitte__Ist` | 8 | 8 | Intel | U8 | 3 | -381 | -381 … 378 | Nm/min | XXX | - | - |
| `PNG_Status_4_1` | 16 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `PNG_Anzeige_blinkend` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ganginfo__PNG_` | 20 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Schaltung_aktiv__Allrad_1_` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Schaltung_Vorwarnung` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Allrad_1_1` | 26 | 5 | Intel | U5 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag_Allrad_1` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kupplungssteifigkeit_Hinten__Is` | 32 | 8 | Intel | U8 | 0.7874 | 0 | 0 … 100 | % | XXX | - | - |

<a id="msg-0x320"></a>
### 0x320 Kombi_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Fahrertuer_4_1` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerstatus_Tank` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Oeldruck` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Dynamische_Oeldruckwarnung` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kuehlmittelmangel` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heissleuchten_Vorwarnung` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Tankwarnlampe` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Vorgluehlampe__Kombi_1_` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Bremsinfo` | 8 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ladekontroll_Lampe__Kombi_` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_im_Stellgliedtest` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Kombi_1_7` | 12 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Tankstop` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Tankinhalt` | 16 | 7 | Intel | U7 | 1 | 0 | 0 … 126 | l | XXX | - | - |
| `Tankwarnung` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Signalquelle_Geschwindigkeit_4_` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Geschwindigkeit__Kombi_1_` | 25 | 15 | Intel | U15 | 0.01 | 0 | 0 … 326 | km/h | XXX | - | - |
| `Frei_Kombi_1_2` | 40 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ADR_Summer_abgeschaltet` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Gesetzte_Zeitluecke__Kombi_1_` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Blinker_links_4_1` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Blinker_rechts_4_1` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Angezeigte_Geschwindigkeit` | 46 | 10 | Intel | U10 | 0.32 | 0 | 0 … 325 | km/h | XXX | - | - |
| `Frei_Kombi_1_3` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x343"></a>
### 0x343 RDK_Status

Sender: XXX · DLC 3 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `RKS_Reifen_VL` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Reifen_VR` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Reifen_HL` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Reifen_HR` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Reifen_RR` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Warnung_2` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Warnung_1` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Systemfehler` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Kalibrier_abgew` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Druckdiff_Vorn` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Druckdiff_Hinten` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Befuellung_RR_low` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Funkstoerung` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_System_Aus` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_KD_Fehler` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Lampe` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Ton` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Gong` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_RDK_Blinkbit` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `RKS_Teillast` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x360"></a>
### 0x360 ADR_System

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `S_Momentenanforderung_ADR` | 0 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99 | MDI | XXX | - | - |
| `S_Zaehler_ADR_1` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `S_Fehler_ADR_1__S_` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Status_ADR_1__S_` | 13 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Verhinderung_Schubabschaltung` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Momentenanforderung_Freigabe` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Synchronisation_Bremsbooster` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Gesetzte_Zeitluecke__ADR_1_` | 18 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `S_Objekt_erfasst` | 22 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Wunschgeschwindigkeit` | 24 | 8 | Intel | U8 | 1 | 0 | 0 … 254 | km/h | XXX | - | - |
| `S_Anzeige_Zeitluecke` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Anzeige_Prioritaet` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Schaltaufforderung` | 34 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Ansteuerung_Gong_1` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Ansteuerung_Gong_2` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Ansteuerung_optischer_Fahrerh` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Anzeige_Sensor_blind` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Bremseingriff_Fahrer` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Loeseschalter_unplausibel` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Eingriff_Bremsbooster` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Bremsbooster_Status` | 43 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_ADR_Relais_geoeffnet` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Fehlerspeichereintrag_Bremsbo` | 46 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Fehlerspeichereintrag_ADR` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Zeitluecke_gemessen` | 48 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `S_Frei_ADR_1_1` | 52 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `S_Checksumme_ADR_1` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x366"></a>
### 0x366 AWV

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | Vector__XXX | - | - |
| `COUNTER` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | Vector__XXX | - | - |
| `AWV_Text` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 14 | - | Vector__XXX | - | 0 = kein_Text, 1 = FrontAssist_aus, 2 = FrontAssist_startet, 3 = FrontAssist_Warnung, 4 = FrontAssist_Sens_reinig, 5 = FrontAssist_Failure, 6 = FrontAssist_Demo, 7 = Vorhalt, 8 = Bremsung_wird_gerade_durchgefuehrt, 9 = Sensor_not_verfuegbar, 10 = Sensor_reinigen, 11 = Service_notwendig_Failure, 12 = Funktion_vom_Fahrer_deactivated, 13 = Funktion_vom_Fahrer_activated, 14 = FrontAssist_zur_Zeit_not_verfuegbar__rev_Failure |
| `AWV_1_Freigabe` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = nicht_freigegeben, 1 = freigegeben |
| `AWV_1_Prefill` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = keine_Prefill_Anf, 1 = Prefill_Anf |
| `AWV_1_Parameter` | 18 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Vector__XXX | - | 0 = Defaultparametersatz, 1 = Par_leicht_erh_Empf, 2 = Par_erh_Empf, 3 = Par_hoechster_Empf |
| `AWV_only` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = ACC_und_AWV_verbaut, 1 = AWV_ohne_ACC_verbaut |
| `AWV_CityANB_Auspraegung` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = autom_Bremsung_im_ges_vBereich, 1 = autom_Bremsung_im_def_vBereich |
| `AWV_Halten` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = keine_Anforderung, 1 = Anforderung_das_Fzg_im_Stillstand_zu_halten |
| `ANB_Teilbremsung_Freigabe` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Teilbremsung_nicht_freigegeben, 1 = Teilbremsung_freigegeben |
| `AWV_2_Status` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Lampe_aus, 1 = Lampe_ein |
| `AWV_2_Fehler` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Lampe_aus, 1 = Lampe_ein |
| `AWV_2_SU_Warnzeit` | 26 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Vector__XXX | - | 0 = frueh, 1 = normal, 2 = spaet, 3 = adaptiv |
| `AWV_2_SU_Bremsruck` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Bremsruck_deaktiviert, 1 = Bremsruck_aktiviert |
| `AWV_2_SU_Gong` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Gong_deaktiviert, 1 = Gong_aktiviert |
| `AWV_2_SU_Lampe` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Lampe_deaktiviert, 1 = Lampe_aktiviert |
| `AWV_2_Umfeldwarn` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = keine_Warnung, 1 = Warnung |
| `AWV_2_Freigabe` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = keine_Ruckfreigabe, 1 = Ruckfreigabe |
| `AWV_2_Ruckprofil` | 33 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | Vector__XXX | - | 0 = kein_Ruck, 1 = Ruckprofil_1, 2 = Ruckprofil_2, 3 = Ruckprofil_3, 4 = Ruckprofil_4, 5 = Ruckprofil_5, 6 = not_erlaubt, 7 = not_erlaubt |
| `AWV_2_Warnton` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Aus, 1 = Ein |
| `AWV_2_Warnsymbol` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Aus, 1 = Ein |
| `AWV_Infoton` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Aus, 1 = Ein |
| `AWV_2_Gurtstraffer` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Gurt_not_straffen, 1 = Gurt_straffen |
| `AWV_Konfiguration_Menueanf` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Menue_deaktivieren, 1 = Menue_aktivieren |
| `AWV_Konfiguration_Vorw_Menueanf` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Menue_deaktivieren, 1 = Menue_aktivieren |
| `AWV_Konfiguration_Status` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = AWV_inaktiv, 1 = AWV_aktiv |
| `AWV_Konfiguration_Vorw_Status` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = AWV_Vorwarnung_inaktiv, 1 = AWV_Vorwarnung_aktiv |
| `AWV_2_Abstandswarnung` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = kein_Warnhinweis, 1 = Warnhinweis |
| `ANB_Zielbremsung_Freigabe` | 52 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Zielbremsung_nicht_freigegeben, 1 = Zielbremsung_freigegeben |
| `ANB_CM_Anforderung` | 53 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = keine_Anforderung, 1 = Anforderung_aktiv |
| `ANB_Ziel_Teilbrems_Verz_Anf` | 54 | 10 | Intel | U10 | 0.024 | -20.016 | 0 … 1023 | Unit_MeterPerSeconSquar | Vector__XXX | - | - |

<a id="msg-0x368"></a>
### 0x368 ACC_System

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `COUNTER` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `ACS_Sta_ADR` | 12 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | 2 = ADR_passiv, 0 = ADR_nicht_aktiv, 1 = ADR_aktiv, 3 = irrev_Fehler |
| `ACS_ADR_Schub` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 1 = Verz_begr_auf_Schub, 0 = Verz_nicht_begr_auf_Schub |
| `ACS_Schubabsch` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 1 = SA_nicht_zulaessig, 0 = SA_zulaessig |
| `ACS_StSt_Info` | 16 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | 3 = Systemfehler, 0 = Motorlauf_nn, 1 = Stoppverbot_Motoranlauf_nn, 2 = Motoranlauf_notwendig |
| `ACS_MomEingriff` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 1 = MomEingr_verhindern, 0 = keine_Beeinfl_MomEingr_Mot |
| `ACS_Typ_ACC` | 19 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | 0 = Basis_ACC, 1 = ACC_mit_FollowToStop, 3 = frei, 2 = frei |
| `ACS_FreigSollB` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Sollbeschl_nicht_freigeg, 1 = Sollbeschl_freigeg |
| `ACS_Sollbeschl` | 24 | 11 | Intel | U11 | 0.005 | -7.22 | -7.22 … 3.005 | Unit_MeterPerSeconSquar | XXX | - | 2046 = ADR_nicht_aktiv, 2047 = Fehler |
| `ACS_Anhaltewunsch` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = kein_Haltewunsch, 1 = Fzg_haelt_an |
| `ACS_Fehler` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 1 = Fehlerspeichereintrag, 0 = kein_Fehlerspeichereintrag |
| `ACS_zul_Regelabw` | 40 | 8 | Intel | U8 | 0.005 | 0 | 0 … 1.265 | Unit_MeterPerSeconSquar | XXX | - | 254 = ADR_nicht_aktiv, 255 = Fehler |
| `ACS_max_AendGrad` | 48 | 8 | Intel | U8 | 0.02 | 0 | 0.02 … 5.06 | Unit_MeterPerSeconSquar | XXX | - | 254 = Neutralwert, 0 = Neutralwert, 255 = Fehler |

<a id="msg-0x380"></a>
### 0x380 Motor_3

Sender: Motor · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

Motor message 3 - Contains accelerator pedal, temperatures, and control signals

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `MO3_Vorgluehen` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Pre-glow active (diesel engines) | 0 = no_Vorgluehen, 1 = Vorgluehen_active |
| `MO3_Prio_Dz` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Transmission | Engine speed priority: 0=request, 1=mandatory | 0 = Wunsch, 1 = Zwang |
| `MO_Schalter_StartStopp` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Start/stop deactivated by driver (LED on when 0) | 0 = StartStopp_ueber_Hauptschalter_deactivated_LED_an, 1 = StartStopp_ueber_Hauptschalter_activated_LED_aus |
| `MO3_Winterprg` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Transmission | Winter driving program active | 0 = not_active, 1 = active |
| `MO3_Sta_Pedal` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Accelerator pedal status: 0=OK, 1=substitute value | 0 = Fahrpedal_iO, 1 = Ersatzwert |
| `MO3_Sta_FPG` (Mux) | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ESP | Accelerator pedal gradient control bit (multiplexer for byte 8) | 0 = Gradient_not_bed, 1 = Gradient_bed |
| `MO3_Sign_FPG` (m1) | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ESP | Accelerator pedal gradient sign (only when MO3_Sta_FPG=1) | 0 = positives_Vorzeichen, 1 = negatives_Vorzeichen |
| `MO3_Sta_Temp` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Intake air temperature status: 0=OK, 1=substitute value | 0 = Geber_iO, 1 = Ersatzwert |
| `MO3_Offsentemp` | 8 | 8 | Intel | U8 | 0.75 | -48 | -48 … 142.5 | °C | Gateway | Intake air temperature | - |
| `MO3_Pedalwert` | 16 | 8 | Intel | U8 | 0.4 | 0 | 0 … 101.6 | % | Gateway | Accelerator pedal raw signal (0-101.6%) | - |
| `MO3_Rad_Wu_Mo` | 24 | 12 | Intel | U12 | 0.39 | 0 | 0 … 1596.66 | % | Transmission | Wheel torque request (12-bit value, 0-1596.66% MDI) | - |
| `MO3_Vorz_RadWu` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Transmission | Wheel torque sign: 0=positive, 1=negative | 0 = positives_Vorzeichen, 1 = negatives_Vorzeichen |
| `MO3_Freigabe_Segeln` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Transmission | Sailing operation enable for transmission | 0 = Segelbetrieb_not_freigegeben, 1 = Segelbetrieb_freigegeben |
| `MO_StartStopp_StoppVorbereitung` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Engine stop in preparation | 0 = Motorstopp_not_in_Vorbereitung, 1 = Motorstopp_in_Vorbereitung |
| `MO3_Dz_Beeinfl` | 40 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.45 | % | Transmission | Engine speed influence factor (0-100% interpolation) | - |
| `MO3_WunschDz` | 48 | 8 | Intel | U8 | 25 | 0 | 0 … 6350 | U/min | Transmission | Desired engine speed (when MO3_Prio_Dz=1) | - |
| `MO3_DKW` (m0) | 56 | 8 | Intel | U8 | 0.4 | 0 | 0 … 101.6 | % | Gateway | Throttle valve position when MO3_Sta_FPG=0 (0-101.6%) | - |
| `MO3_FPGradient` (m1) | 56 | 8 | Intel | U8 | 25 | 0 | 0 … 6350 | %/s | ESP | Accelerator pedal gradient when MO3_Sta_FPG=1 (0-6350%/s) | - |

<a id="msg-0x384"></a>
### 0x384 Motor_Momente

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Momentenangaben_ungenau__Moment` | 35 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x388"></a>
### 0x388 GRA

Sender: XXX · DLC 3 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Zaehler_GRA_alt` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `GRA_alt__ADR___Hauptschalter` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `GRA_alt__ADR___Tipschalter__Aus` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `GRA_alt__ADR___Tipschalter__Set` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `GRA_alt__ADR___Tipschalter__Wie` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `GRA_alt__ADR_verzoegern` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `GRA_alt__ADR_beschleunigen` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `GRA_alt__ADR_Bedienteil_Fehler` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_GRA_alt` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Checksumme_GRA_alt` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x38a"></a>
### 0x38A GRA_Neu

Sender: XXX · DLC 4 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `GRA_Hauptschalt` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Abbrechen` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Down_kurz` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Up_kurz` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Down_lang` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Up_lang` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Fehler_Bed` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Kodierinfo` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Neu_Setzen` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Recall` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Sender` | 18 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `COUNTER` | 20 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `GRA_Tip_Down` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Tip_Up` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Zeitluecke` | 26 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `GRA_Sta_Limiter` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Typ_Hauptschalt` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Sportschalter` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GRA_Fehler_Tip` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x390"></a>
### 0x390 Gate_Komf_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `GK1_Sta_RDK_Warn` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_Anhaen` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_Licht1` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_Licht3` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_Tuerkont` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_Li_vorn` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_SleepAckn` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_CharismaModus` (m1) | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `GK1_SamFktNr` (Mux) | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `GK1_Fa_Tuerkont` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Status of the driver's door rotary latch | - |
| `GK1_RueckfahrSch` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_ELV_verrieg` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_Kessy_2` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_Stdhzg` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_SH_Verbau` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_ParkFrontWi` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_KW_Warm` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BCM_Remotestart_Betrieb` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BSK_HL_geoeffnet` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Status of the rear left door rotary latch | - |
| `BSK_HR_geoeffnet` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Status of the rear right door rotary latch | - |
| `GK1_Rueckfahr` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_BrLi_links` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_BrLi_rechts` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_BrLi_mitte` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_BLS_ILM` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_EDC_ILM` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Blinker_li` | 34 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Blinker_re` | 35 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_def_P_verr` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_LS1_Fernlicht` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_Licht2` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_LSM` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Count_Anhaen` | 40 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `BSK_BT_geoeffnet` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Status of the passenger door rotary latch | - |
| `BSK_HD_Hauptraste` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | Status of trunk lid main detent | - |
| `GK1_BLS_AAG` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_EDC_AAG` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Anhaenger` | 46 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_BrLi_Anhaen` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Abblendlicht` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Fernlicht` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Wischer_vorn` | 50 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Sta_ILM_F_1` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Abbl_VL_def` | 52 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Abbl_VR_def` | 53 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Blink_Autob` | 54 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Warnblk_Status` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_SH_laeuft` | 56 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `SH1_ein_Wasserpumpe` | 57 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Nebel_ein` | 58 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Bremslicht` | 59 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_Anh_abgesteckt` | 60 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_AnhKonLamp` | 61 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LDS_Stellung_AFL` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK1_SH_Zusatzfkt` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x392"></a>
### 0x392 Gate_Komf_2

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `GK2_Sta_LSM` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Sta_Lichtsensor` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Sta_Licht1` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Sta_VSG` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Sta_Schluessel` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Sta_Profil` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Sta_Clima2` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Sta_BSG4` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Sta_Kessy_4` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BS4_Gleitende_Leuchtw_Anf` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BS4_GLW_Fernlicht_Anf` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Blk_L_Kontrolle` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Blk_R_Kontrolle` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_LS_KomFehler` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_LS_def` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Helligkeit` | 16 | 3 | Intel | U3 | 714.286 | 0 | 0 … 5000.002 | Unit_Lux | XXX | - | - |
| `GK2_VD_zu_ver` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_VD_entriegelt` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_VD_offen_ver` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Verdeck_Anf` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_VDKD_auf` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Schluessel` | 24 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `GK2_Hardtop` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_AFL_Schalter` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Nebelschluss` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_EM_LIN_ungueltig` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `GK2_Profil` | 32 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `GK2_Kl_StSt_Info` | 36 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `GK2_BSG_StSt_Info` | 38 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `GK2_BEM_P_Generator` | 40 | 8 | Intel | U8 | 50 | 0 | 0 … 12700 | Unit_Watt | XXX | - | - |
| `GK2_BEM_Abschaltstufen` | 48 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `GK2_BEM_DFM` | 51 | 5 | Intel | U5 | 3.225 | 0.025 | 0.025 … 100 | Unit_PerCent | XXX | - | - |
| `GK2_Kessy_StSt_Info` | 56 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `GK2_BEM_StSt_Info` | 58 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |

<a id="msg-0x3a0"></a>
### 0x3A0 Bremse_10

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `B10_Checksumme` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `B10_Zaehler` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `B10_QB_Wegimp_VL` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_QB_Wegimp_VR` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_QB_Wegimp_HL` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_QB_Wegimp_HR` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_Wegimp_VL` | 16 | 10 | Intel | U10 | 1 | 0 | 0 … 1000 | - | XXX | - | - |
| `B10_Wegimp_VR` | 26 | 10 | Intel | U10 | 1 | 0 | 0 … 1000 | - | XXX | - | - |
| `B10_Wegimp_HL` | 36 | 10 | Intel | U10 | 1 | 0 | 0 … 1000 | - | XXX | - | - |
| `B10_Wegimp_HR` | 46 | 10 | Intel | U10 | 1 | 0 | 0 … 1000 | - | XXX | - | - |
| `B10_QB_Fahrtr_VL` | 56 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_QB_Fahrtr_VR` | 57 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_QB_Fahrtr_HL` | 58 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_QB_Fahrtr_HR` | 59 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_Fahrtr_VL` | 60 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_Fahrtr_VR` | 61 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_Fahrtr_HL` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B10_Fahrtr_HR` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x3ba"></a>
### 0x3BA SWA_1

Sender: SWA · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `SWA_Textbits` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `SWA_Gong` | 16 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `SWA_Sta_passiv` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `SWA_Sta_aktiv` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `SWA_Infostufe_SWA_li` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `SWA_Warnung_SWA_li` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `SWA_Infostufe_SWA_re` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `SWA_Warnung_SWA_re` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `SWA_KD_Fehler` | 59 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x3d0"></a>
### 0x3D0 Lenkhilfe_1

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `LH1_Lastinfo` | 0 | 7 | Intel | U7 | 1 | 0 | 0 … 127 | A | XXX | - | - |
| `LH1_Fehler_LI` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_Sicherheitslamp` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_Fehlerlampe` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_Textbits` | 10 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `LH1_Akustiksign` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_SleepInd` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_Fehlereintr` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_EPS_Diagmode` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_gue_ECU_Temp` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_Kuehlung` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_Mode_Hybrid` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_NL_Untersp` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_NL_Uebersp` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_NL_Uebertemp` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_NL_Sensor` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_NL_CAN_SS` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_NL_ECU` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_NL_Motor` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_NL_LeistDichte` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH1_ULeistung` | 32 | 8 | Intel | U8 | 0.5 | 0 | 0 … 100 | % | XXX | - | - |
| `LH1_ECU_Temp` | 40 | 8 | Intel | U8 | 1 | -70 | -70 … 185 | °C | XXX | - | - |

<a id="msg-0x3d2"></a>
### 0x3D2 Lenkhilfe_2

Sender: XXX · DLC 7 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `COUNTER` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `LH2_Geradeaus` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH2_Sta_Charisma` | 13 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `LH2_Sta_HCA` | 16 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | 0 = disabled, 1 = initializing, 2 = fault, 3 = ready, 4 = rejected, 5 = active, 7 = active |
| `LH2_Ausg_LW1` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH2_Ausg_LW1_gue` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LH2_StatEPS_PLA` | 24 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `LH2_aktLenkeingriff` | 32 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `LH2_PLA_Err` | 48 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `LH2_PLA_Abbr` | 52 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |

<a id="msg-0x3d4"></a>
### 0x3D4 PLA_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `COUNTER` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `PL1_Status_EPS` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `PL1_ArcAngleReq` | 16 | 15 | Intel | U15 | 0.04375 | 0 | 0 … 1433.55625 | Unit_DegreeOfArc | XXX | - | - |
| `PL1_AngleReqSign` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PL1_Stat_PLA_ESP` | 32 | 4 | Intel | U4 | 1 | 0 | 0 … 10 | - | Vector__XXX | - | - |
| `PL1_Bremsmoment` | 40 | 13 | Intel | U13 | 4 | 0 | 0 … 32760 | Unit_NewtoMeter | Vector__XXX | - | - |
| `PL1_void` | 53 | 11 | Intel | U11 | 1 | 0 | 0 … 2047 | - | XXX | - | - |

<a id="msg-0x420"></a>
### 0x420 Kombi_2

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Fehlerstatus_Aussentemp__4_1` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | ambient temp error | - |
| `Fehlerstatus_Oeltemperatur_4_1` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | oil temp error | - |
| `Fehlerst__Kuehlmitteltemp__4_1` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | water temp error | - |
| `Anhaenger_erkannt` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Kombi_2_1` | 4 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag__Kombi_` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Aussentemperatur_gefiltert` | 8 | 8 | Intel | U8 | 0.5 | -50 | -50 … 77 | C | XXX | outside temp, filtered | - |
| `Aussentemp__ungefiltert_4_1__Ko` | 16 | 8 | Intel | U8 | 0.5 | -50 | -50 … 77 | C | XXX | - | - |
| `Oeltemperatur_4_1` | 24 | 8 | Intel | U8 | 1 | -60 | -60 … 194 | C | XXX | kombi oil temperature | - |
| `Kuehlmitteltemp__4_1__Kombi_2_` | 32 | 8 | Intel | U8 | 0.75 | -48 | -48 … 142.5 | C | XXX | kombi coolant temperature | - |
| `Klemme_58d__Kombi_2_` | 40 | 7 | Intel | U7 | 1 | 0 | 0 … 100 | % | XXX | - | - |
| `Fehlerstatus_Kl__58_d` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_58s__Kombi_2_` | 48 | 7 | Intel | U7 | 1 | 0 | 0 … 100 | % | XXX | - | - |
| `Fehlerstatus_Kl__58_s` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Kombi_2_2` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x440"></a>
### 0x440 Getriebe_1

Sender: Transmission · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

Getriebe 1 - Contains gear status, torque requests, and control signals

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `GE1_Schaltung` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Gear shift active: 0=no shift, 1=shift in progress | 0 = no_circuit, 1 = circuit_is_running |
| `GE1_Sta_Schutz` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Motor | Transmission/converter protection request (power reduction or ignition angle) | 0 = no_protection, 1 = protection_requested |
| `GE1_Klimakompr` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Motor | AC compressor off request | 0 = no_Anforderung, 1 = Kompressor_aus |
| `GE1_WK` | 3 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Gateway | Torque converter clutch status | 0 = WK_open, 1 = WK_regulated, 2 = WK_closed, 3 = Failure |
| `GE1_StSt_Info` | 5 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Gateway | Start/stop coordinator info | 0 = Engine_running_not_necessary_stop_release, 1 = Motor_start_not_mandatory_necessary_stop_prohibition, 2 = Motor_start_absolute_necessary_start_request, 3 = System_error |
| `GE1_EGS_Anf` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Motor | EGS request - byte 4 becomes valid when set | 0 = no_requirement, 1 = EGS_requirement |
| `GE1_Zielgang` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | Gateway | Target gear when shifting or current gear | 0 = P (Park) \| Disengaged, 1 = Gear 1 \| Engaged, 2 = Gear 2 \| Engaged, 3 = Gear 3 \| Engaged, 4 = Gear 4 \| Engaged, 5 = Gear 5 \| Engaged, 6 = 1m Gear, 7 = Gang_R, 8 = Gear 6 \| Engaged, 9 = Gear 7 \| Engaged, 10 = Gear 8 \| Engaged, 14 = Speed Not Defined, 15 = Failure |
| `GE1_Wahl_Pos` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | Gateway | Selector lever position / driving program | 8 = P, 7 = R, 6 = N, 5 = D, 9 = U, 12 = S, 14 = T, 10 = T, 11 = T |
| `GE1_Mrad_Mkurb` | 16 | 8 | Intel | U8 | 0.1 | 0 | 0 … 25.4 | - | Gateway | Wheel torque to crankshaft torque ratio (Mrad/Mcrank) | - |
| `GE1_Soll_Mo` | 24 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | Motor | Internal target engine torque request (0-99.06%) | - |
| `GE1_Fahrwistd` | 32 | 8 | Intel | U8 | 0.249 | -31.623 | -31.623 … 31.623 | - | Gateway | Driving resistance index (normalized % slope) | - |
| `GE1_Notlauf` | 40 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | Gateway | Emergency mode status | 0 = No Emergency, 1 = No Switching, 2 = Switch to Neutral, 3 = 50% Shifting Capacity, 4 = Driving Without UK, 5 = No Emeregency, 6 = No Emergency, 7 = No Emergency, Target Shifting Reached, 15 = Failure |
| `GE1_Kuehlung` | 44 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Gateway | Cooling request level | 0 = No Additional Cooling Requested, 1 = 20% Additional Fan Cooling Requested, 2 = 40% Additional Fan Cooling Requested, 3 = Maximum Additional Fan Cooling Requested |
| `GE1_Sta_OBD` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Motor | OBD status - MIL lamp control | 0 = MIL Off, 1 = MIL On |
| `GE1_LaunchControl` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | ESP | Launch control active | 0 = Launch Control Not Active, 1 = Launch Control Program Active |
| `GE1_Infobit` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Transmission type: 0=stepped automatic, 1=CVT | 0 = Standard Automatic, 1 = CVT |
| `GE1_SleepInd` | 50 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Sleep readiness after terminal 15 off | 0 = CAN is Required, 1 = Sleep Ready |
| `COUNTER` | 51 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | Gateway | - | - |
| `GE1_Fehlereintr` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Fault memory entry present | 0 = No Failure Memory Entry, 1 = Failure Memory Entry |
| `GE1_WaVerl_Mo` | 56 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99.06 | % | Motor | Transmission torque loss / clutch torque (0-99.06%) | - |

<a id="msg-0x448"></a>
### 0x448 Waehlhebel_1

Sender: XXX · DLC 4 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Shiftlock_Position` | 0 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Waehlhebel_Initialisierung` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Waehlhebelposition` | 4 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehler_Waehlhebel` | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Waehlhebel_Testergebnis` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `Zaehler_Waehlhebel_1` | 24 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | Counter Waehlhebel_1 | - |
| `Test_aktiv_Flag` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Waehlhebel_1_1` | 29 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x470"></a>
### 0x470 BSG_Kombi

Sender: XXX · DLC 5 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Blinker_links_Kontrollampe` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Blinker_rechts_Kontrollampe` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Anhaenger_Kontrollampe` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Warnblink_Mode` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `DWA_Akku` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Rueckfahrlicht` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_BSG_Kombi_1_1` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lade_Kontrollampe` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fahrertuer_geoeffnet` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Beifahrertuer_geoeffnet` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Tuer_hinten_links_geoeffnet` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Tuer_hinten_rechts_geoeffnet` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Motorhaube_geoeffnet` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heckdeckel_geoeffnet` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_BSG_Kombi_1_2` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Unterspannung` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_58d__BSG_Kombi_` | 16 | 7 | Intel | U7 | 1 | 0 | 0 … 100 | % | XXX | - | - |
| `Fehlerstatus_Kl__58d` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_58s__BSG_Kombi_` | 24 | 7 | Intel | U7 | 1 | 0 | 0 … 100 | % | XXX | - | - |
| `Fehlerstatus_Kl__58s` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerlampe_Lenkhilfe__BSG_Komb` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerlampe_Lenkhilfe_veraltet` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ruecksitzlehne_HL_verr__4_1` | 34 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ruecksitzlehne_HR_verr__4_1` | 35 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_BSG_Kombi_1_3` | 36 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x480"></a>
### 0x480 Motor_5

Sender: Motor · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

Motor message 5 - Contains engine type, consumption, lamps, and diagnostic info

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `MO5_max_Moment` (m0) | 0 | 6 | Intel | U6 | 10 | 0 | 0 … 630 | Nm | Gateway | Maximum torque when Mp_Code=0 (0-630 Nm) | - |
| `MO5_Drehzahl` (m1) | 0 | 6 | Intel | U6 | 100 | 0 | 0 … 6300 | U/min | Gateway | Engine speed at max torque when Mp_Code=1 (0-6300 RPM) | - |
| `MO5_Motortyp` (m2) | 0 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | Gateway | Engine type info when Mp_Code=2 (cylinder count, turbo, fuel type) | - |
| `MO5_Abgastyp` (m3) | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Exhaust type EOBD when Mp_Code=3 | 0 = no_EOBD, 1 = EOBD |
| `MO5_Abgastyp2` (m3) | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Exhaust type OBD when Mp_Code=3 | 0 = no_OBD, 1 = OBD |
| `MO5_Abgastyp3` (m3) | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | DPF installed when Mp_Code=3 | 0 = no_DPF_verbaut, 1 = DPF_verbaut |
| `OBD_Kaltstart_Denominator` (m3) | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Cold start denominator increment when Mp_Code=3 | 0 = Denominator_not_hochzaehlen, 1 = Bedingungen_zum_Inkr_des_Kaltstart_Denom_erf |
| `OBD_Minimum_Trip` (m3) | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Minimum normalized trip when Mp_Code=3 | 0 = no_Min_Normed_Trip, 1 = Min_Normed_Trip |
| `MO5_DPF_reg` (m3) | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | DPF regenerating when Mp_Code=3 | 0 = DPF_regeneriert_nicht, 1 = DPF_regeneriert |
| `MO5_Mp_Code` (Mux) | 6 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Gateway | Multiplex selector - switches every 4 transmissions | 0 = Max_Moment, 1 = Drehzahl, 2 = Motor_Typ, 3 = Abgastyp_od_norm_Verbrauch |
| `MO5_Vorgluehen` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Diesel pre-glow lamp | 0 = Lamp_off, 1 = Lamp_on |
| `MO5_E_Gas` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | E-Gas system lamp (petrol only) | 0 = Lamp_off, 1 = Lamp_on |
| `MO5_OBD_2` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | OBD/MIL lamp control | 0 = Lamp_off, 1 = Lamp_on |
| `MO5_Heissl` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Coolant overheat warning lamp | 0 = Lamp_off, 1 = Lamp_on |
| `MO5_KlimaKompr` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | AC compressor off request | 0 = no_Anforderung, 1 = Klimakompr_aus |
| `MO5_Feld_kuehl` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Map cooling installed and OK | 0 = no, 1 = yes |
| `MO5_KliKo_Red` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | AC compressor power reduction | 0 = no, 1 = yes |
| `MO5_Verbrauch` | 16 | 15 | Intel | U15 | 1 | 0 | 0 … 32767 | µl | KOMBI | Fuel consumption counter (15-bit, µl) | - |
| `MO5_UeberlVerb` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Consumption counter overflow | 0 = no_Ueberlauf, 1 = mindestens_einmal_uebergelaufen |
| `MO5_Luefter` | 32 | 8 | Intel | U8 | 0.4 | 0 | 0 … 101.6 | % | Gateway | Cooling fan PWM control (0-101.6%) | - |
| `MO5_HLeuchte` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Overheat pre-warning | 0 = no_Warnung, 1 = Vorwarnung |
| `MO5_PartikelLamp` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Particle filter lamp | 0 = Lamp_off, 1 = Lamp_on |
| `MO5_Sta_BKU` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Brake vacuum pump status (Porsche only) | 0 = iO_oder_not_verbaut, 1 = n_iO |
| `MO5_TypStartSteu` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Starter control type: 0=BSG/BCM, 1=MSG | 0 = Startersteuerung_BSG_BCM_KessyD1_ZAS, 1 = Startersteuerung_durch_MSG |
| `MO5_TDE_Lampe` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Fuel cap warning lamp (petrol only) | 0 = Lamp_off, 1 = Lamp_on |
| `MO5_TDE_Text` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Fuel cap text display (petrol only) | 0 = no_Text, 1 = Textanzeige_nach_ISO |
| `MO5_DZM_Daempf` | 46 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | RPM gauge damping control | 0 = normale_Daempfung, 1 = dynamische_Daempfung |
| `MO5_Interlock` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Interlock switch for manual transmission start | 0 = Interlock_not_betaetigt, 1 = Interlock_betaetigt |
| `MO5_Start` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Automatic start permission | 0 = Start_not_zulaessig, 1 = Startfreigabe |
| `MO5_Anlasser` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | Starter control permission | 0 = Anlasser_darf_angesteuert_werden, 1 = Anlasser_ausspuren_Ansteuerung_not_moeglich |
| `MO5_GRA_Hauptsch` | 50 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | GRA main switch / readiness | 0 = Off, 1 = On |
| `MO5_Momente` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Gateway | MDI torque values doubled | 0 = Einfach, 1 = Doppelt |
| `MO5_Motortext1` | 52 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Text: Engine workshop | 0 = no_Text, 1 = Text_1 |
| `MO5_Motortext2` | 53 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Text: Exhaust workshop (deprecated) | 0 = no_Text, 1 = Text_2 |
| `MO5_Motortext3` | 54 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Text: Fuel system fault | 0 = no_Text, 1 = Text_3 |
| `MO5_Motortext4` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | KOMBI | Text: Particle filter needs regeneration | 0 = no_Text, 1 = Text_4 |
| `CHECKSUM` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | Gateway | Checksum with final value 0x0 | - |

<a id="msg-0x488"></a>
### 0x488 Motor_6

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Checksumme_Motor_6` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | Checksum Motor_6 | - |
| `Sollmoment_f_r_Getriebe` | 8 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99 | MDI | XXX | Target torque for gearbox | - |
| `Istmoment_f_r_Getriebe` | 16 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99 | MDI | XXX | Actual torque for gear | - |
| `Hoeheninfo__Motor_6_` | 24 | 8 | Intel | U8 | 0.00787 | 0 | 0 … 2 | - | XXX | Altitude Correction | - |
| `GRA_Sollbeschleunigung` | 32 | 8 | Intel | U8 | 0.024 | -3.984 | -3.984 … 2.112 | m/s2 | XXX | GRA target acceleration | - |
| `Ruckmeldung_Momenten` | 40 | 8 | Intel | U8 | 0.39 | 0 | 0 … 100 | - | XXX | Feedback torque-integral gear intervention | - |
| `Frei_Motor_6_3` | 48 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `GRA_Bremseingriff_Freigabe` | 56 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ltemperaturschutz` | 57 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Motor_6_4` | 58 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zaehler_Motor_6` | 60 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | Counter Motor_6 | - |

<a id="msg-0x497"></a>
### 0x497 Parkhilfe_01

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `PH_Abschaltursache` | 13 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `PH_Opt_Anzeige_V_ein` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Opt_Anzeige_H_ein` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Opt_Anz_V_Hindernis` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Opt_Anz_H_Hindernis` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Tongeber_V_aktiv` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Tongeber_H_aktiv` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Tongeber_mute` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Anf_Audioabsenkung` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Frequenz_hinten` | 32 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `PH_Lautstaerke_hinten` | 36 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `PH_Frequenz_vorn` | 40 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `PH_Lautstaerke_vorn` | 44 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `PH_Trigger_Bildaufschaltung` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_StartStopp_Info` | 49 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `PH_Aufbauten_erk` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_BerErk_vorn` | 52 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `PH_BerErk_hinten` | 54 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `PH_defekt` | 56 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_gestoert` | 57 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `PH_Systemzustand` | 58 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `PH_Display_Kundenwunsch` | 61 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `PH_KD_Fehler` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x4a0"></a>
### 0x4A0 Bremse_3

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Frei_Bremse_3_1` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Radgeschw__VL_4_1` | 1 | 15 | Intel | U15 | 0.01 | 0 | 0 … 326 | km/h | XXX | - | - |
| `Frei_Bremse_3_2` | 16 | 1 | Intel | Bool | 0.01 | 0 | 0 … 325 | km/h | XXX | - | - |
| `Radgeschw__VR_4_1` | 17 | 15 | Intel | U15 | 0.01 | 0 | 0 … 326 | km/h | XXX | - | - |
| `Frei_Bremse_3_3` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Radgeschw__HL_4_1` | 33 | 15 | Intel | U15 | 0.01 | 0 | 0 … 326 | km/h | XXX | - | - |
| `Frei_Bremse_3_4` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Radgeschw__HR_4_1` | 49 | 15 | Intel | U15 | 0.01 | 0 | 0 … 326 | km/h | XXX | - | - |

<a id="msg-0x4a8"></a>
### 0x4A8 Bremse_5

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `BR5_Giergeschw` | 0 | 14 | Intel | U14 | 0.01 | 0 | 0 … 100 | Grad/sec | XXX | - | - |
| `BR5_Sta_Gierrate` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Vorzeichen` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Bremsdruck` | 16 | 12 | Intel | U12 | 0.1 | 0 | 0 … 250 | bar | XXX | - | - |
| `BR5_Stillstand` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Druckvalid` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Sta_Druck` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Sign_Druck` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `ESP_Rollenmodus_Deactiveieren` | 34 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `ESP_Stat_FallBack_eBKV` | 35 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `ESP_Anforderung_EPB` | 36 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `ESP_Autohold_active` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `ESP_Autohold_Standby` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Anhi_Sta` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Anhi_akt` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_v_Ueberw` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Bremslicht` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Notbremsung` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_Fahrer_tritt_ZBR_Schw` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_AWV2_Bremsruck` | 46 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_AWV2_Fehler` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_ZT_Rueckk_Umsetz` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_ANB_CM_Rueckk_Umsetz` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_HDC_bereit` | 50 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `BR5_ECD_Lampe` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `COUNTER` | 52 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `CHECKSUM` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x500"></a>
### 0x500 MSG_2

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Ram_Adresse_1` | 0 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `RAM_Adresse_2` | 16 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `RAM_Adresse_3` | 32 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `RAM_Adresse_4` | 48 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x512"></a>
### 0x512 PSG_2

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `RAM_Inhalt_1` | 0 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `RAM_Inhalt_2` | 16 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `RAM_Inhalt_3` | 32 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `RAM_Inhalt_4` | 48 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x514"></a>
### 0x514 NOX_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `NOX_Signal` | 0 | 11 | Intel | U11 | 1 | 0 | 0 … 2047 | ppm | XXX | - | - |
| `Frei_NOX1_1` | 11 | 5 | Intel | U5 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Sauerstoff_linear` | 16 | 11 | Intel | U11 | 1 | 0 | 0 … 2047 | 1000/Lambd | XXX | - | - |
| `Frei_NOX1_2` | 27 | 5 | Intel | U5 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Sauerstoff_binaer` | 32 | 11 | Intel | U11 | 1 | -200 | -200 … 1847 | mV | XXX | - | - |
| `Frei_NOX1_3` | 43 | 5 | Intel | U5 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Offsetkorrektur_NOX` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Sondenheizung_NOX` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `IP0` | 50 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `IP1` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `IP2` | 52 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_NOX1_4` | 53 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `OBD_fuer_NOX` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x520"></a>
### 0x520 Kombi_3

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Kombi_Verbauliste_Motor` (m3) | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Multiplex_Derivat` (m1) | 0 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Multiplex_Sprachvariante` (m0) | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Multiplex_Reifenumfang` (m2) | 0 | 12 | Intel | U12 | 1 | 0 | 0 … 4095 | mm | XXX | - | - |
| `Kombi_Verbauliste_ABS` (m3) | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_Klima` (m3) | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_Airbag` (m3) | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Multiplex_Generation` (m1) | 4 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_ACC` (m3) | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_Bordnetz` (m3) | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_Allrad` (m3) | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_Lenkwinkel` (m3) | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Multiplex_Baureihe` (m1) | 8 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Multiplex_Laendervariante` (m0) | 8 | 6 | Intel | U6 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_Dieselpumpe` (m3) | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_Lenkhilfe` (m3) | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Verbauliste_Niveauregulie` (m3) | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Multiplex_Marke` (m1) | 11 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Multiplex_Code` (Mux) | 14 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Schluesselinfo` | 16 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Frei_Kombi_3_1` | 20 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Standzeit` | 24 | 15 | Intel | U15 | 4 | 0 | 0 … 131068 | sec | XXX | - | - |
| `Fehlerstatus_Standzeit` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kilometerstand` | 40 | 20 | Intel | U20 | 1 | 0 | 0 … 1000000 | km | XXX | - | - |
| `Frei_Kombi_3_2` | 60 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x52c"></a>
### 0x52C ADR_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Momentenanforderung_ADR` | 0 | 8 | Intel | U8 | 0.39 | 0 | 0 … 99 | MDI | XXX | - | - |
| `Zaehler_ADR_1` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Fehler_ADR_1` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Status_ADR_1` | 13 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verhinderung_Schubabschaltung` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Momentenanforderung_Freigabe` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Synchronisation_Bremsbooster` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Gesetzte_Zeitluecke__ADR_1_` | 18 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Objekt_erfasst` | 22 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Wunschgeschwindigkeit` | 24 | 8 | Intel | U8 | 1 | 0 | 0 … 254 | km/h | XXX | - | - |
| `Anzeige_Zeitluecke` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Anzeige_Prioritaet` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Schaltaufforderung` | 34 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ansteuerung_Gong_1` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ansteuerung_Gong_2` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ansteuerung_optischer_Fahrerhin` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Anzeige_Sensor_blind` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Reserviert_ADR_1_1` | 40 | 7 | Intel | U7 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag_ADR` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zeitluecke_gemessen` | 48 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Frei_ADR_1_5` | 52 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Checksumme_ADR_1` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x52e"></a>
### 0x52E Diag_Lenkhilfe

Sender: XXX · DLC 3 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Befehl_Fehlerspeicher_loeschen` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Befehl_Kennliniencodierung_Lenk` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Befehl_Sensorcodierung_Lenkhilf` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Multiplex_Signal__Diag_` | 13 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `Werkstattcode__Diag_` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x530"></a>
### 0x530 Navigation_1

Sender: XXX · DLC 7 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Gierrate` | 0 | 14 | Intel | U14 | 0.01 | 0 | 0 … 100 | deg/sek | XXX | - | - |
| `Gierratenfehler` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Vorzeichen_Gierrate______` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Laenderkennung` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Strassentyp` | 24 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Anzahl_Fahrbahnen__0_ist_unguel` | 28 | 3 | Intel | U3 | 1 | 0 | 1 … 7 | - | XXX | - | - |
| `Fehler_Navigation` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Vorausliegender_Kurvenverlauf` | 32 | 7 | Intel | U7 | 50 | 0 | 0 … 6350 | m | XXX | - | - |
| `Voarusliegende_Kurvenrichtung` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Entfernung_bis_Kurvenanfang` | 40 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | m | XXX | - | - |
| `Entfernung_bis_Kreuzung` | 48 | 6 | Intel | U6 | 5 | 0 | 0 … 315 | m | XXX | - | - |
| `Kreuzungstyp` | 54 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |

<a id="msg-0x534"></a>
### 0x534 Sitz_info

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Checksumme_Sitzinfo` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zaehler_Sitzinfo` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Fahrersitz_im_vorderen_Drittel` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Positionserkennung_Fahrersitz_u` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Beifahrersitz_im_vorderen_Dritt` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Positionserkennung_Beifahrersit` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x538"></a>
### 0x538 Wischer_1

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Frontwischer__eingeschaltet` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ansteuerung_Wascher_Front` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ansteuerung_Frontwischer_Normal` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ansteuerung_Frontwischer_Schnel` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Parklage_Frontwischer` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Status_Waschduesenheizung` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Wischer_1_1` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Blockierung_Frontwischer_erkann` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Scheibenwischer_Heck_eingeschal` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ansteuerung_Wascher_Heck` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ansteuerung_Scheibenwischer_Hec` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag__Wischer_` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Wischer_1_2` | 12 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Blockierung_Heckwischer_erkannt` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x53c"></a>
### 0x53C Fahrwerk_1

Sender: XXX · DLC 1 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Ansteuererung_Fahrzeugniveau` | 0 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Einstellung_Fahrwerkdaempfung_4` | 4 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `Frei_Fahrwerk_1_1` | 6 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Fahrwerk_1_2` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x540"></a>
### 0x540 Getriebe_2

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `LFR_Adaption_Freigabeflag` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Schubabschaltunterstuetzung` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ecomatic__4_1_` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zwischengasflag` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zahler_Getriebe_2` | 4 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | Counter Getriebe_2 | - |
| `Leerlaufsolldrehzahl__Getriebe` | 8 | 8 | Intel | U8 | 10 | 0 | 0 … 2540 | U/min | XXX | - | - |
| `Gradientenbegrenzung` | 16 | 8 | Intel | U8 | 10 | 0 | 0 … 2540 | Nm/s | XXX | - | - |
| `Synchronisations_Wunschdrehzahl` | 24 | 8 | Intel | U8 | 25 | 0 | 0 … 6350 | U/min | XXX | - | - |
| `invertierte_Synchronisations_Wu` | 32 | 8 | Intel | U8 | 25 | 0 | 0 … 6350 | U/min | XXX | - | - |
| `Synchronisationszeit` | 40 | 8 | Intel | U8 | 20 | 0 | 0 … 5080 | ms | XXX | - | - |
| `Hochschaltlampe` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | Upshift Flag | - |
| `Starter_wird_angesteuert` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Gong` | 50 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Unterdrueckung_von_Warnungen` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Shift_Lock_Lampe` | 52 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ECO_Anzeige__4_1_` | 53 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Anforderung_Kriechadaption` | 54 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerlampe_f_r_Kupplung_bei_VL` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ganganzeige_Kombi___Getriebe_Va` | 56 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `eingelegte_Fahrstufe` | 60 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x548"></a>
### 0x548 Getriebe_4

Sender: XXX · DLC 3 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Shiftlock_Getriebe_4` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Handbremserinnerung_s_Lampe` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Testfreigabeflag` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Getriebe_4_1` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Waehlhebelausleuchtung` | 4 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Testparameter_1` | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `Testparameter_2` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |

<a id="msg-0x550"></a>
### 0x550 Airbag_2

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Checksumme_Airbag_2__reserviert` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Belegungserkennung_Beifahrersit` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Belegungserkennung_hinten_links` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Belegungserkennung_hinten_recht` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Belegungserkennung_hinten_mitte` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `OOP_Fahrer` | 12 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `OOP_Beifahrer` | 14 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x56a"></a>
### 0x56A ACC_GRA_Anzeige

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `ACA_StaACC` | 8 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | 6 = ACC_rev_aus, 0 = Hauptschalter_aus, 4 = ACC_im_Hintergrund, 3 = ACC_aktiv, 1 = Reserve, 2 = ACC_passiv, 7 = ACC_irrev_aus, 5 = frei |
| `ACA_ID_StaACC` | 11 | 5 | Intel | U5 | 1 | 0 | 0 … 31 | - | XXX | - | 0 = keine_Anzeige |
| `ACA_Fahrerhinw` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 1 = Ein, 0 = Aus |
| `ACA_AnzDisplay` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 1 = Anzeige_erw, 0 = Anzeige_nicht_erw |
| `ACA_Zeitluecke` | 18 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | 3 = Zeitluecke3, 10 = Zeitluecke10, 4 = Zeitluecke4, 14 = Zeitluecke14, 11 = Zeitluecke11, 2 = Zeitluecke2, 13 = Zeitluecke13, 9 = Zeitluecke9, 1 = Zeitluecke1, 8 = Zeitluecke8, 5 = Zeitluecke5, 15 = Zeitluecke15, 0 = nicht_definiert, 12 = Zeitluecke12, 6 = Zeitluecke6, 7 = Zeitluecke7 |
| `ACA_V_Wunsch` | 24 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | Unit_KiloMeterPerHour | XXX | 255=unset | 255 = kein_Wert_im_Speicher |
| `ACA_kmh_mph` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = km_h, 1 = mph |
| `ACA_Akustik1` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = kein_Gong, 1 = Gong |
| `ACA_Akustik2` | 34 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = kein_Summer, 1 = Summer |
| `ACA_PrioDisp` | 35 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | 1 = mittlere_Prio, 3 = keine_Anzeige_Anf, 0 = hohe_Prio, 2 = niedrige_Prio |
| `ACA_gemZeitl` | 40 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | 6 = Zeitluecke6, 2 = Zeitluecke2, 7 = Zeitluecke7, 13 = Zeitluecke13, 11 = Zeitluecke11, 4 = Zeitluecke4, 8 = Zeitluecke8, 12 = Zeitluecke12, 10 = Zeitluecke10, 0 = Kein_Objekt_erfasst, 1 = Zeitluecke1, 3 = Zeitluecke3, 9 = Zeitluecke9, 15 = Zeitluecke15, 14 = Zeitluecke14, 5 = Zeitluecke5 |
| `ACA_ACC_Verz` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = ACC_verzoegert_nicht, 1 = ACC_verzoegert |
| `ACA_StaGRA` | 48 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | 3 = GRA_aktiv, 4 = GRA_uebertreten, 2 = GRA_passiv, 0 = Hauptschalter_aus, 6 = frei, 7 = GRA_Fehler, 1 = Reserve, 5 = frei |
| `ACA_ID_StaGRA` | 51 | 5 | Intel | U5 | 1 | 0 | 0 … 31 | - | XXX | - | 0 = keine_Anzeige |
| `ACA_Codierung` | 56 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = ACC, 1 = GRA |
| `ACA_Tachokranz` | 57 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = nicht_beleuchtet, 1 = beleuchtet |
| `ACA_Aend_Zeitluecke` | 58 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 1 = Anzeige_angef, 0 = keine_Anzeige |
| `COUNTER` | 60 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |

<a id="msg-0x570"></a>
### 0x570 BSG_Last

Sender: XXX · DLC 4 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `ZAS_Klemme_S` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ZAS_Klemme_15` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ZAS_Klemme_X` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ZAS_Klemme_50` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_BSG_Last_1_1` | 4 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_L` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `LL_Drehzahlanhebung` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zustand_der_Bordnetzbatterie` | 9 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zustand_der_Starterbatterie` | 11 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag__BSG_Last` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Leuchtweitenregulierung` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Motorhaubenkontakt` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Batteriespannung_Bordnetzbatter` | 16 | 8 | Intel | U8 | 0.05 | 5 | 5 … 17.7 | V | XXX | - | - |
| `Heizbare_Heckscheibe_abschalten` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heizbare_Frontscheibe_abschalte` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heizbare_Aussenspiegel_abschalt` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heizbare_Sitze_abschalten` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkradheizung_abschalten` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Wischwasserheizung_abschalten` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Sitzbelueftung_abschalten` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klimaanlage_abschalten` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x572"></a>
### 0x572 ZAS_1

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `S_Kontakt__Schl_ssel_steckt_` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_15__Z_ndung_ein_` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_X__Startvorgang_` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_50__Starten_` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_P__Parklichtstellung_` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_ZAS_1_1` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_15_SV` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_ZAS_1_2` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_ZAS_1_3` | 8 | 7 | Intel | U7 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag__ZAS_` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x578"></a>
### 0x578 BatMan_1

Sender: XXX · DLC 1 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Startmodus` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ladung_Starterbatterie` | 1 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Umschaltrelais_Bordnetzbatterie` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zustand_Starterleitung` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Messung_Starterleitung` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Leistungsrelais` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag__BatMan_` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x580"></a>
### 0x580 Motor_Flexia

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Multiplex_Schalter_Motor_Flexia` (Mux) | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zaehler_Motor_Flexia` | 1 | 3 | Intel | U3 | 1 | 0 | 0 … 15 | - | XXX | Counter Motor_Flexia | - |
| `Driving_Cycle` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Warm_Up_Cycle` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Motor_Flexia_1` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verbrennungsart` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | Type of combustion | - |
| `Russindex` | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 254 | - | XXX | - | - |
| `Verschleissindex` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 254 | - | XXX | - | - |
| `Oelniveauschwelle` (m1) | 24 | 8 | Intel | U8 | 0.25 | 0 | 0 … 63.75 | cm | XXX | - | - |
| `Drehzahl_MaxNorm` (m0) | 24 | 8 | Intel | U8 | 100 | 0 | 0 … 25500 | U/min | XXX | RPM of maximum torque | - |
| `Max_Drehmoment` (m0) | 32 | 8 | Intel | U8 | 10 | 0 | 0 … 2550 | Nm | XXX | Maximum torque | - |
| `Normierter_Verbrauch` (m1) | 32 | 8 | Intel | U8 | 10 | 0 | 0 … 2550 | l/Zyl. | XXX | - | - |
| `Hersteller_Code` (m1) | 40 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Motorleistung` (m0) | 40 | 9 | Intel | U9 | 1 | 0 | 0 … 512 | KW | XXX | Maximum engine power | - |
| `Bewertungsfaktor_Verschleissind` (m1) | 44 | 6 | Intel | U6 | 0.1 | 0 | 0 … 6.3 | - | XXX | - | - |
| `Anzahl_Ventile` (m0) | 49 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | Vent./Zyl. | XXX | Number of valves | - |
| `Bewertungsfaktor_Russindex_Turb` (m1) | 50 | 6 | Intel | U6 | 0.1 | 0 | 0 … 6.3 | - | XXX | - | - |
| `Anzahl_Zylinder` (m0) | 52 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | Vent./Zyl. | XXX | Number of cylinders | - |
| `Hubraum` (m0) | 56 | 7 | Intel | U7 | 0.1 | 0 | 0 … 12.7 | l | XXX | Displacement | - |
| `Steigung_der_Befuellungskennlin` (m1) | 56 | 8 | Intel | U8 | 0.001 | 0 | 0 … 0.255 | l/mm | XXX | - | - |
| `Ansaugsystem` (m0) | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | Induction System | - |

<a id="msg-0x588"></a>
### 0x588 Motor_7

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Leerlauf_Solldrehzahl_auf_Max_W` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Geschwindigkegrenzung_aktivierb` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Statusbit_Geschwindikeitsbegren` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag__Motor_7_` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Motor_7_1` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `PTC___Gluehstifte_ausgeschaltet` | 5 | 3 | Intel | U3 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klemme_DFM` | 8 | 8 | Intel | U8 | 0.4 | 0 | 0 … 101.6 | % | XXX | - | - |
| `Hoeheninfo__Motor_7_` | 16 | 8 | Intel | U8 | 0.00787 | 0 | 0 … 2 | - | XXX | Altitude correction factor | - |
| `Motordrehzahlgradient` | 24 | 7 | Intel | U7 | 1 | 0 | 0 … 126 | U/min | XXX | Engine speed gradient | - |
| `Vorzeichen_Motordrehzahlgradien` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Ladedruck` | 32 | 8 | Intel | U8 | 0.01 | 0 | 0 … 2.54 | bar | XXX | Boost Pressure | - |
| `Frei_Motor_7_3` | 40 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Oltemperatur` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | Oil temperature | - |

<a id="msg-0x590"></a>
### 0x590 Niveau_1

Sender: XXX · DLC 6 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Checksumme_Niveau_1` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zaehler_Niveau_1` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | Zaehler | XXX | - | - |
| `Frei_Niveau_1_1` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Warnlampe_Niveau_1` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ESP_Beeinflussung` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `MSG_Einschraenkung` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Niveaustati` | 16 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Zwischenniveau` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Parkniveau` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Taster_Niveau` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Niveau_1_5` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kompressorlauf_in_Kuerze` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verstellung_aktiv` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Anhebung_Fahrzeug` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Absenkung_Fahrzeug` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verstellung_VR` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verstellung_VL` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verstellung_HR` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verstellung_HL` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Textbits__Niveau_1_` | 32 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fahrzeugart_Niveau` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Reserve_Fahrzeugart` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Systemstatus__Niveau_1_` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag__Niveau_1` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Beladungszustand` | 40 | 8 | Intel | U8 | 1 | 0 | 0 … 253 | Zuladung | XXX | - | - |

<a id="msg-0x598"></a>
### 0x598 Daempfer_1

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Status_Daempferregelung_4_1` | 0 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `Frei_Daempfer_1_2` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Status_CDC_Taster` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Daempfer_1_3` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Systemstatus__Daempfer_1_` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeicherbit__Daempfer_1_` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Textbits_Daempfer` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Daempfer_1_4` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x5a0"></a>
### 0x5A0 Bremse_2

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Timer` (m1) | 0 | 8 | Intel | U8 | 0.04 | 0 | 0 … 10.2 | usec | XXX | - | - |
| `Querbeschleunigung` (m0) | 0 | 8 | Intel | U8 | 0.01 | -1.27 | -1.27 … 1.27 | g | XXX | - | - |
| `Querbeschl__TimerTic` (Mux) | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `mittlere_Raddrehzahl__Bremse_2` | 9 | 15 | Intel | U15 | 0.002 | 0 | 0 … 65.278 | U/sec | XXX | - | - |
| `Zeitstempel` | 24 | 16 | Intel | U16 | 1 | 0 | 0 … 65535 | tics | XXX | - | - |
| `Wegimpulse_Vorderachse` | 40 | 11 | Intel | U11 | 1 | 0 | 0 … 2047 | - | XXX | - | - |
| `Wegimpulszaehlerstatus` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag_Bremse` | 52 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Warnlampe_DDS` | 53 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Bremse_2_5` | 54 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerstatus_Wegimpulse_4_1` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Impulszahl` | 56 | 6 | Intel | U6 | 1 | 0 | 0 … 63 | - | XXX | - | - |
| `Frei_Bremse_2_2` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `gemessene_Querbeschleunigung` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x5b7"></a>
### 0x5B7 Bremse_11

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CHECKSUM` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `COUNTER` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `B11_HydHalten` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B11_Br_StSt_Info` | 14 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `B11_OBD_Nib_VL` | 16 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `B11_OBD_Nib_VR` | 20 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `B11_OBD_Nib_HL` | 24 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `B11_OBD_Nib_HR` | 28 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `B11_EPB_Steller_akt` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `B11_EPB_Steller_gue` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x5b8"></a>
### 0x5B8 WFS_1

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `WFS_LED` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_WFS_1_1` | 1 | 7 | Intel | U7 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `WFS_Textbits` | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x5be"></a>
### 0x5BE LDW_Status

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `LDW_Lernmodus_rechts` | 0 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `LDW_Lernmodus_links` | 2 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `LDW_Lernmodus` | 9 | 3 | Intel | U3 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `LDW_Textbits` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `LDW_Gong` | 16 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |
| `LDW_Kameratyp` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LDW_Lampe_gelb` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LDW_Lampe_gruen` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LDW_SW_Warnung_links` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LDW_SW_Warnung_rechts` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LDW_KD_Fehler` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LDW_DLC` | 24 | 8 | Intel | U8 | 0.01 | -1.25 | -1.25 … 1.25 | - | XXX | - | - |
| `LDW_TLC` | 32 | 5 | Intel | U5 | 0.1 | 0 | 0 … 3 | - | XXX | - | - |
| `LDW_Seite_DLCTLC` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `LDW_Frueh_Spaet` | 38 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | - |

<a id="msg-0x5c0"></a>
### 0x5C0 EPB_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `COUNTER` | 0 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `EP1_Fehler_Sta` | 4 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | Vector__XXX | - | 0 = volle_Funktion, 1 = linke_Seite_fehlerhaft, 2 = rechte_Seite_fehlerhaft, 3 = beide_Seiten_fehlerhaft |
| `EP1_Sta_EPB` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Bremse_geoeffnet, 1 = Bremse_geschlossen |
| `EP1_Sta_Schalter` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = volle_Funktion, 1 = Schalter_ausser_Funktion |
| `EP1_Spannkraft` | 8 | 5 | Intel | U5 | 1 | 0 | 0 … 30 | Unit_KiloNewto | XXX | - | 31 = Fehler |
| `EP1_Schalterinfo` | 13 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | 0 = keine_Fahreranforderung, 1 = Fahreranforderung_oeffnen, 2 = Fahreranforderung_schliessen, 3 = Schalterfehler |
| `EP1_Sta_NWS` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = volle_Funktion, 1 = keine_Funktion |
| `EP1_Neig_winkel` | 16 | 8 | Intel | U8 | 1 | -128 | -128 … 127 | Unit_PerCentOfForceOfGravi | XXX | - | - |
| `EP1_Verzoegerung` | 24 | 8 | Intel | U8 | 0.048 | -7.968 | -7.968 … 4.224 | Unit_MeterPerSeconSquar | XXX | - | - |
| `EP1_Fehlereintr` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = kein_Fehlerspeichereintrag, 1 = Fehlerspeichereintrag |
| `EP1_Freigabe_Ver` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Verzoegerungsanf_nicht_freigegeb, 1 = Verzoegerungsanf_freigegeben |
| `EP1_AutoHold_zul` | 34 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Pers_nicht_zulaessig, 1 = Pers_zulaessig |
| `EP1_AutoHold_aktiv` | 35 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = nein, 1 = ja |
| `EP1_SleepInd` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = CAN_wird_benoetigt, 1 = Sleep_bereit |
| `EP1_Status_Kl_15` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Kl_15_aus, 1 = Kl_15_ein |
| `EP1_Lampe_AutoP` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Lampe_aus, 1 = Lampe_ein |
| `EP1_Bremslicht` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Aus, 1 = Ein |
| `EP1_Warnton1` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Aus, 1 = Ein |
| `EP1_Warnton2` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Aus, 1 = Ein |
| `EP1_AnfShLock` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Aus, 1 = Ein |
| `EPB_Autoholdlampe` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Lampe_aus, 1 = Lampe_ein |
| `EP1_QualNeigWi` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = gueltiger_Wert, 1 = Ersatz_Init_oder_Fehlerwert |
| `EP1_KuppModBer` | 45 | 2 | Intel | U2 | 1 | 0 | 0 … 3 | - | XXX | - | 0 = Kuppsensor_aus_Modbereich, 1 = Kupplsensor_im_Modbereich, 2 = Sensorsignal_ungenau, 3 = Sensor_defekt |
| `EP1_HydrHalten` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Fzg_nicht_hydr_geh, 1 = Fzg_hydr_geh |
| `EP1_Fkt_Lampe` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = Lampe_aus, 1 = Lampe_ein |
| `EP1_Warnton` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Warnton_aus, 1 = Warnton_an |
| `EP1_Fehler_BKL` | 50 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | Vector__XXX | - | 0 = BKL_aus, 1 = BKL_an |
| `EP1_Fehler_gelb` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | 0 = Lampe_aus, 1 = Lampe_ein |
| `EP1__Text` | 52 | 4 | Intel | U4 | 1 | 0 | 0 … 8 | - | Vector__XXX | - | 0 = kein_Text, 1 = Text_1, 2 = Text_2, 3 = Text_3, 4 = Text_4, 5 = Text_5, 6 = reserviert, 7 = reserviert, 8 = Text_8 |
| `CHECKSUM` | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |

<a id="msg-0x5d0"></a>
### 0x5D0 Systeminfo_1

Sender: XXX · DLC 6 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `CAN_Extern_zugeschaltet` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `CAN_Antrieb_in_Diagnose` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Sleep_CAN_Komfort` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `CAN_Komfort_in_Diagnose` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Sleep_CAN_Infotainment` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `CAN_Infotainment_in_Diagnose` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `CAN_Infotainment_verbaut` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verbauinformation_gueltig` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fahrzeug_Klasse` | 8 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Fahrzeug_Marke_2` | 12 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Fahrzeug_Derivat` | 16 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Fahrzeug_Generation` | 20 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `Fahrzeug_Index` | 24 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Rechtslenker` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Viertuerer` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Systeminfo_1_6` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `CAN_Stand_4_1_Komfort_Daten_Neb` | 32 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `CAN_Stand_4_1_Komfort_Daten_Hau` | 36 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `CAN_Stand_4_1_Antrieb_Daten_Neb` | 40 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |
| `CAN_Stand_4_1_Antrieb_Daten_Hau` | 44 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | - | XXX | - | - |

<a id="msg-0x5d2"></a>
### 0x5D2 Ident

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `IDT_Mux` (Mux) | 0 | 2 | Intel | U2 | 1 | 0 | 0 … 2 | - | XXX | - | - |
| `IDT_Geheimnis_1` (m0) | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_4` (m1) | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_11` (m2) | 8 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_Geheimnis_2` (m0) | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_5` (m1) | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_12` (m2) | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_Geheimnis_3` (m0) | 24 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_6` (m1) | 24 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_13` (m2) | 24 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_Geheimnis_4` (m0) | 32 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_7` (m1) | 32 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_14` (m2) | 32 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_1` (m0) | 40 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_8` (m1) | 40 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_15` (m2) | 40 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_2` (m0) | 48 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_9` (m1) | 48 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_16` (m2) | 48 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_3` (m0) | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_10` (m1) | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |
| `IDT_VIN_17` (m2) | 56 | 8 | Intel | U8 | 1 | 0 | 0 … 255 | - | XXX | - | - |

<a id="msg-0x5d8"></a>
### 0x5D8 Verbauliste_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Motorsteuergeraet` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Getriebesteuergeraet` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ABS` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Climatronic_Antrieb` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Airbag` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ADR` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `BSG_Antrieb` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Allrad` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkwinkel` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Einspritzpumpe` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombi_Antrieb` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Bremsbooster` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `EZS___Kessy__Antrieb_` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Niveauregulierung` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Daempfer_SG` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Batteriemanager` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Waehlhebel` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Stabi_Entkopplung` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Verbauliste_1_8` | 19 | 5 | Intel | U5 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `BSG_Komfort` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `ZKE` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `TSG_FT` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `TSG_BT` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `TSG_HL` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `TSG_HR` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Memory` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Dachmodul` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombiinstrument_Komfort` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Reifendruck` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenksaeulenmodul` | 34 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Gateway` | 35 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klimasteuergeraet_Komfort` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Einparkhilfe` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Verbauliste_1_9` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Standheizung` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Verdecksteuergeraet` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `EZS___Kessy__Komfort_` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Wischermodul` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Energiemanagement` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Anh_ngersteuergeraet` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Sitzmemory_Beifahrer` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Sitzmemory_hinten` | 46 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Memory_hinter_Fahrer` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Assistenzfahrlicht` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Tankgeber` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Verbauliste_1_10` | 50 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Klimabedienteil_HL` | 52 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Verbauliste_1_11` | 53 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `MMI_hinten` | 54 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `MMI_vorne` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Radio` | 56 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Telefon` | 57 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Navigation` | 58 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Telematik` | 59 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Spracheingabe` | 60 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `CD_Wechsler` | 61 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `DSP` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `TV_Tuner` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x5dc"></a>
### 0x5DC Soll_Verbauliste_neu

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `VL1_Motor_SG` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Getr_SG` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_ABS` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Kombi` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_LSM` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Airbag` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Lenkhilfe` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_dyn_LWR` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_res_08` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Allrad` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_ADR` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_ADR_getrennt` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_EPB` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_res_13` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Daempfer` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Quersperre` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_MotorSlave` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_SWA` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_HCA` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_RKA_Plus` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_PLA` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_WFS_KBI` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Kombi_KBI` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Soll_eq_Ist` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_BSG_Komf` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_ZKE` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_TSG_FT` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_TSG_BT` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_TSG_HL` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_TSG_HR` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Memory` | 30 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Dachmodul_K` | 31 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Zentralelektrik_II` | 32 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_RDK` | 33 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Lenksaeule` | 34 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Gateway` | 35 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Clima_Komf` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Einparkhilfe` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_PTC_Heizung` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Standheiz` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Verdeck` | 40 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_RSE_I` | 41 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_res_42` | 42 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_MDI_I` | 43 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Anhaenger` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | SWA | - | - |
| `VL1_Memory_BF` | 45 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Easy_Entry_VF` | 46 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Easy_Entry_VB` | 47 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Heckdeckel` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Rearview` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Sonderfzg_SG` | 50 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Tastenmodul` | 51 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Kompass` | 52 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_WFS_K` | 53 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_GSM_Pager` | 54 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_InfoElektronik` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_DSP` | 56 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_DAB` | 57 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Telematik` | 58 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Navigation` | 59 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_TV_Tuner` | 60 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Neigungsmodul_I` | 61 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Radio` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `VL1_Telefon` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x5de"></a>
### 0x5DE Lenkhilfe_Fehler

Sender: XXX · DLC 7 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Spannung_Kl_30_zu_gro_` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Spannung_Kl_30_zu_gro__sp_` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Spannung_Kl_30_zu_klein` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Spannung_Kl_30_zu_klein_sp_` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Lenkhilfe_1_5` | 4 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Spannung_Kl_15_zu_klein` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Spannung_Kl_15_zu_klein_sp_` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhifesensor_Ks_nach_Masse` | 8 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfesensor_Ks_nach_Masse_s` | 9 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfesensor_Unterbrechung` | 10 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfesensor_Unterbrechung_s` | 11 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfesensor_defekt` | 12 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfesensor_defekt_sp_` | 13 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfesensor_keine_Kommunik_` | 14 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfesensor_k__Kommunik__sp` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Betrieb_unpl__Signal` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Betrieb_unpl__Sig__sp` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Temperaturschutz` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Temperaturschutz_sp_` | 19 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Spannung_Ks__nach__` | 20 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Spannung_Ks__nach___s` | 21 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Spannung_Ks_nach_Mass` | 22 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Spannung_Ks__nach_Mas` | 23 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Steuergeraet_keine_CAN_Kommunik` | 24 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Steuergeraet_k__CAN_Kommunik__S` | 25 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombiinstrument_k__CAN_Kommunik` | 26 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombiinstr__k__CAN_Kommunik__sp` | 27 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Geber_f__Lenkwinkel_k__CAN_Komm` | 28 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Geber_f__Lenkwi__k__CAN_Komm_sp` | 29 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Lenkhilfe_1_4` | 30 | 2 | Intel | U2 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Lenkhilfe_1_3` | 32 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Lenkhilfe_Steuergeraet_defekt` | 36 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Steuerger__Speicher_auslesen` | 37 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kombiinstr__Speicher_auslesen` | 38 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Geber_f__Lenkw__Speicher_ausles` | 39 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kennliniencodierung_Lenkhilfe` | 40 | 4 | Intel | U4 | 1 | 0 | 0 … 15 | Kennlinie | XXX | - | - |
| `Sensorcodierung_Lenkhilfe` | 44 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Multiplex_Signal` | 45 | 3 | Intel | U3 | 1 | 0 | 0 … 7 | - | XXX | - | - |
| `Werkstattcode` | 48 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x5e0"></a>
### 0x5E0 Klima_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Drehzahlanhebung` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fahrerwunsch_Zuheizer` | 1 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heizbare_Heckscheibe` | 2 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heizbare_Frontscheibe` | 3 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kompressorzustand__4_1_` | 4 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Keine_Heizleistg_gewuenscht_4_1` | 5 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kompressormoment_veraltet_4_1` | 6 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kaeltemitteldruck_veraltet` | 7 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Aussentemp__ungef__4_1__Klima_1` | 8 | 8 | Intel | U8 | 0.5 | -50 | -50 … 77 | C | XXX | - | - |
| `Klimadrucksignal__Klima_1_` | 16 | 8 | Intel | U8 | 0.2 | 0 | 0 … 50.8 | bar | XXX | - | - |
| `Kompressorlast` | 24 | 8 | Intel | U8 | 0.25 | 0 | 0 … 63.5 | Nm | XXX | - | - |
| `Geblaeselast_4_1` | 32 | 8 | Intel | U8 | 0.4 | 0 | 0 … 101.6 | % | XXX | - | - |
| `Kuehlerluefteransteuerung__Klim` | 40 | 8 | Intel | U8 | 0.4 | 0 | 0 … 101.6 | % | XXX | - | - |
| `Temperatureinheit` | 48 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `AC_Schalter` | 49 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Klima_1_5` | 50 | 5 | Intel | U5 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Fehlerspeichereintrag__Klima_` | 55 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Aussentemp__ungef__Sto_f__4_1` | 56 | 8 | Intel | U8 | 0.5 | -50 | -50 … 77 | C | XXX | - | - |

<a id="msg-0x60e"></a>
### 0x60E Einheiten_1

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `MFA_v_Einheit_02` | 0 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |

<a id="msg-0x700"></a>
### 0x700 MSG_3

Sender: XXX · DLC 3 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Lage_des_OT_Impuls` | 0 | 16 | Intel | U16 | 0.01172 | -384 | -384 … 384 | KW | XXX | - | - |
| `MSG_Konfiguration` | 16 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x704"></a>
### 0x704 Motor_NOX

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Abgasdruck_NOX` | 0 | 8 | Intel | U8 | 5 | 600 | 600 … 1870 | mbar | XXX | - | - |
| `Abgastemperatur_NOX` | 8 | 8 | Intel | U8 | 5 | -40 | -40 … 1230 | C | XXX | - | - |
| `Betriebsbereich` | 16 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Offsetkorrektur_moeglich` | 17 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Heizleistungsanforderung` | 18 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Motor_NOX_1_1` | 19 | 5 | Intel | U5 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_Motor_NOX_1_2` | 24 | 40 | Intel | U40 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x712"></a>
### 0x712 PSG_3

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Selbsttestergebnis` | 0 | 16 | Intel | U16 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x7c0"></a>
### 0x7C0 LWS_Kalibrierung

Sender: XXX · DLC 2 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Command_Dode_Word` | 0 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_LWS_Kalibrierung_1_2` | 4 | 4 | Intel | U4 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `LWS_Identifier` | 8 | 7 | Intel | U7 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Frei_LWS_Kalibrierung_1_1` | 15 | 1 | Intel | Bool | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x7c2"></a>
### 0x7C2 Lenkwinkel_Init

Sender: XXX · DLC 4 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `Identiifier_f_r_LWS_Init` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 0 | - | XXX | - | - |
| `Kodierbytes` | 8 | 24 | Intel | U24 | 1 | 0 | 0 … 0 | - | XXX | - | - |

<a id="msg-0x7d0"></a>
### 0x7D0 Diagnose_1

Sender: XXX · DLC 8 · Zyklus - · [zur Übersicht](#übersicht)

| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |
| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| `DI1_VerlernZaehl` | 0 | 8 | Intel | U8 | 1 | 0 | 0 … 254 | - | XXX | - | - |
| `DI1_km_Stand` | 8 | 20 | Intel | U20 | 1 | 0 | 0 … 1048575 | Unit_KiloMeter | XXX | - | - |
| `DI1_Jahr` | 28 | 7 | Intel | U7 | 1 | 2000 | 2000 … 2127 | Unit_Year | XXX | - | - |
| `DI1_Monat` | 35 | 4 | Intel | U4 | 1 | 0 | 1 … 12 | Unit_Month | XXX | - | - |
| `DI1_Tag` | 39 | 5 | Intel | U5 | 1 | 0 | 0 … 31 | Unit_Day | XXX | - | - |
| `DI1_Stunde` | 44 | 5 | Intel | U5 | 1 | 0 | 0 … 23 | Unit_Hours | XXX | - | - |
| `DI1_Minute` | 49 | 6 | Intel | U6 | 1 | 0 | 0 … 59 | Unit_Minut | XXX | - | - |
| `DI1_Sekunde` | 55 | 6 | Intel | U6 | 1 | 0 | 0 … 59 | Unit_Secon | XXX | - | - |
| `DI1_KM_Stand_alt` | 62 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
| `DI1_Zeit_alt` | 63 | 1 | Intel | Bool | 1 | 0 | 0 … 1 | - | XXX | - | - |
