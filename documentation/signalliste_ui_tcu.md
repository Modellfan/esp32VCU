# Signalliste VCU, Touch-UI und TCU (Stand 2026-11-11)

Übersicht über alle Werte, die die VCU kennt, sendet oder braucht, mit Quelle, Masterreferenz und Ort in der Touch-UI. **Diese Liste ist eine Zusammenfassung, keine Masterquelle:** Bitlage, Skalierung, Zyklus und Wertetabellen stehen in den Referenzen der Tabelle unten. Bei Widerspruch gilt die Referenz.

Vollständigkeit: Abschnitt 2–8 enthalten alle 280 Einträge der Firmware-Signaltabelle (`src/services/signal_table.cpp`) und die übrigen Felder von `/api/live` (`src/services/webinterface.cpp`); Familien gleicher Struktur stehen in einer Zeile. Abschnitt 9 listet Werte, die UI, TCU oder VCU-Funktionen brauchen, die aber noch kein Steuergerät liefert.

## 1. Legende

### Referenzen (Masterquellen)

| Kürzel | Datei | Inhalt |
|---|---|---|
| CS | [can_signals.md](can_signals.md) | maßgeblich für CAN1/CAN2 (986-Fahrzeug, DME-Emulation, OpenInverter) |
| DBC986 | [porsche_986_996_compiled_working.dbc](porsche_986_996_compiled_working.dbc), Hintergrund [PQ_messages.md](PQ_messages.md), [vw_pq.dbc](vw_pq.dbc) | 986/996- und PQ-Botschaften |
| ZS | [ZombieSlave_teensybms_dcdc_mvcu.dbc](ZombieSlave_teensybms_dcdc_mvcu.dbc) (Kopie von `Modellfan/Zombie-Slave` 205cc7c, Master im Zombie-Slave-Repo `documentation/`), Kurzfassung in CS, Abschnitt CAN0 | CAN0: TeensyBMS, Zombie-Slave, Tesla-DC/DC, mVCU-Heizung |
| BMS | `TeensyVCU/documentation/CAN_Message_Packing.md` | Packing und CRC der TeensyBMS-Botschaften 0x41A–0x41E |
| mVCU | `Zombie-Slave/docs/mVCU_Integration.md` | Schnittstelle Zombie-Slave ↔ mVCU (diese VCU), Heizung 0x43A |
| TCU | `ESP32S3-TCU/specs/001-vehicle-companion-spec/data-model.md`, `ESP32S3-TCU/data/can/eboxster-demo.dbc.json` | Werte, die die TCU erwartet |
| REQ / DEC | [REQUIREMENTS.md](../REQUIREMENTS.md), [DECISIONS.md](../DECISIONS.md) | Soll-Verhalten der internen Funktionen |

### Spalten

- **Richtung:** RX = von einem Steuergerät empfangen · TX = von der VCU gesendet · intern = in der VCU berechnet · Einst. = Einstellung über `/api/set` · Ausg. = Hardware-Ausgang der VCU · Diag. = Diagnose der VCU selbst. **gesp.** = im EEPROM gespeichert (VCU-REQ-020).
- **Veraltet nach:** empfangene Werte gelten nach 500 ms ohne Botschaft als `stale`, 0x41D/0x41E nach 2,5 s (`rxTimeoutMs()`).
- **UI heute:** wo die PWA den Wert heute tatsächlich anzeigt (`pwa/src/js/app.js`). Alte, nicht mehr erreichbare Konfigurationen (Bereich `sdu`, alte Zeilen der Seiten Vehicle, Ventilation und Stopwatch) zählen nicht.
  - Seiten: **Home** · **Fzg/Motor**, **Fzg/Akku**, **Fzg/Laden**, **Fzg/Fzg** (Seite Vehicle, Tabs Engine/Battery/Charger/Vehicle) · **Kühl** (Coolant) · **EFluss** (Energiefluss) · **Uhr/Trip**, **Uhr/Beschl**, **Uhr/Runde** (Stopwatch) · **Stil** (Porsche-Referenztest) · **[E]Cluster/…**, **[E]Adjust/…** (nur Engineering-Modus).
  - Zusatz **(E)** = Zeile nur im Engineering-Modus.
