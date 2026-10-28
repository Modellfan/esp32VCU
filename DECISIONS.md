# Technische Entscheidungen – esp32VCU

Hier stehen nur **wesentliche** Architektur- und Technologieentscheidungen. Keine Aufgabenlisten und keine Wiederholung der Anforderungen.

Status: `proposed` (Vorschlag), `accepted` (vom Nutzer bestätigt oder bestehende, bewusst weitergeführte Umsetzung), `superseded`. Stand 2026-10-28.

## VCU-DEC-001 – Plattform ESP32-S3 mit zwei MCP2515
- **Status:** accepted (Bestand)
- **Betroffene Requirements:** VCU-REQ-001, -003, -004
- **Entscheidung:** ESP32-S3-DevKitC-1 (8 MB Flash), zwei MCP2515 (8-MHz-Quarz, 500 kbit/s) an einem SPI-Bus, Arduino-Framework über PlatformIO (pioarduino-Plattform 55.03.37, Arduino-Core 3.3.7).
- **Konsequenzen:** Die Partitionstabelle `default_8MB.csv` lässt 1,5 MB für SPIFFS. Die pioarduino-Plattform installiert den Compiler auf Windows mit einer Ordnerebene zu viel (siehe README, Abschnitt Build).

## VCU-DEC-002 – Ebenenmodell nach AUTOSAR-Prinzip
- **Status:** accepted (Nutzerwunsch 2026-10-28)
- **Betroffene Requirements:** alle
- **Entscheidung:** Hardware → Treiber (`canmanager`, `can3_twai`, `coolant_fan_control`, `digipot_control`) → Kommunikation (Codecs `986_*_messages`, `tesla_sdu_messages`, `battery_messages`) → Daten (`params::*`, `vcu_params`, `signal_table`) → Anwendung (`vcu_app`, `cluster_calc`). Dienste daneben: Echtzeit-Task (`vcu_rt`), Webserver (`webinterface`), GVRET (`gvret_bridge`), Log (`vcu_log`). Bild: `documentation/architecture.svg`.
- **Regel:** Anwendung und Webserver lesen und schreiben nur Daten in `params`; nur die Treiberschicht berührt Hardware.

