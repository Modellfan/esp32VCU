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
| `tests/` | pytest + Playwright (Chromium, 1024×600) |
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

- **Service Worker nur über HTTPS oder localhost.** Über `http://` vom Zugangspunkt der VCU registriert der Browser keinen Service Worker. Manifest, Icons und Vollbild beim Ablegen auf dem Homescreen funktionieren trotzdem; Offline-Start aus dem Cache nicht. Getestet ist der Service Worker auf localhost.
- **Intro-Video entfällt.** `intro.mp4` (735 KB) passt nicht ins Budget; das Intro zeigt das Standbild `i/intro.webp`. Für ein Video bräuchte es eine kleinere Fassung (z. B. mit ffmpeg).
- **Energiefluss-Editor** (`?energyEditor=1`) speichert über `POST /api/energy-flow-layout`, das nur der Python-Testserver kennt, nicht die Firmware.
- Getestet nur mit Chromium. Welchen Browser das Navi-Display nutzt, ist offen.

## Auf die VCU bringen (noch offen, Firmware-Aufgabe)

1. `platformio.ini`: `data_dir = pwa/dist` (heute `data`; deren 18 MB passen nicht ins 1,5-MB-SPIFFS).
2. Firmware: `server.serveStatic("/", SPIFFS, "/")` mit Standarddatei `index.html`, MIME-Typ für `.webp` und `.json`, `Cache-Control` für statische Dateien; die alten Einzelrouten (`/touch.html`, Favicons, `index.html` der Handy-UI) entfallen.
3. `python pwa/tools/build.py`, dann `pio run -t uploadfs`.
