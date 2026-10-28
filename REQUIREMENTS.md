# Requirements – esp32VCU (Porsche 986 Elektro-Umbau)

Dies ist der **Soll-Zustand** der VCU. Nur Anforderungen dokumentieren, keine Implementierungsaufgaben oder ausformulierten Architekturbegründungen.

## Konvention
- Stabile IDs: `VCU-REQ-001`, `VCU-REQ-002` …; IDs nie wiederverwenden.
- Status: `draft` (Vorschlag), `approved` (bestätigt), `implemented` (umgesetzt), `verified` (Akzeptanzkriterium nachweislich geprüft).
- Jede Anforderung beschreibt **was**, nicht detailliert **wie**. CAN-Bitlayouts stehen in `documentation/can_signals.md` und den DBC-Dateien, nicht hier.
- Mindestens ein überprüfbares Akzeptanzkriterium pro Requirement.
- Offene Annahmen und Einschränkungen explizit ausweisen.
- Bei `verified`: Test/Prüfung mit Pfad oder genauer Fundstelle benennen.
- **Umsetzung:** Stand des Codes nach Commit `8222d88`. „Umgesetzt“ heißt nicht „verifiziert“.

Quellen: aus Code, `documentation/IMPLEMENTATION_LOG.md` und `documentation/can_signals.md` rückwirkend abgeleitet am 2026-10-28, ergänzt um Nutzerentscheidungen vom 2026-10-28. Alle Einträge stehen auf `draft`, bis der Nutzer sie bestätigt.

## Systemkontext
Die VCU ersetzt im elektrifizierten Porsche 986 das Verbrennungs-Motorsteuergerät auf dem Fahrzeug-CAN, liest den Antriebsumrichter (Tesla SDU mit OpenInverter stm32-sine) über einen zweiten CAN-Bus, passt das Kombiinstrument an die E-Maschine an, steuert Kühlmittellüfter und ein Digitalpotentiometer und liefert eine Touch-Oberfläche für das Navi-Display (1024×600). Das Projekt ist eigenständig; es gibt keine Abhängigkeit zu TCU, Backend oder App anderer Projekte (Nutzerentscheidung 2026-10-28).

## Anforderungen

### Fahrzeug-CAN und Motorsteuergerät-Ersatz

### VCU-REQ-001 – Emulation des Motorsteuergeräts
- **Typ:** funktional / Sicherheit
- **Priorität:** must
- **Status:** draft
- **Anforderung:** Die VCU muss auf dem Fahrzeug-CAN (500 kbit/s) die Botschaften des Motorsteuergeräts senden, die die übrigen Steuergeräte (Kombiinstrument, ABS/PSM, Getriebe) erwarten: 0x280 (Motor_1), 0x289 (Motor_2), 0x298 (Motor_3), 0x4E0 (MO5) und 0x7A0. Jede Botschaft wird alle 30 ms gesendet; die Sendezeitpunkte sind gleichmäßig verteilt. Nicht genutzte Bits tragen die in `can_signals.md` dokumentierten Standardwerte.
- **Akzeptanz:** Gegeben eine laufende VCU am Fahrzeug-CAN; wenn 10 s mitgeschnitten werden; dann erscheint jede der fünf IDs mit 30 ms ± 3 ms Abstand, die Inhalte entsprechen der DBC, und das Kombiinstrument zeigt keine Motorsteuergeräte-Störung.
- **Abhängigkeiten:** keine
- **Umsetzung:** vorhanden (`src/main.cpp` `emulateECUCAN`, 6-ms-Zeitschlitze; `src/986_ecu_messages.cpp`). Laut Log am 2026-02-25 geflasht; Zyklus und Fahrzeugreaktion nicht per Mitschnitt belegt.
- **Offen:** zulässige Zykluszeittoleranz; ob 0x4F8 (definiert, aber nicht gesendet) benötigt wird.

