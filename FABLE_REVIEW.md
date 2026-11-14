# Fable Review – esp32VCU (Stand 2026-11-14, Branch `esp32vcu_986`, Commit 8a534d8)

Review der gesamten Repo: Fehlersuche, Nutzer-Review, Architektur- und Performance-Vorschläge, Code-Review.
Dieses Dokument ist ein Befundbericht, keine Anforderungsquelle. Was davon umgesetzt wird, entscheidet der Nutzer (neues Verhalten über `/re-refine`).

## 1. Kurzfazit

Die Repo ist für ein Einzelprojekt ungewöhnlich sauber: klare Schichten, nachvollziehbare Entscheidungen, echte Tests. Die Firmware ist am Prüfstand brauchbar; fürs Fahrzeug fehlen vor allem **Fehlerbehandlung und Rückfallverhalten** (veraltete Daten, Controller-Ausfall, Watchdog).

Die fünf wichtigsten Punkte:

1. **Kombiinstrument friert bei Umrichter-Ausfall ein** (B-01): `updateCluster()` prüft nicht, ob die SDU-Werte frisch sind. Fällt CAN2 aus, bleiben Zeiger und Temperatur auf dem letzten Wert stehen, die Warnung kommt nie.
2. **Kein Watchdog, keine Wiederanlauf-Logik** (B-05, B-06): Ein MCP2515, der beim Start nicht initialisiert, oder ein CAN0 im Bus-off bleibt bis zum Reset tot. Die LED blinkt trotzdem grün.
3. **Webserver bedient nur eine Verbindung und wartet bis zu 5 s auf stumme Verbindungen** (B-07). Das erklärt sehr wahrscheinlich den offenen README-Punkt „WLAN antwortet zeitweise mit 0,4 bis 2,4 s“.
4. **Die Oberfläche zeigt erfundene Werte, die echt aussehen** (U-01, bekannt): SoC 28 %, 85 km, 193 kW ohne Datenquelle. Dazu wird `stale` außerhalb der Signals-Seite nicht ausgewertet (VCU-REQ-013, `must`).
5. **Schreibzugriffe per `GET` ohne CSRF-Schutz** (B-08): Jede Webseite, die ein Gerät im Heim-WLAN öffnet, kann ECU-Overrides setzen. Das geht über die akzeptierte Regel „wer im WLAN ist, darf setzen“ hinaus.

## 2. Umfang und Methode

| Was | Wie geprüft |
|---|---|
| Firmware `src/` (ohne `vw_pq_messages.*`, ungenutzt laut VCU-DEC-013), `lib/X9C103S` | vollständig gelesen |
| ACAN2515-Fork und Arduino-`WebServer` | gezielt nachgelesen (Sperren, Wartezeiten) |
| PWA | `net.js`, `sw.js`, `pwa.js`, `signals.js` vollständig; `app.js` (4394 Zeilen) nur Datenfluss, Polling, Rendering |
| `REQUIREMENTS.md`, `DECISIONS.md`, `TESTPLAN.md`, `README.md`, `can_signals.md` (Auszüge) | gelesen und gegen den Code abgeglichen |
| SIL-Tests | ausgeführt: **43 bestanden, 3 xfail** (57 s) |
| PWA-Tests | ausgeführt: **32 bestanden** (9 min 24 s, ein Lauf) |

**Nicht geprüft:** kein Firmware-Build, kein Flash, kein Prüfstandstest, kein CAN-Mitschnitt. Alle Firmware-Befunde stammen aus dem Lesen des Codes. `tools/*.py`, `pwa/tools/*`, CSS und die DBC-Dateien habe ich nicht im Detail geprüft. Es wurde kein Code geändert.

Sicherheit der Befunde: **sicher** = im Code eindeutig, **wahrscheinlich** = Code eindeutig, Wirkung auf Hardware nicht gemessen, **prüfen** = Hypothese.

## 3. Bugs und Risiken

### 3.1 Neu gefunden

