# Signalliste für Touch-UI und TCU (Arbeitsstand 2026-11-09)

Arbeitsliste für die Entscheidung, welche Werte die PWA zeigt und welche die VCU an die TCU weitergibt. Bitlage und Skalierung stehen in [can_signals.md](can_signals.md) und den DBC-Dateien; hier nur Quelle, Stand und Vorschlag. Alle Spalten „Vorschlag“ sind **nicht entschieden**.

Legende Stand: **API** = heute in `/api/live` (Name), **UI** = heute in der PWA genutzt, **–** = fehlt.
Legende Vorschlag: **H** Startseite, **E** Energiefluss, **A** Akku, **L** Laden, **M** Antrieb, **B** Bordnetz/Klima, **F** Fahrdynamik/Stoppuhr, **D** nur Diagnose/Engineering, **T** an TCU.

## 1. Fahrzeug-CAN (986, can1)

Mitschnitt `can_logs/986_log_stand.csv` (Stand, Originalfahrzeug): 0x0C0 (5,6 ms), 0x1A0, 0x2A8, 0x510, 0x520 (je 11 ms), 0x4A1 (55 ms) sowie die DME-Botschaften 0x280/0x289/0x298/0x4E0/0x7A0 (10 ms). **0x440 (PRND vom Getriebesteuergerät), 0x4F8 und 0x560 kommen im Mitschnitt nicht vor** – offen, ob das Fahrzeug ein Tiptronic-Getriebesteuergerät hat.

| Signal | ID | Einheit | API heute | UI heute | Vorschlag |
|---|---|---|---|---|---|
| Fahrzeuggeschwindigkeit (Referenz) | 0x1A0 | km/h | `vref_kmh` | UI | H, F, **T** |
| Geschwindigkeit Kombi | 0x510 | km/h | `vehicle_speed_510` | UI | D (Abgleich) |
| Radgeschwindigkeiten VL/VR/HL/HR | 0x2A8 | km/h | `w*_legacy_kmh` | – | F (Schlupf), D |
| ABS/ASR/MSR/EDS/ESP-Eingriff | 0x1A0 | bool | `abs`, `tcs`, `msr`, `abd`, `fdr` | UI (abs, tcs) | F |
| Warnlampen ABS/Bremse/PSM | 0x1A0 | bool | `abs_lamp`, `brake_lamp`, `psm_button_lamp`, `fdr_lamp*` | – | H (Warnsymbol), **T** |
| Bremslichtschalter | 0x1A0 | bool | `brake_sw` | – | D |
| Handbremse | 0x1A0 | bool | `handbrake` | – | H (Hinweis), **T** |
| Querbeschleunigung | 0x1A0 | g | `lat_g` | UI | F |
| Längsbeschleunigung | 0x4A1 | m/s² | `acceleration` | UI | F |
| Gierrate | 0x4A1 | °/s | `yaw_rate` | UI | F |
| Fahrer bremst | 0x4A1 | bool | `driver_braking` | – | F, D |
| Lenkwinkel, LWS OK | 0x0C0 | ° | `lenkwinkel_rb`, `lws_ok_rb` | UI | F |
| Blinker links/rechts | 0x510 | bool | `indicator_left/right` | UI | H |
| Tempomat-, Lenkradtasten | 0x510 | bool | `tempomat_button`, `steeringwheel_button_*` | – | Bedienung der UI? (offen) |
| Außentemperatur | 0x520 | °C | `ambient_c` | UI | H, B, **T** |
| Lichtdimmer | 0x520 | – | `light_dimmer` | UI | UI-Helligkeit/Nachtmodus |
| Öldruck | 0x520 | bar | `oil_pressure_bar` | UI | entfällt (kein Verbrenner) |
| PRND | 0x440 | – | `prnd_pos` | UI | H, **T** – nur wenn 0x440 existiert |
| Kilometerstand | – | km | – | – | **T** gewünscht, auf CAN nicht bekannt (offen) |

Die VCU sendet selbst 0x280/0x289/0x298/0x4E0/0x7A0 (Drehzahl, Kühlmitteltemperatur, Warnlampen, Verbrauch). Diese Werte sind Ausgaben und für die UI nur als Diagnose interessant (Seite Kombiinstrument, D).

