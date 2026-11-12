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
- **Entscheidung:** Hardware → Treiber (`canmanager` mit CAN0/CAN1/CAN2, `coolant_fan_control`, `digipot_control`) → Kommunikation (Codecs `986_*_messages`, `tesla_sdu_messages`, `battery_messages`) → Daten (`params::*`) → Anwendung (`cluster_calc`). Dienste daneben: Webserver (`webinterface`, `signal_table`, `params_lock`), GVRET (`gvret_bridge`), Log (`vcu_log`). Bild: `documentation/architecture.svg`.
- **Regel:** Anwendung und Webserver lesen und schreiben nur Daten in `params`; nur die Treiberschicht berührt Hardware.
- **Geändert 2026-11-09 (VCU-DEC-015):** Keine eigene Datenschicht mehr: jedes Modul besitzt seine `params::*`-Gruppe; `vcu_rt`, `vcu_app` und `vcu_params` sind aufgelöst.

## VCU-DEC-003 – Echtzeit-Task getrennt von den Diensten
- **Status:** accepted (Vorschlag 1, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-001, VCU-REQ-002, VCU-REQ-014
- **Kontext:** Vorher lief alles in `loop()`. Der WLAN-Start blockierte bis 15 s, und jede Webanfrage verzögerte die Motorsteuergerät-Emulation.
- **Optionen:** A: FreeRTOS-Task mit festem Takt auf Kern 1, Dienste auf Kern 0; B: alles in `loop()` belassen und blockierende Stellen entfernen.
- **Entscheidung:** A. `vcu_rt` läuft alle 6 ms (`vTaskDelayUntil`) auf Kern 1 mit hoher Priorität: CAN empfangen, Anwendung rechnen, eine ECU-Botschaft senden, Ausgänge setzen. Webserver, GVRET und LED laufen in einer Task auf Kern 0. Die Task startet vor dem WLAN.
- **Geändert 2026-11-09 (VCU-DEC-015):** Die Task heißt CAN-Task und steht als `canTask()` in `src/main.cpp`; Takt, Kern, Priorität und Reihenfolge sind unverändert.
- **Konsequenzen:** `params` werden von zwei Kernen genutzt. Ein Mutex schützt sie; der Webserver kopiert sie in einen Schnappschuss und baut das JSON ohne Sperre. Laufzeitkennzahlen (`rt_cycles`, `rt_overruns`, `rt_max_cycle_us`) stehen in `/api/live`. Auf Hardware noch nicht gemessen.

## VCU-DEC-004 – Aktor-Sollwerte als Daten, Lüfterautomatik
- **Status:** accepted im Grundsatz (Vorschlag 2, Nutzer 2026-10-28); Schwellen und Fail-safe `proposed`
- **Betroffene Requirements:** VCU-REQ-008, VCU-REQ-009
- **Entscheidung:** Webserver und Anwendung schreiben nur Sollwerte in `params::fan` (Lüfter-Modus, Hand-Stufe) und `params::digipot` (Poti-Sollwert). `coolantFanAutoUpdate()` berechnet die Automatik-Stufe; der CAN-Task setzt die Ausgänge (Poti nur bei Änderung, ca. 2 ms).
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
- **Entscheidung:** `src/services/signal_table.cpp` enthält je Signal Name, Datengruppe, Offset, Speichertyp, Ausgabeformat, Quellbotschaft und Schreibrecht. `/api/live`, `/api/set-ecu` und `/api/signals` (Liste aller Signale) entstehen daraus. Feldnamen sind unverändert zur alten `getLiveJson()`.
- **Konsequenzen:** Ein neues Signal braucht genau einen Tabelleneintrag; eine neue Datengruppe genau einen Eintrag in `SIGNAL_GROUPS` (`signal_table.h`). Zahlen werden beim Setzen auf Typ und Wertebereich geprüft; ungültige Schlüssel stehen in der Antwort unter `rejected`.

## VCU-DEC-008 – CAN0 über den TWAI-Controller
- **Status:** accepted im Grundsatz (Vorschlag 6, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-018
- **Optionen:** A: eingebauter TWAI-Controller des ESP32-S3 mit Transceiver; B: dritter MCP2515 am SPI-Bus.
- **Entscheidung:** A. Treibergerüst `can3_twai` und Codec `battery_messages` sind vorhanden, aber inaktiv, bis Pins und Botschaften feststehen.
- **Geändert 2026-11-09 (Nutzer):** Der TWAI-Bus heißt CAN0, die MCP2515 heißen `can1`/`can2`. CAN0 sitzt im `canmanager` (kein eigenes Modul) und nutzt die Bibliothek ACAN_ESP32 (pierremolinaro, ab 3.0.3; seit 2026-11-10, vorher ESP32-TWAI-CAN wie auf `main`), Pins laut gebauter Platine TX = GPIO20, RX = GPIO19 (korrigiert 2026-11-09; `main` hatte auskommentiert 21/20). GPIO19/20 sind USB D-/D+: Der native USB-Port entfällt, das Textlog läuft über UART0. Botschaften von BMS/Lader weiter offen.

## VCU-DEC-009 – Pinbelegung zentral in `board.h`
- **Status:** accepted (Vorschlag 7, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-008, alle Hardware-REQs
- **Entscheidung:** Alle GPIOs stehen in `src/board.h`; ein `static_assert` bricht den Build bei Doppelbelegung ab (geprüft: Lüfter auf GPIO7 → Build-Fehler). GPIO7 ist CAN2-CS (Nutzer 2026-10-28). Nicht zugewiesene Pins (`GPIO_UNASSIGNED`) schalten die zugehörige Funktion ab.

## VCU-DEC-010 – GVRET als Beobachter, Log auf eigener Schnittstelle
- **Status:** accepted (Vorschlag 8, Nutzer 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-012
- **Entscheidung:** Der CAN-Task legt jeden empfangenen und gesendeten Frame in eine Warteschlange (128 Einträge, bei Überlauf verworfen und gezählt); die Dienst-Task schreibt sie über UART0 an SavvyCAN, sobald GVRET im Binärmodus ist. Textausgaben gingen über den nativen USB-Port; seit 2026-11-09 (CAN0 auf GPIO20 = USB D+) laufen sie über UART0, solange GVRET nicht im Binärmodus ist. Ebenso Textbefehle (Version mit Build-Datum, WLAN-Status, Reset) wie in der CANivore-Firmware; Bytes, die GVRET nicht auswertet, gehen an `serial_console`.
- **Geändert 2026-11-12:** UART0 läuft mit 2 000 000 Baud (`board::SERIAL_BAUD`, CH343 an COM10). Bei 115200 Baud passten nur etwa 570 GVRET-Frames/s durch, am Prüfstand kommen etwa 1000/s an; jetzt etwa 10 000/s. SavvyCAN und Terminal müssen auf dieselbe Rate gestellt werden. Die Frage nach COM16/COM24 ist damit hinfällig (nur noch COM10).

## VCU-DEC-011 – WLAN: Zugangspunkt plus Heimnetz, Zugangsdaten außerhalb von Git
- **Status:** accepted (Nutzerentscheidung 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-014, VCU-REQ-015, VCU-REQ-016
- **Entscheidung:** `WIFI_AP_STA`: eigener WPA2-Zugangspunkt (`eboxster`) immer an, Heim-WLAN zusätzlich, nicht blockierend mit automatischem Wiederverbinden. Zugangsdaten in `src/secrets.h` (git-ignoriert), Vorlage `src/secrets.example.h`. Wer im WLAN ist, darf Werte setzen.
- **Konsequenzen:** Das alte Heim-WLAN-Passwort bleibt in der Git-Historie (Repo laut Nutzer privat).

## VCU-DEC-012 – Touch-UI als PWA aus dem SPIFFS
- **Status:** accepted (Nutzerentscheidung 2026-10-28)
- **Betroffene Requirements:** VCU-REQ-011
- **Entscheidung:** Die Touch-UI liegt als PWA in `pwa/`, wird mit `pwa/tools/build.py` nach `pwa/dist` gebaut (WebP, gzip, Budget 1,1 MB) und ist das SPIFFS-Image (`data_dir = pwa/dist`). Die Firmware liefert sie unter `/` aus (eigener Handler: `.gz`-Varianten, WebP-MIME-Typ). Die Handy-UI (`data/index.html`) ist entfernt. Die frühere Touch-UI in `data/` ist seit 2026-11-09 entfernt (Nutzerentscheidung); `pwa/src` ist die einzige Quelle, Originalbilder liegen in `pwa/assets-src/`.
- **Offline-Cache (Nutzerentscheidung 2026-10-29):** Auf dem Android-Radio wird Chrome per Flag „Insecure origins treated as secure“ für `http://192.168.4.1` eingestellt; der Service Worker legt dann alle Dateien einschließlich aller Bilder im Cache ab (nacheinander, mit Wiederholung). Verworfene Alternativen: HTTPS auf der VCU mit eigener CA (TLS-Aufwand, Leistung, Radio-Kompatibilität), eigene Android-App (APK-Build und -Pflege). Zusätzlich tragen alle Dateiverweise einen Inhalts-Hash (`?v=`); die VCU liefert sie mit einem Jahr Cache-Dauer aus.
- **Konsequenzen:** Ohne das Flag läuft die PWA ohne Offline-Start. Das Flag ist Handarbeit am Radio und kann durch Chrome-Updates verloren gehen; die App zeigt den Zustand im Netzwerk-Fenster. Nach Änderungen an der UI: `build.py`, dann `uploadfs`.

## VCU-DEC-013 – VW-PQ-Codec bleibt im Repo
- **Status:** accepted (Nutzer 2026-10-28)
- **Entscheidung:** `src/vw_pq_messages.*` (ca. 10 000 Zeilen, ungenutzt) bleibt zum Abgleich mit den 986-Botschaften erhalten.

## VCU-DEC-014 – Software-in-the-Loop mit unverändertem Firmware-Code
- **Status:** accepted (Nutzerauftrag 2026-11-05)
- **Betroffene Requirements:** VCU-REQ-005, -006, -007 (Demo-Prüfling Kombi-Funktion), künftig alle Anwendungsfunktionen
- **Optionen:** A: einzelne Komponenten unverändert als PC-Bibliothek, Stubs für Arduino/FreeRTOS, C-Shim + Python (ctypes), Kopplung über die Signaltabelle, Tests auf Applikationsebene ohne CAN (Nutzervorgabe 2026-11-05); B: Funktion in Python nachbauen; C: `pio test -e native` (Unity, ohne Umgebungsmodell).
- **Entscheidung:** A, Ordner `sil/`. Compiler: Clang aus dem PyPI-Paket `ziglang` (kein Visual Studio/MinGW nötig).
- **Begründung:** Getestet wird der echte Code; die Signaltabelle ist bereits die Datenschnittstelle aller Module; ein Python-Modell erlaubt geschlossene Szenarien (Beschleunigung, Thermik).
- **Konsequenzen:** Komponenten, die in SIL laufen sollen, dürfen nur über `Arduino.h`-Grundfunktionen (Zeit) und den FreeRTOS-Mutex von der Plattform abhängen und tauschen Daten nur über `params` aus. CAN-Kodierung wird in SIL nicht geprüft. Zeit- und Treiberverhalten auf dem ESP32 bleiben Prüfstandsthema.

## VCU-DEC-015 – Ursprünglicher Aufbau mit eigener CAN-Task
- **Status:** accepted (Nutzerentscheidung 2026-11-09)
- **Betroffene Requirements:** VCU-REQ-001, -002, -008, -009, -010, -013, -014
- **Kontext:** Seit dem Umbau am 2026-10-28 (VCU-DEC-002/003) verteilte sich der Ablauf auf `vcu_rt`, `vcu_app` und `vcu_params`; Datengruppen lagen teils in den Modulen, teils zentral. Das war schwer zu überblicken und erzeugte Doppelungen.
- **Optionen:** A: zurück zum ursprünglichen Stil (bis 2026-10-27: `main.cpp` zeigt den Ablauf, Module besitzen ihre Daten), aber die CAN-Schleife bleibt eine eigene Task auf Kern 1; B: ganz zurück zu einem `loop()` mit TaskScheduler (Webanfragen und WLAN-Start könnten den 6-ms-Takt stören); C: Zwei-Task-Aufbau behalten, nur innen aufräumen.
- **Entscheidung:** A. `src/main.cpp` enthält `pollVehicleCAN()`, `pollTeslaCAN()`, `pollBatteryCAN()`, `emulateECUCAN()` und `canTask()` (6 ms, Kern 1). Jedes Modul definiert seine `params::*`-Gruppe: Overrides im ECU-Codec, Batterie und Ladeparameter im Batterie-Codec, Lüfter (mit Automatik) und Poti in ihren Treibern, Empfangsüberwachung in `comms/can_rx_monitor` (seit 2026-11-10 in `drivers/canmanager_api`, öffentliche Schnittstelle des CAN-Managers). Lock, Snapshot und Signaltabelle gehören zum Webserver (`src/services/`). Ordner: `app/`, `comms/`, `drivers/`, `services/`.
- **Geändert 2026-11-09 (Nutzer):** Empfang und Rechnung getrennt: `loop()` (Kern 1) pollt CAN0/1/2 ständig (1 ms Pause), setzt alle 100 ms die Ausgänge (`updateOutputs`: Lüfter, Tankanzeige) und liest alle 300 ms die Fehlerzähler; `ecuTask` (6 ms, höhere Priorität) rechnet nur `updateCluster()` und sendet eine ECU-Botschaft. Fehlerzähler (TEC/REC/EFLG/Bus-off) und Rx-Zähler je Bus im `canmanager`, Textlog mit `[DIAG]`-Zeile auf COM10, solange GVRET nicht binär ist.
- **Konsequenzen:** Reihenfolge und Takt je Zyklus sind unverändert (empfangen → Kombi → Overrides → Lüfter → eine ECU-Botschaft → Ausgänge). PC-Vergleich über 20 000 Zyklen: gesendete Frames, Lüfterstufe und Poti-Sollwert bitgleich zum Stand davor. Auf Hardware nicht gemessen; vor dem Einsatz im Fahrzeug CAN-Mitschnitt nötig.

## VCU-DEC-016 – CAN3 und CAN-Start vor dem WLAN
- **Status:** accepted (Nutzerentscheidung 2026-11-10)
- **Betroffene Requirements:** VCU-REQ-001, -002, -009, -018
- **Kontext:** Auf der Platine sitzt ein dritter MCP2515 (CAN3, CS GPIO5, INT GPIO6, gemeinsamer SPI-Bus). GPIO5/6 waren INC/UD des Tankanzeige-Potis. CAN0 lief beim Start über (Puffer 32), weil `setup()` erst nach dem WLAN-Start zurückkehrte und `loop()` so lange nicht pollte.
- **Entscheidung:** CAN3 wird gestartet, gezählt und an GVRET (Bus 4) gespiegelt; Rolle offen. Poti abgeschaltet, bis neue Pins feststehen. Alle CAN-Busse starten in `setup()`; WLAN, SPIFFS und Webserver startet die Web-Task selbst (Kern 0, Priorität 1 = niedrigste Anwendungspriorität). Einstellungen (Bitrate, SPI-Takt, Puffer, Budget, Perioden, Prioritäten) und Status aller Busse und der ECU-Task stehen in `params::can` (`src/drivers/canmanager_api.h`) und damit in `/api/live`; Testseite `/can` in der Firmware.
- **Konsequenzen:** Tankanzeige ohne Funktion. CAN0-Empfangspuffer 64. Die JSON-Felder `rt_*`, `can0_active`, `can1_error`/`can2_error` entfallen zugunsten von `ecu_*`, `canN_up`, `canN_error`. Die Einstellungen werden nur beim Start gelesen (nicht schreibbar).

## VCU-DEC-017 – Persistenz im AT24C mit Slot-Umlauf
- **Status:** accepted (Nutzerentscheidung 2026-11-10)
- **Betroffene Requirements:** VCU-REQ-017, -020
- **Kontext:** Auf der Platine sitzt ein AT24C256 (I2C, SCL GPIO39, SDA GPIO37). TeensyVCU hat bereits eine erprobte Speicherlogik (`src/persistent_data_storage.h`).
- **Entscheidung:** Die Logik wird übernommen: Datensatz mit Magic `EVCU`, Version, Länge, Sequenznummer und FNV-1a-Prüfsumme in 16 Slots ab Adresse 0 (je 32 Byte), reihum beschrieben; beim Start gilt der gültige Datensatz mit der höchsten Sequenz. Erst die Daten, dann der Kopf schreiben. Der Dienst prüft in der Web-Task einmal pro Sekunde auf Änderungen und schreibt nur dann. Bibliothek `robtillaart/I2C_EEPROM`; nach dem Schreiben 6 ms warten, weil das ACK-Polling der Bibliothek am ESP32-S3 zu früh „bereit“ meldet.
- **Konsequenzen:** Layout-Änderung → `kVersion` erhöhen, alte Datensätze gelten dann als ungültig (Standardwerte). Ein Schreibvorgang dauert etwa 15 ms in der Web-Task, nicht in der CAN-Task. Bei 16 Slots und Schreiben nur bei Änderung ist die Lebensdauer (1 Mio. Zyklen je Zelle) unkritisch.

## VCU-DEC-018 – CAN0 am Bus von TeensyBMS und Zombie-Slave
- **Status:** accepted (Nutzerentscheidung 2026-11-10)
- **Betroffene Requirements:** VCU-REQ-013, -018, -021
- **Kontext:** CAN0 der VCU wird in den Bus von TeensyBMS, Zombie-Slave (und Tesla-DC/DC) gehängt. Die Botschaften stehen in der DBC des Zombie-Slave; TeensyVCU (TeensyBMS) dokumentiert dieselben in `CAN_Message_Packing.md`.
- **Entscheidung:** Die VCU liest mit und sendet nur 0x43A (mVCU-Heizungsanforderung, 100 ms, immer, Standard „aus“). Die DBC wird nach `documentation/` kopiert (Projekt bleibt eigenständig). Empfangene Frames mit Zähler/CRC werden bei falscher CRC verworfen, der Zähler wird nicht geprüft. 1-Hz-Botschaften (0x41D/0x41E) gelten erst nach 2,5 s als veraltet (`rxTimeoutMs()`), alle anderen nach 500 ms. Werte, die nur intern im Zombie-Slave liegen (Stecker, Ladeart, 12 V, Ladeparameter zum Lader), werden nicht geschätzt, sondern als fehlend gemeldet.
- **Konsequenzen:** Erste Sendebotschaft der VCU auf CAN0. Fällt die VCU aus, schaltet der Zombie-Slave die Heizung nach 500 ms ab. 0x210 (DC/DC) kollidiert mit der TCU-Demo-ID, falls die TCU an diesen Bus kommt.

## VCU-DEC-019 – MCP2515-Treiber: Tasks auf Kern 1, SPI-Blockübertragung
- **Status:** accepted (Nutzerentscheidung 2026-11-12)
- **Betroffene Requirements:** VCU-REQ-001, -002, -009
- **Kontext:** Am Prüfstand (alle drei MCP2515 am selben Bus, VCU sendet die DME-Frames) verlor CAN2 35–110 von 2700 Frames (RX-Überlauf, EFLG 0x40). Ohne eigenes Senden gab es keinen Verlust, so wie beim CAN-Grabber mit derselben Bibliothek. Messung: Jeder MCP2515 hat eine eigene Treiber-Task (Priorität 16, ohne festen Kern) am gemeinsamen SPI-Mutex. Kommt ein Frame bei allen gleichzeitig an, wartet der letzte Controller 0,3–0,9 ms; zwei Empfangspuffer reichen bei aufeinanderfolgenden Frames aber nur für etwa 0,25–0,46 ms.
- **Optionen:** A: nur Kern und Priorität der Treiber-Tasks ändern (Verlust noch 8–19); B: nur schneller auslesen (Verlust 0); C: beides.
- **Entscheidung:** C. Im Fork `Modellfan/ACAN2515`, Branch `esp32-task-priority` (in `platformio.ini` auf einen Commit festgelegt): Build-Optionen `ACAN2515_ESP32_TASK_PRIORITY` und `ACAN2515_ESP32_TASK_CORE`, für die VCU 24 und Kern 1 (über der ECU-Task, Priorität 23). Ein Frame wird in einem SPI-Block gelesen bzw. geschrieben; das Lesen mit READ RX BUFFER löscht das Empfangsflag selbst, der zusätzliche Bit-Modify-Befehl entfällt. Die Standardwerte des Forks bleiben unverändert (CAN-Grabber nutzt `master`).
- **Konsequenzen:** Prüfstand: 0 Frames Verlust bei 2700 Burst-Frames und bei 300 Einzelframes, Sequenz/CRC ohne Fehler, gesendete DME-Frames unverändert (IDs, DLC, 30-ms-Takt). Die Treiber-Tasks unterbrechen die ECU-Task für jeweils ein paar Dutzend µs. Kein CAN-Mitschnitt im Fahrzeug; Messung mit getrennten Bussen offen.

