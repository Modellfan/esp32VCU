# Implementation Log
This file is the permanent record for development work in this project.

## Required For Every Future Task
- Write planned implementation steps before making changes.
- Document all file/code changes after implementation.
- Document every automated test/command run and whether it passed or failed.
- Include a `Result` section for outcome metrics/values.
- Include an `Artifacts` section with markdown links to produced files.
- Put blockers/assumptions only in `Notes` (do not place run metrics/artifact lists there).
- If a test is not run, explicitly state why.
- Every entry title MUST include date, time, and timezone.

## Entry Template
`YYYY-MM-DD HH:MM:SS +/-TZ - Task Title`

### Planned Steps
- Step 1
- Step 2
- Step 3

### Changes Made
- `path/to/file`: short description

### Automated Tests Run
- `command`: PASS/FAIL
- `command`: PASS/FAIL

### Result
- `result_key: value`

### Artifacts
- `[artifact-name](path/to/artifact)`

### Notes
- Optional blockers, follow-ups, or assumptions.

---

## 2026-02-23 21:42:38 +01:00 - Add Tesla SDU codec module and web dashboard page

### Planned Steps
- Create `src/tesla_sdu_messages.h` and `src/tesla_sdu_messages.cpp` with Tesla SDU signal struct in `namespace params` and per-message `decode0x...` / `encode0x...` functions.
- Integrate Tesla SDU decode calls in `src/main.cpp` so live values are updated from incoming CAN frames.
- Extend `src/webinterface.cpp` `/api/live` payload with Tesla SDU signals.
- Add a third page in `data/index.html` and `data_spiffs/index.html` to display all Tesla SDU signals.
- Run a PlatformIO build validation.

### Changes Made
- `src/tesla_sdu_messages.h`: Added Tesla SDU signal container `params::tesla_sdu` and per-message function declarations for IDs `0x320`-`0x324`.
- `src/tesla_sdu_messages.cpp`: Implemented decode/encode functions for:
  - `0x320` Motor_Status
  - `0x321` Voltage_Current
  - `0x322` Speed_Input
  - `0x323` Steering
  - `0x324` Temperatur_LowVoltage
- `src/main.cpp`: Included Tesla SDU codec header and integrated decode calls in `interpreteCANframe()` for IDs `0x320`-`0x324`.
- `src/webinterface.cpp`: Added Tesla SDU values to `/api/live` JSON payload (`sdu_*` keys) and included Tesla SDU header.
- `data/index.html`: Added third page (`Tesla SDU`) with live tiles for all Tesla SDU signals; upgraded pager/nav to 3-page navigation and bound all SDU fields in `refreshNow()`.
- `data/index.html`: Hardened `setText()` to avoid runtime errors if an element id is absent.
- `data_spiffs/index.html`: Mirrored updated UI from `data/index.html`.

### Automated Tests Run
- `powershell -ExecutionPolicy Bypass -File tools\stage_archive.ps1 -Action snapshot -Label "before_tesla_sdu_codec_web_page" -Paths src\main.cpp src\webinterface.cpp src\webinterface.h data\index.html data_spiffs\index.html src\986_ecu_messages.cpp src\986_vehicle_messages.cpp`: FAIL (PowerShell argument parsing for `-Paths`)
- `powershell -ExecutionPolicy Bypass -File tools\stage_archive.ps1 -Action snapshot -Label "before_tesla_sdu_codec_web_page" -Paths @("src/main.cpp","src/webinterface.cpp","src/webinterface.h","data/index.html","data_spiffs/index.html","src/986_ecu_messages.cpp","src/986_vehicle_messages.cpp")`: FAIL (PowerShell argument parsing for `-Paths`)
- `powershell -ExecutionPolicy Bypass -File tools\stage_archive.ps1 -Action snapshot -Label before_tesla_sdu_codec_web_page -Paths "src/main.cpp","src/webinterface.cpp","src/webinterface.h","data/index.html","data_spiffs/index.html","src/986_ecu_messages.cpp","src/986_vehicle_messages.cpp"`: FAIL (treated as one combined path string)
- `powershell -ExecutionPolicy Bypass -File tools\stage_archive.ps1 --% -Action snapshot -Label before_tesla_sdu_codec_web_page -Paths src/main.cpp src/webinterface.cpp src/webinterface.h data/index.html data_spiffs/index.html src/986_ecu_messages.cpp src/986_vehicle_messages.cpp`: FAIL (PowerShell argument parsing for `-Paths`)
- `powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_tesla_sdu_codec_web_page' -Paths @('src/main.cpp','src/webinterface.cpp','src/webinterface.h','data/index.html','data_spiffs/index.html','src/986_ecu_messages.cpp','src/986_vehicle_messages.cpp') }"`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `tesla_sdu_codec_files_added: 2`
- `tesla_sdu_message_ids_supported: 5 (0x320-0x324)`
- `web_api_tesla_fields_added: 19`
- `web_ui_pages: 3 (Vehicle Data, Einstellungen, Tesla SDU)`
- `build_status: success`
- `stage_snapshot_created: 20260223_214307_before_tesla_sdu_codec_web_page`