### VCU-REQ-002 – Schneller Start der Emulation
- **Typ:** Laufzeit / Sicherheit
- **Priorität:** must
- **Status:** draft
- **Anforderung:** Nach dem Einschalten muss die VCU die Botschaften aus VCU-REQ-001 spätestens nach 1 s senden, unabhängig vom Zustand von WLAN, Dateisystem oder Weboberfläche.
- **Akzeptanz:** Gegeben eine VCU ohne erreichbares Heim-WLAN; wenn sie eingeschaltet wird; dann erscheint die erste 0x280-Botschaft spätestens 1 s nach Anlegen der Versorgung.
- **Abhängigkeiten:** VCU-REQ-001, VCU-REQ-014
- **Umsetzung:** **nicht erfüllt.** `webinterfaceBegin()` wartet in `setup()` bis zu 15 s auf das Heim-WLAN; die Emulation startet erst danach (`src/webinterface.cpp`, `src/main.cpp`).
- **Nutzerentscheidung 2026-10-28:** Grenzwert 1 s bestätigt.

### VCU-REQ-003 – Dekodierung des Fahrzeug-CAN
- **Typ:** funktional
- **Priorität:** must
- **Status:** draft
- **Anforderung:** Die VCU muss die Fahrzeugbotschaften 0x1A0 (Bremse_1), 0x2A8 (Raddrehzahlen), 0x4A1, 0x510 und 0x520 (Kombiinstrument), 0x560, 0x440 (Getriebe) und 0x0C0 (Lenkwinkel) gemäß `can_signals.md` dekodieren und als Live-Werte bereitstellen.
- **Akzeptanz:** Gegeben ein CAN-Mitschnitt mit bekannten Werten; wenn er eingespielt wird; dann liefert `/api/live` für jedes dokumentierte Signal den nach DBC erwarteten Wert.
- **Abhängigkeiten:** keine
- **Umsetzung:** vorhanden (`src/986_vehicle_messages.cpp`).

### VCU-REQ-004 – Dekodierung des Antriebsumrichters
- **Typ:** funktional
- **Priorität:** must
- **Status:** draft
- **Anforderung:** Die VCU muss auf dem zweiten CAN-Bus (500 kbit/s) die Botschaften 0x500–0x508 des OpenInverter stm32-sine (Tesla SDU) dekodieren: Spannungen, Ströme, Drehzahl, Richtung, Temperaturen, Betriebsart, Fehler, digitale Eingänge und Laufzeit.
- **Akzeptanz:** Gegeben ein Mitschnitt des Umrichters; wenn er eingespielt wird; dann stimmen die Werte unter `sdu_*` in `/api/live` mit den Parametern des Umrichters (OpenInverter-Weboberfläche) überein.
- **Abhängigkeiten:** keine
- **Umsetzung:** vorhanden (`src/tesla_sdu_messages.cpp`).

### VCU-REQ-005 – Kombiinstrument für den Elektroantrieb
- **Typ:** funktional
- **Priorität:** should
- **Status:** draft
- **Anforderung:** Ist die Kombiinstrument-Anpassung aktiv, muss die VCU
  - im Drehzahlmesser die relative Antriebsleistung anzeigen: Rückwärtsgang und Neutral als feste Zeigerstellungen, Antrieb von der Mittelstellung bis Vollausschlag, Rekuperation unterhalb der Mittelstellung, ±5 % um null als Mittelstellung;
  - in der Kühlmitteltemperaturanzeige die Motortemperatur zeigen;
  - die Kühlmittelwarnung setzen, sobald Kühlkörper ≥ 80 °C oder Motor ≥ 160 °C erreicht.
  Die Anpassung lässt sich über die Oberfläche ein- und ausschalten und ist nach dem Start eingeschaltet.
- **Akzeptanz:** Gegeben die Anpassung ist aktiv; wenn der Umrichter 0 %, 50 % und 100 % Antriebsleistung sowie Rekuperation meldet; dann zeigt der Drehzahlmesser jeweils die zugeordnete Stellung, und bei 160 °C Motortemperatur leuchtet die Kühlmittelwarnung.
- **Abhängigkeiten:** VCU-REQ-001, VCU-REQ-004
- **Umsetzung:** vorhanden (`src/cluster_calc.cpp`); Bezugsleistung fest 348 V × 300 A (Antrieb) bzw. 100 A (Rekuperation).
- **Nutzerentscheidung 2026-10-28:** Der Schalter wird vorerst nicht gespeichert; nach jedem Start ist die Anpassung aktiv.