## 2. Antriebsumrichter Tesla SDU (OpenInverter stm32-sine, can2)

Mitschnitt `can_logs/20260419_fahrt_tesla.csv`: 0x500–0x505 je 100 ms; **0x506–0x508 nicht im Mitschnitt** (digitale Eingänge, Umdrehungen, Uptime).

| Signal | ID | Einheit | API heute | UI heute | Vorschlag |
|---|---|---|---|---|---|
| DC-Spannung `udc` | 0x500 | V | `sdu_udc` | UI | M, E |
| DC-Strom `idc` | 0x500 | A | `sdu_idc` | UI | M, E |
| Antriebsleistung `udc·idc` | abgeleitet | kW | – | (UI rechnet) | H, E, M, **T** |
| Motordrehzahl `speed` | 0x501 | rpm | `sdu_speed` | UI | M |
| Frequenz, AC-Spannung, Strom­grenze | 0x501 | Hz/V/A | `sdu_fstat`, `sdu_uac`, `sdu_ilmax` | UI | D |
| Phasenströme, RMS, boost, fweak | 0x500/0x502 | A/– | `sdu_il*`, `sdu_boostcalc`, `sdu_fweakcalc` | UI | D |
| Pedal `pot`, `pot2`, `potnom` | 0x503 | dig/% | `sdu_pot*` | UI | M (Pedalbalken), D |
| `amp`, `angle` | 0x503 | – | `sdu_amp`, `sdu_angle` | UI | D |
| Rekuperation `regenpreset` | 0x504 | % | `sdu_regenpreset` | UI | M |
| Fahrtrichtung `seldir`/`rotordir` | 0x504 | enum | `sdu_seldir`, `sdu_rotordir` | UI | H (D/N/R), **T** |
| Kühlkörpertemperatur `tmphs` | 0x504 | °C | `sdu_tmphs` | UI | M, B, **T** |
| Motortemperatur `tmpm` | 0x504 | °C | `sdu_tmpm` | UI | M, B, **T** |
| 12-V-Spannung `uaux` | 0x504 | V | `sdu_uaux` | UI | B, **T** |
| Betriebsart `opmode` | 0x505 | enum | `sdu_opmode` | UI | M, **T** |
| Letzter Fehler `lasterr` | 0x505 | enum | `sdu_lasterr` | UI | H (Warnung), M, **T** |
| `status`, `canio`, `pwmio`, `cpuload` | 0x505 | Bits | `sdu_status`, … | UI | D |
| Digitale Eingänge | 0x506/0x507 | enum | `sdu_din_*` | UI | D |
| Tempomatdrehzahl | 0x507 | rpm | `sdu_cruisespeed` | UI | D |
| Umdrehungen, Uptime | 0x508 | –/s | `sdu_turns`, `sdu_uptime*` | UI | D |

Doppelte bzw. unklare Einträge in der Signaltabelle: `sdu_regenpresent`, `sdu_regenpresent32`, `sdu_seldir43`, `sdu_temp_heatsink`, `sdu_temp_heatsink74`, `sdu_din_ocur51` – Kandidaten zum Aufräumen.

## 3. Batterie / Zombie-Slave (CAN0, TWAI)

Quelle: `Modellfan/Zombie-Slave`, `documentation/ZombieSlave_teensybms_dcdc_mvcu.dbc` und `docs/mVCU_Integration.md` (mVCU = diese VCU). In der VCU ist davon **noch nichts dekodiert** (`decodeBattery()` leer, VCU-REQ-018). Die TeensyBMS- und Zombie-Slave-Botschaften tragen 4-Bit-Zähler und CRC (STM32-CRC, Low-Byte über Byte 0–6). Offen: ob TeensyBMS, Zombie-Slave, Tesla-DCDC und VCU-CAN0 wirklich ein Bus sind.

### 3.1 Direkt auf CAN