### Artifacts
- [tesla sdu header](src/tesla_sdu_messages.h)
- [tesla sdu implementation](src/tesla_sdu_messages.cpp)
- [main decode integration](src/main.cpp)
- [web api integration](src/webinterface.cpp)
- [web ui source](data/index.html)
- [spiffs web ui](data_spiffs/index.html)
- [stage snapshot manifest](archives/stages/20260223_214307_before_tesla_sdu_codec_web_page/manifest.json)

### Notes
- Assumption: Tesla SDU DBC fields were implemented as signed where `@1-` indicates signed little-endian; scaling was applied exactly as listed.

---

## 2026-02-23 20:17:21 +01:00 - Split 986 CAN codec into ECU and vehicle modules

### Planned Steps
- Create dedicated ECU codec module with per-message encode/decode functions and `params::ecu`.
- Create dedicated vehicle codec module with per-message encode/decode functions and `params::vehicle`.
- Integrate `main.cpp` to decode via the new modules and keep status output aligned.
- Run a full PlatformIO build for compile verification.

### Changes Made
- `src/986_can_codec.h`: Added shared little-endian bit helpers (`readBitsLE`, `writeBitsLE`, `signExtend`) and clamp helpers.
- `src/986_ecu_messages.h`: Added ECU signal struct declaration in `namespace params` and public function declarations for `decode0x...`/`encode0x...`.
- `src/986_ecu_messages.cpp`: Implemented `decode0x280`, `decode0x289`, `decode0x298`, `decode0x4E0` and matching encode functions; defined `params::ecu`.
- `src/986_vehicle_messages.h`: Added vehicle signal struct declaration in `namespace params` and public function declarations for `decode0x...`/`encode0x...`.
- `src/986_vehicle_messages.cpp`: Implemented `decode0x2A8`, `decode0x1A0`, `decode0x520`, `decode0x00C0` and matching encode functions; defined `params::vehicle`.
- `src/main.cpp`: Replaced in-file decode switch with `decodeECUFrame`/`decodeVehicleFrame`; removed duplicate signal globals and helper bit functions; updated status print to use `params::ecu` and `params::vehicle`; switched manipulation helper calls to shared codec helpers.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `build_status: success`
- `new_modules_added: 4`
- `shared_helper_header_added: 1`
- `decode_dispatch_in_main: modularized`

### Artifacts
- [986 CAN codec helper](src/986_can_codec.h)
- [986 ECU message header](src/986_ecu_messages.h)
- [986 ECU message implementation](src/986_ecu_messages.cpp)
- [986 vehicle message header](src/986_vehicle_messages.h)
- [986 vehicle message implementation](src/986_vehicle_messages.cpp)
- [main integration](src/main.cpp)

### Notes
- ABS ID `0x1A0` bit 48 has variant-dependent interpretation; encode path currently writes the intervention torque representation for deterministic output.

---

## 2026-02-23 20:27:24 +01:00 - Revert unintended main.cpp integration changes

### Planned Steps
- Revert only `src/main.cpp` to remove unintended integration edits.
- Keep new `986_*` codec files as added artifacts.
- Verify git status reflects the rollback.

### Changes Made
- `src/main.cpp`: Restored to pre-integration state per request.