- **UI Vorschlag:** **nicht entschieden**, Arbeitsgrundlage für die Seitenaufteilung. Im Engineering-Modus sollen zusätzlich **alle** Signale anzeigbar sein (Abschnitt 10), das steht nicht in jeder Zeile.
- **TCU:** ✓ = die TCU braucht den Wert (Quelle TCU).

## 2. Fahrzeug-CAN 986 (CAN1, empfangen)

Referenz CS, DBC986; offene Dekodierung: CS, Abschnitt „Open decoding work, Porsche 986“. Mitschnitt Stand `can_logs/986_log_stand.csv`: 0x0C0, 0x1A0, 0x2A8, 0x4A1, 0x510, 0x520 vorhanden; **0x440 (PRND) und 0x560 fehlen** (offen: hat das Fahrzeug ein Getriebesteuergerät?).

| API-Name | Bedeutung | Einheit | Quelle | Richtung | UI heute | UI Vorschlag | TCU |
|---|---|---|---|---|---|---|---|
| `vref_kmh` | Referenzgeschwindigkeit | km/h | ABS/PSM 0x1A0 | RX | Fzg/Fzg, Uhr/Trip (Ersatz für 0x510) | Home, Uhr | ✓ speed |
| `vehicle_speed_510` | Geschwindigkeit Kombi | km/h | Kombi 0x510 | RX | Fzg/Fzg, Uhr/Trip, [E]Adjust/Drive, [E]Cluster/Power, Cluster/Accel; Ladeerkennung der UI | Fzg/Fzg | |
| `wfl_legacy_kmh`, `wfr_legacy_kmh`, `wrl_legacy_kmh`, `wrr_legacy_kmh` | Radgeschwindigkeiten | km/h | ABS 0x2A8 | RX | – | Uhr (Schlupf) | |
| `abs`, `tcs`, `msr`, `abd`, `fdr` | Regeleingriff ABS/ASR/MSR/EDS/PSM | bool | 0x1A0 | RX | – | Uhr, Fzg/Fzg | |
| `asr_mode`, `tcs_slow_pct`, `tcs_fast_pct`, `msr_inv_pct`, `itq_pct` | ASR-Modus, Eingriffsmomente (**`itq_pct` ist ein Zähler**, can_signals.md To-do 1) | –/% | 0x1A0 | RX | – | – | |
| `abs_lamp`, `brake_lamp`, `fdr_lamp`, `fdr_lamp2`, `psm_button_lamp` | Warnlampen | bool | 0x1A0 | RX | [E]Cluster/Inputs (`fdr_lamp`, `psm_button_lamp`) | Home (Warnsymbol) | ✓ |
| `brake_sw`, `brake_sw_inv` | Bremslichtschalter | bool | 0x1A0 | RX | – | – | |
| `handbrake` | Handbremse | bool | 0x1A0 | RX | – | Home (Hinweis) | ✓ |
| `lat_g` | Querbeschleunigung | g | 0x1A0 | RX | – | Uhr | |
| `br1_counter`, `br1_asr_esp`, `br1_espasr_passive`, `br1_sta_esp`, `br1_ersatz_kmh`, `rough_road_suppression`, `rough_road_suppression_status` | Zähler und Statusbits Bremse_1 | – | 0x1A0 | RX | [E]Cluster/Inputs (`rough_road_suppression`) | – | |
| `acceleration` | Längsbeschleunigung | g | ABS 0x4A1 | RX | Fzg/Fzg; Uhr/Runde (als Ersatz für „Delta“, Fehler) | Uhr | |
| `yaw_rate`, `yaw_rate_sign` | Gierrate | °/s | 0x4A1 | RX | – | Uhr | |
| `driver_braking` | Fahrer bremst (**falsch dekodiert: Zählerbit**, can_signals.md To-do 2) | bool | 0x4A1 | RX | [E]Adjust/Info, [E]Cluster/Inputs | – | |
| `brake_normal_or_offset` | Bremsdruck/Offset | % | 0x4A1 | RX | [E]Adjust/Drive | – | |
| `no_actively_built_up_brake_pressure`, `acc_related_error`, `receipt_for_acc_message`, `regulator_error_fdr`, `error_driver_brake_detection`, `multiplex_byte4` | Status- und Fehlerbits ABS_2 | – | 0x4A1 | RX | [E]Adjust/Info, [E]Cluster/Inputs (`receipt_for_acc_message`) | – | |
| `lenkwinkel_rb`, `vorzeichen_rb` | Lenkwinkel | ° | LWS 0x0C0 | RX | Fzg/Fzg | Uhr | |
| `lws_ok_rb`, `lws_abgleich_rb`, `fine_check_rb`, `coarse_check_rb`, `frei_lenkwinkel_1_1_rb` | Gültigkeit/Abgleich Lenkwinkel | bool | 0x0C0 | RX | – | – | |
| `indicator_left`, `indicator_right` | Blinker | bool | Kombi 0x510 | RX | Fzg/Fzg | Home | |
| `tempomat_button`, `steeringwheel_button_up`, `steeringwheel_button_down` | Tempomat- und Lenkradtasten | bool | 0x510 | RX | [E]Adjust/Info, [E]Cluster/Inputs | Bedienung der UI? (offen) | |
| `ambient_c` | Außentemperatur | °C | Kombi 0x520 | RX | [E]Cluster/Power | Home, Klima | ✓ |
| `light_dimmer` | Lichtdimmer | – | 0x520 | RX | [E]Adjust/Setup, [E]Cluster/Inputs | Nachtmodus der UI | |
| `oil_pressure_bar` | Öldruck | bar | 0x520 | RX | [E]Cluster/Power | – (kein Verbrenner) | |
| `cluster_counter` | Zähler Kombi | – | 0x520 | RX | [E]Adjust/Info, [E]Cluster/Inputs; Uhr/Runde (als Rundenzähler, Fehler) | – | |
| `prnd_pos` | Wählhebel P/R/N/D | enum | Getriebe 0x440 | RX | Fzg/Fzg | Home | ✓ |