| Signal | ID | Sender | Einheit | API heute | Vorschlag |
|---|---|---|---|---|---|
| Packspannung | 0x41A | TeensyBMS | V | `battery_pack_voltage_v` (leer) | A, E, **T** |
| Packstrom | 0x41A | TeensyBMS | A | `battery_pack_current_a` (leer) | A, E, **T** |
| min./max. Zellspannung | 0x41A | TeensyBMS | V | – | A |
| min./max. Zelltemperatur | 0x41B | TeensyBMS | °C | – | A, **T** (batteryTemperature) |
| Zellspannungsdifferenz, Balancing-Spannung | 0x41B | TeensyBMS | mV/V | – | A |
| Packleistung | 0x41B | TeensyBMS | W | – | H, E, **T** |
| max. Entlade-/Ladestrom | 0x41C | TeensyBMS | A | – | A, M (Leistungsgrenze) |
| Schützstatus | 0x41C | TeensyBMS | enum | – | H, A, **T** |
| BMS-Fehlerflags | 0x41C | TeensyBMS | Bits | – | H (Warnung), A, **T** |
| SoC | 0x41D | TeensyBMS | % | `battery_soc_pct` (leer) | H, A, L, **T** |
| SoH | 0x41D | TeensyBMS | % | – | A, **T** |
| Balancing-Status, BMS-Zustand | 0x41D | TeensyBMS | enum | – | A |
| mittlere Leistung | 0x41E | TeensyBMS | kW | – | H (Verbrauch), **T** |
| Restzeit | 0x41E | TeensyBMS | s | – | L, **T** (remainingTime) |
| Restenergie | 0x41E | TeensyBMS | kWh | – | H (Reichweite), A, **T** |
| Fahrzeugzustand (SLEEP … DRIVE, CHARGE, ERROR) | 0x437 | Zombie-Slave | enum | – | H, **T** (vehicleReady, Schlafsteuerung) |
| HV-Anforderung, Zwangsabschaltung | 0x437 | Zombie-Slave | bool | – | D |
| aktuelle Ladeleistung (AC-Eingang geschätzt) | 0x438 | Zombie-Slave | W | – | L, E, **T** (chargingPower) |
| max. Ladeleistung | 0x438 | Zombie-Slave | W | – | L |
| Heizung aktiv, Schütz, Fehler, Thermoschalter | 0x439 | Zombie-Slave | bool | – | B, **T** (Klima) |
| DCDC Ausgangsspannung/-strom, Eingangsleistung | 0x210 | Tesla-DCDC | V/A/W | – | B, E |
| DCDC Kühlmitteltemperatur | 0x210 | Tesla-DCDC | °C | – | B |
| DCDC Fehlerbits (12 Stück) | 0x210 | Tesla-DCDC | bool | – | B (Sammelwarnung), D |

**Steuerbotschaft der VCU:** 0x43A `Heater_CloseRequest` (mVCU → Zombie-Slave, 4-Bit-Zähler + CRC, Zeitlimit 500 ms). In der VCU nicht umgesetzt. Ein Heizungsschalter in der UI wäre neues Systemverhalten (`/re-refine`).

### 3.2 Nur intern im Zombie-Slave (heute nicht auf CAN)

Diese Werte bräuchten eine Erweiterung des Zombie-Slave (eigene Botschaft oder libopeninv-CAN-Map) oder SDO-Abfragen:

- **Laden (VW-MLB-Lader):** Steckerstatus, Ladeanforderung, Lader-Ist-Modus, AC-Netzspannung, HV-Ist-Spannung/-Strom, Ladertemperatur, Abregelgründe (Temperatur, Buchsentemperatur), Stecker verriegelt, CP-/PP-Erkennung (Stromlimit), Warn-/Fehlerzustand, LED-Zustand. → für Seite **L** und TCU (connectorConnected, Ladezustand) nötig.
- **Bordnetz:** 12-V-Batteriespannung (`LVDU_12v_battery_voltage`), Zündung, Ready-Eingang.
- **Kühlung/Hilfsaggregate:** Kühlmittelpumpe (Drehzahl, Fehler), Tesla-Ventil (Ist/Soll), Vakuumpumpe (an/aus, Unterdruck zu gering), EPS-Zustand.
- **Lader-Energie:** keine Energiezähler; Ladeenergie einer Sitzung müsste die VCU aus 0x438 aufsummieren.