### Automated Tests Run
- `git restore -- src/main.cpp`: PASS
- `git status --short`: PASS

### Result
- `main_cpp_restored: true`
- `new_codec_files_retained: true`

### Artifacts
- [restored-main](src/main.cpp)

### Notes
- Rollback was requested by user to avoid modifying main at this stage.

---

## 2026-02-23 20:27:46 +01:00 - Remove unnecessary decodeECUFrame aggregator

### Planned Steps
- Remove `decodeECUFrame` declaration from `src/986_ecu_messages.h`.
- Remove `decodeECUFrame` definition from `src/986_ecu_messages.cpp`.
- Verify no remaining references.

### Changes Made
- `src/986_ecu_messages.h`: Removed `bool decodeECUFrame(const CANMessage &frame);` declaration.
- `src/986_ecu_messages.cpp`: Removed `decodeECUFrame` aggregator definition.

### Automated Tests Run
- `rg -n "decodeECUFrame" src\986_ecu_messages.h src\986_ecu_messages.cpp src\main.cpp`: PASS (no matches)

### Result
- `decode_ecu_aggregator_removed: true`

### Artifacts
- [ecu-header](src/986_ecu_messages.h)
- [ecu-impl](src/986_ecu_messages.cpp)

### Notes
- No blockers.

---

## 2026-02-23 21:33:40 +01:00 - Upload SPIFFS and flash firmware to COM15

### Planned Steps
- Upload firmware image to ESP32-S3 on `COM15`.
- Upload SPIFFS filesystem image to ESP32-S3 on `COM15`.
- Confirm both operations report success.

### Changes Made
- Uploaded SPIFFS filesystem image to ESP32-S3 on `COM15`.
- Uploaded firmware image to ESP32-S3 on `COM15` (successful sequential retry after initial port contention).

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15` (parallel attempt): FAIL (`COM15` busy due concurrent upload jobs)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15` (sequential retry): PASS

### Result
- `firmware_upload: success`
- `spiffs_upload: success`
- `target_port: COM15`

### Artifacts
- [spiffs web content](data_spiffs/index.html)
- [web backend](src/webinterface.cpp)

### Notes
- No blockers.

---

## 2026-02-23 21:30:33 +01:00 - Expand live vehicle page and add slider-based engine/coolant/oil controls

### Planned Steps
- Extend `/api/live` payload to include all currently tracked vehicle/ECU signals used by the dashboard.
- Add `/api/set` control endpoint to set `engine rpm`, `coolant`, and `oil temp` overrides.
- Redesign page 1 to show all signals as scrollable tiles and enable visible themed scrollbar.
- Add control sliders on page 2 for `engine rpm`, `coolant`, and `oil temp` and wire them to `/api/set`.
- Mirror UI changes to both `data/index.html` and `data_spiffs/index.html`.
- Build and upload firmware + SPIFFS to `COM15`.

### Changes Made
- `src/webinterface.cpp`: Expanded `/api/live` JSON payload to include full signal set (ECU + vehicle + cluster + steering + override state).
- `src/webinterface.cpp`: Added `/api/set` endpoint to apply slider-set override values:
- `rpm` -> `engine_speed_override_rpm` (+activate)
- `coolant` -> `coolant_temperature_override_c` (+activate)
- `oil` -> `oil_temperature_override_c` (+activate)
- `data/index.html`: Page 1 redesigned as a fully scrollable all-signal tile dashboard with themed right-side scrollbar.
- `data/index.html`: Page 2 extended with new control sliders for `Set Engine RPM`, `Set Coolant`, and `Set Oil Temp`.
- `data/index.html`: Added JS wiring for slider control updates to `/api/set` plus expanded live tile updates from `/api/live`.
- `data_spiffs/index.html`: Mirrored updated UI from `data/index.html`.
- `src/webinterface.cpp`: Fixed JSON float formatting ambiguity via explicit cast in `String((double)value, (unsigned int)decimals)`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: FAIL (initial compile; `String(float,uint8_t)` overload ambiguity in `src/webinterface.cpp`)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS (after cast fix)
- Structure checks (`Select-String`) in `data/index.html` and `data_spiffs/index.html` for settings toggles/sliders and initialization hooks: PASS

