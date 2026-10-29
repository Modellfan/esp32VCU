# Touch-UI als PWA (`pwa/`)

Die Touch-Oberfläche für das Navi-Display (1024×600) als Progressive Web App. Die VCU liefert sie aus ihrem SPIFFS aus (VCU-REQ-011). Die bisherige UI in `data/touch.html` und `data/touch/` bleibt unverändert; dort liegen auch die Bilder in voller Auflösung.

## Aufbau

| Pfad | Inhalt |
|---|---|
| `src/` | Quelltext der PWA (kurze, SPIFFS-taugliche Pfade, Bilder als WebP) |
| `src/js/net.js` | Netzwerkschicht: Zeitlimit, Antwortprüfung, Setz-Warteschlange, Abfrage mit Backoff |
| `src/js/pwa.js`, `src/sw.js`, `src/manifest.json` | PWA-Hülle: Registrierung, Service Worker, Manifest |
| `tools/import_touch_ui.py` | einmalige Übernahme aus `data/touch` (Bilder → WebP, Pfade umschreiben); handgepflegte Dateien werden nur mit `--force` überschrieben |
| `tools/build.py` | baut `dist/` (Text gzip-komprimiert, Service Worker mit Version und Precache-Liste) und prüft Budget, Dateinamen und Verweise |
| `tools/serve.py` | Testserver, der sich wie die VCU verhält (gzip-Auslieferung, Mock-API mit Fehlereinspeisung) |
| `tests/` | pytest + Playwright (Chromium, 1024×600), u. a. Offline-Szenario des Radios |
| `dist/` | Build-Ergebnis für das SPIFFS-Image (nicht versioniert) |

## Befehle

```bash
python pwa/tools/build.py
python pwa/tools/serve.py            # http://127.0.0.1:8090/
python -m pytest pwa/tests -q        # baut selbst, ca. 5 min
```

## Härtung gegenüber der bisherigen Touch-UI

- Jede Abfrage hat ein Zeitlimit (2,5 s); eine hängende VCU blockiert die Oberfläche nicht.
- Höchstens eine Live-Abfrage gleichzeitig; nach Fehlern wartet die App 1, 2, 4, max. 5 s.
- „Verbindung unterbrochen“ erscheint erst nach zwei Fehlversuchen (kein Flackern), sofort beim Start ohne Daten.
- Antworten müssen ein JSON-Objekt sein; kaputtes JSON, Arrays, HTTP-Fehler und verschachtelte Werte werden verworfen.
- Setzbefehle laufen nacheinander, je Wert gilt nur der letzte (Schieberegler erzeugt keine Anfrageflut). Ein gesetzter Wert springt nicht durch eine ältere Live-Antwort zurück (1,5 s Schutzzeit).
- Im Hintergrund (Seite verdeckt) wird nicht abgefragt.
- Intro höchstens 6 s, auch wenn ein Bild nicht lädt.
- Behobener Fehler der alten UI: Zeilen mit Reglern (Ziel-Ladestand, AC-Strom, Poti) öffneten das Diagramm statt des Einstelldialogs; der Dialog war nie erreichbar.

## Grenzen

- **Service Worker nur in sicherem Kontext.** Browser starten Service Worker nur über HTTPS oder localhost. Für das Android-Radio wird Chrome deshalb so eingestellt, dass es die VCU-Adresse als sicher behandelt (siehe unten). Ohne diese Einstellung läuft die App ohne Offline-Cache; die Bilder kommen dann aus dem normalen Browser-Cache (versionierte Adressen, 1 Jahr gültig).
- **Intro-Video entfällt.** `intro.mp4` (735 KB) passt nicht ins Budget; das Intro zeigt das Standbild `i/intro.webp`. Für ein Video bräuchte es eine kleinere Fassung (z. B. mit ffmpeg).
- **Energiefluss-Editor** (`?energyEditor=1`) speichert über `POST /api/energy-flow-layout`, das nur der Python-Testserver kennt, nicht die Firmware.
- Getestet nur mit Chromium. Welchen Browser das Navi-Display nutzt, ist offen.

## Offline-Cache auf dem Android-Radio (Chrome)

Einmalig am Radio, solange es mit dem WLAN `eboxster` verbunden ist:

1. Chrome öffnen, `chrome://flags` eingeben.
2. Nach **insecure origins** suchen: Eintrag „Insecure origins treated as secure“.
3. In das Textfeld `http://192.168.4.1` eintragen und auf **Enabled** stellen.
4. Unten **Relaunch** tippen.
5. `http://192.168.4.1/` öffnen und warten, bis im Netzwerk-Fenster (Tipp auf die Uhrzeit oben rechts) bei **Offline** „aktiv (… Dateien)“ steht.
6. Chrome-Menü → **Zum Startbildschirm hinzufügen** bzw. **App installieren**.

Danach startet die App auch ohne VCU aus dem Cache, mit allen Bildern; die Live-Werte zeigen „VCU connection lost“, bis die VCU wieder erreichbar ist. Nach einem `uploadfs` mit neuer Version lädt der Service Worker die geänderten Dateien beim nächsten Verbindungsaufbau nach und übernimmt sie, sobald die App im Hintergrund ist.

Hinweise:
- Die Adresse muss genau stimmen: `http://192.168.4.1` (Zugangspunkt der VCU, ohne Schrägstrich am Ende). Über die Heimnetz-Adresse gilt die Einstellung nicht.
- Chrome-Updates können Flags zurücksetzen; dann zeigt das Netzwerk-Fenster „aus: http ohne Chrome-Flag“.
- Getestet mit Chromium und genau diesem Flag (`tests/test_pwa.py::test_android_radio_chrome_flag_offline`), nicht auf dem Radio selbst.

## Auf die VCU bringen

`platformio.ini` nutzt `data_dir = pwa/dist`; die Firmware liefert die Dateien unter `/` aus (eigener Handler mit `.gz`-Varianten und WebP-MIME-Typ, VCU-DEC-012).

```bash
python pwa/tools/build.py
pio run -e esp32s3 -t uploadfs
```

Auf echter Hardware noch nicht geprüft.
