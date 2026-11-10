# Projektregeln – RE-Light (esp32VCU)

## Dokumentationsquellen
- `README.md`: Projektziel, Hardware, Build, Aufbau und offene Fragen.
- `REQUIREMENTS.md`: verbindlicher Soll-Zustand der VCU (Requirements `VCU-REQ-…`).
- `DECISIONS.md`: Begründungen und Konsequenzen wesentlicher technischer Entscheidungen (`VCU-DEC-…`).
- `TESTPLAN.md`: Testfälle je Requirement, Teststufen und Abnahmeregeln für `verified`.
- `documentation/can_signals.md` und die DBC-Dateien: CAN-Signaldefinitionen (maßgeblich für Kodierung und Dekodierung).
- `documentation/IMPLEMENTATION_LOG.md`: Änderungsprotokoll (siehe Arbeitsregel 8).
- Das Projekt ist eigenständig; es gibt keine Abhängigkeit zu anderen Repos (Nutzerentscheidung 2026-10-28).

Lies gezielt nur die für die Aufgabe benötigten Abschnitte. Erzeuge keine weiteren Spec-, Proposal-, Plan- oder Task-Dateien, außer der Nutzer verlangt dies ausdrücklich.

## Arbeitsregeln
1. **Kleine Änderungen:** Korrigiere reine Tippfehler, Refactorings ohne Verhaltensänderung und kleine Bugs ohne zusätzlichen RE-Prozess. Passe Dokumente nur bei geänderten Aussagen an.
2. **Neues oder geändertes Systemverhalten:** Verwende `/re-refine`. Ordne Anforderungen vorhandenen VCU-REQ-IDs zu oder ergänze neue IDs. Formuliere beobachtbare Akzeptanzkriterien. Erfinde keine bestätigten Anforderungen.
3. **Architekturentscheidungen:** Dokumentiere nur signifikante Entscheidungen in `DECISIONS.md`, insbesondere Änderungen von Schnittstellen, CAN-Botschaften, Persistenz, Zeitverhalten, Pinbelegung, Sicherheits- und Hardwaregrenzen.
4. **Vor riskanten oder größeren Änderungen:** Verwende `/re-review` und untersuche Abhängigkeiten, Widersprüche, Auswirkungen und Testbarkeit. Bei Unsicherheit betroffene Stellen als offen kennzeichnen statt als geklärt auszugeben.
5. **Implementierung:** Maßgeblich sind bestätigte Anforderungen und Entscheidungen. Verifiziere betroffene Akzeptanzkriterien mit passenden Tests oder beschreibe ehrlich, welche Prüfungen nicht durchgeführt wurden.
6. **Dokumentation nachziehen:** Aktualisiere nur die betroffenen Abschnitte. Keine Doppelpflege derselben Details in README, Requirements, Decisions und `can_signals.md`.
7. **Abschluss:** Berichte knapp: geändertes Verhalten, betroffene REQ-/DEC-IDs, Tests und offene Fragen.
8. **Änderungsprotokoll:** Jede Code- oder Flash-Aufgabe bekommt einen Eintrag in `documentation/IMPLEMENTATION_LOG.md` nach der dortigen Vorlage (Zeitstempel mit Zeitzone, Änderungen, ausgeführte Befehle mit PASS/FAIL, Ergebnis, Artefakte). Das Log ist Nachweisquelle, keine Anforderungsquelle.

## Projektspezifische Grenzen
- **Fahrzeugsicherheit:** Die VCU sendet auf dem Fahrzeug-CAN anstelle des Motorsteuergeräts. Änderungen an gesendeten Botschaften, Zeitschlitzen oder Standardbits nur mit Bezug auf `can_signals.md`/DBC und mit CAN-Mitschnitt prüfen. Nie ungeprüft auf das Fahrzeug flashen.
- **Pinbelegung:** Vor neuen Ausgängen alle belegten GPIOs in `src/board.h` prüfen (dort mit `static_assert` gegen Doppelbelegung).
- **Hardwarezugriff:** Am Prüfstand (COM10) nach Änderungen selbständig flashen (`--target upload`; nach PWA-Änderungen `python pwa/tools/build.py` und `--target uploadfs`) und mit `tools/vcu_cmd.py`/`vcu_log.py` prüfen, ohne vorher zu fragen (Nutzerentscheidung 2026-11-09). Nie ungeprüft auf das Fahrzeug flashen.
- **Keine Zugangsdaten im Repo:** WLAN-Passwörter und Schlüssel gehören in `src/secrets.h` (nicht versioniert, VCU-REQ-015).
- **Nur Referenz:** `documentation/touch-ui-reference/` und Bildmaterial von Porsche nur als Gestaltungsreferenz; geschützte Icon-Sätze bleiben außerhalb von Git (`.gitignore`).