### Result
- `page1_all_signal_tiles: true`
- `page1_scrollbar_visible: true`
- `page2_control_sliders_added: true`
- `slider_to_firmware_control_path: active (/api/set)`
- `build_status: success`

### Artifacts
- [web backend/api](src/webinterface.cpp)
- [ui template](data/index.html)
- [spiffs ui template](data_spiffs/index.html)

### Notes
- No blockers.

---

## 2026-02-23 21:28:22 +01:00 - Add toggle tiles and slider tiles on settings page

### Planned Steps
- Replace current settings-page card content with styled control tiles.
- Add two-column binary toggle tiles with OEM-like switch visuals.
- Add one-column slider tiles with min/max/current value labels.
- Mirror updated HTML from `data/index.html` to `data_spiffs/index.html`.
- Validate expected settings controls exist in both files.

### Changes Made
- `data/index.html`: Replaced simple settings card with:
- 2-column binary toggle tile grid (`Door Left`, `Door Right`, `Seat Heater L`, `Seat Heater R`)
- 1-column slider tile stack (`Cabin Temp`, `Fan Speed`, `Display Brightness`)
- `data/index.html`: Added style blocks for toggle and slider controls in the existing dark/red theme.
- `data/index.html`: Added JS control bindings (`initSettingsControls`, `bindToggle`, `bindSlider`) for live visual state updates.
- `data_spiffs/index.html`: Mirrored updated settings page markup/CSS/JS from `data/index.html`.

### Automated Tests Run
- Structure verification via `Select-String` in both HTML files for:
- `settings-grid`, `control-tile`, `class="toggle"`, `slider-stack`, `class="slider"`, `initSettingsControls()`: PASS

### Result
- `settings_binary_tiles: added`
- `settings_slider_tiles: added`
- `layout: 2col_toggles_plus_1col_sliders`

### Artifacts
- [ui template](data/index.html)
- [spiffs ui template](data_spiffs/index.html)

### Notes
- No blockers.

---

## 2026-02-23 21:24:35 +01:00 - Upload SPIFFS image to COM15

### Planned Steps
- Upload SPIFFS filesystem image to ESP32-S3 on `COM15`.
- Confirm upload success from PlatformIO output.

### Changes Made
- Uploaded SPIFFS filesystem image to ESP32-S3 on `COM15`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS

### Result
- `spiffs_upload: success`
- `target_port: COM15`

### Artifacts
- [spiffs web content](data_spiffs/index.html)
- [spiffs assets](data_spiffs/assets)

### Notes
- No blockers.

---

## 2026-02-23 21:22:05 +01:00 - Flash firmware to COM15

### Planned Steps
- Upload current firmware build to ESP32-S3 on `COM15`.
- Confirm upload result from PlatformIO output.

### Changes Made
- Flashed current firmware image to ESP32-S3 on `COM15`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS

### Result
- `firmware_flash: success`
- `target_port: COM15`

### Artifacts
- [platformio config](platformio.ini)

### Notes
- No blockers.

---

## 2026-02-23 21:18:00 +01:00 - Remove manual refresh button and keep auto JSON polling

### Planned Steps
- Remove `REFRESH` button from live page in both HTML files.
- Keep existing auto-refresh logic (`setInterval(refreshNow, 500)`).
- Verify no `REFRESH` button remains.

### Changes Made
- `data/index.html`: Removed manual live-page refresh button.
- `data/index.html`: Removed settings-page sync button that triggered `refreshNow()`.
- `data_spiffs/index.html`: Removed manual live-page refresh button.
- `data_spiffs/index.html`: Removed settings-page sync button that triggered `refreshNow()`.

### Automated Tests Run
- `Select-String` check for `onclick="refreshNow()"` in both HTML files: PASS (`NO_MANUAL_REFRESH_BUTTONS`)
- `Select-String` check for `setInterval(refreshNow, 500)` in both HTML files: PASS (`AUTO_REFRESH_PRESENT`)

### Result
- `manual_refresh_controls_removed: true`
- `auto_json_polling_active: true`

### Artifacts
- [ui template](data/index.html)
- [spiffs ui template](data_spiffs/index.html)

