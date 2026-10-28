# Agentenanweisungen – RE-Light (esp32VCU)

## Dokumentationsquellen
- `README.md`: Projektziel, Hardware, Build, Aufbau und offene Fragen.
- `REQUIREMENTS.md`: verbindlicher Soll-Zustand der VCU (Requirements `VCU-REQ-…`).
- `DECISIONS.md`: Begründungen und Konsequenzen wesentlicher technischer Entscheidungen (`VCU-DEC-…`).
- `SUB_AGENTS.md`: Rollen, Delegationsregeln und Ausgabeformate (bei Delegation lesen).
- `TESTPLAN.md`: Testfälle je Requirement, Teststufen und Abnahmeregeln für `verified`.
- `documentation/can_signals.md` und die DBC-Dateien: CAN-Signaldefinitionen (maßgeblich für Kodierung und Dekodierung).
- `documentation/IMPLEMENTATION_LOG.md`: Änderungsprotokoll (siehe Arbeitsregel 8).
- Das Projekt ist eigenständig; es gibt keine Abhängigkeit zu anderen Repos (Nutzerentscheidung 2026-10-28).

Lies gezielt nur die für die Aufgabe benötigten Abschnitte. Erzeuge keine weiteren Spec-, Proposal-, Plan- oder Task-Dateien, außer der Nutzer verlangt dies ausdrücklich.

## Arbeitsregeln
1. **Kleine Änderungen:** Korrigiere reine Tippfehler, Refactorings ohne Verhaltensänderung und kleine Bugs ohne zusätzlichen RE-Prozess. Passe Dokumente nur bei geänderten Aussagen an.
2. **Neues oder geändertes Systemverhalten:** Verwende `$re-refine`. Ordne Anforderungen vorhandenen VCU-REQ-IDs zu oder ergänze neue IDs. Formuliere beobachtbare Akzeptanzkriterien. Erfinde keine bestätigten Anforderungen.
3. **Architekturentscheidungen:** Dokumentiere nur signifikante Entscheidungen in `DECISIONS.md`, insbesondere Änderungen von Schnittstellen, CAN-Botschaften, Persistenz, Zeitverhalten, Pinbelegung, Sicherheits- und Hardwaregrenzen.
4. **Vor riskanten oder größeren Änderungen:** Verwende `$re-review` und untersuche Abhängigkeiten, Widersprüche, Auswirkungen und Testbarkeit. Bei Unsicherheit betroffene Stellen als offen kennzeichnen statt als geklärt auszugeben.
5. **Implementierung:** Maßgeblich sind bestätigte Anforderungen und Entscheidungen. Verifiziere betroffene Akzeptanzkriterien mit passenden Tests oder beschreibe ehrlich, welche Prüfungen nicht durchgeführt wurden.
6. **Dokumentation nachziehen:** Aktualisiere nur die betroffenen Abschnitte. Keine Doppelpflege derselben Details in README, Requirements, Decisions und `can_signals.md`.
7. **Abschluss:** Berichte knapp: geändertes Verhalten, betroffene REQ-/DEC-IDs, Tests und offene Fragen.
8. **Änderungsprotokoll:** Jede Code- oder Flash-Aufgabe bekommt einen Eintrag in `documentation/IMPLEMENTATION_LOG.md` nach der dortigen Vorlage (Zeitstempel mit Zeitzone, Änderungen, ausgeführte Befehle mit PASS/FAIL, Ergebnis, Artefakte). Das Log ist Nachweisquelle, keine Anforderungsquelle.

## Projektspezifische Grenzen
- **Fahrzeugsicherheit:** Die VCU sendet auf dem Fahrzeug-CAN anstelle des Motorsteuergeräts. Änderungen an gesendeten Botschaften, Zeitschlitzen oder Standardbits nur mit Bezug auf `can_signals.md`/DBC und mit CAN-Mitschnitt prüfen. Nie ungeprüft auf das Fahrzeug flashen.
- **Pinbelegung:** Vor neuen Ausgängen alle belegten GPIOs prüfen (`platformio.ini` `build_flags`, `src/canmanager.cpp`, `src/utils.h`).
- **Hardwarezugriff:** Flashen (`pio run -t upload`, `-t uploadfs`) und serielle Befehle an die angeschlossene Hardware nur auf ausdrückliche Anweisung des Nutzers. Lesen des seriellen Monitors ist erlaubt.
- **Keine Zugangsdaten im Repo:** WLAN-Passwörter und Schlüssel gehören in `src/secrets.h` (nicht versioniert, VCU-REQ-015).
- **Nur Referenz:** `documentation/touch-ui-reference/` und Bildmaterial von Porsche nur als Gestaltungsreferenz; geschützte Icon-Sätze bleiben außerhalb von Git (`.gitignore`).

## Delegation
- Supervisor trägt die Integrationsverantwortung und entscheidet, was ins Repo geschrieben wird.
- Verwende `SUB_AGENTS.md`, wenn Recherche, Anforderungsanalyse, Architekturabgleich oder Review sinnvoll unabhängig delegiert werden können.
- Reviewer sollen grundsätzlich nur lesen und Befunde zurückgeben. Keine parallelen Schreibzugriffe auf dieselben Dateien.
- Sub-Agenten dürfen keine Produkt- oder Architekturentscheidungen stillschweigend als verbindlich festlegen.

## Definition of Done
Eine verhaltensrelevante Änderung ist erst abgeschlossen, wenn Implementierung und betroffene Akzeptanzkriterien überprüft wurden, relevante Dokumente aktuell sind und nicht prüfbare Punkte explizit benannt wurden. `verified` nur bei tatsächlich vorhandener Evidenz setzen.
