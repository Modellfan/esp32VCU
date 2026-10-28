---
name: re-refine
description: Nutze diesen Skill, wenn neue Funktionen oder geändertes Systemverhalten aus Nutzerwünschen in überprüfbare Requirements übertragen werden sollen oder eine wesentliche Architekturentscheidung dokumentiert werden muss.
---

# RE-Light: Anforderungen verfeinern

1. Lies die relevanten Abschnitte von `README.md`, `REQUIREMENTS.md` und bei Architekturbezug `DECISIONS.md`. Wenn nötig, prüfe betroffenen Code.
2. Bestimme, ob die Anfrage eine neue Anforderung, eine Änderung an bestehendem Verhalten oder nur eine Aufgabe ohne neue Anforderung ist.
3. Wiederverwende vorhandene REQ-IDs für geändertes Verhalten; vergib neue IDs nur für neue, eigenständige Anforderungen.
4. Schreibe kurze, eindeutige Aussagen über beobachtbares Soll-Verhalten; ergänze testbare Akzeptanzkriterien, Priorität und Status. Keine vorgetäuschten Freigaben.
5. Prüfe wesentliche Randbedingungen (z. B. Fehlerfälle, Sicherheit, Echtzeit, Offline-Nutzung, Migration) nur soweit relevant. Keine Vollständigkeitsbürokratie.
6. Erfordert die Änderung eine wesentliche technische Abwägung, schlage einen kompakten DEC-Eintrag mit Alternativen, Begründung und Konsequenzen vor. Technische Details gehören nicht in Anforderungen.
7. Aktualisiere ausschließlich betroffene Dokumente und die README nur bei einer tatsächlich veränderten Projektübersicht. Falls eine Produkt- oder Architekturfrage entscheidend offen ist, kennzeichne sie und triff keine stille Festlegung.
8. Gib einen kurzen Änderungsbericht mit REQ-/DEC-IDs und offenen Fragen zurück.

## Qualitätstest
Jedes Requirement muss verständlich, einzeln prüfbar und möglichst unabhängig von einer bestimmten technischen Lösung sein. Ein Test oder eine konkrete Beobachtung muss die Erfüllung bestimmen können.