### VCU-REQ-006 – Verbrauchssignal für das Kombiinstrument
- **Typ:** funktional
- **Priorität:** could
- **Status:** draft
- **Anforderung:** Die VCU muss im MO5-Verbrauchssignal den tatsächlichen Energieverbrauch des Antriebs abbilden, sodass der Bordcomputer einen echten Verbrauch anzeigt. Im Stand ohne Leistungsabgabe steigt der Zähler nicht.
- **Akzeptanz:** Gegeben die Kombiinstrument-Anpassung ist aktiv; wenn eine Strecke mit bekannter Antriebsenergie (aus `sdu_udc` × `sdu_idc`) gefahren wird; dann entspricht der Verbrauch im Bordcomputer dieser Energie nach dem festgelegten Umrechnungsfaktor (± 5 %), und im Stand bleibt der Zählerstand gleich.
- **Abhängigkeiten:** VCU-REQ-005
- **Umsetzung:** vorhanden als Simulation: der Zähler steigt bei aktiver Anpassung unabhängig vom Fahrzustand um 0,5 ml alle 30 ms bis 20 l (`src/cluster_calc.cpp`).
- **Nutzerentscheidung 2026-10-28:** echten Verbrauch zeigen. Die heutige Simulation erfüllt das nicht.
- **Offen:** Umrechnungsfaktor (z. B. 1 kWh = 1 l oder Benzinäquivalent 8,9 kWh = 1 l); ob Rekuperation abgezogen wird.

### VCU-REQ-007 – Beschleunigungsmessung
- **Typ:** funktional
- **Priorität:** could
- **Status:** draft
- **Anforderung:** Die VCU muss die Zeiten 0–50 km/h und 0–100 km/h automatisch messen und je Disziplin den letzten und den besten Wert bereitstellen. Ein Lauf startet, wenn die Geschwindigkeit zuvor ≤ 0,5 km/h war und 1 km/h überschreitet; er endet bei 100 km/h oder bei erneutem Stillstand.
- **Akzeptanz:** Gegeben ein Geschwindigkeitsverlauf mit bekannter Zeit bis 50 und 100 km/h; wenn er eingespielt wird; dann weichen die gemeldeten Zeiten höchstens 0,1 s ab, und ein Lauf ohne vorherigen Stillstand wird nicht gewertet.
- **Abhängigkeiten:** VCU-REQ-003 (Geschwindigkeit aus 0x510)
- **Umsetzung:** vorhanden (`src/cluster_calc.cpp`, Anzeige Touch-UI „Stopwatch“).
- **Nutzerentscheidung 2026-10-28:** Bestwerte werden vorerst nicht gespeichert.

### Ausgänge

### VCU-REQ-008 – Kühlmittellüfter zweistufig
- **Typ:** funktional
- **Priorität:** should
- **Status:** draft
- **Anforderung:** Die VCU muss den Kühlmittellüfter in den Stufen Aus, Stufe 1 und Stufe 2 ansteuern. Die Stufe wird automatisch aus Motor- und Kühlkörpertemperatur bestimmt (mit Hysterese). Über die Oberfläche lässt sich die Stufe manuell übersteuern und die Automatik wieder einschalten.
- **Akzeptanz:** Gegeben eine VCU am Prüfstand; wenn Stufe 0, 1 und 2 manuell gesetzt werden; dann führen die Ausgänge Stufe 1/Stufe 2 die Pegel 0/0, 1/0, 1/1, und der CAN-Bus 2 arbeitet dabei ungestört weiter. Wenn im Automatikbetrieb die Temperatur die Schwelle für Stufe 1 überschreitet; dann schaltet Stufe 1 ein und erst unterhalb der Schwelle minus Hysterese wieder aus.
- **Abhängigkeiten:** keine
- **Umsetzung:** nur manuell vorhanden (`src/coolant_fan_control.cpp`), **Pinkonflikt:** Lüfter Stufe 1 (`COOLANT_FAN_LOW_PIN=7`, `platformio.ini`) und Chip-Select von CAN 2 (`MCP2515_CS_CAN2 = 7`, `src/canmanager.cpp`) nutzen beide GPIO7.
- **Nutzerentscheidung 2026-10-28:** automatisch und manuell übersteuerbar. GPIO7 ist der Chip-Select von CAN 2; der Lüfter-Pin ist falsch.
- **Offen:** tatsächlicher Pin für Stufe 1 (vor dem nächsten Flashen an der Platine prüfen); Schalt- und Hysteresewerte.

