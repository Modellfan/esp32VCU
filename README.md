# esp32VCU – Fahrzeugsteuergerät für den elektrifizierten Porsche 986

Die VCU ersetzt das Motorsteuergerät auf dem Fahrzeug-CAN, liest den Antriebsumrichter (Tesla SDU mit OpenInverter), passt das Kombiinstrument an den E-Antrieb an, steuert Kühlmittellüfter und Tankanzeige und liefert die Touch-Oberfläche für das Navi-Display (1024×600).

- Anforderungen: [REQUIREMENTS.md](REQUIREMENTS.md) · Entscheidungen: [DECISIONS.md](DECISIONS.md) · Tests: [TESTPLAN.md](TESTPLAN.md)
- Architektur: [documentation/architecture.html](documentation/architecture.html) (interaktiv, Details im JSON-Block der Datei) · Übersicht [architecture.svg](documentation/architecture.svg) · CAN-Signale: [documentation/can_signals.md](documentation/can_signals.md)
- Änderungsprotokoll: [documentation/IMPLEMENTATION_LOG.md](documentation/IMPLEMENTATION_LOG.md)

## Hardware
ESP32-S3-DevKitC-1 (8 MB), 2× MCP2515 (CAN1 Fahrzeug, CAN2 Umrichter), TWAI als CAN0 (BMS/Lader, GPIO21/20), X9C103S (Tankgeber), zwei Lüfterrelais, RGB-LED. Pinbelegung: [src/board.h](src/board.h).

## Aufbau
| Ordner | Inhalt |
|---|---|
| `src/` | Firmware: `main.cpp` zeigt den Ablauf (CAN-Task, VCU-DEC-015); `app/` Kombi, `comms/` CAN-Codecs, `drivers/` Hardware, `services/` Webserver, Signaltabelle, GVRET, Log |
| `pwa/` | Touch-UI als PWA, Build und Tests ([pwa/README.md](pwa/README.md)) |
| `documentation/` | CAN-Signale, DBC-Dateien, Architekturbild, Protokoll |
| `tools/` | `canivore_py_test_tool/` (Submodul: CAN-Tests über den CANable-Stick, Start mit `tools\can_test.cmd`), `gvret_listen.py` (mitlesen, was die VCU je Bus empfängt), `savvy_csv_to_asc.py` (SavvyCAN-Log → ASC für die Wiedergabe), `generate_pq_messages_md.py` |
| `documentation/can_logs/` | Mitschnitte: Stand am 986 (Fahrzeugbus), Fahrt mit Tesla SDU (SavvyCAN-CSV) |

## Build und Flashen
```bash
copy src\secrets.example.h src\secrets.h   # einmalig, Zugangsdaten eintragen
python pwa/tools/build.py                   # PWA nach pwa/dist
platformio run -e esp32s3                              # Firmware
platformio run --target upload --upload-port COM10     # Firmware flashen
platformio run --target uploadfs --upload-port COM10   # PWA ins SPIFFS
```
`platformio` liegt unter `%USERPROFILE%\.platformio\penv\Scripts\platformio.exe` (nicht im PATH), z. B. `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run --target upload --upload-port COM10`. COM10 ist UART0 des Boards (CH343).

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
Pin Lüfter Stufe 1, Lüfterschwellen, Umrechnung Verbrauch, BMS-/Ladegerät-Botschaften auf CAN0, Zeitgrenze für veraltete Werte, Browser des Navi-Displays. Details in REQUIREMENTS.md und DECISIONS.md.
