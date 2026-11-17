# Signalliste VCU, Touch-UI und TCU (Stand 2026-11-14)

Alle Werte, die die VCU empfängt, sendet, berechnet oder braucht, je Bus und Botschaft, eine Zeile je Signal, mit Ort in der Touch-UI und Bedarf der TCU. **Zusammenfassung, keine Masterquelle:** Bitlage, Skalierung, Zyklus und Wertetabellen stehen in der Masterquelle, die bei jedem Bus verlinkt ist. Bei Widerspruch gilt die Masterquelle.

Vollständigkeit: alle 280 Einträge der Firmware-Signaltabelle (`src/services/signal_table.cpp`) und die übrigen Felder von `/api/live` (`src/services/webinterface.cpp`). Die Engineering-Seite **[E]Signals** (VCU-REQ-022) zeigt alle Werte live.

Arbeitsweise: Spalte **UI Vorschlag** je Signal festlegen (heute **nicht entschieden**); danach wird die PWA umgestellt.

## 1. Legende und Masterquellen

### Referenzen (Masterquellen)

| Kürzel | Datei | Inhalt |
|---|---|---|
| CS | [can_messages.md](can_messages.md) | maßgeblich für CAN2 (OpenInverter) und CAN0-Kurzfassung |
| P986 | [porsche_986_messages.md](porsche_986_messages.md) | Spiegel der 986-DBC, offene Punkte des Reverse Engineering |
| DBC986 | [porsche_986_996_compiled_working.dbc](porsche_986_996_compiled_working.dbc), Hintergrund [vw_pq_messages.md](vw_pq_messages.md), [vw_pq.dbc](vw_pq.dbc) | 986/996- und PQ-Botschaften |
| ZS | [ZombieSlave_teensybms_dcdc_mvcu.dbc](ZombieSlave_teensybms_dcdc_mvcu.dbc) (Kopie von Zombie-Slave 205cc7c; Master: [Zombie-Slave/documentation](https://github.com/Modellfan/Zombie-Slave/blob/master/documentation/ZombieSlave_teensybms_dcdc_mvcu.dbc)), Kurzfassung in CS, Abschnitt CAN0 | CAN0: TeensyBMS, Zombie-Slave, Tesla-DC/DC, mVCU-Heizung |
| BMS | [TeensyVCU/documentation/CAN_Message_Packing.md](https://github.com/Modellfan/TeensyVCU/blob/master/documentation/CAN_Message_Packing.md) | Packing und CRC der TeensyBMS-Botschaften 0x41A–0x41E |
| mVCU | [Zombie-Slave/docs/mVCU_Integration.md](https://github.com/Modellfan/Zombie-Slave/blob/master/docs/mVCU_Integration.md) | Schnittstelle Zombie-Slave ↔ mVCU (diese VCU), Heizung 0x43A |
| TCU | [ESP32S3-TCU data-model.md](https://github.com/Modellfan/ESP32S3-TCU/blob/001-vehicle-companion-spec/specs/001-vehicle-companion-spec/data-model.md), [eboxster-demo.dbc.json](https://github.com/Modellfan/ESP32S3-TCU/blob/001-vehicle-companion-spec/data/can/eboxster-demo.dbc.json) | Werte, die die TCU erwartet |
| REQ / DEC | [REQUIREMENTS.md](../requirements/REQUIREMENTS.md), [DECISIONS.md](../requirements/DECISIONS.md) | Soll-Verhalten der internen Funktionen |

### Spalten

- **Bemerkung:** Hinweise und Aliasnamen; eine Zeile je Signal, damit „UI Vorschlag“ je Signal entschieden werden kann.
- **Richtung:** RX = von einem Steuergerät empfangen · TX = von der VCU gesendet · intern = in der VCU berechnet · Einst. = Einstellung über `/api/set` · Ausg. = Hardware-Ausgang der VCU · Diag. = Diagnose der VCU selbst. **gesp.** = im EEPROM gespeichert (VCU-REQ-020).
- **Veraltet nach:** empfangene Werte gelten nach 500 ms ohne Botschaft als `stale`, 0x41D/0x41E nach 2,5 s (`rxTimeoutMs()`).
- **UI heute:** wo die PWA den Wert heute tatsächlich anzeigt (`pwa/src/js/app.js`). Alte, nicht mehr erreichbare Konfigurationen (Bereich `sdu`, alte Zeilen der Seiten Vehicle, Ventilation und Stopwatch) zählen nicht.
  - Seiten: **Home** · **Fzg/Motor**, **Fzg/Akku**, **Fzg/Laden**, **Fzg/Fzg** (Seite Vehicle, Tabs Engine/Battery/Charger/Vehicle) · **Kühl** (Coolant) · **EFluss** (Energiefluss) · **Uhr/Trip**, **Uhr/Beschl**, **Uhr/Runde** (Stopwatch) · **Stil** (Porsche-Referenztest) · **[E]Cluster/…**, **[E]Adjust/…**, **[E]Signals/…** (nur Engineering-Modus).
  - Zusatz **(E)** = Zeile nur im Engineering-Modus.
- **UI Vorschlag:** **nicht entschieden**, Arbeitsgrundlage für die Seitenaufteilung. Im Engineering-Modus zeigt die Seite **[E]Signals** (VCU-REQ-022) zusätzlich **alle** Signale, Tabs Vehicle (CAN1 und DME-Sendung), Drive (CAN2), Battery (CAN0 und Heizung), VCU (intern, CAN-Diagnose, Status); das steht nicht in jeder Zeile.
- **TCU:** ✓ = die TCU braucht den Wert (Quelle TCU).

## 2. CAN1 – Porsche 986

Master: [porsche_986_messages.md](porsche_986_messages.md) (Spiegel von [porsche_986_996_compiled_working.dbc](porsche_986_996_compiled_working.dbc), dieses Repo).

Mitschnitt `can_logs/986_log_stand.csv` (Stand): 0x0C0, 0x1A0, 0x2A8, 0x4A1, 0x510, 0x520 vorhanden; **0x440 (PRND) und 0x560 fehlen**.

### 2.1 Von der VCU gesendet (DME-Emulation)

Die VCU ersetzt das Motorsteuergerät; Werte sind Ausgaben ans Kombi. Setzbar über `/api/set-ecu` (W_ECU) bzw. als Override (OVR). 0x4F8 und 0x7A0 sendet die VCU als feste Rohbytes.

#### 0x280 Engine_1

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `engine_speed_rpm` | Drehzahlnadel (zeigt Leistung) | rpm | TX 0x280 | – | – |  | Alias: = `rpm`, `set_rpm` |
| `throttle_position_280` | Drosselklappe | % | TX 0x280 | [E]Adjust/Drive | – |  | Alias: = `throttle_pct` |
| `pedal_position_1` | Pedalstellung 1 | % | TX 0x280 | [E]Adjust/Drive | – |  | Alias: = `pedal1_pct` |
| `pedal_position_2` | Pedalstellung 2 | % | TX 0x280 | [E]Adjust/Drive | – |  | Alias: = `pedal2_pct` |

#### 0x289 Motor_2

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `motor2_coolant_temperature` | Kühlmittelanzeige Kombi | °C | TX 0x289 | [E]Cluster/Thermal | – |  | Alias: = `coolant_c`, `set_coolant` |
| `motor2_coolant_temp_not_ok` | Kühlmittelwarnung | bool | TX 0x289 | – | – |  | Alias: = `coolant_level` |
| `motor2_brake_light_switch` | Bremslichtschalter | – | TX 0x289 | – | – |  |  |
| `motor2_brake_test_switch` | Bremstestschalter | – | TX 0x289 | – | – |  |  |
| `motor2_ac_compressor_active` | Klimakompressor aktiv | – | TX 0x289 | – | – |  |  |
| `motor2_normal_operation_active` | Normalbetrieb | – | TX 0x289 | – | – |  |  |
| `motor2_drivetrain_coordinator_active` | Triebstrangkoordinator aktiv | – | TX 0x289 | – | – |  |  |
| `motor2_cruise_control_status` | Tempomatstatus | – | TX 0x289 | – | – |  |  |
| `motor2_speed_limiter_selected` | Geschwindigkeitsbegrenzer gewählt | – | TX 0x289 | – | – |  |  |
| `motor2_cruise_control_regulating` | Tempomat regelt | – | TX 0x289 | – | – |  | Alias: = `cruise_active` |
| `motor2_sport_error` | Sport-Fehler | – | TX 0x289 | – | – |  |  |
| `motor2_offroad_mode_active` | Offroad-Modus | – | TX 0x289 | – | – |  |  |
| `motor2_cruise_target_speed_kmh` | Tempomat-Sollgeschwindigkeit | km/h | TX 0x289 | – | – |  |  |
| `motor2_idle_target_speed_rpm` | Leerlauf-Solldrehzahl | rpm | TX 0x289 | – | – |  |  |
| `motor2_limited_torque_percent` | begrenztes Moment | % | TX 0x289 | – | – |  |  |
| `motor2_min_torque_ignition_retard_percent` | Mindestmoment bei Zündwinkelrücknahme | % | TX 0x289 | – | – |  |  |

#### 0x298 Engine_2

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `inlet_air_temperature` | Ansauglufttemperatur | °C | TX 0x298 | [E]Cluster/Thermal (`inlet_air_temperature`) | – |  | Alias: = `iat_c` |
| `barometric_pressure_mbar` | Luftdruck | mbar | TX 0x298 | [E]Cluster/Thermal (`inlet_air_temperature`) | – |  | Alias: = `baro_mbar` |
| `thr1_298_pct` | Drosselklappe 1 (0x298) | % | TX 0x298 | [E]Cluster/Thermal (`inlet_air_temperature`) | – |  |  |
| `thr2_298_pct` | Drosselklappe 2 (0x298) | % | TX 0x298 | [E]Cluster/Thermal (`inlet_air_temperature`) | – |  |  |
| `v298_raw` | Geschwindigkeit roh (Skalierung unbekannt) | – | TX 0x298 | [E]Cluster/Thermal (`inlet_air_temperature`) | – |  |  |

#### 0x4E0 EngineStat

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `check_engine_light` | Motorkontrollleuchte | bool | TX 0x4E0 | – | – |  |  |
| `check_engine_light_bit1` | Motorkontrollleuchte Bit 1 | bool | TX 0x4E0 | – | – |  |  |
| `engine_stat_bool2` | Statusbit 2 | bool | TX 0x4E0 | – | – |  |  |
| `reduced_power` | Leistungsreduzierung | bool | TX 0x4E0 | – | – |  |  |
| `fan_error` | Lüfterfehler | bool | TX 0x4E0 | – | – |  |  |
| `engine_stat_bool5` | Statusbit 5 | bool | TX 0x4E0 | – | – |  |  |
| `engine_stat_bool6` | Statusbit 6 | bool | TX 0x4E0 | – | – |  |  |
| `engine_stat_bool7` | Statusbit 7 | bool | TX 0x4E0 | – | – |  |  |
| `mo5_vorgluehen` | Vorglühlampe | bool | TX | – | – |  |  |
| `mo5_e_gas` | E-Gas-Lampe | bool | TX | – | – |  |  |
| `mo5_obd_2` | OBD-/MIL-Lampe | bool | TX | – | – |  | Alias: = `mil` |
| `mo5_heissl` | Kühlmittel-Überhitzungslampe | bool | TX | – | – |  |  |
| `mo5_klimakompr` | Klimakompressor aus | bool | TX | – | – |  |  |
| `mo5_feld_kuehl` | Kennfeldkühlung | bool | TX | – | – |  |  |
| `mo5_kliko_red` | Klimakompressor reduziert | bool | TX | – | – |  |  |
| `mo5_ueberlverb` | Überlauf Verbrauchszähler | bool | TX | – | – |  |  |
| `mo5_verbrauch_ul` | Verbrauchszähler (Bordcomputer) | µl | TX | [E]Cluster/Power | – |  | Alias: = `set_oil` |

### 2.2 Vom Fahrzeug empfangen

#### 0x0C0 SWA (Lenkwinkelsensor) · Sender Lenkwinkelsensor

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `lenkwinkel_rb` | Lenkwinkel | ° | RX | Fzg/Fzg | Uhr |  |  |
| `vorzeichen_rb` | Lenkwinkel | ° | RX | Fzg/Fzg | Uhr |  |  |
| `lws_ok_rb` | Gültigkeit/Abgleich Lenkwinkel | bool | RX | – | – |  |  |
| `lws_abgleich_rb` | Gültigkeit/Abgleich Lenkwinkel | bool | RX | – | – |  |  |
| `fine_check_rb` | Gültigkeit/Abgleich Lenkwinkel | bool | RX | – | – |  |  |
| `coarse_check_rb` | Gültigkeit/Abgleich Lenkwinkel | bool | RX | – | – |  |  |
| `frei_lenkwinkel_1_1_rb` | Gültigkeit/Abgleich Lenkwinkel | bool | RX | – | – |  |  |

#### 0x1A0 ABS_1 (ABS/PSM) · Sender ABS/PSM

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `vref_kmh` | Referenzgeschwindigkeit | km/h | RX | Fzg/Fzg, Uhr/Trip (Ersatz für 0x510) | Home, Uhr | ✓ speed |  |
| `abs` | ABS | bool | RX | – | Uhr, Fzg/Fzg |  | Regeleingriff ABS/ASR/MSR/EDS/PSM |
| `tcs` | ASR | bool | RX | – | Uhr, Fzg/Fzg |  | Regeleingriff ABS/ASR/MSR/EDS/PSM |
| `msr` | MSR | bool | RX | – | Uhr, Fzg/Fzg |  | Regeleingriff ABS/ASR/MSR/EDS/PSM |
| `abd` | EDS | bool | RX | – | Uhr, Fzg/Fzg |  | Regeleingriff ABS/ASR/MSR/EDS/PSM |
| `fdr` | PSM | bool | RX | – | Uhr, Fzg/Fzg |  | Regeleingriff ABS/ASR/MSR/EDS/PSM |
| `asr_mode` | ASR-Modus, Eingriffsmomente (**`itq_pct` ist ein Zähler**, [986 #1](porsche_986_messages.md#offene-punkte)) | –/% | RX | – | – |  |  |
| `tcs_slow_pct` | ASR-Modus, Eingriffsmomente (**`itq_pct` ist ein Zähler**, [986 #1](porsche_986_messages.md#offene-punkte)) | –/% | RX | – | – |  |  |
| `tcs_fast_pct` | ASR-Modus, Eingriffsmomente (**`itq_pct` ist ein Zähler**, [986 #1](porsche_986_messages.md#offene-punkte)) | –/% | RX | – | – |  |  |
| `msr_inv_pct` | ASR-Modus, Eingriffsmomente (**`itq_pct` ist ein Zähler**, [986 #1](porsche_986_messages.md#offene-punkte)) | –/% | RX | – | – |  |  |
| `itq_pct` | ASR-Modus, Eingriffsmomente (**`itq_pct` ist ein Zähler**, [986 #1](porsche_986_messages.md#offene-punkte)) | –/% | RX | – | – |  |  |
| `abs_lamp` | Warnlampen | bool | RX | [E]Cluster/Inputs (`fdr_lamp`, `psm_button_lamp`) | Home (Warnsymbol) | ✓ |  |
| `brake_lamp` | Warnlampen | bool | RX | [E]Cluster/Inputs (`fdr_lamp`, `psm_button_lamp`) | Home (Warnsymbol) | ✓ |  |
| `fdr_lamp` | Warnlampen | bool | RX | [E]Cluster/Inputs (`fdr_lamp`, `psm_button_lamp`) | Home (Warnsymbol) | ✓ |  |
| `fdr_lamp2` | Warnlampen | bool | RX | [E]Cluster/Inputs (`fdr_lamp`, `psm_button_lamp`) | Home (Warnsymbol) | ✓ |  |
| `psm_button_lamp` | Warnlampen | bool | RX | [E]Cluster/Inputs (`fdr_lamp`, `psm_button_lamp`) | Home (Warnsymbol) | ✓ |  |
| `brake_sw` | Bremslichtschalter | bool | RX | – | – |  |  |
| `brake_sw_inv` | Bremslichtschalter | bool | RX | – | – |  |  |
| `handbrake` | Handbremse | bool | RX | – | Home (Hinweis) | ✓ |  |
| `lat_g` | Querbeschleunigung | g | RX | – | Uhr |  |  |
| `br1_counter` | Zähler und Statusbits Bremse_1 | – | RX | [E]Cluster/Inputs (`rough_road_suppression`) | – |  |  |
| `br1_asr_esp` | Zähler und Statusbits Bremse_1 | – | RX | [E]Cluster/Inputs (`rough_road_suppression`) | – |  |  |
| `br1_espasr_passive` | Zähler und Statusbits Bremse_1 | – | RX | [E]Cluster/Inputs (`rough_road_suppression`) | – |  |  |
| `br1_sta_esp` | Zähler und Statusbits Bremse_1 | – | RX | [E]Cluster/Inputs (`rough_road_suppression`) | – |  |  |
| `br1_ersatz_kmh` | Zähler und Statusbits Bremse_1 | – | RX | [E]Cluster/Inputs (`rough_road_suppression`) | – |  |  |
| `rough_road_suppression` | Zähler und Statusbits Bremse_1 | – | RX | [E]Cluster/Inputs (`rough_road_suppression`) | – |  |  |
| `rough_road_suppression_status` | Zähler und Statusbits Bremse_1 | – | RX | [E]Cluster/Inputs (`rough_road_suppression`) | – |  |  |

#### 0x2A8 Vehicle_Speed (ABS/PSM) · Sender ABS/PSM

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `wfl_legacy_kmh` | Radgeschwindigkeiten | km/h | RX | – | Uhr (Schlupf) |  |  |
| `wfr_legacy_kmh` | Radgeschwindigkeiten | km/h | RX | – | Uhr (Schlupf) |  |  |
| `wrl_legacy_kmh` | Radgeschwindigkeiten | km/h | RX | – | Uhr (Schlupf) |  |  |
| `wrr_legacy_kmh` | Radgeschwindigkeiten | km/h | RX | – | Uhr (Schlupf) |  |  |

#### 0x4A1 ABS_2 (ABS/PSM) · Sender ABS/PSM

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `acceleration` | Längsbeschleunigung | g | RX | Fzg/Fzg; Uhr/Runde (als Ersatz für „Delta“, Fehler) | Uhr |  |  |
| `yaw_rate` | Gierrate | °/s | RX | – | Uhr |  |  |
| `yaw_rate_sign` | Gierrate | °/s | RX | – | Uhr |  |  |
| `driver_braking` | Fahrer bremst (**falsch dekodiert: Zählerbit**, [986 #2](porsche_986_messages.md#offene-punkte)) | bool | RX | [E]Adjust/Info, [E]Cluster/Inputs | – |  |  |
| `brake_normal_or_offset` | Bremsdruck/Offset | % | RX | [E]Adjust/Drive | – |  |  |
| `no_actively_built_up_brake_pressure` | Status- und Fehlerbits ABS_2 | – | RX | [E]Adjust/Info, [E]Cluster/Inputs (`receipt_for_acc_message`) | – |  |  |
| `acc_related_error` | Status- und Fehlerbits ABS_2 | – | RX | [E]Adjust/Info, [E]Cluster/Inputs (`receipt_for_acc_message`) | – |  |  |
| `receipt_for_acc_message` | Status- und Fehlerbits ABS_2 | – | RX | [E]Adjust/Info, [E]Cluster/Inputs (`receipt_for_acc_message`) | – |  |  |
| `regulator_error_fdr` | Status- und Fehlerbits ABS_2 | – | RX | [E]Adjust/Info, [E]Cluster/Inputs (`receipt_for_acc_message`) | – |  |  |
| `error_driver_brake_detection` | Status- und Fehlerbits ABS_2 | – | RX | [E]Adjust/Info, [E]Cluster/Inputs (`receipt_for_acc_message`) | – |  |  |
| `multiplex_byte4` | Status- und Fehlerbits ABS_2 | – | RX | [E]Adjust/Info, [E]Cluster/Inputs (`receipt_for_acc_message`) | – |  |  |

#### 0x510 InstrCluster_1 (Kombi) · Sender Kombi

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `vehicle_speed_510` | Geschwindigkeit Kombi | km/h | RX | Fzg/Fzg, Uhr/Trip, [E]Adjust/Drive, [E]Cluster/Power, Cluster/Accel; Ladeerkennung der UI | Fzg/Fzg |  |  |
| `indicator_left` | Blinker | bool | RX | Fzg/Fzg | Home |  |  |
| `indicator_right` | Blinker | bool | RX | Fzg/Fzg | Home |  |  |
| `tempomat_button` | Tempomat- und Lenkradtasten | bool | RX | [E]Adjust/Info, [E]Cluster/Inputs | Bedienung der UI? (offen) |  |  |
| `steeringwheel_button_up` | Tempomat- und Lenkradtasten | bool | RX | [E]Adjust/Info, [E]Cluster/Inputs | Bedienung der UI? (offen) |  |  |
| `steeringwheel_button_down` | Tempomat- und Lenkradtasten | bool | RX | [E]Adjust/Info, [E]Cluster/Inputs | Bedienung der UI? (offen) |  |  |

#### 0x520 InstrCluster_2 (Kombi) · Sender Kombi

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `ambient_c` | Außentemperatur | °C | RX | [E]Cluster/Power | Home, Klima | ✓ |  |
| `light_dimmer` | Lichtdimmer | – | RX | [E]Adjust/Setup, [E]Cluster/Inputs | Nachtmodus der UI |  |  |
| `oil_pressure_bar` | Öldruck | bar | RX | [E]Cluster/Power | – (kein Verbrenner) |  |  |
| `cluster_counter` | Zähler Kombi | – | RX | [E]Adjust/Info, [E]Cluster/Inputs; Uhr/Runde (als Rundenzähler, Fehler) | – |  |  |

#### 0x440 AT_Line1 (Getriebe) · Sender Getriebesteuergerät

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `prnd_pos` | Wählhebel P/R/N/D | enum | RX | Fzg/Fzg | Home | ✓ |  |

## 3. CAN2 – Tesla SDU (OpenInverter)

Master: [can_messages.md, Abschnitt OpenInverter](can_messages.md#openinverter-stm32-sine) (CAN-Mapping des Umrichters; Firmware [jsphuebner/stm32-sine](https://github.com/jsphuebner/stm32-sine)).

Mitschnitt `can_logs/20260419_fahrt_tesla.csv`: 0x500–0x505; **0x506–0x508 fehlen**. Die VCU sendet auf CAN2 nichts.

### 3.1 Vom Umrichter empfangen

#### 0x500 DC electrical

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_udc` | DC-Spannung | V | RX | Fzg/Motor, Fzg/Akku, [E]Adjust/Power; UI rechnet daraus Leistung (Home, EFluss) | Fzg/Motor, EFluss | ✓ (Leistung) | , DC-Strom |
| `sdu_idc` | DC-Strom | A | RX | Fzg/Motor, Fzg/Akku, [E]Adjust/Power; UI rechnet daraus Leistung (Home, EFluss) | Fzg/Motor, EFluss | ✓ (Leistung) | DC-Spannung, DC-Strom |
| `sdu_il1` | Phasenströme | A | RX | Fzg/Motor (E) | – |  |  |
| `sdu_il2` | Phasenströme | A | RX | Fzg/Motor (E) | – |  |  |

#### 0x501 Motor output

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_speed` | Motordrehzahl | rpm | RX | Fzg/Motor | Fzg/Motor |  |  |
| `sdu_fstat` | Frequenz | Hz | RX | Fzg/Motor (E) | – |  | , AC-Spannung, Stromgrenze |
| `sdu_uac` | AC-Spannung | V | RX | Fzg/Motor (E) | – |  | Frequenz, AC-Spannung, Stromgrenze |
| `sdu_ilmax` | Stromgrenze | A | RX | Fzg/Motor (E) | – |  | Frequenz, AC-Spannung, Stromgrenze |

#### 0x502 RMS and derived values

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_il1rms` | RMS-Ströme, Boost, Feldschwächung | A, –, Hz | RX | Fzg/Motor (E) | – |  |  |
| `sdu_il2rms` | RMS-Ströme, Boost, Feldschwächung | A, –, Hz | RX | Fzg/Motor (E) | – |  |  |
| `sdu_boostcalc` | RMS-Ströme, Boost, Feldschwächung | A, –, Hz | RX | Fzg/Motor (E) | – |  |  |
| `sdu_fweakcalc` | RMS-Ströme, Boost, Feldschwächung | A, –, Hz | RX | Fzg/Motor (E) | – |  |  |

#### 0x503 Driver demand

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_pot` | Gaspedal roh / nominal | dig, % | RX | Fzg/Motor (`potnom`; `pot`, `pot2` E) | Fzg/Motor (Pedalbalken) |  |  |
| `sdu_pot2` | Gaspedal roh / nominal | dig, % | RX | Fzg/Motor (`potnom`; `pot`, `pot2` E) | Fzg/Motor (Pedalbalken) |  |  |
| `sdu_potnom` | Gaspedal roh / nominal | dig, % | RX | Fzg/Motor (`potnom`; `pot`, `pot2` E) | Fzg/Motor (Pedalbalken) |  |  |
| `sdu_amp` | Stellgröße | – | RX | Fzg/Motor (E) | – |  | , Rotorwinkel |
| `sdu_angle` | Rotorwinkel | ° | RX | Fzg/Motor (E) | – |  | Stellgröße, Rotorwinkel |

#### 0x504 Direction, regen, thermal

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_regenpreset` | Rekuperationsstufe | % | RX | Fzg/Motor | Fzg/Motor |  |  |
| `sdu_seldir` | Fahrtrichtung gewählt | enum | RX | Fzg/Motor (`rotordir` E) | Home (D/N/R) | ✓ | / Rotor |
| `sdu_rotordir` | Rotor | enum | RX | Fzg/Motor (`rotordir` E) | Home (D/N/R) | ✓ | Fahrtrichtung gewählt / Rotor |
| `sdu_tmphs` | Kühlkörper- | °C | RX | Fzg/Motor, [E]Cluster/Thermal | Fzg/Motor, Kühl | ✓ | , Motortemperatur |
| `sdu_tmpm` | Motortemperatur | °C | RX | Fzg/Motor, [E]Cluster/Thermal | Fzg/Motor, Kühl | ✓ | Kühlkörper-, Motortemperatur |
| `sdu_uaux` | 12-V-Spannung am Umrichter | V | RX | Fzg/Motor, Fzg/Akku | Bordnetz | ✓ (Ersatz für 12-V-Batterie) |  |
| `sdu_regenpresent` | doppelte bzw. unklare Einträge | – | RX | – | aufräumen |  |  |
| `sdu_regenpresent32` | doppelte bzw. unklare Einträge | – | RX | – | aufräumen |  |  |
| `sdu_seldir43` | doppelte bzw. unklare Einträge | – | RX | – | aufräumen |  |  |
| `sdu_temp_heatsink` | doppelte bzw. unklare Einträge | – | RX | – | aufräumen |  |  |
| `sdu_temp_heatsink74` | doppelte bzw. unklare Einträge | – | RX | – | aufräumen |  |  |
| `sdu_din_ocur51` | doppelte bzw. unklare Einträge | – | RX | – | aufräumen |  |  |

#### 0x505 Mode, fault, status

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_opmode` | Betriebsart | enum | RX | Fzg/Motor | Fzg/Motor | ✓ |  |
| `sdu_lasterr` | letzter Fehler | enum | RX | Fzg/Motor (E) | Home (Warnung) | ✓ |  |
| `sdu_status` | Statusbits | Bits, % | RX | Fzg/Motor (E) | – |  | , CAN-/PWM-Eingänge, CPU-Last |
| `sdu_canio` | CAN- | Bits, % | RX | Fzg/Motor (E) | – |  | Statusbits, CAN-/PWM-Eingänge, CPU-Last |
| `sdu_pwmio` | PWM-Eingänge | Bits, % | RX | Fzg/Motor (E) | – |  | Statusbits, CAN-/PWM-Eingänge, CPU-Last |
| `sdu_cpuload` | CPU-Last | Bits, % | RX | Fzg/Motor (E) | – |  | Statusbits, CAN-/PWM-Eingänge, CPU-Last |

#### 0x506 Digital inputs A

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_din_cruise` | digitale Eingänge A | enum | RX | Fzg/Motor (E) | – |  |  |
| `sdu_din_start` | digitale Eingänge A | enum | RX | Fzg/Motor (E) | – |  |  |
| `sdu_din_brake` | digitale Eingänge A | enum | RX | Fzg/Motor (E) | – |  |  |
| `sdu_din_mprot` | digitale Eingänge A | enum | RX | Fzg/Motor (E) | – |  |  |
| `sdu_din_forward` | digitale Eingänge A | enum | RX | Fzg/Motor (E) | – |  |  |
| `sdu_din_reverse` | digitale Eingänge A | enum | RX | Fzg/Motor (E) | – |  |  |
| `sdu_din_emcystop` | digitale Eingänge A | enum | RX | Fzg/Motor (E) | – |  |  |
| `sdu_din_ocur` | digitale Eingänge A | enum | RX | Fzg/Motor (E) | – |  |  |

#### 0x507 Digital inputs B, cruise

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_din_desat` | digitale Eingänge B, Tempomatdrehzahl | enum, rpm | RX | Fzg/Motor (E) | – |  |  |
| `sdu_din_bms` | digitale Eingänge B, Tempomatdrehzahl | enum, rpm | RX | Fzg/Motor (E) | – |  |  |
| `sdu_cruisespeed` | digitale Eingänge B, Tempomatdrehzahl | enum, rpm | RX | Fzg/Motor (E) | – |  |  |

#### 0x508 Turns and uptime

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `sdu_turns` | Umdrehungen, Laufzeit | – | RX | Fzg/Motor (E); Uhr/Trip nutzt `sdu_uptime` als Fahrzeit (Fehler) | – |  |  |
| `sdu_uptime` | Umdrehungen, Laufzeit | 10 ms | RX | Fzg/Motor (E); Uhr/Trip nutzt `sdu_uptime` als Fahrzeit (Fehler) | – |  |  |
| `sdu_uptime_s` | Umdrehungen, Laufzeit | s | RX | Fzg/Motor (E); Uhr/Trip nutzt `sdu_uptime` als Fahrzeit (Fehler) | – |  |  |

## 4. CAN0 – Akku (TeensyBMS, Zombie-Slave, Tesla-DC/DC)

Master: [ZombieSlave_teensybms_dcdc_mvcu.dbc](https://github.com/Modellfan/Zombie-Slave/blob/master/documentation/ZombieSlave_teensybms_dcdc_mvcu.dbc) (Repo Zombie-Slave; Kopie: [ZombieSlave_teensybms_dcdc_mvcu.dbc](ZombieSlave_teensybms_dcdc_mvcu.dbc)) · Packing/CRC der BMS-Botschaften: [CAN_Message_Packing.md](https://github.com/Modellfan/TeensyVCU/blob/master/documentation/CAN_Message_Packing.md) (Repo TeensyVCU) · Schnittstelle mVCU: [mVCU_Integration.md](https://github.com/Modellfan/Zombie-Slave/blob/master/docs/mVCU_Integration.md) (Repo Zombie-Slave).

Decoder `src/comms/battery_messages.cpp`; Frames mit falscher CRC werden verworfen. Empfang am Prüfstand bestanden 2026-11-10 (TC-BAT-01), **am echten Bus noch nicht**. Fehlende Signale: [eBoxster#57](https://github.com/Modellfan/eBoxster/issues/57).

### 4.1 Von der VCU gesendet

#### 0x43A mVCU_HeaterControl · Sender VCU

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `heater_request` | Heizung einschalten (an Zombie-Slave, 100 ms, Zähler + CRC; nach Start aus, nicht gespeichert) | bool | TX 0x43A (Referenz ZS, mVCU; VCU-REQ-021) | – (nur `/io`) | Klima (Schalter) | ✓ (Klima) |  |

### 4.2 Empfangen

#### 0x41A TeensyBMS_VoltageCurrent · Sender TeensyBMS

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `battery_pack_voltage_v` | Packspannung | V | RX | – | Fzg/Akku, EFluss | ✓ | , Packstrom |
| `battery_pack_current_a` | Packstrom | A | RX | – | Fzg/Akku, EFluss | ✓ | Packspannung, Packstrom |
| `battery_cell_v_min` | min. | V | RX | – | Fzg/Akku |  | /max. Zellspannung |
| `battery_cell_v_max` | max. Zellspannung | V | RX | – | Fzg/Akku |  | min./max. Zellspannung |

#### 0x41B TeensyBMS_CellTempPower · Sender TeensyBMS

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `battery_cell_t_min_c` | min. | °C | RX | – | Fzg/Akku | ✓ | /max. Zelltemperatur |
| `battery_cell_t_max_c` | max. Zelltemperatur | °C | RX | – | Fzg/Akku | ✓ | min./max. Zelltemperatur |
| `battery_balancing_v` | Balancing-Spannung | V | RX | – | Fzg/Akku |  | , Zelldifferenz |
| `battery_cell_delta_mv` | Zelldifferenz | mV | RX | – | Fzg/Akku |  | Balancing-Spannung, Zelldifferenz |
| `battery_pack_power_kw` | Packleistung | kW | RX | – | Home, EFluss | ✓ |  |

#### 0x41C TeensyBMS_LimitsFault · Sender TeensyBMS

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `battery_max_discharge_a` | Strombegrenzung Entladen | A | RX | – | Fzg/Akku |  | /Laden |
| `battery_max_charge_a` | Laden | A | RX | – | Fzg/Akku |  | Strombegrenzung Entladen/Laden |
| `battery_contactor_state` | Schützzustand | enum | RX | – | Fzg/Akku |  |  |
| `battery_dtc_flags` | BMS-Fehlerflags | Bits | RX | – | Home (Warnung), Fzg/Akku |  |  |

#### 0x41D TeensyBMS_SocSoh · Sender TeensyBMS

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `battery_soc_pct` | Ladestand | % | RX | – (UI sucht `battery_soc` u. a.) | Home, Fzg/Akku, Fzg/Laden | ✓ | , Gesundheit |
| `battery_soh_pct` | Gesundheit | % | RX | – (UI sucht `battery_soc` u. a.) | Home, Fzg/Akku, Fzg/Laden | ✓ | Ladestand, Gesundheit |
| `battery_balancing_status` | Balancing | enum | RX | – | Fzg/Akku |  | , BMS-Zustand |
| `battery_bms_state` | BMS-Zustand | enum | RX | – | Fzg/Akku |  | Balancing, BMS-Zustand |

#### 0x41E TeensyBMS_Hmi · Sender TeensyBMS

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `battery_avg_power_kw` | mittlere Leistung | kW | RX | – | Uhr/Trip |  |  |
| `battery_remaining_time_s` | Restzeit | s | RX | – | Fzg/Laden | ✓ |  |
| `battery_remaining_energy_kwh` | Restenergie | kWh | RX | – | Fzg/Akku |  |  |

#### 0x437 ZombieSlave_VcuStatus · Sender Zombie-Slave

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `battery_vehicle_state` | Fahrzeugzustand (SLEEP … DRIVE, CHARGE, ERROR) | enum | RX | – | Home | ✓ vehicleReady |  |
| `battery_charging_active` | lädt (Zustand CHARGE) | bool | RX (abgeleitet) | – (UI sucht `charging_active`, `is_charging`) | Home, Fzg/Laden, EFluss | ✓ |  |
| `battery_hv_request` | HV-Anforderung | – | RX | – | – |  | , Zwangsabschaltung |
| `battery_force_shutdown` | Zwangsabschaltung | – | RX | – | – |  | HV-Anforderung, Zwangsabschaltung |

#### 0x438 mVCU_ChargePowerStatus · Sender Zombie-Slave

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `battery_charge_power_w` | Ladeleistung Ist | W | RX | – (UI sucht `charge_power_kw`) | Fzg/Laden, EFluss | ✓ | /Max |
| `battery_charge_power_max_w` | Max | W | RX | – (UI sucht `charge_power_kw`) | Fzg/Laden, EFluss | ✓ | Ladeleistung Ist/Max |

#### 0x439 mVCU_HeaterStatus · Sender Zombie-Slave

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `heater_active` | Heizung: aktiv | bool | RX | – | Klima | ✓ (Klima) | , Schütz, Fehler, Thermoschalter, Ausgang, empfangene Anforderung |
| `heater_contactor_closed` | Schütz | bool | RX | – | Klima | ✓ (Klima) | Heizung: aktiv, Schütz, Fehler, Thermoschalter, Ausgang, empfangene Anforderung |
| `heater_fault` | Fehler | bool | RX | – | Klima | ✓ (Klima) | Heizung: aktiv, Schütz, Fehler, Thermoschalter, Ausgang, empfangene Anforderung |
| `heater_thermal_ok` | Thermoschalter | bool | RX | – | Klima | ✓ (Klima) | Heizung: aktiv, Schütz, Fehler, Thermoschalter, Ausgang, empfangene Anforderung |
| `heater_contactor_out` | Ausgang | bool | RX | – | Klima | ✓ (Klima) | Heizung: aktiv, Schütz, Fehler, Thermoschalter, Ausgang, empfangene Anforderung |
| `heater_can_request` | empfangene Anforderung | bool | RX | – | Klima | ✓ (Klima) | Heizung: aktiv, Schütz, Fehler, Thermoschalter, Ausgang, empfangene Anforderung |

#### 0x210 TeslaDCDC_Status · Sender Tesla-DC/DC

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `dcdc_output_v` | DC | V | RX | – | Bordnetz, EFluss |  | /DC Ausgang, Eingangsleistung |
| `dcdc_output_a` | DC Ausgang | A | RX | – | Bordnetz, EFluss |  | DC/DC Ausgang, Eingangsleistung |
| `dcdc_input_w` | Eingangsleistung | W | RX | – | Bordnetz, EFluss |  | DC/DC Ausgang, Eingangsleistung |
| `dcdc_coolant_c` | DC/DC Kühlmitteltemperatur, Kühlanforderung | °C | RX | – | Kühl |  |  |
| `dcdc_coolant_request` | DC/DC Kühlmitteltemperatur, Kühlanforderung | bool | RX | – | Kühl |  |  |
| `dcdc_fault_bits` | DC/DC Fehlerbits | Bits | RX | – | Bordnetz (Sammelwarnung) |  |  |
| `battery_plug_connected` | Ladestecker gesteckt | bool | – | – | Fzg/Laden | ✓ |  |
| `battery_crc_errors` | verworfene CAN0-Frames | Anzahl | Diag. | – | – |  |  |

## 5. CAN3 – Reserve

Rolle offen; die VCU zählt Frames und spiegelt sie an GVRET (Bus 3), dekodiert nichts.

## 6. Interne Funktionen und Ausgänge der VCU

Master: [REQUIREMENTS.md](../requirements/REQUIREMENTS.md) und Code; Pins in `src/board.h`.

| API-Name | Bedeutung | Einheit | Richtung | UI heute | UI Vorschlag | TCU | Bemerkung |
|---|---|---|---|---|---|---|---|
| `coolant_fan_mode` | Lüftermodus automatisch/manuell | enum | Einst., gesp. | – | Kühl |  |  |
| `coolant_fan_level` | wirksame Lüfterstufe 0/1/2; Setzen schaltet auf manuell | 0–2 | Einst. | Kühl | Kühl |  |  |
| `coolant_fan_level_manual` | Stufe manuell (gesp.) / automatisch aus `sdu_tmpm`, `sdu_tmphs` | 0–2 | intern | – | Kühl (E) |  |  |
| `coolant_fan_level_auto` | Stufe manuell (gesp.) / automatisch aus `sdu_tmpm`, `sdu_tmphs` | 0–2 | intern | – | Kühl (E) |  |  |
| `coolant_fan_low_output` | Ausgang Lüfterstufe 1 | bool | Ausg. | Kühl | Kühl |  | / 2 (**Pin Stufe 1 fehlt**) |
| `coolant_fan_high_output` | 2 | bool | Ausg. | Kühl | Kühl |  | Ausgang Lüfterstufe 1 / 2 (**Pin Stufe 1 fehlt**) |
| `coolant_fan_outputs_enabled` | Lüfterausgänge zugeordnet | bool | Diag. | – | – |  |  |
| `digital_pot_request_pct` | angeforderte Tankanzeige (X9C103S ersetzt den Tankgeber) | % | Einst. | Fzg/Laden (Bezeichnung „Pilot output“, falsch), [E]Adjust/Setup | Kühl/Kombi (E) |  | Alias: API-Setzname `digital_pot_pct` |
| `digital_pot_pct` | eingestellte Position | % | Ausg. | – | – |  | , Schleiferstufe |
| `digital_pot_wiper` | Schleiferstufe | 0–99 | Ausg. | – | – |  | eingestellte Position, Schleiferstufe |
| `digital_pot_enabled` | Poti-Pins zugeordnet (**INC/UD fehlen**, VCU-REQ-009) | bool | Diag. | – | – |  |  |
| `target_charge_pct` | Ziel-Ladestand | % | Einst., gesp. | Fzg/Laden, [E]Adjust/Setup; Stil (nur lokal) | Fzg/Laden |  |  |
| `ac_current_limit_a` | AC-Ladestromgrenze (**geht heute an kein Steuergerät**, Abschnitt 9) | A | Einst., gesp. | Fzg/Laden, [E]Adjust/Setup | Fzg/Laden |  |  |
| `cluster_activated` | Kombi-Ansteuerung aktiv (`/api/set-cluster`) | bool | Einst. | [E]Cluster/State | – |  |  |
| `cluster_power_percent_max` | Leistung in % für die Drehzahlnadel | % | intern | [E]Cluster/Power | – |  |  |
| `cluster_power_percent_dyn` | Leistung in % für die Drehzahlnadel | % | intern | [E]Cluster/Power | – |  |  |
| `cluster_needle_position` | Nadelstellung | – | intern → `engine_speed_rpm` | [E]Cluster/State | – |  |  |
| `cluster_total_fuel_l` | Tankinhalt fürs Kombi (heute fest) | l | intern → `mo5_verbrauch_ul` | [E]Cluster/State | – |  |  |
| `cluster_heatsink_temp_critical` | Temperaturgrenze überschritten | bool | intern → Kühlmittelanzeige | [E]Cluster/State, /Thermal | Home (Warnung) |  |  |
| `cluster_motor_temp_critical` | Temperaturgrenze überschritten | bool | intern → Kühlmittelanzeige | [E]Cluster/State, /Thermal | Home (Warnung) |  |  |
| `cluster_acceleration_run_active` | Beschleunigungsmessung läuft | bool | intern | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl |  | , aktuelle Zeit |
| `cluster_acceleration_run_time_s` | aktuelle Zeit | s | intern | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl |  | Beschleunigungsmessung läuft, aktuelle Zeit |
| `cluster_acceleration_0_50_last_s` | letzte Zeit 0–50 | s | intern | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl |  | / 0–100 |
| `cluster_acceleration_0_100_last_s` | 0–100 | s | intern | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl |  | letzte Zeit 0–50 / 0–100 |
| `cluster_acceleration_0_50_best_s` | Bestzeit, gültig | s, bool | intern, gesp. | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl |  |  |
| `cluster_acceleration_0_100_best_s` | Bestzeit, gültig | s, bool | intern, gesp. | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl |  |  |
| `cluster_acceleration_0_50_valid` | Bestzeit, gültig | s, bool | intern, gesp. | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl |  |  |
| `cluster_acceleration_0_100_valid` | Bestzeit, gültig | s, bool | intern, gesp. | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl |  |  |

## 7. CAN-Manager (Diagnose, `params::can`)

Referenz README (CAN-Hardware), VCU-DEC-016. Anzeige heute nur auf der Testseite `/can`.

| API-Name | Bedeutung | Richtung | UI heute | UI Vorschlag |
|---|---|---|---|---|
| `can_bitrate`, `can_spi_clock_hz`, `can_mcp2515_quartz_hz`, `can_mcp2515_rx_buffer`, `can_mcp2515_tx_buffer`, `can_can0_rx_buffer`, `can_can0_tx_buffer`, `can_rx_budget_per_bus`, `can_ecu_period_ms`, `can_error_refresh_ms`, `can_ecu_task_priority`, `can_web_task_priority` | Einstellungen des CAN-Managers (fest im Code) | Diag. | `/can` | Engineering |
| `can0_…`, `can1_…`, `can2_…`, `can3_…` je `up`, `error`, `rx_frames`, `tec`, `rec`, `eflg`, `bus_off` | Zustand je Bus (28 Felder) | Diag. | `/can` | Engineering; Home-Warnung bei `bus_off` |
| `ecu_cycles`, `ecu_overruns`, `ecu_max_cycle_us`, `ecu_tx_failed`, `ecu_started_ms` | Zeitverhalten DME-Emulation | Diag. | `/can` | Engineering |
| `can0_rx_peak`, `can0_rx_overflow` | Füllstand/Überlauf CAN0-Puffer | Diag. | `/can` | Engineering |

## 8. VCU-Status (nur `/api/live`, nicht in der Signaltabelle)

| API-Name | Bedeutung | UI heute | Bemerkung |
|---|---|---|---|
| `uptime_ms`, `free_heap`, `gvret_dropped` | Laufzeit, freier Speicher, verlorene GVRET-Frames | – | Engineering |
| `sta_connected`, `sta_ssid`, `sta_ip`, `ap_ssid`, `ap_ip`, `ap_clients` | WLAN-Zustand | Netzwerkfenster (Uhrzeit oben rechts), [E]ECU/Wi-Fi | |
| `rpm_override`, `coolant_override`, `coolant_warning_override`, `fuel_override`, `set_rpm_active`, `set_coolant_active`, `set_oil_active` | aktive Overrides der DME-Emulation | – | Engineering |
| `stale` | Liste der veralteten Felder | UI blendet veraltete Werte aus | |

## 9. Benötigt, aber von keinem Steuergerät geliefert

| Wert | Gebraucht von | Mögliche Quelle | Stand |
|---|---|---|---|
| Ladestecker gesteckt, Stecker verriegelt | UI Fzg/Laden, EFluss; TCU connectorConnected | MLB-Lader im Zombie-Slave (nur intern) | an eBoxster Master gemeldet 2026-11-10 |
| Ladeart (AC/DC), Lader-Ist-Modus, AC-Netzspannung, CP/PP-Stromgrenze, Ladertemperatur, Abregelgrund, Lader-Fehler | UI Fzg/Laden; TCU Ladezustand | Zombie-Slave (nur intern) | gemeldet 2026-11-10 |
| AC-Ladestromgrenze an den Lader (`ac_current_limit_a`), Ziel-Ladestand an den Zombie-Slave (`target_charge_pct`) | Ladefunktion | neue Botschaft VCU → Zombie-Slave | offen; die Einstellungen wirken heute nirgends |
| 12-V-Batteriespannung | UI Bordnetz; TCU auxiliaryVoltage | Zombie-Slave `LVDU_12v_battery_voltage` (intern); Ersatz `dcdc_output_v`, `sdu_uaux` | offen |
| Zündung, Ready-Eingang | Schlaf-/Weckverhalten | Zombie-Slave (intern) | offen |
| Kühlmittelpumpe, Tesla-Ventil, Vakuumpumpe, EPS-Zustand | UI Kühl, Warnungen | Zombie-Slave (intern) | offen |
| Kilometerstand | TCU odometer | vielleicht 0x520 Byte 5 (gemultiplext, [986 #5](porsche_986_messages.md#offene-punkte)); sonst VCU summiert auf (nicht eichfähig) | offen |
| Reichweite | UI Home, Fzg/Akku | VCU aus `battery_remaining_energy_kwh` und Verbrauch | offen; UI zeigt heute einen Schätzwert 3,05 km je % |
| Verbrauch kWh/100 km, Fahrtstrecke, Fahrzeit | UI Uhr/Trip | VCU integriert Leistung und Geschwindigkeit | offen; UI zeigt heute Platzhalter (24,2 kWh/100 km, Laufzeit des Umrichters) |
| Ladeenergie je Ladung | TCU energy | VCU oder TCU integriert `battery_charge_power_w` | offen |
| Akkukapazität | UI Ladeende-Zeit | Konstante oder TeensyBMS | offen; UI sucht `battery_capacity_kwh` |
| Rundenzeiten, Rundenzahl | UI Uhr/Runde | keine (bräuchte Start/Ziel, z. B. GPS) | UI zeigt Platzhalter |
| PRND | UI Home; TCU | 0x440 fehlt im Mitschnitt; Ersatz `sdu_seldir` | offen |
| Fahrmodus, Fahrwerk, Lenkunterstützung, Sport-Sound, Klima-Solltemperatur | UI Stil (Porsche-Referenz) | keine | nur Attrappe |

## 10. Befunde und offene Entscheidungen

Befunde aus dem Abgleich PWA ↔ Firmware:
- Die PWA sucht Akku- und Ladewerte unter erfundenen Namen (`battery_soc`, `soc`, `charge_power_kw`, `plug_connected`, `charging_active` …). Die Firmware liefert `battery_soc_pct`, `battery_charge_power_w`, `battery_charging_active`. Deshalb zeigen Home, Fzg/Akku, Fzg/Laden und EFluss heute keine CAN0-Werte, sondern Ersatzrechnungen aus `sdu_udc`·`sdu_idc` bzw. feste Platzhalter (SoC 28 %, 193 kW).
- Uhr/Trip nutzt die Laufzeit des Umrichters als Fahrzeit, Uhr/Runde `cluster_counter` und `acceleration` als Rundenzahl und Delta.
- Fzg/Laden bezeichnet das Tankanzeigen-Poti als „Pilot output“.
- Doppelte Namen in der Signaltabelle (Abschnitt 3 und 5.1, Aliasse in Klammern) und LTE-Felder sind Kandidaten zum Aufräumen.

Offene Entscheidungen:
1. Seitenaufteilung der UI (Spalte „UI Vorschlag“).
2. ~~Engineering-Modus: alle Signale anzeigbar~~ erledigt: Seite [E]Signals (VCU-REQ-022).
3. PWA auf die Firmware-Namen umstellen und Platzhalter durch „--“ ersetzen.
4. Welche Werte aus Abschnitt 9 der Zombie-Slave senden soll und welche die VCU selbst berechnet.
5. Welche Werte die VCU für die TCU sendet (VCU-REQ-013); ID-Konflikt 0x210 (TCU-Demo-Map `WakeCommand` gegen Tesla-DC/DC).