### VCU-REQ-009 – Digitales Potentiometer
- **Typ:** funktional
- **Priorität:** should
- **Status:** draft
- **Anforderung:** Die VCU muss über ein digitales Potentiometer (X9C103S, 10 kΩ) den Tankgeber des Kombiinstruments nachbilden und in 1-%-Schritten von 0 bis 100 % einstellen. Vorerst wird der Wert manuell über die Oberfläche gesetzt.
- **Akzeptanz:** Gegeben das Potentiometer am Prüfstand; wenn 0 %, 50 % und 100 % gesetzt werden; dann misst man am Schleifer ca. 0 Ω, 5 kΩ und 10 kΩ gegen das Fußende (± 20 % Bauteiltoleranz).
- **Abhängigkeiten:** keine
- **Umsetzung:** vorhanden (`src/digipot_control.cpp`, `lib/X9C103S`).
- **Nutzerentscheidung 2026-10-28:** Tankanzeige, vorerst manuell; Wert wird nicht gespeichert.
- **Offen:** spätere Kopplung an SoC (VCU-REQ-018); Zuordnung Prozent → Zeigerstellung der Tankanzeige.

### Oberfläche und Schnittstellen

### VCU-REQ-010 – Lokale Live-Schnittstelle
- **Typ:** Schnittstelle
- **Priorität:** must
- **Status:** draft
- **Anforderung:** Die VCU muss alle dekodierten, berechneten und gesetzten Werte als JSON unter `GET /api/live` bereitstellen; mit `fields=` lässt sich die Antwort auf einzelne Felder beschränken. Sollwerte werden über `/api/set`, `/api/set-ecu` und `/api/set-cluster` gesetzt; die Antwort enthält den neuen Live-Stand.
- **Akzeptanz:** Gegeben eine laufende VCU; wenn `/api/live?fields=sdu_udc,rpm` abgerufen wird; dann enthält die Antwort genau diese Felder als gültiges JSON. Wenn `/api/set?coolant_fan_level=1` aufgerufen wird; dann meldet die Antwort `coolant_fan_level` 1.
- **Abhängigkeiten:** VCU-REQ-003, VCU-REQ-004
- **Umsetzung:** vorhanden (`src/webinterface.cpp`). Die Feldliste ist nur im Code definiert.
- **Lücke:** Die Touch-UI fordert Felder an, die die Firmware nicht liefert (`soc`, `range_km`, Ladestecker-/Ladezustand, `cluster_trip_distance_km`) und ruft `POST /api/energy-flow-layout` auf, das es nur im Python-Testserver gibt.

### VCU-REQ-011 – Touch-Oberfläche für das Navi-Display
- **Typ:** funktional / Bedienung
- **Priorität:** must
- **Status:** draft
- **Anforderung:** Die VCU muss eine Oberfläche für das Navi-Display mit 1024×600 Pixeln ausliefern; sie ist die einzige Oberfläche der VCU und unter `/` erreichbar. Inhalte: Startseite mit Akku- und Fahrzeugdarstellung, Energiefluss, Fahrzeug (Antrieb, Akku, Lader), Einstellungen (Ziel-Ladestand, AC-Ladestrom, Potentiometer), Belüftung, Kühlung, Kombiinstrument und Stoppuhr. Beim Start läuft ein Intro-Video von etwa 1 s. Elemente ohne Datenquelle sind als Muster gekennzeichnet und schreiben keine Werte.
- **Akzeptanz:** Gegeben ein Browser mit 1024×600; wenn `/` und jede Hauptseite geöffnet wird; dann passt der Inhalt ohne Scrollbalken, Live-Werte aktualisieren sich, und die Screenshot-Prüfung (`tools/touch_ui_screenshot_validate.py`) läuft ohne Fehler.
- **Abhängigkeiten:** VCU-REQ-010
- **Umsetzung:** vorhanden unter `/touch.html` (`data/touch.html`, `data/touch/`). `/` liefert noch die frühere Handy-Oberfläche (`data/index.html`); sie war Designvorlage für ein anderes Projekt und wird entfernt (Nutzerentscheidung 2026-10-28). Die Seite `styleTest` enthält Musterbedienelemente ohne Funktion (Fahrwerkshöhe, Lenkunterstützung, Sound, Simulationsschalter).
- **Offen:** Welche Musterelemente echte Funktionen werden sollen.