## 3. Antriebsumrichter Tesla SDU (CAN2, OpenInverter, empfangen)

Referenz CS, Abschnitt OpenInverter stm32-sine. Mitschnitt `can_logs/20260419_fahrt_tesla.csv`: 0x500–0x505; **0x506–0x508 fehlen im Mitschnitt**.

| API-Name | Bedeutung | Einheit | Quelle | Richtung | UI heute | UI Vorschlag | TCU |
|---|---|---|---|---|---|---|---|
| `sdu_udc`, `sdu_idc` | DC-Spannung, DC-Strom | V, A | SDU 0x500 | RX | Fzg/Motor, Fzg/Akku, [E]Adjust/Power; UI rechnet daraus Leistung (Home, EFluss) | Fzg/Motor, EFluss | ✓ (Leistung) |
| `sdu_il1`, `sdu_il2` | Phasenströme | A | 0x500 | RX | Fzg/Motor (E) | – | |
| `sdu_speed` | Motordrehzahl | rpm | 0x501 | RX | Fzg/Motor | Fzg/Motor | |
| `sdu_fstat`, `sdu_uac`, `sdu_ilmax` | Frequenz, AC-Spannung, Stromgrenze | Hz, V, A | 0x501 | RX | Fzg/Motor (E) | – | |
| `sdu_il1rms`, `sdu_il2rms`, `sdu_boostcalc`, `sdu_fweakcalc` | RMS-Ströme, Boost, Feldschwächung | A, –, Hz | 0x502 | RX | Fzg/Motor (E) | – | |
| `sdu_pot`, `sdu_pot2`, `sdu_potnom` | Gaspedal roh / nominal | dig, % | 0x503 | RX | Fzg/Motor (`potnom`; `pot`, `pot2` E) | Fzg/Motor (Pedalbalken) | |
| `sdu_amp`, `sdu_angle` | Stellgröße, Rotorwinkel | –, ° | 0x503 | RX | Fzg/Motor (E) | – | |
| `sdu_regenpreset` | Rekuperationsstufe | % | 0x504 | RX | Fzg/Motor | Fzg/Motor | |
| `sdu_seldir`, `sdu_rotordir` | Fahrtrichtung gewählt / Rotor | enum | 0x504 | RX | Fzg/Motor (`rotordir` E) | Home (D/N/R) | ✓ |
| `sdu_tmphs`, `sdu_tmpm` | Kühlkörper-, Motortemperatur | °C | 0x504 | RX | Fzg/Motor, [E]Cluster/Thermal | Fzg/Motor, Kühl | ✓ |
| `sdu_uaux` | 12-V-Spannung am Umrichter | V | 0x504 | RX | Fzg/Motor, Fzg/Akku | Bordnetz | ✓ (Ersatz für 12-V-Batterie) |
| `sdu_opmode` | Betriebsart | enum | 0x505 | RX | Fzg/Motor | Fzg/Motor | ✓ |
| `sdu_lasterr` | letzter Fehler | enum | 0x505 | RX | Fzg/Motor (E) | Home (Warnung) | ✓ |
| `sdu_status`, `sdu_canio`, `sdu_pwmio`, `sdu_cpuload` | Statusbits, CAN-/PWM-Eingänge, CPU-Last | Bits, % | 0x505 | RX | Fzg/Motor (E) | – | |
| `sdu_din_cruise`, `sdu_din_start`, `sdu_din_brake`, `sdu_din_mprot`, `sdu_din_forward`, `sdu_din_reverse`, `sdu_din_emcystop`, `sdu_din_ocur` | digitale Eingänge A | enum | 0x506 | RX | Fzg/Motor (E) | – | |
| `sdu_din_desat`, `sdu_din_bms`, `sdu_cruisespeed` | digitale Eingänge B, Tempomatdrehzahl | enum, rpm | 0x507 | RX | Fzg/Motor (E) | – | |
| `sdu_turns`, `sdu_uptime`, `sdu_uptime_s` | Umdrehungen, Laufzeit | –, 10 ms, s | 0x508 | RX | Fzg/Motor (E); Uhr/Trip nutzt `sdu_uptime` als Fahrzeit (Fehler) | – | |
| `sdu_regenpresent`, `sdu_regenpresent32`, `sdu_seldir43`, `sdu_temp_heatsink`, `sdu_temp_heatsink74`, `sdu_din_ocur51` | doppelte bzw. unklare Einträge | – | 0x504/0x506 | RX | – | aufräumen | |