### Notes
- No blockers.

---

## 2026-02-23 21:17:16 +01:00 - Remove active nav underline and keep red icon-only selection

### Planned Steps
- Remove `.tab.active::after` underline rule from UI CSS.
- Apply change to both `data/index.html` and `data_spiffs/index.html`.
- Verify CSS no longer contains the underline selector.

### Changes Made
- `data/index.html`: Removed `.tab.active::after` CSS rule (active underline).
- `data_spiffs/index.html`: Removed `.tab.active::after` CSS rule (active underline).

### Automated Tests Run
- `Select-String` check for `.tab.active::after` in both HTML files: PASS (`UNDERLINE_RULE_REMOVED`)

### Result
- `active_tab_style: red_icon_only`

### Artifacts
- [ui template](data/index.html)
- [spiffs ui template](data_spiffs/index.html)

### Notes
- No blockers.

---

## 2026-02-23 21:13:56 +01:00 - Convert live list metrics to tile format

### Planned Steps
- Remove the live metrics list section from HTML UI pages.
- Add those metrics (`Throttle`, `Pedal 1 / 2`, `Wheel Speeds`, `MIL`) as tiles in the main grid.
- Keep existing IDs for JS updates and adjust tile value font sizing where needed.
- Validate by checking updated files for expected structure/IDs.

### Changes Made
- `data/index.html`: Removed list/row live section and converted `Throttle`, `Pedal 1 / 2`, `Wheel Speeds`, `MIL` into tile cards in the main grid.
- `data/index.html`: Added `.value.sm` style for compact multi-value tiles.
- `data_spiffs/index.html`: Removed list/row live section and converted `Throttle`, `Pedal 1 / 2`, `Wheel Speeds`, `MIL` into tile cards in the main grid.
- `data_spiffs/index.html`: Added `.value.sm` style for compact multi-value tiles.

### Automated Tests Run
- `Select-String` structure check on `data/index.html` and `data_spiffs/index.html`: PASS (tile IDs `thr`, `ped`, `wheel`, `mil` present; list/row blocks removed)

### Result
- `live_metrics_layout: tile_only`
- `js_binding_ids_retained: true`

### Artifacts
- [ui template](data/index.html)
- [spiffs ui template](data_spiffs/index.html)

### Notes
- No blockers.

---

## 2026-02-23 21:04:38 +01:00 - Integrate webinterface into main and verify on COM15 serial

### Planned Steps
- Include `webinterface.h` in `src/main.cpp`.
- Call `webinterfaceBegin()` in `setup()` and `webinterfaceHandle()` in `loop()`.
- Build and upload firmware + SPIFFS to `COM15`.
- Verify AP startup message on `COM15` serial.

### Changes Made
- `src/main.cpp`: Added `#include "webinterface.h"`.
- `src/main.cpp`: Added `webinterfaceBegin()` call in `setup()` after CAN init.
- `src/main.cpp`: Added `webinterfaceHandle()` call in `loop()`.
- Uploaded updated firmware and SPIFFS image to `COM15`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS (after one retry due temporary COM15 busy collision)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS
- `COM15 serial probe @115200 with reset pulse`: PASS (ROM boot output present)
- `COM15 serial probe @1000000 raw capture`: FAIL (no readable app log, null stream)

### Result
- `webinterface_integrated_into_main: true`
- `firmware_upload: success`
- `spiffs_upload: success`
- `serial_runtime_confirmation: partial` (board boots on COM15; app-level web log not readable on current UART stream settings)

### Artifacts
- [main integration](src/main.cpp)
- [webinterface module](src/webinterface.cpp)
- [spiffs content root](data_spiffs/index.html)

### Notes
- This addresses missing integration that prevented AP startup.

---

## 2026-02-23 21:12:51 +01:00 - Set firmware UART baud to 115200

### Planned Steps
- Change `Serial.begin(...)` in `src/main.cpp` from `1000000` to `115200`.
- Run a build check to confirm compile success.

### Changes Made
- `src/main.cpp`: Changed `Serial.begin(1000000)` to `Serial.begin(115200)`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `serial_baud: 115200`
- `build_status: success`

### Artifacts
- [main baud update](src/main.cpp)