### VCU-REQ-012 – CAN-Diagnose über USB
- **Typ:** Diagnose
- **Priorität:** could
- **Status:** draft
- **Anforderung:** Die VCU muss über die USB-Schnittstelle das GVRET-Protokoll sprechen, sodass SavvyCAN beide CAN-Busse mitlesen kann; gesendete Frames werden von empfangenen unterscheidbar markiert.
- **Akzeptanz:** Gegeben SavvyCAN über USB verbunden; wenn die VCU läuft; dann zeigt SavvyCAN die Frames beider Busse und die emulierten Motorsteuergeräte-Frames.
- **Abhängigkeiten:** VCU-REQ-001
- **Umsetzung:** teilweise. `gvret_loop()` läuft, die Weiterleitung `sendFrameToUSB` ist in `src/main.cpp` auskommentiert. Statusausgaben über `Serial` mischen sich in den GVRET-Datenstrom.

### VCU-REQ-013 – Gültigkeit und Alter der Werte
- **Typ:** funktional / Datenqualität
- **Priorität:** must
- **Status:** draft
- **Anforderung:** Für jedes CAN-Signal muss die VCU erkennen, ob es innerhalb einer festgelegten Zeit empfangen wurde. Fehlende oder veraltete Werte werden als ungültig gekennzeichnet und weder als 0 noch als letzter Wert ausgegeben.
- **Akzeptanz:** Gegeben der Umrichter-CAN ist getrennt; wenn 1 s vergeht; dann kennzeichnet `/api/live` alle `sdu_*`-Werte als ungültig, und die Oberfläche zeigt „–“ statt 0.
- **Abhängigkeiten:** VCU-REQ-003, VCU-REQ-004
- **Umsetzung:** fehlt. Werte starten mit Ersatzwerten (meist 0) und bleiben nach Busausfall stehen.
- **Offen:** Zeitgrenze je Botschaft (Vorschlag: 3 × Zykluszeit, mindestens 300 ms) und Darstellung im JSON.

### Netzwerk und Sicherheit

### VCU-REQ-014 – WLAN im Fahrzeug und zu Hause
- **Typ:** funktional
- **Priorität:** must
- **Status:** approved (Nutzerentscheidung 2026-10-28)
- **Anforderung:** Die VCU muss immer einen eigenen WLAN-Zugangspunkt bereitstellen, über den die Oberfläche erreichbar ist. Ist das Heim-WLAN erreichbar, verbindet sie sich zusätzlich damit. Der Verbindungsaufbau blockiert weder die CAN-Verarbeitung noch die Emulation.
- **Akzeptanz:** Gegeben kein Heim-WLAN in Reichweite; wenn die VCU startet; dann ist der eigene Zugangspunkt nach spätestens 5 s sichtbar und `/` erreichbar. Gegeben Heim-WLAN erreichbar; dann ist die VCU zusätzlich unter ihrer Heimnetz-Adresse erreichbar.
- **Abhängigkeiten:** VCU-REQ-002, VCU-REQ-015
- **Umsetzung:** fehlt. Aktuell nur Heim-WLAN als Client, Start blockiert bis 15 s (`src/webinterface.cpp`). Früher gab es einen offenen Zugangspunkt `eboxster` (Log 2026-02-23).
- **Offen:** Name und Passwort des Zugangspunkts (VCU-DEC-005).

