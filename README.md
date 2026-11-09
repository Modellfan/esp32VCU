# esp32VCU – Fahrzeugsteuergerät für den elektrifizierten Porsche 986

Die VCU ersetzt das Motorsteuergerät auf dem Fahrzeug-CAN, liest den Antriebsumrichter (Tesla SDU mit OpenInverter), passt das Kombiinstrument an den E-Antrieb an, steuert Kühlmittellüfter und Tankanzeige und liefert die Touch-Oberfläche für das Navi-Display (1024×600).

- Anforderungen: [REQUIREMENTS.md](REQUIREMENTS.md) · Entscheidungen: [DECISIONS.md](DECISIONS.md) · Tests: [TESTPLAN.md](TESTPLAN.md)
- Architektur: [documentation/architecture.html](documentation/architecture.html) (interaktiv, Details im JSON-Block der Datei) · Übersicht [architecture.svg](documentation/architecture.svg) · CAN-Signale: [documentation/can_signals.md](documentation/can_signals.md)
- Änderungsprotokoll: [documentation/IMPLEMENTATION_LOG.md](documentation/IMPLEMENTATION_LOG.md)

## Hardware
ESP32-S3-DevKitC-1 (8 MB), 2× MCP2515 (CAN 1 Fahrzeug, CAN 2 Umrichter), X9C103S (Tankgeber), zwei Lüfterrelais, RGB-LED. Pinbelegung: [src/board.h](src/board.h).

## Aufbau
| Ordner | Inhalt |
|---|---|
| `src/` | Firmware nach Ebenen: `app/`, `data/`, `can/`, `drivers/`, `services/` (siehe Kopf von `src/main.cpp`) |
| `pwa/` | Touch-UI als PWA, Build und Tests ([pwa/README.md](pwa/README.md)) |
| `documentation/` | CAN-Signale, DBC-Dateien, Architekturbild, Protokoll |
| `tools/` | `generate_pq_messages_md.py`: erzeugt `documentation/PQ_messages.md` aus der VW-DBC |

## Build und Flashen
```bash
copy src\secrets.example.h src\secrets.h   # einmalig, Zugangsdaten eintragen
python pwa/tools/build.py                   # PWA nach pwa/dist
pio run -e esp32s3                          # Firmware
pio run -e esp32s3 -t upload                # Firmware flashen
pio run -e esp32s3 -t uploadfs              # PWA ins SPIFFS
```
`pio` liegt unter `%USERPROFILE%\.platformio\penv\Scripts\pio.exe`.

**Bekanntes Problem (Windows, pioarduino 55.03.37):** Der Compiler `toolchain-xtensa-esp-elf` landet mit einer Ordnerebene zu viel (`xtensa-esp-elf/bin`). Abhilfe am 2026-10-28: Inhalt von `%USERPROFILE%\.platformio\tools\toolchain-xtensa-esp-elf\xtensa-esp-elf\` flach nach `...\packages\toolchain-xtensa-esp-elf\` kopiert, `package.json` und `.piopm` dazu.

## Schnittstellen
- `GET /` – Touch-UI (PWA)
- `GET /api/live[?fields=a,b]` – alle Werte; `stale` listet veraltete Felder
- `GET /api/signals` – Signalliste mit Typ, Quelle, Schreibrecht
- `GET /api/set?…` – Ladeparameter, Lüfter (`coolant_fan_level`, `coolant_fan_mode=auto|manual`), Poti, `rpm`/`coolant`/`oil` (Overrides)
- `GET /api/set-ecu?…` – schreibbare Motorsteuergerät-Signale; `auto` hebt einen Override auf
- `GET /api/set-cluster?activated=0|1`
- UART0 (115200): GVRET für SavvyCAN · nativer USB-Port: Textlog

## Offene Punkte
Pin Lüfter Stufe 1, Lüfterschwellen, Umrechnung Verbrauch, BMS-/Ladegerät-Botschaften und CAN-3-Pins, Zeitgrenze für veraltete Werte, Browser des Navi-Displays. Details in REQUIREMENTS.md und DECISIONS.md.