### Notes
- No blockers.

---

## 2026-02-23 20:57:11 +01:00 - Configure web AP as open network (no password)

### Planned Steps
- Remove AP password usage from `src/webinterface.cpp`.
- Start AP with SSID-only call (`WiFi.softAP(ssid)`).
- Run build validation.

### Changes Made
- `src/webinterface.cpp`: Removed AP password constant and switched AP start to `WiFi.softAP(kApSsid)` for open network.
- `src/webinterface.cpp`: Extended startup log to include AP start status (`OPEN=true/false`).

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `ap_mode: open`
- `ap_ssid: eboxster`
- `build_status: success`

### Artifacts
- [webinterface open AP change](src/webinterface.cpp)

### Notes
- User requested direct computer-connectable AP without password.

---

## 2026-02-23 20:58:43 +01:00 - Upload firmware+SPIFFS and verify runtime over serial

### Planned Steps
- Upload firmware to ESP32-S3 on `COM15`.
- Upload SPIFFS image (HTML/assets) to `COM15`.
- Read serial monitor output to confirm firmware is running.

### Changes Made
- Uploaded current firmware image to ESP32-S3 on `COM15`.
- Uploaded SPIFFS image (`data_spiffs`) to ESP32-S3 on `COM15`.
- Performed serial runtime probes on detected COM ports and active monitor command probe (`?`).

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS
- `[System.IO.Ports.SerialPort]::GetPortNames()`: PASS (detected `COM15`, `COM23`, `COM9`)
- `$sp = New-Object System.IO.Ports.SerialPort COM23,115200...; $sp.WriteLine('?') ...`: FAIL (`NO_RESPONSE`)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" device list`: PASS (confirmed USB-UART bridge ports only)

### Result
- `firmware_upload: success`
- `spiffs_upload: success`
- `serial_runtime_confirmation: inconclusive`

### Artifacts
- [platformio config](platformio.ini)
- [webinterface implementation](src/webinterface.cpp)
- [spiffs html](data_spiffs/index.html)

### Notes
- No USB CDC monitor port is currently enumerated on this hardware setup; monitor command probe on available UART monitor candidate (`COM23`) returned no response.

---

## 2026-02-23 21:01:30 +01:00 - Remove remaining anonymous namespace from webinterface.cpp

### Planned Steps
- Remove anonymous namespace block from `src/webinterface.cpp`.
- Convert internal symbols to file-local `static` declarations.
- Run build validation.

### Changes Made
- `src/webinterface.cpp`: Removed anonymous namespace wrapper.
- `src/webinterface.cpp`: Converted internal module symbols to file-local `static` (`kApSsid`, `server`, `getLiveJson`, `handleRoot`, `handleLive`, `handleNotFound`).

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `extra_namespace_removed: true`
- `build_status: success`

### Artifacts
- [webinterface implementation](src/webinterface.cpp)

### Notes
- No blockers.

---

## 2026-02-23 20:56:14 +01:00 - Change AP SSID to eboxster

### Planned Steps
- Update AP SSID constant in `src/webinterface.cpp` to `eboxster`.
- Update UI default SSID placeholders in SPIFFS HTML files.
- Run a build check.

### Changes Made
- `src/webinterface.cpp`: Changed AP SSID constant from `ESP32VCU-UI` to `eboxster`.
- `data/index.html`: Updated default AP SSID field value to `eboxster`.
- `data_spiffs/index.html`: Updated default AP SSID field value to `eboxster`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `ap_ssid: eboxster`
- `build_status: success`

### Artifacts
- [webinterface AP config](src/webinterface.cpp)
- [ui template](data/index.html)
- [spiffs ui template](data_spiffs/index.html)

### Notes
- No blockers.

---

## 2026-02-23 20:55:15 +01:00 - Remove namespace from webinterface module

### Planned Steps
- Remove `webui986` namespace from `src/webinterface.h` and `src/webinterface.cpp`.
- Rename exported API symbols to unambiguous global names.
- Run a build check to ensure compile safety.