### VCU-REQ-015 – Keine Zugangsdaten im Repository
- **Typ:** Sicherheit
- **Priorität:** must
- **Status:** approved (Nutzerentscheidung 2026-10-28)
- **Anforderung:** WLAN-Passwörter und andere Zugangsdaten dürfen nicht in versionierten Dateien stehen.
- **Akzeptanz:** `git grep` nach dem Heim-WLAN-Passwort im aktuellen Stand findet nichts; ein frischer Klon baut mit einer Vorlagedatei für die Zugangsdaten.
- **Abhängigkeiten:** keine
- **Umsetzung:** fehlt (Klartext in `src/webinterface.cpp`, seit Commit `48a13b7` auf `origin/esp32vcu_986`). Das Repo ist laut Nutzer privat; die Git-History enthält das Passwort weiterhin.

### VCU-REQ-016 – Schutz der Schreibzugriffe
- **Typ:** Sicherheit
- **Priorität:** should
- **Status:** approved (Nutzerentscheidung 2026-10-28)
- **Anforderung:** Werte, die das Fahrzeugverhalten beeinflussen (emulierte Motorsteuergeräte-Signale, Warnlampen, Lüfter, Potentiometer, Ladeparameter), dürfen nur von berechtigten Geräten gesetzt werden.
- **Akzeptanz:** Gegeben ein Gerät ohne WLAN-Passwort; dann kann es sich nicht mit dem Zugangspunkt verbinden und keinen Setz-Endpunkt erreichen.
- **Abhängigkeiten:** VCU-REQ-014
- **Umsetzung:** fehlt. Alle Setz-Endpunkte sind ungeschützte GET-Aufrufe.
- **Nutzerentscheidung 2026-10-28:** Ein WPA2-Passwort am Zugangspunkt genügt; wer im WLAN ist, darf setzen. Gilt mit VCU-REQ-014 als erfüllt.

### Laden und Batterie

### VCU-REQ-017 – Ladeparameter
- **Typ:** funktional
- **Priorität:** should
- **Status:** draft
- **Anforderung:** Die VCU muss den Ziel-Ladestand (50–100 % in 5-%-Schritten) und das AC-Ladestromlimit (6–32 A in 1-A-Schritten) annehmen und beim Laden anwenden.
- **Akzeptanz:** Gegeben Ziel 80 % und 10 A; wenn geladen wird; dann überschreitet der AC-Strom 10 A nicht, und das Laden endet bei 80 %.
- **Abhängigkeiten:** VCU-REQ-018
- **Umsetzung:** teilweise. Werte lassen sich in der Touch-UI setzen und werden begrenzt (`src/webinterface.cpp`), wirken aber auf kein Ladegerät.
- **Nutzerentscheidung 2026-10-28:** Werte werden vorerst nicht über einen Neustart gespeichert.

### VCU-REQ-018 – Batterie- und Ladedaten
- **Typ:** Schnittstelle
- **Priorität:** should
- **Status:** draft
- **Anforderung:** Die VCU muss über einen eigenen, dritten CAN-Bus Ladestand (SoC), Batteriespannung, Batteriestrom, Ladestecker verbunden, Ladeart und Ladeleistung von BMS und Ladegerät empfangen und in der Oberfläche anzeigen.
- **Akzeptanz:** Gegeben BMS und Ladegerät am Bus; wenn geladen wird; dann liefert `/api/live` SoC, Batteriestrom und Steckerstatus mit gültigem Alter, und die Startseite zeigt sie an.
- **Abhängigkeiten:** VCU-REQ-013
- **Umsetzung:** fehlt. Weder BMS- noch Ladegerätebotschaften werden dekodiert; die Touch-UI erwartet die Werte bereits (inkl. Reichweite).
- **Nutzerentscheidung 2026-10-28:** eigener dritter CAN-Bus (zusätzlicher Controller nötig).
- **Offen:** Botschaften von BMS und Ladegerät, Controller und Pins des dritten Busses; ob die Reichweite in der VCU berechnet wird.

### Sonstiges

### VCU-REQ-019 – Betriebsanzeige
- **Typ:** Diagnose
- **Priorität:** could
- **Status:** draft
- **Anforderung:** Die eingebaute RGB-LED blinkt im Betrieb grün mit 1 Hz und geringer Helligkeit, solange die Hauptschleife läuft.
- **Akzeptanz:** Sichtprüfung nach dem Start.
- **Abhängigkeiten:** keine
- **Umsetzung:** vorhanden (`src/main.cpp` `blinkLED`, 10 % Helligkeit).