| ID | Schwere | Sicherheit | Befund |
|---|---|---|---|
| B-01 | hoch | sicher | Kombi-Rechnung ohne Frischeprüfung |
| B-02 | hoch | prüfen | Negative SDU-Temperaturen werden als > 200 °C gelesen |
| B-03 | mittel | sicher | Lüfter im Handbetrieb hat keinen Übertemperaturschutz |
| B-04 | mittel | sicher | Beschleunigungsmessung ohne Plausibilität, Bestzeit nicht rücksetzbar |
| B-05 | mittel | sicher | Kein Watchdog |
| B-06 | mittel | wahrscheinlich | Kein Wiederanlauf nach CAN-Init-Fehler oder Bus-off |
| B-07 | mittel | wahrscheinlich | HTTP-Server blockiert bis 5 s durch stumme Verbindungen |
| B-08 | mittel | sicher | Setzen per `GET`, kein CSRF-Schutz |
| B-09 | mittel | wahrscheinlich | Heimnetz-Suche im Fahrzeug stört den Zugangspunkt |
| B-10 | mittel | sicher | Antwort auf `/api/set` meldet den alten Ausgangszustand |
| B-11 | niedrig | wahrscheinlich | EEPROM-Start kann den ECU-Start verzögern |
| B-12 | niedrig | sicher | GVRET-Parser: drei kleine Fehler |
| B-13 | niedrig | sicher | Fahrzeug- und SDU-Decoder prüfen `ext`/`rtr` nicht |
| B-14 | niedrig | sicher | Service Worker: Offline-Rückfall unerreichbar |
| B-15 | niedrig | sicher | PWA-Update kommt auf dem Radio nie an |
| B-16 | niedrig | sicher | Kleinere Nebenläufigkeits- und Diagnosepunkte |

