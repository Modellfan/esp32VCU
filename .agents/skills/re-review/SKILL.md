---
name: re-review
description: Nutze diesen Skill vor großen oder riskanten Änderungen, bei Architekturwechseln und beim Abschluss relevanter Features, um Requirements, technische Entscheidungen, Implementierung und Nachweise auf Konflikte zu prüfen.
---

# RE-Light: Review

1. Lege zuerst den Prüfbereich fest (geänderte REQ-/DEC-IDs, betroffene Codepfade, Tests). Lies nur zugehörige Dokumentation.
2. Prüfe die Anforderungen auf Eindeutigkeit, Testbarkeit, Widersprüche, fehlende kritische Fälle und unbestätigte Annahmen.
3. Bei Implementierungsbezug prüfe, ob Code und Tests das gewünschte Verhalten tatsächlich stützen. Suche nach Regressionen und Auswirkungen auf andere Anforderungen.
4. Berichte Befunde sortiert nach `kritisch`, `relevant`, `Hinweis`, jeweils mit konkretem Beleg und nachvollziehbarer Empfehlung.
5. Unterscheide `implementiert` strikt von `verifiziert`. Behaupte keine ausgeführten Tests, die nicht ausgeführt wurden.
6. Schlage kleine, gezielte Korrekturen für Requirements, Decisions, Tests oder Code vor. Keine weiteren Protokoll- oder Auditdateien erzeugen.
7. Fasse zum Schluss zusammen: geprüfter Scope, wichtigste Befunde, fehlende Nachweise, Restunsicherheit.

## Ausgabeformat

```text
Scope:
Befunde:
- [Schweregrad] Datei:Abschnitt – Beobachtung – Auswirkung – Empfehlung
Fehlende Nachweise:
Offene Fragen:
```