### Changes Made
- `src/webinterface.h`: Removed `webui986` namespace and exported global API declarations:
- `webinterfaceBegin()`
- `webinterfaceHandle()`
- `webinterfaceApSsid()`
- `webinterfaceApIp()`
- `src/webinterface.cpp`: Removed `webui986` namespace and renamed implementations to the global API names above.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `webinterface_namespace_removed: true`
- `build_status: success`

### Artifacts
- [webinterface header](src/webinterface.h)
- [webinterface implementation](src/webinterface.cpp)

### Notes
- No blockers.

---

## 2026-02-23 20:53:30 +01:00 - Rename web module files to generic names

### Planned Steps
- Rename `src/986_web_interface.h` to `src/webinterface.h`.
- Rename `src/986_web_interface.cpp` to `src/webinterface.cpp`.
- Update include guard and include statements to match renamed files.
- Run a build check.

### Changes Made
- `src/986_web_interface.h`: Renamed to `src/webinterface.h`.
- `src/986_web_interface.cpp`: Renamed to `src/webinterface.cpp`.
- `src/webinterface.h`: Updated include guard to `WEBINTERFACE_H`.
- `src/webinterface.cpp`: Updated include from `"986_web_interface.h"` to `"webinterface.h"`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `web_module_renamed_generic: true`
- `build_status: success`

### Artifacts
- [generic web header](src/webinterface.h)
- [generic web implementation](src/webinterface.cpp)

### Notes
- No blockers.

---

## 2026-02-23 20:52:47 +01:00 - Ignore potentially copyrighted asset directories

### Planned Steps
- Add `.gitignore` rules to exclude icon asset directories from version control.
- Keep directory structure available via `.gitkeep`.
- Validate with `git status --short`.

### Changes Made
- `.gitignore`: Added ignore rules for `data/assets/*` and `data_spiffs/assets/*`, with `.gitkeep` exceptions.
- `data/assets/.gitkeep`: Added placeholder to preserve directory while ignoring asset files.
- `data_spiffs/assets/.gitkeep`: Added placeholder to preserve directory while ignoring asset files.

### Automated Tests Run
- `git check-ignore -v data/assets/car.svg data_spiffs/assets/car.svg`: PASS

### Result
- `asset_directories_ignored: true`

### Artifacts
- [gitignore update](.gitignore)
- [data assets placeholder](data/assets/.gitkeep)
- [spiffs assets placeholder](data_spiffs/assets/.gitkeep)

### Notes
- No blockers.

---

## 2026-02-23 20:35:32 +01:00 - Add standalone WiFi AP web interface module for live CAN data

### Planned Steps
- Create a new web interface header/cpp pair under `src/` without modifying `main.cpp`.
- Start ESP32 in AP mode and host HTTP endpoints for UI and live JSON data.
- Build UI using provided design template and wire periodic `/api/live` polling.
- Run a compile check to verify the new module builds.