## Delegation
- Sub-Agenten nur für unabhängige Recherche, Anforderungsanalyse, Architekturabgleich oder Review; die Integrationsverantwortung und die Entscheidung, was ins Repo geschrieben wird, bleiben beim Hauptagenten.
- Reviewer lesen nur und geben Befunde zurück. Keine parallelen Schreibzugriffe auf dieselben Dateien.
- Sub-Agenten legen keine Produkt- oder Architekturentscheidungen stillschweigend als verbindlich fest.

## Definition of Done
Eine verhaltensrelevante Änderung ist erst abgeschlossen, wenn Implementierung und betroffene Akzeptanzkriterien überprüft wurden, relevante Dokumente aktuell sind und nicht prüfbare Punkte explizit benannt wurden. `verified` nur bei tatsächlich vorhandener Evidenz setzen.

## Werkzeuge
- PlatformIO: `%USERPROFILE%\.platformio\penv\Scripts\platformio.exe` (nicht im PATH). Build: `platformio run -e esp32s3`; Upload: `platformio run --target upload --upload-port COM10`. Aus PowerShell starten, nicht aus Git Bash (`idf_tools.py` der Plattform bricht unter MSYS ab, danach fehlt esptool); `PYTHONIOENCODING=utf-8` setzen und die Ausgabe nicht filtern (pio stürzt sonst beim Fortschrittsbalken ab).
- Compiler-Problem der pioarduino-Plattform unter Windows und Abhilfe: README, Abschnitt „Build und Flashen“.
- Tests auf dem PC: SIL `python -m pytest sil/tests` (Compiler über das PyPI-Paket `ziglang`, siehe `sil/README.md`), PWA `python -m pytest pwa/tests` (Playwright). Eine PlatformIO-Umgebung `native` gibt es nicht.
- Touch-UI-Testserver: `python pwa/tools/serve.py` → http://127.0.0.1:8090/

## CAN-Hardwaretests
- Immer mit dem CANivore Py Test Tool (Submodul `tools/canivore_py_test_tool`, vorher `git submodule update --init --recursive`), gestartet über den Wrapper `.\tools\can_test.cmd <Befehl> [Optionen]` (eigene Python-Umgebung unter `.pio\canivore-py-test-tool-venv`).
- Prüfstand: CANable/SLCAN-Stick auf COM22 (`--port 22`, nur die Nummer), 500 000 bit/s; CAN1 und CAN2 der VCU hängen parallel am Stick, CAN0 (TWAI) nicht. Board (GVRET, Upload) auf COM10.
- Standardtest: `.\tools\can_test.cmd tx-pattern --port 22 --can-bitrate 500000 --tx-id 611 --stream-id <neu> --tx-count 100 --tx-interval-ms 10`. Für jeden Lauf eine neue Stream-ID.
- Bestanden ist ein Test erst durch den Empfänger, nicht durch „Sent N frames“. Bevorzugt das Textlog auf COM10 mitlesen: `.\.pio\canivore-py-test-tool-venv\Scripts\python.exe tools\vcu_log.py --port COM10 --duration <s> --diag-only` – in den `[DIAG]`-Zeilen muss `rx` von CAN1 um genau N steigen, von CAN2 um N plus die ECU-Frames (= Zuwachs von `cyc`); TEC, REC, EFLG 0, kein BUS-OFF, `ovr`/`txfail` unverändert. Für Sequenz/CRC zusätzlich `tools\gvret_listen.py --pattern-id <id>` (schaltet COM10 bis zum nächsten Reset auf GVRET; danach gibt es kein Textlog mehr).
- Befehle an die VCU (nur ohne GVRET-Binärmodus): `.\.pio\canivore-py-test-tool-venv\Scripts\python.exe tools\vcu_cmd.py version|wifi-status|reset|help`; Reset über RTS mit Bootlog: `tools\vcu_cmd.py --hard-reset --duration 10`.
- CANable-Fehlerregister `0x14`/`0x54` ist bei diesem Adapter oft gelatcht: nur Warnung, `--strict-adapter-errors` nicht verwenden.
- Logs abspielen: `tools/savvy_csv_to_asc.py` (CSV aus `documentation/can_logs/` → ASC, VCU-Sende-IDs mit `--exclude` entfernen), dann `can_test.cmd playback --playback-log-file …`.
- Änderungen am Tool zuerst im Submodul committen und pushen, danach den Submodul-Zeiger hier committen.