## 4. CAN0: TeensyBMS, Zombie-Slave, Tesla-DC/DC (empfangen)

Referenz ZS (Bitlage), BMS (Packing, CRC), mVCU. Decoder `src/comms/battery_messages.cpp`, Frames mit falscher CRC werden verworfen. Empfang am Prüfstand bestanden 2026-11-10 (TC-BAT-01), **am echten Bus noch nicht**.

| API-Name | Bedeutung | Einheit | Quelle | Richtung | UI heute | UI Vorschlag | TCU |
|---|---|---|---|---|---|---|---|
| `battery_pack_voltage_v`, `battery_pack_current_a` | Packspannung, Packstrom | V, A | TeensyBMS 0x41A | RX | – | Fzg/Akku, EFluss | ✓ |
| `battery_cell_v_min`, `battery_cell_v_max` | min./max. Zellspannung | V | 0x41A | RX | – | Fzg/Akku | |
| `battery_cell_t_min_c`, `battery_cell_t_max_c` | min./max. Zelltemperatur | °C | 0x41B | RX | – | Fzg/Akku | ✓ |
| `battery_balancing_v`, `battery_cell_delta_mv` | Balancing-Spannung, Zelldifferenz | V, mV | 0x41B | RX | – | Fzg/Akku | |
| `battery_pack_power_kw` | Packleistung | kW | 0x41B | RX | – | Home, EFluss | ✓ |
| `battery_max_discharge_a`, `battery_max_charge_a` | Strombegrenzung Entladen/Laden | A | 0x41C | RX | – | Fzg/Akku | |
| `battery_contactor_state` | Schützzustand | enum | 0x41C | RX | – | Fzg/Akku | |
| `battery_dtc_flags` | BMS-Fehlerflags | Bits | 0x41C | RX | – | Home (Warnung), Fzg/Akku | |
| `battery_soc_pct`, `battery_soh_pct` | Ladestand, Gesundheit | % | 0x41D (veraltet nach 2,5 s) | RX | – (UI sucht `battery_soc` u. a.) | Home, Fzg/Akku, Fzg/Laden | ✓ |
| `battery_balancing_status`, `battery_bms_state` | Balancing, BMS-Zustand | enum | 0x41D | RX | – | Fzg/Akku | |
| `battery_avg_power_kw` | mittlere Leistung | kW | 0x41E (2,5 s) | RX | – | Uhr/Trip | |
| `battery_remaining_time_s` | Restzeit | s | 0x41E | RX | – | Fzg/Laden | ✓ |
| `battery_remaining_energy_kwh` | Restenergie | kWh | 0x41E | RX | – | Fzg/Akku | |
| `battery_vehicle_state` | Fahrzeugzustand (SLEEP … DRIVE, CHARGE, ERROR) | enum | Zombie-Slave 0x437 | RX | – | Home | ✓ vehicleReady |
| `battery_charging_active` | lädt (Zustand CHARGE) | bool | 0x437 | RX (abgeleitet) | – (UI sucht `charging_active`, `is_charging`) | Home, Fzg/Laden, EFluss | ✓ |
| `battery_hv_request`, `battery_force_shutdown` | HV-Anforderung, Zwangsabschaltung | – | 0x437 | RX | – | – | |
| `battery_charge_power_w`, `battery_charge_power_max_w` | Ladeleistung Ist/Max | W | 0x438 | RX | – (UI sucht `charge_power_kw`) | Fzg/Laden, EFluss | ✓ |
| `heater_active`, `heater_contactor_closed`, `heater_fault`, `heater_thermal_ok`, `heater_contactor_out`, `heater_can_request` | Heizung: aktiv, Schütz, Fehler, Thermoschalter, Ausgang, empfangene Anforderung | bool | 0x439 | RX | – | Klima | ✓ (Klima) |
| `dcdc_output_v`, `dcdc_output_a`, `dcdc_input_w` | DC/DC Ausgang, Eingangsleistung | V, A, W | Tesla-DC/DC 0x210 | RX | – | Bordnetz, EFluss | |
| `dcdc_coolant_c`, `dcdc_coolant_request` | DC/DC Kühlmitteltemperatur, Kühlanforderung | °C, bool | 0x210 | RX | – | Kühl | |
| `dcdc_fault_bits` | DC/DC Fehlerbits | Bits | 0x210 | RX | – | Bordnetz (Sammelwarnung) | |
| `battery_plug_connected` | Ladestecker gesteckt | bool | **fehlt** (nur intern im Zombie-Slave, Abschnitt 9) | – | – | Fzg/Laden | ✓ |
| `battery_crc_errors` | verworfene CAN0-Frames | Anzahl | VCU | Diag. | – | – | |