## 4. Was die TCU braucht

Quelle: `ESP32S3-TCU/specs/001-vehicle-companion-spec/data-model.md` und `data/can/eboxster-demo.dbc.json`. Die TCU dekodiert beliebige Botschaften über ihre `can_map.json`; was die VCU sendet, ist also frei festlegbar.

| TCU-Wert | Beste Quelle | Muss die VCU senden? |
|---|---|---|
| soc, packVoltage, packCurrent | TeensyBMS 0x41A/0x41D | nein, falls TCU am CAN0 hängt |
| batteryTemperature | TeensyBMS 0x41B | nein (wie oben) |
| auxiliaryVoltage | SDU `uaux` (can2) bzw. DCDC 0x210 | **ja** (SDU-Bus sieht die TCU nicht) |
| vehicleReady / Schlafzustand | Zombie-Slave 0x437 `LVDU_VehicleState` | nein (als `state_source` der TCU geeignet) |
| speed | Fahrzeug-CAN 0x1A0 | **ja** |
| odometer | nicht verfügbar | offen (VCU könnte aus der Geschwindigkeit aufsummieren, nicht eichfähig) |
| connectorConnected, Ladezustand | MLB-Lader im Zombie-Slave | Zombie-Slave müsste senden (3.2) |
| chargingPower | Zombie-Slave 0x438 | nein |
| energy (kWh je Ladung) | nicht verfügbar | offen (VCU oder TCU integriert 0x438) |
| remainingTime | TeensyBMS 0x41E | nein |
| Klima/Heizung | Zombie-Slave 0x439 | nein |
| Engineering: Motor-/Umrichtertemperatur, `opmode`, `lasterr`, Fahrtrichtung, Antriebsleistung, Warnlampen, Außentemperatur | can1/can2 | **ja** |

**Vorschlag (nicht entschieden):** Die VCU sendet auf CAN0 zwei eigene Botschaften (z. B. 100 ms bzw. 1 s) mit Geschwindigkeit, Fahrtrichtung/PRND, Antriebsleistung, Motor- und Umrichtertemperatur, `opmode`, `lasterr`, 12-V-Spannung, Außentemperatur und einem Gültigkeitsbit je Wert (VCU-REQ-013). Nie auf dem Fahrzeug-CAN. IDs, Zyklus, Zähler/CRC sind eine Entscheidung für `DECISIONS.md`.

**Konflikt:** Die Demo-Map der TCU nutzt 0x200/0x201 (Empfang) und **0x210** (`WakeCommand`, Senden). 0x210 ist auf dem Zombie-Slave-Bus die Statusbotschaft des Tesla-DCDC. Hängt die TCU an diesem Bus, muss die TCU-ID verschoben werden.

## 5. Befunde nebenbei

- Die PWA sucht den SoC unter `battery_soc`, `soc`, `hv_soc` …, die Firmware liefert `battery_soc_pct`; ebenso `plug_connected`/`charging_active` gegen `battery_plug_connected`/`battery_charging_active`. Die UI würde den SoC also auch mit funktionierendem BMS-Decoder nicht anzeigen.
- Die Signaltabelle führt manche Werte mehrfach (`rpm`, `set_rpm`, `engine_speed_rpm`; `coolant_c`, `set_coolant`, `motor2_coolant_temperature`).

## 6. Offene Entscheidungen

1. Hängen TeensyBMS, Zombie-Slave, Tesla-DCDC, TCU und VCU-CAN0 an einem gemeinsamen Bus?
2. Hat das Fahrzeug ein Getriebesteuergerät (0x440)? Sonst PRND aus SDU `seldir`.
3. Welche Werte aus 3.2 soll der Zombie-Slave zusätzlich senden (vor allem Laden und 12 V)?
4. Welche Werte und welche Botschaften sendet die VCU für die TCU (Abschnitt 4)?
5. Seitenaufteilung der UI (Vorschlagsbuchstaben in den Tabellen).