### Changes Made
- `src/986_web_interface.h`: Added standalone interface API (`webui986::begin`, `webui986::handle`, AP getters).
- `src/986_web_interface.cpp`: Implemented WiFi AP startup, HTTP server, template-based UI page, `/api/live` JSON endpoint with live signal values, and static `/assets/*` serving from LittleFS.
- `platformio.ini`: Set `board_build.filesystem = littlefs` for reliable asset file support.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15` (before filesystem config change): FAIL (SPIFFS file-add error with `data/assets` set)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15` (after web module LittleFS update): PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15` (after `board_build.filesystem = littlefs`): PASS

### Result
- `web_module_added: true`
- `build_status: success`
- `firmware_uploaded_to_com15: true`
- `littlefs_assets_uploaded_to_com15: true`

### Artifacts
- [web interface header](src/986_web_interface.h)
- [web interface implementation](src/986_web_interface.cpp)
- [filesystem config](platformio.ini)

### Notes
- Main integration hooks are exposed but not yet called from `main.cpp` (kept unchanged by request pattern).

---

## 2026-02-23 20:47:32 +01:00 - Switch web UI storage to SPIFFS and external HTML file

### Planned Steps
- Change filesystem target from LittleFS to SPIFFS in build config.
- Move embedded HTML from C++ into `data/index.html`.
- Update web server code to serve `/index.html` and `/assets/*` from SPIFFS.
- Upload firmware and SPIFFS image to `COM15`.

### Changes Made
- `platformio.ini`: Switched filesystem to SPIFFS (`board_build.filesystem = spiffs`).
- `platformio.ini`: Added `[platformio] data_dir = data_spiffs` to control SPIFFS image contents.
- `src/986_web_interface.cpp`: Replaced LittleFS usage with SPIFFS (`SPIFFS.begin`, SPIFFS-backed `/index.html` and `/assets/*` serving).
- `data/index.html`: Added standalone web UI HTML file (externalized from C++ string).
- `data_spiffs/index.html`: Added SPIFFS-served UI page.
- `data_spiffs/assets/car.svg`: Added SPIFFS asset.
- `data_spiffs/assets/map.svg`: Added SPIFFS asset.
- `data_spiffs/assets/compass.svg`: Added SPIFFS asset.
- `data_spiffs/assets/user.svg`: Added SPIFFS asset.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15` (with raw `data/assets`): FAIL (`SPIFFS_write error(-10010)` due incompatible asset set/path constraints)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15` (after `data_spiffs` data_dir fix): PASS

### Result
- `filesystem_backend: spiffs`
- `html_served_from_spiffs: true`
- `assets_served_from_spiffs: true`
- `spiffs_upload_to_com15: success`

### Artifacts
- [web interface implementation](src/986_web_interface.cpp)
- [external HTML template](data/index.html)
- [SPIFFS HTML](data_spiffs/index.html)
- [SPIFFS assets folder](data_spiffs/assets)
- [platformio config](platformio.ini)

### Notes
- Requested explicitly to load filesystem to SPIFFS and host HTML there.

---

## 2026-02-23 20:30:31 +01:00 - Remove unnecessary decodeVehicleFrame aggregator

### Planned Steps
- Remove `decodeVehicleFrame` declaration from `src/986_vehicle_messages.h`.
- Remove `decodeVehicleFrame` definition from `src/986_vehicle_messages.cpp`.
- Verify no remaining references.

### Changes Made
- `src/986_vehicle_messages.h`: Removed `bool decodeVehicleFrame(const CANMessage &frame);` declaration.
- `src/986_vehicle_messages.cpp`: Removed `decodeVehicleFrame` aggregator definition.

### Automated Tests Run
- `rg -n "decodeVehicleFrame" src\986_vehicle_messages.h src\986_vehicle_messages.cpp src\main.cpp`: PASS (no matches)

### Result
- `decode_vehicle_aggregator_removed: true`

### Artifacts
- [vehicle-header](src/986_vehicle_messages.h)
- [vehicle-impl](src/986_vehicle_messages.cpp)

### Notes
- No blockers.

---

## 2026-02-23 20:28:31 +01:00 - Move clamp helpers to generic can_utils

### Planned Steps
- Create a generic `src/can_utils.h` for clamp helpers.
- Remove clamp helpers from `src/986_can_codec.h`.
- Update ECU/vehicle codec files to use generic clamp helpers.
- Run a build to verify.

### Changes Made
- `src/can_utils.h`: Added generic clamp helpers `canutils::clampU8` and `canutils::clampU16`.
- `src/986_can_codec.h`: Removed clamp helper implementations from `can986` namespace.
- `src/986_ecu_messages.cpp`: Included `can_utils.h` and switched clamp calls from `can986::clamp*` to `canutils::clamp*`.
- `src/986_vehicle_messages.cpp`: Included `can_utils.h` and switched clamp calls from `can986::clamp*` to `canutils::clamp*`.

### Automated Tests Run
- `if (rg -n "can986::clampU8|can986::clampU16" src\986_can_codec.h src\986_ecu_messages.cpp src\986_vehicle_messages.cpp src\can_utils.h) { exit 1 } else { Write-Host "NO_OLD_CLAMP_REFS" }`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `clamp_helpers_moved_to_generic_utils: true`
- `legacy_clamp_references_removed: true`
- `build_status: success`

### Artifacts
- [generic CAN utils](src/can_utils.h)
- [986 CAN codec header](src/986_can_codec.h)
- [986 ECU messages](src/986_ecu_messages.cpp)
- [986 vehicle messages](src/986_vehicle_messages.cpp)

### Notes
- No blockers.