## 5. Von der VCU gesendet

### 5.1 DME-Emulation auf CAN1 (0x280, 0x289, 0x298, 0x4E0, 0x7A0)

Referenz CS, Abschnitt Porsche 986 ECU. Die VCU ersetzt das Motorsteuergerät; Werte sind Ausgaben ans Kombi. Setzbar über `/api/set-ecu` (W_ECU) bzw. als Override (OVR).

| API-Name | Bedeutung | Einheit | Quelle | Richtung | UI heute | UI Vorschlag | TCU |
|---|---|---|---|---|---|---|---|
| `engine_speed_rpm` (= `rpm`, `set_rpm`) | Drehzahlnadel (zeigt Leistung) | rpm | intern aus `cluster_needle_position`, OVR | TX 0x280 | – | – | |
| `throttle_position_280` (= `throttle_pct`), `pedal_position_1`, `pedal_position_2` (= `pedal1_pct`, `pedal2_pct`) | Drosselklappe, Pedal | % | Vorgabe, W_ECU | TX 0x280 | [E]Adjust/Drive | – | |
| `motor2_coolant_temperature` (= `coolant_c`, `set_coolant`) | Kühlmittelanzeige Kombi | °C | intern aus Motor-/Kühlkörpertemp., OVR | TX 0x289 | [E]Cluster/Thermal | – | |
| `motor2_coolant_temp_not_ok` (= `coolant_level`) | Kühlmittelwarnung | bool | intern, OVR | TX 0x289 | – | – | |
| `motor2_brake_light_switch`, `motor2_brake_test_switch`, `motor2_ac_compressor_active`, `motor2_normal_operation_active`, `motor2_drivetrain_coordinator_active`, `motor2_cruise_control_status`, `motor2_speed_limiter_selected`, `motor2_cruise_control_regulating` (= `cruise_active`), `motor2_sport_error`, `motor2_offroad_mode_active`, `motor2_cruise_target_speed_kmh`, `motor2_idle_target_speed_rpm`, `motor2_limited_torque_percent`, `motor2_min_torque_ignition_retard_percent` | Statusbits und Werte Motor_2 | – | Vorgabe, W_ECU | TX 0x289 | – | – | |
| `inlet_air_temperature` (= `iat_c`), `barometric_pressure_mbar` (= `baro_mbar`), `thr1_298_pct`, `thr2_298_pct`, `v298_raw` | Ansauglufttemperatur, Luftdruck, Werte 0x298 | °C, mbar, % | Vorgabe, W_ECU | TX 0x298 | [E]Cluster/Thermal (`inlet_air_temperature`) | – | |
| `check_engine_light`, `check_engine_light_bit1`, `engine_stat_bool2`, `reduced_power`, `fan_error`, `engine_stat_bool5`, `engine_stat_bool6`, `engine_stat_bool7` | Warnlampen und Status Motor | bool | Vorgabe, W_ECU | TX 0x4E0 | – | – | |
| `mo5_vorgluehen`, `mo5_e_gas`, `mo5_obd_2` (= `mil`), `mo5_heissl`, `mo5_klimakompr`, `mo5_feld_kuehl`, `mo5_kliko_red`, `mo5_ueberlverb` | Lampen- und Statusbits Motor_5 | bool | Vorgabe, W_ECU | TX | – | – | |
| `mo5_verbrauch_ul` (= `set_oil`) | Verbrauchszähler (Bordcomputer) | µl | intern aus `cluster_total_fuel_l`, OVR | TX | [E]Cluster/Power | – | |