**B-01 – Kombi-Rechnung ohne Frischeprüfung.** [cluster_calc.cpp:211](src/app/cluster_calc.cpp#L211) liest `udc`, `idc`, `seldir`, `tmpm`, `tmphs` direkt aus `params::tesla_sdu`. Der Lüfter fragt `rxFresh(RX_SDU_504)` ab ([coolant_fan_control.cpp:51](src/drivers/coolant_fan_control.cpp#L51)), die Kombi-Rechnung nicht. Fällt CAN2 während der Fahrt aus, zeigt der Drehzahlmesser dauerhaft die letzte Leistung, die Temperaturanzeige den letzten Wert, und die Kühlmittelwarnung bleibt aus. Dasselbe gilt für die Geschwindigkeit aus 0x510 in der Beschleunigungsmessung.
Vorschlag: Bei veralteten 0x500/0x504 einen festen Rückfall senden (z. B. Zeiger auf Neutral, Warnung an). Das ist neues Verhalten → `/re-refine`, passt zu VCU-REQ-013.

**B-02 – Negative SDU-Temperaturen.** [tesla_sdu_messages.cpp:140](src/comms/tesla_sdu_messages.cpp#L140) liest `tmphs`/`tmpm` als `uint8_t`; `can_signals.md` nennt ebenfalls U8. OpenInverter rechnet intern mit Vorzeichen. Sendet der Umrichter bei Frost −5 °C als Zweierkomplement, liest die VCU 251 °C: Kühlmittelwarnung im Kombi, Lüfterautomatik auf Stufe 2.
Prüfen: Mitschnitt bei kaltem Fahrzeug oder CAN-Mapping im Umrichter ansehen. Falls vorzeichenbehaftet: `int8_t` wie bei `seldir`.

**B-03 – Handbetrieb ohne Übertemperaturschutz.** Der Lüfter startet im Handbetrieb mit Stufe 0, und diese Einstellung wird im EEPROM gespeichert ([persistence.cpp:42](src/services/persistence.cpp#L42)). Im Handbetrieb zählt nur `fan_level_manual` ([coolant_fan_control.cpp:55](src/drivers/coolant_fan_control.cpp#L55)); auch bei 160 °C Motortemperatur bleibt der Lüfter aus. Heute ohne Wirkung, weil die Ausgänge abgeschaltet sind – wird aber scharf, sobald der Pin feststeht.
Vorschlag: `max(manuell, Automatik)` oberhalb einer kritischen Schwelle.

**B-04 – Beschleunigungsmessung.** [cluster_calc.cpp:84](src/app/cluster_calc.cpp#L84):
- Ein einzelner Ausreißer der Geschwindigkeit (ein fehlerhafter Frame ≥ 100 km/h kurz nach dem Anfahren) erzeugt eine Bestzeit von wenigen Millisekunden. Sie landet im EEPROM und lässt sich weder über die API noch über die Oberfläche löschen.
- Ein Lauf wird nie abgebrochen: Anfahren, auf 30 km/h bremsen, später auf 100 beschleunigen ergibt einen „letzten Wert“ von mehreren Minuten. Das entspricht VCU-REQ-007, ist für den Nutzer aber Datenmüll.

Vorschlag: Mindestzeit (z. B. 0–100 nicht unter 2 s), Abbruch bei deutlicher Verzögerung, Rücksetzbefehl für die Bestzeiten.

**B-05 – Kein Watchdog.** Weder `ecuTask` noch `loop()` noch die Web-Task hängen am Task-Watchdog. Hängt die ECU-Task (z. B. SPI-Mutex), verstummt die DME-Emulation, ohne dass die VCU neu startet.
Vorschlag: `esp_task_wdt` für `ecuTask` und `loop()`; Reset-Grund beim Start loggen und in `/api/live` zeigen.

**B-06 – Kein Wiederanlauf.**
- [canmanager.cpp:86](src/drivers/canmanager.cpp#L86): Schlägt `begin()` eines MCP2515 fehl, wird das nur geloggt. `ecuTask` ruft trotzdem `can1.tryToSend()` auf, `loop()` trotzdem `can1.available()`. Einen zweiten Versuch gibt es nicht.
- CAN0 (TWAI): `bus_off` wird gemessen und veröffentlicht ([canmanager.cpp:179](src/drivers/canmanager.cpp#L179)), aber niemand reagiert. Ob ACAN_ESP32 nach Bus-off selbst wieder anläuft, habe ich nicht geprüft; falls nicht, fehlen BMS-Daten und Heizungsanforderung bis zum Reset.
- Die LED blinkt in jedem dieser Zustände grün ([webinterface.cpp:510](src/services/webinterface.cpp#L510)).

Vorschlag: Init alle paar Sekunden wiederholen, CAN0 bei Bus-off neu starten (`startCan0()` gibt es schon), LED-Farbe nach Zustand.

**B-07 – HTTP-Server blockiert.** Der Arduino-`WebServer` hält genau eine Verbindung. Öffnet ein Browser eine Verbindung, ohne sofort etwas zu senden (Chrome macht das routinemäßig als Vorab-Verbindung), wartet der Server bis `HTTP_MAX_DATA_WAIT` = 5000 ms auf Daten und nimmt so lange keine andere Verbindung an (`WebServer.cpp`, Zustand `HC_WAIT_READ`). GVRET und Persistenz laufen weiter, nur HTTP steht. Mit zwei Clients (Radio und Laptop, oder PWA und `/can`) tritt das häufiger auf.
Am Prüfstand messbar: Antwortzeiten mit einem und mit zwei Browsern vergleichen. Abhilfe siehe A-03.

**B-08 – Setzen per `GET`.** Alle Setz-Endpunkte sind `GET` ([webinterface.cpp:559](src/services/webinterface.cpp#L559)). Folgen:
- Eine beliebige Webseite kann mit `<img src="http://192.168.4.1/api/set-ecu?rpm=7000">` Werte setzen, sobald ein Gerät im WLAN sie öffnet. Der Browser führt einfache `GET`-Anfragen ohne CORS-Prüfung aus.
- Vorab-Laden, Link-Vorschauen und Browser-Verlauf können Setzbefehle auslösen.

Vorschlag: `POST` plus Pflicht-Header (z. B. `X-VCU: 1`); das blockt Fremdseiten ohne Anmeldung. Optional Setzen nur aus dem AP-Netz 192.168.4.0/24.

**B-09 – Heimnetz-Suche im Fahrzeug.** [webinterface.cpp:540](src/services/webinterface.cpp#L540): `WIFI_AP_STA` mit `setAutoReconnect(true)`. Unterwegs ist das Heimnetz nie da; der ESP32 sucht dann dauernd auf allen Kanälen. Zugangspunkt und Station teilen sich ein Funkmodul, während der Suche verpasst der Zugangspunkt Pakete. Das ist ein bekanntes ESP32-Verhalten und der zweite Kandidat für die sporadischen Antwortzeiten.
Vorschlag: Nach einigen Fehlversuchen nur noch alle 1–5 min suchen, oder die Suche aussetzen, solange ein Client am Zugangspunkt hängt.

**B-10 – `/api/set` meldet alten Zustand.** `waitForOutputs()` wartet 14 ms ([webinterface.cpp:326](src/services/webinterface.cpp#L326)); der Kommentar stammt aus der Zeit, als die Ausgänge im 6-ms-Takt liefen. Seit VCU-DEC-015 laufen sie alle 100 ms ([main.cpp:109](src/main.cpp#L109)). `coolant_fan_level` ist die *angewendete* Stufe, die Antwort zeigt also meist noch den alten Wert – das Akzeptanzkriterium von VCU-REQ-010 trifft nur zufällig zu. Die PWA überdeckt das mit ihrer 1,5-s-Sperre.
Verwandt: `digital_pot_pct` liefert die angewendete Stellung; solange das Poti abgeschaltet ist, bleibt sie 0, der Regler in der Oberfläche springt voraussichtlich nach 1,5 s zurück.
Vorschlag: Im Handler unter der Sperre `coolantFanAutoUpdate()` aufrufen und für das Poti den Sollwert zurückmelden.

**B-11 – Startreihenfolge.** [main.cpp:167](src/main.cpp#L167): `eepromBegin()` und `persistenceBegin()` laufen vor dem Anlegen der ECU-Task. Bei gestörtem I2C (SDA hängt auf Masse) summieren sich die Zeitüberschreitungen über 8 Adressen und 16 Slots; die 1-s-Grenze aus VCU-REQ-002 ist dann gefährdet.
Vorschlag: ECU-Task direkt nach `canManager_setup()` starten.

**B-12 – GVRET-Parser** ([gvret.h](src/services/gvret.h)):
- Zeile 157–160: Die Schleife füllt `buff[2..16]`, gesendet werden 18 Byte – das letzte ist uninitialisiert.
- Zeile 175: Ein unbekanntes Kommando lässt den Zustand auf `GET_COMMAND`; das nächste Byte wird als Kommando gelesen.
- Zeile 198: Der Zeitstempel entsteht beim Senden an den PC, nicht beim Empfang. Während eine HTTP-Antwort gebaut wird, sind das mehrere Millisekunden Fehler – genau die Größe, die TC-ECU-01 (30 ms ± 3 ms) messen soll.

**B-13 – `ext`/`rtr`.** `decodeBattery()` verwirft Extended- und Remote-Frames, `decode986Vehicle()` und `decodeTeslaSdu()` nicht. Ein Remote-Frame mit passender ID würde Werte aus undefinierten Datenbytes schreiben.

**B-14 – Service Worker.** [sw.js:121](pwa/src/sw.js#L121): `cached || caches.match("./") || offlineApi()` – `caches.match()` gibt ein Promise zurück und ist immer wahr. `offlineApi()` wird nie erreicht; ohne Cache endet die Navigation mit einem Netzwerkfehler statt mit der 503-Antwort.

**B-15 – PWA-Update.** [pwa.js:49](pwa/src/js/pwa.js#L49): Neu geladen wird nur, wenn die Seite im Hintergrund ist. Auf dem Radio ist sie das praktisch nie; eine neue Version greift erst nach einem Neustart des Browsers. Außerdem löst die Erstinstallation (`clients.claim()`) ein unnötiges Neuladen beim ersten Verstecken aus.

**B-16 – Kleinkram.**
- `gDropped = gDropped + 1` aus zwei Tasks ([gvret_bridge.cpp:35](src/services/gvret_bridge.cpp#L35)): Zählfehler möglich, nur Diagnose.
- `gvret_binOutput` ist `static` in einem Header und wird von beiden Kernen gelesen, ohne `volatile`/atomar.
- `can0-pintest` startet TWAI aus der Web-Task (Kern 0) neu; der Interrupt landet dann auf Kern 0. Nur Prüfstand.
- `ecu_overruns` misst nur die Rechenzeit, nicht die Verspätung des Aufwachens. Ein verspäteter Frame wird nicht gezählt.
- `/api/set?oil=` setzt den Verbrauchs-Override ([webinterface.cpp:364](src/services/webinterface.cpp#L364)) – irreführender Name.
- X9C103S: `setPercent()` fährt bei jeder Änderung erst 99 Schritte nach unten ([X9C103S.cpp:28](lib/X9C103S/X9C103S.cpp#L28)). Der Tankgeber steht kurz auf 0 Ω. Besser relativ steppen, nur beim Start nullen.

### 3.2 Bereits bekannt (bestätigt, steht schon in den Dokumenten)

| Punkt | Fundstelle | Anmerkung |
|---|---|---|
| `seldir > 0` wird als Rückwärts gewertet | VCU-REQ-005 Befund | Vor dem Fahrzeugeinsatz klären (SIL-Befund TC-SIL-02b/c) |
| Verbrauch ist Simulation (60 l/h, Stopp bei 20 l) | VCU-REQ-006 | Nach 20 min steht der Zähler |
| DME sendet original alle 10 ms, VCU alle 30 ms | `can_signals.md` Nr. 10 | Größtes offenes Fahrzeugrisiko neben B-01 |
| 0x1A0 Byte 6 und 0x4A1 Bits 13–15 sind Zähler | `can_signals.md` Nr. 1, 2 | `itq_pct`, `driver_braking` sind falsch |
| PWA fragt Felder ab, die es nicht gibt | VCU-REQ-010 Lücke | siehe U-01 |
| Heim-WLAN-Passwort in der Git-Historie | VCU-REQ-015 | bei Veröffentlichung Passwort wechseln |

## 4. Nutzer-Review

Blickwinkel: Fahrer am Navi-Display und Entwickler am Prüfstand.

**U-01 – Erfundene Werte (hoch).** Ohne Daten zeigt die Startseite SoC 28 %, Reichweite 85 km und 193 kW ([app.js:177](pwa/src/js/app.js#L177), [app.js:195](pwa/src/js/app.js#L195), [app.js:1907](pwa/src/js/app.js#L1907)). Die Firmware liefert `battery_soc_pct`; die PWA sucht `battery_level`, `battery_percent`, `battery_soc`, `soc`, `hv_soc`, `state_of_charge`. Ein Fahrer kann echte und erfundene Werte nicht unterscheiden. Schnellste Abhilfe: Firmware-Namen eintragen und statt Ersatzwerten „–“ zeigen.

**U-02 – Veraltete Werte sehen aktuell aus (hoch).** `stale` wird nur auf der Signals-Seite ausgewertet. Auf allen Fahrerseiten steht nach einem Busausfall der letzte Wert (VCU-REQ-013 verlangt „–“).

**U-03 – Verbindungsverlust wird spät gemeldet.** Zwei Fehlversuche mit je 2,5 s Zeitlimit plus Wartezeit: rund 6 s bis zur Meldung. So lange stehen alte Werte da. Vorschlag: Werte nach 2 s ohne Antwort ausgrauen.

**U-04 – 1 Hz ist für Fahrwerte zu langsam.** Stoppuhr und Leistungsanzeige springen im Sekundentakt. Die laufende Zeit ließe sich im Browser zwischen zwei Abfragen weiterzählen; für Leistung sind 5–10 Hz mit kleinem `fields=`-Satz machbar.

**U-05 – Vergessene Overrides.** Ein gesetzter ECU-Override (Drehzahl, Temperatur, Warnung) bleibt bis zum Neustart aktiv. Außerhalb des Engineering-Modus ist er nicht sichtbar. Wer am Vortag getestet hat, fährt mit festgenageltem Drehzahlmesser los. Vorschlag: Hinweis in der Kopfzeile, solange ein Override aktiv ist; optional Ablauf nach einigen Minuten.

**U-06 – LED sagt nichts.** Grün blinkt, solange die Web-Task läuft – auch wenn kein CAN-Bus initialisiert ist. Vorschlag: grün = alles gut, gelb = ein Bus fehlt oder veraltet, rot = CAN1 sendet nicht.

**U-07 – Textlog weg bis zum Reset.** Nach dem ersten 0xE7 gibt es kein Textlog und keine Befehle mehr. Wer SavvyCAN schließt, muss die VCU neu starten (im Fahrzeug: Zündung aus). Vorschlag: Rückkehr in den Textmodus, wenn einige Sekunden kein GVRET-Keepalive kam.

**U-08 – Sprachmix.** „VCU nicht erreichbar“ neben „VCU connection lost“, englische Tabs neben deutschen Meldungen.

**U-09 – Bestzeiten nicht löschbar** (siehe B-04).

**U-10 – Inbetriebnahme im Fahrzeug.** Es gibt keinen Modus „nur mitlesen“. Für den ersten Anschluss ans Auto mit noch verbautem DME wäre ein Schalter „CAN1 nicht senden“ (MCP2515 Listen-only) die sicherste Hilfe. Der README nennt den fehlenden Ablauf selbst als offenen Punkt.

Positiv: Die Netzwerkschicht (Zeitlimit, Warteschlange „letzter Wert gewinnt“, Pause im Hintergrund) ist durchdacht. `vcu_cmd.py`, `[DIAG]`-Zeile und die Seiten `/can` und `/io` machen die Arbeit am Prüfstand angenehm.

## 5. Architektur-Vorschläge

Nach Nutzen sortiert. A-01 bis A-03 lösen jeweils mehrere Befunde auf einmal.

**A-01 – Gültigkeit gehört zu den Daten.** Heute liegen Wert (`params::*`) und Alter (`rxFresh()`) getrennt; jeder Verbraucher muss selbst daran denken (Lüfter ja, Kombi nein → B-01). Vorschlag: pro Quelle eine Funktion wie `sduValid(nowMs)`, die die Anwendung zwingend benutzt, und eine Regel in VCU-DEC-006: „Keine Anwendungsfunktion liest CAN-Werte ohne Frischeprüfung.“ In SIL leicht testbar.

**A-02 – Systemzustand und Überwachung.** Ein kleines Modul `health`: Zustand je Bus (ok / fehlt / Bus-off / Init fehlgeschlagen), Wiederanlauf, Watchdog, Reset-Grund, LED, ein Feld `vcu_state` in `/api/live`. Löst B-05, B-06, U-06.

**A-03 – Webserver ersetzen.** `esp_http_server` (in ESP-IDF enthalten, mehrere Verbindungen) oder ESPAsyncWebServer. Dazu:
- Live-Werte per Server-Sent Events statt Polling (ein Kanal, die Firmware bestimmt die Rate).
- Schreiben per `POST` (B-08).
- Löst B-07 und macht U-04 billig.

Aufwand mittel: betroffen sind nur `webinterface.cpp` und `net.js`; die Signaltabelle bleibt.

**A-04 – GVRET in eigener Task.** Heute teilt sich GVRET die Schleife mit HTTP, Persistenz und LED. Eine HTTP-Antwort oder ein EEPROM-Schreibvorgang (15 ms) verzögert die Ausgabe; bei 1000 Frames/s läuft die 128er-Warteschlange in rund 130 ms voll. Eigene Task auf Kern 0, Zeitstempel schon in `gvretBridgeObserve()` (B-12).

**A-05 – CAN-Empfang ohne Parametersperre.** `loop()` hält die Sperre über das Abholen aller vier Busse, inklusive SPI-Mutex für jedes `available()`/`receive()` ([main.cpp:184](src/main.cpp#L184)). Besser: Frames ohne Sperre in ein lokales Feld holen, dann einmal sperren und dekodieren. Verkürzt die Zeit, die die ECU-Task maximal warten kann. Das am Prüfstand gemessene `max` ist gut; es ist eine Robustheitsfrage, kein akuter Fehler.

**A-06 – Codecs testen oder erzeugen.** Die Codecs sind reine Funktionen, aber in SIL ausdrücklich ausgenommen (VCU-DEC-014). Billiger Gewinn: Rundlauf-Test je Botschaft und Abspielen von `documentation/can_logs/` gegen die DBC (Python `cantools`). Das hätte die Zähler-Befunde in 0x1A0/0x4A1 früher gezeigt und sichert B-02 ab. Später möglich: Codecs aus der DBC erzeugen.

**A-07 – Ein Vertrag zwischen Firmware und PWA.** Die PWA rät Feldnamen (sechs Varianten für SoC). Vorschlag: `build.py` erzeugt aus `signal_table.cpp` eine Namensliste; ein PWA-Test schlägt fehl, wenn die Oberfläche ein Feld abfragt, das es nicht gibt. Dazu `fw_version` und `api_version` in `/api/live`, damit eine offline gecachte PWA eine unpassende Firmware erkennt.

**A-08 – `app.js` aufteilen.** 174 KB, 299 Funktionen in einer Datei; `home-battery.js`, `signals.js` und `coolant-stage.js` zeigen bereits das Muster. Eine Datei je Seite plus ein Modul für Zustand und Datenabgleich. Erleichtert auch das Angleichen des GUI-Worktrees.

**A-09 – Konfiguration von Zustand trennen.** `EcuSignals` mischt feste Bitmuster (`standard_280_force_one_bits`), einen Laufzähler und Signale; `CanManagerSignals` mischt Einstellungen und Status. Feste Werte als `constexpr`, dann wird auch der Schnappschuss kleiner.

**A-10 – `vw_pq_messages.cpp` aus dem Build nehmen.** 8400 ungenutzte Zeilen werden bei jedem vollen Build übersetzt. Nach `documentation/reference/` verschieben oder per `build_src_filter` ausschließen; der Zweck aus VCU-DEC-013 (Abgleich) bleibt erhalten.

## 6. Performance-Vorschläge

Vorweg: Die Echtzeitseite ist nicht das Problem. 6-ms-Takt, Rechenzeit im Mikrosekundenbereich, kein Frameverlust nach VCU-DEC-019. Die bitweisen Schleifen in `readBitsLE`/`writeBitsLE` und die lineare Suche in `rxMark()` sind unkritisch – nicht anfassen. Gewinn liegt auf der Web-Seite.

| ID | Vorschlag | Wirkung |
|---|---|---|
| P-01 | **Setz-Antworten verkleinern.** Jede Reglerbewegung liefert das komplette `/api/live` (rund 300 Felder, etwa 11 KB). `net.js` kann `fields=` mitschicken; die Firmware wertet es bei den Setz-Endpunkten schon aus. | weniger Last und Latenz bei jeder Bedienung; kleinster Aufwand |
| P-02 | **JSON ohne `String`-Verkettung.** `buildLiveJson()` reserviert 11 KB und legt je Zahl ein temporäres `String` an ([webinterface.cpp:115](src/services/webinterface.cpp#L115)). Fester Puffer mit `snprintf` und stückweises Senden. | kein Heap-Zerfasern im Dauerbetrieb, konstante Antwortzeit |
| P-03 | **Feldfilter einmal auflösen.** `FieldFilter::wants()` durchsucht für jedes der rund 300 Signale die ganze `fields`-Zeichenkette. Einmal je Anfrage in eine Bitmaske über `kSignals` übersetzen. | Signals-Seite: aus etwa 1 Mio. Zeichenvergleichen werden wenige Tausend |
| P-04 | **`/can` und `/io` mit `fields=`.** Beide Seiten holen jede Sekunde alles. | weniger Grundlast, entschärft B-07 |
| P-05 | **PWA: nur geänderte Werte ins DOM.** `renderView()` baut nach jeder Abfrage große Teile per `innerHTML` neu. Auf dem Android-Radio kostet das Zeit und verwirft Fokus und Druckzustände. | flüssigere Bedienung |
| P-06 | **Push statt Polling** (A-03). | geringere Latenz, eine Verbindung statt einer je Sekunde |
| P-07 | **UART-Sendepuffer für GVRET vergrößern** (`Serial.setTxBufferSize()` vor `begin()`). `Serial.write()` blockiert sonst die Web-Task, wenn der PC langsam liest. | gleichmäßigerer GVRET-Strom |
| P-08 | **PWA-Tests beschleunigen.** 32 Tests in 9 min 24 s. Browser je Sitzung statt je Test, oder `pytest-xdist`. | schnellere Rückmeldung; vermutlich hilft es auch bei den zwei sporadischen Tests |

## 7. Code-, Doku- und Test-Review

### Was gut ist
- **Schichten und Besitz:** Jedes Modul besitzt seine `params::*`-Gruppe; `main.cpp` zeigt den ganzen Ablauf auf 200 Zeilen.
- **`board.h` mit `static_assert`:** Doppelbelegung bricht den Build. Nicht zugewiesene Pins schalten die Funktion ab, statt zu raten.
- **Signaltabelle:** Ein Eintrag je Signal, Typ aus `decltype`, daraus API, Setzprüfung, SIL-Zugriff und Mock.
- **Persistenz:** Slot-Umlauf, Prüfsumme, erst Daten dann Kopf, Wertebereichsprüfung beim Laden.
- **Ehrliche Dokumente:** „umgesetzt heißt nicht verifiziert“, Befunde stehen bei den Requirements, offene Punkte sind benannt.
- **SIL mit unverändertem Firmware-Code** und einem Mock, der die Signaltabelle aus der Quelle liest.

### Dokumente, die vom Code abweichen
| Stelle | Abweichung |
|---|---|
| VCU-REQ-001 Umsetzung, VCU-DEC-003, DEC-004 | nennen `canTask()`; die Funktion heißt seit VCU-DEC-015 `ecuTask()` |
| VCU-REQ-007 | „Bestwerte werden vorerst nicht gespeichert“ – VCU-REQ-020 speichert sie |
| VCU-REQ-011 | „28 automatische Tests“ – es sind 32 |
| TESTPLAN, Kopf | „Kein Test auf Hardware“ – die Tabelle darunter nennt Prüfstandsnachweise vom 2026-11-10 und -12 |
| [webinterface.cpp:542](src/services/webinterface.cpp#L542) | Kommentar „Full TX power (19.5 dBm)“ über einer Zeile, die 8,5 dBm setzt |
| [webinterface.cpp:324](src/services/webinterface.cpp#L324) | Kommentar „next CAN task cycle“ (siehe B-10) |
| [canmanager.h:58](src/drivers/canmanager.h#L58) | „30 ms per frame“ – gemeint ist je Botschaft |
| `sil/shim/sil_api.cpp` | Komponente 2 rechnet die Lüfterautomatik alle 6 ms, die Firmware alle 100 ms |

### Testlücken
- **Codecs:** kein automatischer Test (A-06).
- **Web-API der Firmware:** `fields`, `rejected`, Wertebereiche sind nur gegen den Python-Mock geprüft. Die Parser (`parseLevel`, `writeSignal`, `setOverride`) ließen sich in SIL mittesten.
- **Lüfterlogik:** im TESTPLAN als „nächster Kandidat“ genannt; mit B-03 lohnt es sich jetzt.
- **Fehlerfälle der Firmware:** Busausfall, Init-Fehler, EEPROM fehlt – kein Testfall im TESTPLAN.
- **Kein automatischer Lauf** bei Commits; ein Skript `tools/check.cmd` (Build, SIL, PWA) würde reichen.

## 8. Empfohlene Reihenfolge

**Vor dem ersten Flash ins Fahrzeug**
1. B-01 Rückfallverhalten bei veralteten SDU-Daten festlegen (`/re-refine`).
2. `seldir`-Richtung und 10-ms- gegen 30-ms-Takt entscheiden (beide bekannt).
3. B-02 mit einem Mitschnitt klären.
4. B-05 Watchdog, B-06 Wiederanlauf, U-06 LED.
5. U-10 Listen-only-Schalter für die Inbetriebnahme.

**Kleine Änderungen mit großer Wirkung (je unter einer Stunde)**
- P-01 `fields=` bei Setzbefehlen.
- B-10 Antwort auf `/api/set`.
- B-11 ECU-Task vor dem EEPROM starten.
- B-12, B-13, B-14 Kleinfehler.
- U-01 Firmware-Feldnamen in der PWA, „–“ statt Ersatzwerten.
- Doku-Abweichungen aus Abschnitt 7.

**Danach**
- A-03 Webserver (löst B-07, B-08, U-04).
- A-06 Codec-Tests.
- U-02 `stale` in der Oberfläche.
- B-03, B-04, B-09.
- A-07, A-08 beim GUI-Aufräumen.