## VCU-DEC-003 – Echtzeit-Task getrennt von den Diensten
- **Status:** accepted (Vorschlag 1, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-001, VCU-REQ-002, VCU-REQ-014
- **Kontext:** Vorher lief alles in `loop()`. Der WLAN-Start blockierte bis 15 s, und jede Webanfrage verzögerte die Motorsteuergerät-Emulation.
- **Optionen:** A: FreeRTOS-Task mit festem Takt auf Kern 1, Dienste auf Kern 0; B: alles in `loop()` belassen und blockierende Stellen entfernen.
- **Entscheidung:** A. `vcu_rt` läuft alle 6 ms (`vTaskDelayUntil`) auf Kern 1 mit hoher Priorität: CAN empfangen, Anwendung rechnen, eine ECU-Botschaft senden, Ausgänge setzen. Webserver, GVRET und LED laufen in einer Task auf Kern 0. Die Task startet vor dem WLAN.
- **Konsequenzen:** `params` werden von zwei Kernen genutzt. Ein Mutex schützt sie; der Webserver kopiert sie in einen Schnappschuss und baut das JSON ohne Sperre. Laufzeitkennzahlen (`rt_cycles`, `rt_overruns`, `rt_max_cycle_us`) stehen in `/api/live`. Auf Hardware noch nicht gemessen.

## VCU-DEC-004 – Aktor-Sollwerte als Daten, Lüfterautomatik
- **Status:** accepted im Grundsatz (Vorschlag 2, Nutzer 2026-10-28); Schwellen und Fail-safe `proposed`
- **Betroffene Requirements:** VCU-REQ-008, VCU-REQ-009
- **Entscheidung:** Webserver und Anwendung schreiben nur `params::outputs` (Lüfter-Modus, Hand-Stufe, Poti-Sollwert). Die Anwendung berechnet die Automatik-Stufe; der Echtzeit-Task setzt die Ausgänge (Poti nur bei Änderung, ca. 2 ms).
- **Vorschläge (offen):** Schwellen Stufe 1 ab Motor 70 °C / Kühlkörper 50 °C, Stufe 2 ab 110 °C / 65 °C, Hysterese 5 K. Fehlen Umrichterdaten im Automatikbetrieb, gilt Stufe 2. Bis zur Bestätigung startet der Lüfter im Handbetrieb.
- **Konsequenzen:** Lüfterausgänge bleiben abgeschaltet, bis der Pin für Stufe 1 feststeht (VCU-DEC-009).

## VCU-DEC-005 – Override-Felder für Motorsteuergerät-Signale
- **Status:** accepted (Vorschlag 3, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-005, VCU-REQ-010
- **Kontext:** Kombi-Rechnung und `/api/set-ecu` schrieben dieselben Felder; Vorgaben aus der Oberfläche wurden alle 6 ms überschrieben.
- **Entscheidung:** Drehzahl, Kühlmitteltemperatur, Kühlmittelwarnung und Verbrauchszähler werden über `params::ecu_override` gesetzt. Vorrang: Override vor Rechnung vor Standardwert. Der Wert `auto` hebt den Override auf. Status in `/api/live` (`rpm_override` usw.).

## VCU-DEC-006 – Gültigkeit über Empfangszeitpunkte
- **Status:** accepted im Mechanismus (Vorschlag 4, Nutzer 2026-10-28); Zeitgrenze `proposed`
- **Betroffene Requirements:** VCU-REQ-013
- **Entscheidung:** Nach jeder erfolgreich dekodierten Botschaft wird ihr Empfangszeitpunkt gespeichert. Jedes Signal der Tabelle kennt seine Botschaft. `/api/live` listet veraltete oder nie empfangene Felder unter `stale` (Array). Nicht endliche Zahlen werden als `null` ausgegeben.
- **Vorschlag (offen):** einheitlich 500 ms für alle Botschaften, bis die Zykluszeiten dokumentiert sind.
- **Konsequenzen:** Die Werte bleiben für ältere Oberflächen unverändert im JSON; Oberflächen müssen `stale` auswerten, um „–“ zu zeigen (in der PWA noch nicht umgesetzt).

## VCU-DEC-007 – Zentrale Signaltabelle
- **Status:** accepted (Vorschlag 5, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-010
- **Entscheidung:** `src/signal_table.cpp` enthält je Signal Name, Datengruppe, Offset, Speichertyp, Ausgabeformat, Quellbotschaft und Schreibrecht. `/api/live`, `/api/set-ecu` und `/api/signals` (Liste aller Signale) entstehen daraus. Feldnamen sind unverändert zur alten `getLiveJson()`.
- **Konsequenzen:** Ein neues Signal braucht genau einen Tabelleneintrag. Zahlen werden beim Setzen auf Typ und Wertebereich geprüft; ungültige Schlüssel stehen in der Antwort unter `rejected`.

## VCU-DEC-008 – CAN 3 über den TWAI-Controller
- **Status:** accepted im Grundsatz (Vorschlag 6, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-018
- **Optionen:** A: eingebauter TWAI-Controller des ESP32-S3 mit Transceiver; B: dritter MCP2515 am SPI-Bus.
- **Entscheidung:** A. Treibergerüst `can3_twai` und Codec `battery_messages` sind vorhanden, aber inaktiv, bis Pins und Botschaften feststehen.

## VCU-DEC-009 – Pinbelegung zentral in `board.h`
- **Status:** accepted (Vorschlag 7, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-008, alle Hardware-REQs
- **Entscheidung:** Alle GPIOs stehen in `src/board.h`; ein `static_assert` bricht den Build bei Doppelbelegung ab (geprüft: Lüfter auf GPIO7 → Build-Fehler). GPIO7 ist CAN2-CS (Nutzer 2026-10-28). Nicht zugewiesene Pins (`GPIO_UNASSIGNED`) schalten die zugehörige Funktion ab.

## VCU-DEC-010 – GVRET als Beobachter, Log auf eigener Schnittstelle
- **Status:** accepted (Vorschlag 8, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-012
- **Entscheidung:** Der Echtzeit-Task legt jeden empfangenen und gesendeten Frame in eine Warteschlange (128 Einträge, bei Überlauf verworfen und gezählt); die Dienst-Task schreibt sie über UART0 an SavvyCAN, sobald GVRET im Binärmodus ist. Textausgaben gehen über den nativen USB-Port (USB-Serial-JTAG).
- **Offen:** Welcher der beiden COM-Ports (COM16/COM24) welcher Schnittstelle entspricht, ist nicht geprüft.

## VCU-DEC-011 – WLAN: Zugangspunkt plus Heimnetz, Zugangsdaten außerhalb von Git
- **Status:** accepted (Nutzerentscheidung 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-014, VCU-REQ-015, VCU-REQ-016
- **Entscheidung:** `WIFI_AP_STA`: eigener WPA2-Zugangspunkt (`eboxster`) immer an, Heim-WLAN zusätzlich, nicht blockierend mit automatischem Wiederverbinden. Zugangsdaten in `src/secrets.h` (git-ignoriert), Vorlage `src/secrets.example.h`. Wer im WLAN ist, darf Werte setzen.
- **Konsequenzen:** Das alte Heim-WLAN-Passwort bleibt in der Git-Historie (Repo laut Nutzer privat).

## VCU-DEC-012 – Touch-UI als PWA aus dem SPIFFS
- **Status:** accepted (Nutzerentscheidung 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-011
- **Entscheidung:** Die Touch-UI liegt als PWA in `pwa/`, wird mit `pwa/tools/build.py` nach `pwa/dist` gebaut (WebP, gzip, Budget 1,1 MB) und ist das SPIFFS-Image (`data_dir = pwa/dist`). Die Firmware liefert sie unter `/` aus (eigener Handler: `.gz`-Varianten, WebP-MIME-Typ). Die Handy-UI (`data/index.html`) ist entfernt; `data/touch*` bleibt als Vorlage.
- **Konsequenzen:** Ohne HTTPS registriert der Browser keinen Service Worker; die PWA läuft dann ohne Offline-Cache. Nach Änderungen an der UI: `build.py`, dann `uploadfs`.

## VCU-DEC-013 – VW-PQ-Codec bleibt im Repo
- **Status:** accepted (Nutzer 2026-10-28)
- **Entscheidung:** `src/vw_pq_messages.*` (ca. 10 000 Zeilen, ungenutzt) bleibt zum Abgleich mit den 986-Botschaften erhalten.