### 5.2 Heizungsanforderung auf CAN0

| API-Name | Bedeutung | Einheit | Quelle | Richtung | UI heute | UI Vorschlag | TCU |
|---|---|---|---|---|---|---|---|
| `heater_request` | Heizung einschalten (an Zombie-Slave, 100 ms, Zähler + CRC; nach Start aus, nicht gespeichert) | bool | Einst. (Testseite `/io`) | TX 0x43A (Referenz ZS, mVCU; VCU-REQ-021) | – (nur `/io`) | Klima (Schalter) | ✓ (Klima) |

## 6. Interne Funktionen und Ausgänge der VCU

Referenz REQ und Code. Die Pins stehen in `src/board.h`.

| API-Name | Bedeutung | Einheit | Quelle | Richtung | UI heute | UI Vorschlag | TCU |
|---|---|---|---|---|---|---|---|
| `coolant_fan_mode` | Lüftermodus automatisch/manuell | enum | Einst. über `coolant_fan_level` | Einst., gesp. | – | Kühl | |
| `coolant_fan_level` | wirksame Lüfterstufe 0/1/2; Setzen schaltet auf manuell | 0–2 | intern | Einst. | Kühl | Kühl | |
| `coolant_fan_level_manual`, `coolant_fan_level_auto` | Stufe manuell (gesp.) / automatisch aus `sdu_tmpm`, `sdu_tmphs` | 0–2 | intern | intern | – | Kühl (E) | |
| `coolant_fan_low_output`, `coolant_fan_high_output` | Ausgang Lüfterstufe 1 / 2 (**Pin Stufe 1 fehlt**) | bool | `coolant_fan_control.cpp` | Ausg. | Kühl | Kühl | |
| `coolant_fan_outputs_enabled` | Lüfterausgänge zugeordnet | bool | `board.h` | Diag. | – | – | |
| `digital_pot_request_pct` (API-Setzname `digital_pot_pct`) | angeforderte Tankanzeige (X9C103S ersetzt den Tankgeber) | % | Einst. | Einst. | Fzg/Laden (Bezeichnung „Pilot output“, falsch), [E]Adjust/Setup | Kühl/Kombi (E) | |
| `digital_pot_pct`, `digital_pot_wiper` | eingestellte Position, Schleiferstufe | %, 0–99 | `digipot_control.cpp` | Ausg. | – | – | |
| `digital_pot_enabled` | Poti-Pins zugeordnet (**INC/UD fehlen**, VCU-REQ-009) | bool | `board.h` | Diag. | – | – | |
| `target_charge_pct` | Ziel-Ladestand | % | Einst. | Einst., gesp. | Fzg/Laden, [E]Adjust/Setup; Stil (nur lokal) | Fzg/Laden | |
| `ac_current_limit_a` | AC-Ladestromgrenze (**geht heute an kein Steuergerät**, Abschnitt 9) | A | Einst. | Einst., gesp. | Fzg/Laden, [E]Adjust/Setup | Fzg/Laden | |
| `cluster_activated` | Kombi-Ansteuerung aktiv (`/api/set-cluster`) | bool | Einst. | Einst. | [E]Cluster/State | – | |
| `cluster_power_percent_max`, `cluster_power_percent_dyn` | Leistung in % für die Drehzahlnadel | % | intern aus `sdu_udc`, `sdu_idc` | intern | [E]Cluster/Power | – | |
| `cluster_needle_position` | Nadelstellung | – | intern | intern → `engine_speed_rpm` | [E]Cluster/State | – | |
| `cluster_total_fuel_l` | Tankinhalt fürs Kombi (heute fest) | l | intern | intern → `mo5_verbrauch_ul` | [E]Cluster/State | – | |
| `cluster_heatsink_temp_critical`, `cluster_motor_temp_critical` | Temperaturgrenze überschritten | bool | intern aus `sdu_tmphs`, `sdu_tmpm` | intern → Kühlmittelanzeige | [E]Cluster/State, /Thermal | Home (Warnung) | |
| `cluster_acceleration_run_active`, `cluster_acceleration_run_time_s` | Beschleunigungsmessung läuft, aktuelle Zeit | bool, s | intern aus `vehicle_speed_510` | intern | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl | |
| `cluster_acceleration_0_50_last_s`, `cluster_acceleration_0_100_last_s` | letzte Zeit 0–50 / 0–100 | s | intern | intern | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl | |
| `cluster_acceleration_0_50_best_s`, `cluster_acceleration_0_100_best_s`, `cluster_acceleration_0_50_valid`, `cluster_acceleration_0_100_valid` | Bestzeit, gültig | s, bool | intern | intern, gesp. | Uhr/Beschl, [E]Cluster/Accel | Uhr/Beschl | |

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
| `sta_connected`, `sta_ssid`, `sta_ip`, `ap_ssid`, `ap_ip`, `ap_clients`, `network_mode`, `network_label` | WLAN-Zustand | Netzwerkfenster (Uhrzeit oben rechts), [E]Adjust, [E]Cluster/State | |
| `lte_connected`, `lte_operator`, `lte_signal` | LTE | – | entfällt (Radio nur per WLAN), Kandidat zum Entfernen |
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
| Kilometerstand | TCU odometer | vielleicht 0x520 Byte 5 (gemultiplext, can_signals.md To-do 5); sonst VCU summiert auf (nicht eichfähig) | offen |
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
2. Engineering-Modus: alle Signale anzeigbar, z. B. als eigene Seite, die die Signaltabelle der Firmware nach Gruppen auflistet.
3. PWA auf die Firmware-Namen umstellen und Platzhalter durch „--“ ersetzen.
4. Welche Werte aus Abschnitt 9 der Zombie-Slave senden soll und welche die VCU selbst berechnet.
5. Welche Werte die VCU für die TCU sendet (VCU-REQ-013); ID-Konflikt 0x210 (TCU-Demo-Map `WakeCommand` gegen Tesla-DC/DC).
