# esp32VCU – Fahrzeugsteuergerät für den elektrifizierten Porsche 986

Die VCU ersetzt das Motorsteuergerät auf dem Fahrzeug-CAN, liest den Antriebsumrichter (Tesla SDU mit OpenInverter), passt das Kombiinstrument an den E-Antrieb an, steuert Kühlmittellüfter und Tankanzeige und liefert die Touch-Oberfläche für das Navi-Display (1024×600).

- Anforderungen: [REQUIREMENTS.md](REQUIREMENTS.md) · Entscheidungen: [DECISIONS.md](DECISIONS.md) · Tests: [TESTPLAN.md](TESTPLAN.md)
- Architektur: [documentation/architecture.html](documentation/architecture.html) (interaktiv, Details im JSON-Block der Datei) · Übersicht [architecture.svg](documentation/architecture.svg) · CAN-Signale: [documentation/can_signals.md](documentation/can_signals.md)
- Änderungsprotokoll: [documentation/IMPLEMENTATION_LOG.md](documentation/IMPLEMENTATION_LOG.md)

## Hardware
ESP32-S3-DevKitC-1 (8 MB), 3× MCP2515 an einem SPI-Bus (CAN1 Fahrzeug, CAN2 Umrichter, CAN3 Reserve), TWAI als CAN0 (BMS/Lader, TX GPIO20, RX GPIO19), X9C103S (Tankgeber, zurzeit ohne Pins), AT24C256-EEPROM (I2C, SCL GPIO39, SDA GPIO37, Adresse 0x50), zwei Lüfterrelais, RGB-LED. Pinbelegung: [src/board.h](src/board.h).

Terminierung: CAN0 hat **keinen** 120-Ω-Abschlusswiderstand auf der Platine; CAN1 bis CAN3 sind je per Jumper zuschaltbar.

## Aufbau
| Ordner | Inhalt |
|---|---|
| `src/` | Firmware: `main.cpp` zeigt den Ablauf (Empfang aller Busse in `loop()`, ECU-Task alle 6 ms, VCU-DEC-015); `app/` Kombi, `comms/` CAN-Codecs, `drivers/` Hardware, `services/` Webserver, Signaltabelle, GVRET, Log, serielle Befehle |
| `pwa/` | Touch-UI als PWA, Build und Tests ([pwa/README.md](pwa/README.md)) |
| `documentation/` | CAN-Signale, DBC-Dateien, Architekturbild, Protokoll |
| `tools/` | `canivore_py_test_tool/` (Submodul: CAN-Tests über den CANable-Stick, Start mit `tools\can_test.cmd`), `vcu_log.py` (Textlog/[DIAG] von COM10), `vcu_cmd.py` (serielle Befehle, Reset über RTS), `gvret_listen.py` (GVRET je Bus mitlesen, Sequenz/CRC), `savvy_csv_to_asc.py` (SavvyCAN-Log → ASC für die Wiedergabe), `pio_build_info.py` (aktuelles Build-Datum), `generate_pq_messages_md.py` |
| `documentation/can_logs/` | Mitschnitte: Stand am 986 (Fahrzeugbus), Fahrt mit Tesla SDU (SavvyCAN-CSV) |

## Build und Flashen
```bash
copy src\secrets.example.h src\secrets.h   # einmalig, Zugangsdaten eintragen
python pwa/tools/build.py                   # PWA nach pwa/dist
platformio run -e esp32s3                              # Firmware
platformio run --target upload --upload-port COM10     # Firmware flashen
platformio run --target uploadfs --upload-port COM10   # PWA ins SPIFFS
```
`platformio` liegt unter `%USERPROFILE%\.platformio\penv\Scripts\platformio.exe` (nicht im PATH), z. B. `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run --target upload --upload-port COM10`. COM10 ist UART0 des Boards (CH343). Der Firmware-Upload schreibt das SPIFFS nicht: Nach Änderungen an `pwa/src` sind `build.py` und `uploadfs` nötig, sonst liefert das Board die alte Oberfläche aus. Firmware-Version: `APP_VERSION` in `platformio.ini`.

PlatformIO aus PowerShell oder cmd starten, nicht aus Git Bash: Das Werkzeug-Setup der Plattform (`idf_tools.py`) bricht unter MSYS mit „MSys/Mingw is not supported“ ab, danach fehlt esptool.

**Bekanntes Problem (Windows, pioarduino 55.03.37):** Der Compiler `toolchain-xtensa-esp-elf` landet mit einer Ordnerebene zu viel (`xtensa-esp-elf/bin`). Abhilfe am 2026-10-28: Inhalt von `%USERPROFILE%\.platformio\tools\toolchain-xtensa-esp-elf\xtensa-esp-elf\` flach nach `...\packages\toolchain-xtensa-esp-elf\` kopiert, `package.json` und `.piopm` dazu.

## Schnittstellen
- `GET /` – Touch-UI (PWA)
- `GET /api/live[?fields=a,b]` – alle Werte; `stale` listet veraltete Felder
- `GET /api/signals` – Signalliste mit Typ, Quelle, Schreibrecht
- `GET /can` – schlanke Testseite: Status, Zähler und Einstellungen aller CAN-Busse und der ECU-Task (`params::can`, Felder `can_*`, `can0_…can3_*`, `ecu_*` in `/api/live`)
- `GET /api/set?…` – Ladeparameter, Lüfter (`coolant_fan_level`, `coolant_fan_mode=auto|manual`), Poti, `rpm`/`coolant`/`oil` (Overrides)
- `GET /api/set-ecu?…` – schreibbare Motorsteuergerät-Signale; `auto` hebt einen Override auf
- `GET /api/set-cluster?activated=0|1`
- WLAN: Zugangspunkt `eboxster` (http://192.168.4.1/) und Heimnetz aus `secrets.h` (Hostname `eboxster-vcu`, IP mit `vcu_cmd.py wifi-status`)
- UART0 (COM10, 115200): Textlog (`[VERSION]`, `[WIFI]`, `[DIAG]` jede Sekunde) und Befehle `help`, `version`, `wifi-status`, `can0-status`, `can0-send`, `can0-pintest`, `eeprom-test`, `reset` (`tools/vcu_cmd.py`). Sobald SavvyCAN GVRET auf binär schaltet (0xE7), nur noch GVRET bis zum nächsten Reset. Der native USB-Port entfällt (GPIO19/20 = CAN0).

## Offene Punkte
Pins Tankanzeige (X9C103S INC/UD, GPIO5/6 jetzt CAN3), Pin Lüfter Stufe 1, Rolle von CAN3, Lüfterschwellen, Umrechnung Verbrauch, BMS-/Ladegerät-Botschaften auf CAN0, Zeitgrenze für veraltete Werte, Browser des Navi-Displays. Details in REQUIREMENTS.md und DECISIONS.md.
