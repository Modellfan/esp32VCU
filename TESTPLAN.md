# Testplan – esp32VCU

Legt fest, **wie** die Anforderungen aus [REQUIREMENTS.md](REQUIREMENTS.md) nachgewiesen werden. Ein Requirement wird erst `verified`, wenn die zugeordneten Testfälle mit Evidenz (Pfad, Lauf, Datum) bestanden sind.

Stand 2026-10-28: Firmware- und SPIFFS-Build grün; PWA-Suite 28/28 (zwei Läufe). **Kein Test auf Hardware**: Die VCU war nicht angeschlossen.

## 1. Teststufen

| Stufe | Kürzel | Gegenstand | Werkzeug |
|---|---|---|---|
| Build | B | Firmware, SPIFFS-Image, Pin-Prüfung | `pio run -e esp32s3`, `pio run -t buildfs`, `python pwa/tools/build.py` |
| Web | W | PWA gegen VCU-ähnlichen Mock mit Fehlereinspeisung | `python -m pytest pwa/tests` (Chromium, 1024×600) |
| SIL | S | Einzelne Komponenten auf Applikationsebene (Parameter rein/raus, kein CAN), optional mit Python-Umgebungsmodell | `python -m pytest sil/tests` (siehe `sil/README.md`) |
| Prüfstand | P | VCU mit beiden CAN-Bussen, Mitschnitt, Messgeräte | SavvyCAN/CAN-Adapter, Multimeter, Browser |
| Fahrzeug | F | VCU im 986 | Sichtprüfung Kombiinstrument, Mitschnitt |

Host-Tests laufen über die SIL-Umgebung (`sil/`, Compiler aus dem PyPI-Paket `ziglang`). Bisher abgedeckt: Kombi-Funktion; Lüfterlogik und Signaltabelle sind die nächsten Kandidaten.

## 2. Zuordnung

| REQ | Testfall | Stufe | Stand |
|---|---|---|---|
| VCU-REQ-001 | TC-ECU-01: 10 s Mitschnitt, 5 IDs je 30 ms ± 3 ms, Inhalt nach DBC | P | offen |
| VCU-REQ-002 | TC-ECU-02: Einschalten ohne Heim-WLAN, erste 0x280 ≤ 1 s | P | offen |
| VCU-REQ-003 | TC-RX-01: Mitschnitt einspielen, `/api/live` gegen DBC | P | offen |
| VCU-REQ-004 | TC-RX-02: Umrichter-Mitschnitt, `sdu_*` gegen OpenInverter-Werte | P | offen |
| VCU-REQ-005 | TC-CL-01: Leistung 0/50/100 % und Rekuperation → Zeigerstellung; 160 °C → Warnung | S/P/F | SIL TC-SIL-01…15 bestanden 2026-11-05 (`sil/tests`); Befund Richtung (TC-SIL-02b/c); P/F offen |
| VCU-REQ-006 | TC-CL-02: Verbrauch über Fahrt gegen Antriebsenergie | S/F | SIL: TC-SIL-17 xfail (Verbrauch noch simuliert); Umrechnung offen |
| VCU-REQ-007 | TC-CL-03: Geschwindigkeitsverlauf einspielen, Zeiten ± 0,1 s | S/P | SIL TC-SIL-11…13 bestanden 2026-11-05; P offen |
| VCU-REQ-008 | TC-OUT-01: Stufen 0/1/2 an den Ausgängen, CAN 2 läuft weiter; Automatik mit Hysterese | B/P | Pin-Doppelbelegung → Build-Fehler: bestanden 2026-10-28; Rest offen |
| VCU-REQ-009 | TC-OUT-02: 0/50/100 % → Widerstand am Schleifer | P | offen |
| VCU-REQ-010 | TC-API-01: `fields`-Filter, Setzen mit Antwort, `rejected`, `/api/signals` | W/P | Format im Mock geprüft (W); Firmware offen |
| VCU-REQ-011 | TC-UI-01..: alle Seiten 1024×600, Fehlerfälle, Setzbefehle, Langlauf | W | 28/28 bestanden (`pwa/tests/test_pwa.py`), nur Chromium |
| VCU-REQ-012 | TC-GVRET-01: SavvyCAN zeigt Frames beider Busse und gesendete Frames | P | offen |
| VCU-REQ-013 | TC-STALE-01: CAN 2 trennen → `sdu_*` nach 1 s in `stale` | P | offen |
| VCU-REQ-014 | TC-NET-01: ohne Heim-WLAN AP ≤ 5 s sichtbar, `/` erreichbar; mit Heim-WLAN zusätzlich erreichbar | P | offen |
| VCU-REQ-015 | TC-SEC-01: `git grep` nach Heim-Passwort leer; frischer Klon baut mit Vorlage | B | `git grep` leer 2026-10-28; Klon-Build offen |
| VCU-REQ-016 | TC-SEC-02: ohne Passwort keine Verbindung zum AP | P | offen |
| VCU-REQ-017 | TC-CHG-01: Ladeparameter wirken am Ladegerät | F | blockiert (kein Ladegerät angebunden) |
| VCU-REQ-018 | TC-BAT-01: BMS/Lader auf CAN 3 → Werte mit gültigem Alter | P | blockiert (Pins, Botschaften offen) |
| VCU-REQ-019 | TC-LED-01: Sichtprüfung 1 Hz grün | P | offen |

## 3. Abnahmeregeln
- Prüfstand und Fahrzeug: Mitschnitt oder Foto mit Datum im Implementierungsprotokoll verlinken.
- Vor jedem Flashen ins Fahrzeug: TC-ECU-01 auf dem Prüfstand bestanden.
