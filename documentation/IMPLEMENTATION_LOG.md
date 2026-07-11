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

## 2026-02-25 21:16:17 +01:00 - Remove duplicated main signal globals and use params directly

### Planned Steps
- Remove duplicated non-override signal globals from `src/main.cpp`.
- Remove sync bridge function and use `params::ecu` / `params::vehicle` directly in CAN path.
- Update `src/webinterface.cpp` to read live values directly from `params` instead of `extern` globals.
- Build to validate.

### Changes Made
- `src/main.cpp`: Removed duplicated non-override signal globals (`engine_speed_rpm`, `wheel_speed_*`, `abs_*`, etc.) and removed `syncGlobalsFromParams()`.
- `src/main.cpp`: Updated emulation override handling to modify `params::ecu` directly (`engine_speed_rpm`, `coolant_temperature`, `oil_temperature`) only when override flags are enabled.
- `src/webinterface.cpp`: Removed large `extern` block for signal mirrors from `main.cpp`.
- `src/webinterface.cpp`: Switched live JSON source values to `params::ecu` / `params::vehicle` directly for all 986 signal exports.
- `src/webinterface.cpp`: Removed legacy global back-sync section in `/api/set-ecu` since UI now operates directly on `params`.

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.

### Result
- `duplicated_main_signal_globals_removed: true`
- `webinterface_reads_params_directly: true`
- `legacy_sync_bridge_removed: true`
- `build_status: success`

### Artifacts
- [main](src/main.cpp)
- [webinterface](src/webinterface.cpp)

### Notes
- Keep override globals in `main.cpp` for compatibility with `/api/set`.

---

## 2026-02-25 21:11:01 +01:00 - Implement full ECU controls on page 2 with live send path

### Planned Steps
- Extend backend live JSON with all requested ECU fields and add `/api/set-ecu` handler to update ECU params from UI controls.
- Update `main.cpp` emulation to transmit `0x288` so `motor2_*` controls are actually emitted on CAN.
- Replace page 2 UI with controls for the full requested ECU field list and wire auto-apply calls to `/api/set-ecu`.
- Build and flash firmware + upload SPIFFS.

### Changes Made
- `src/webinterface.cpp`: Added ECU live JSON exports for all requested page-2 fields and added `/api/set-ecu` endpoint to write all requested ECU parameters.
- `src/webinterface.cpp`: Added parsing/sync logic so UI updates apply to `params::ecu` and keep legacy globals aligned for existing consumers.
- `src/main.cpp`: Added `0x288` transmit path to `emulateECUCAN()` at `30 ms`, so `motor2_*` controls are emitted on CAN.
- `src/main.cpp`: Set `engine_speed_override_active` default to `false` so ECU page controls are effective by default.
- `data_spiffs/index.html`: Replaced page-2 placeholder controls with full ECU control grid covering all requested fields; wired auto-apply updates to `/api/set-ecu`.
- `data_spiffs/index.html`: Added live backfill from `/api/live` into ECU controls while preserving active input editing.
- `data/index.html`: Synced with updated SPIFFS UI.

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS.
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: FAIL (first attempt; COM15 busy / access denied).
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS (retry).

### Result
- `ecu_page2_full_control_list_implemented: true`
- `ecu_controls_live_apply_to_params: true`
- `motor2_controls_transmitted_via_0x288: true`
- `firmware_flashed_com15: true`
- `spiffs_uploaded_com15: true`

### Artifacts
- [web API + ECU setter](src/webinterface.cpp)
- [main CAN emulation](src/main.cpp)
- [SPIFFS UI](data_spiffs/index.html)
- [dev UI mirror](data/index.html)

### Notes
- No blockers.

---

## 2026-02-25 21:08:05 +01:00 - Apply Tesla SDU signal UI names and units

### Planned Steps
- Update Tesla SDU tile labels on page 3 in `data_spiffs/index.html` to match the provided UI naming list.
- Update Tesla SDU value formatting in JS refresh logic to append the requested units.
- Mirror the updated UI file to `data/index.html` and run a build validation.

### Changes Made
- `data_spiffs/index.html`: Updated Tesla SDU tile labels to the requested UI names:
- `Operating Mode`, `Last Error`, `Status`, `Digital Input OCUR`, `Digital Input OCUR51`, `Digital Input BMS`, `DC Bus Voltage (UDC)`, `DC Bus Current (IDC)`, `Motor Speed`, `Cruise Speed Setpoint`, `Pedal Input 1 (POT)`, `Pedal Input 2 (POT2)`, `Regen Available 1`, `Regen Available 2`, `Selected Direction`, `Selected Direction Aux`, `Heatsink Temperature 1`, `Heatsink Temperature 2`, `Auxiliary Voltage`.
- `data_spiffs/index.html`: Updated Tesla SDU value formatting to append units in live refresh:
- `V`, `A`, `rpm`, `counts`, `%`, `degC`, and `V` where applicable.
- `data/index.html`: Synced with `data_spiffs/index.html`.

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.

### Result
- `tesla_ui_names_applied: true`
- `tesla_ui_units_applied: true`
- `spiffs_and_data_html_synced: true`
- `build_status: success`

### Artifacts
- [SPIFFS UI](data_spiffs/index.html)
- [dev UI mirror](data/index.html)

### Notes
- No blockers.

---

## 2026-02-25 21:06:26 +01:00 - Refactor main.cpp to requested CAN polling/emulation structure

### Planned Steps
- Rewrite `src/main.cpp` to the requested structure with `pollVehicleCAN`, `pollTeslaCAN`, `emulateECUCAN`, `blinkLED`, `setup`, and `loop`.
- Keep compatibility with existing web interface extern globals and add decode/encode wiring to 986 + Tesla codecs.
- Implement ECU emulation scheduling using configured time slices and push frames to USB trace.
- Build and validate.

### Changes Made
- `src/main.cpp`: Rewritten to the requested structure:
- Added includes/defines layout matching your template (`GVRET_PORT`, no USB CDC usage).
- Added functions `pollVehicleCAN()`, `pollTeslaCAN()`, `emulateECUCAN()`, and kept `pollCAN()` as orchestrator.
- `pollVehicleCAN()`: receives from `can`, forwards to USB bus `1`, decodes via `decode986Ecu()` / `decode986Vehicle()`.
- `pollTeslaCAN()`: receives from `can2`, forwards to USB bus `2`, decodes via `decodeTeslaSdu()`.
- `emulateECUCAN()`: periodically encodes/sends 986 ECU frames on `can` with 30 ms slices for `0x280`, `0x289`, `0x298`, `0x4E0`, `0x7A0`, and forwards sent frames to USB bus `0`.
- Added `syncGlobalsFromParams()` to keep existing web interface extern globals populated from modular `params::ecu` / `params::vehicle`.
- Preserved required web override globals (`engine_speed_override_active`, `engine_speed_override_rpm`, `engine_temp_override_active`, `coolant_temperature_override_c`, `engine_stat_override_active`, `oil_temperature_override_c`).
- Updated `setup()`/`loop()` to match requested flow: init serial, LED, CAN manager, web interface; then run `webinterfaceHandle()`, scheduler, `pollCAN()`, `gvret_loop()`.

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.

### Result
- `main_structure_refactored_to_requested_flow: true`
- `vehicle_tesla_poll_split: true`
- `ecu_emulation_timeslice_30ms_applied: true`
- `build_status: success`

### Artifacts
- [main](src/main.cpp)

### Notes
- No blockers.

---

## 2026-02-25 21:01:47 +01:00 - Implement full vehicle signal tile UI and flash

### Planned Steps
- Extend `/api/live` in `src/webinterface.cpp` to include every listed vehicle signal key/value.
- Replace first-page hardcoded live tiles in `data_spiffs/index.html` with a generated tile grid for all listed vehicle signals (label + unit format).
- Mirror the same UI changes to `data/index.html`.
- Build, upload firmware, upload SPIFFS, and record results.

### Changes Made
- `src/webinterface.cpp`: Added `#include "986_vehicle_messages.h"` and extended `/api/live` JSON with all requested vehicle-signal keys: `fdr_lamp`, `fdr_lamp2`, `rough_road_suppression`, `rough_road_suppression_status`, `psm_button_lamp`, `yaw_rate_sign`, `yaw_rate`, `no_actively_built_up_brake_pressure`, `acc_related_error`, `receipt_for_acc_message`, `driver_braking`, `regulator_error_fdr`, `acceleration`, `error_driver_brake_detection`, `multiplex_byte4`, `brake_normal_or_offset`, `vehicle_speed_510`, `indicator_left`, `indicator_right`, `tempomat_button`, `steeringwheel_button_up`, `steeringwheel_button_down`, `prnd_pos`.
- `data_spiffs/index.html`: Replaced static first-page tiles with generated tile grid (`vehicleSignalGrid`) driven by a `vehicleSignals` list matching your provided signal/UI-name/unit mapping.
- `data_spiffs/index.html`: Reworked live refresh path to render values for all listed vehicle signals via `vehicleSignals.forEach(...)`.
- `data/index.html`: Synced with `data_spiffs/index.html` for local parity.

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS.
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS.

### Result
- `vehicle_signal_tiles_implemented: true`
- `vehicle_signals_in_ui_count: 51`
- `firmware_flashed_com15: true`
- `spiffs_uploaded_com15: true`

### Artifacts
- [web API + JSON](src/webinterface.cpp)
- [SPIFFS UI](data_spiffs/index.html)
- [dev UI mirror](data/index.html)

### Notes
- No blockers.

---

## 2026-02-25 21:00:31 +01:00 - Set 0x7A0 standard default payload

### Planned Steps
- Set `params::ecu.unknown_7a0` default bytes to `85 0C 00 03 00 00 FF 02` in `src/986_ecu_messages.h`.
- Add matching standard-payload note for `0x7A0` in `documentation/can_signals.md` and `documentation/porsche_986_996_compiled_working.dbc`.
- Build to validate.

### Changes Made
- `src/986_ecu_messages.h`: Set `unknown_7a0` default payload to `{0x85, 0x0C, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x02}`.
- `documentation/can_signals.md`: Added `0x7A0` standard payload note (`85 0C 00 03 00 00 FF 02`).
- `documentation/porsche_986_996_compiled_working.dbc`: Added standard payload to `CM_ BO_ 1952` comment.

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.

### Result
- `unknown_7a0_default_payload_updated: true`
- `payload_hex: 85 0C 00 03 00 00 FF 02`
- `build_status: success`

### Artifacts
- [ecu defaults](src/986_ecu_messages.h)
- [can signals reference](documentation/can_signals.md)
- [986 dbc](documentation/porsche_986_996_compiled_working.dbc)

### Notes
- No blockers.

---

## 2026-02-25 21:01:24 +01:00 - Add ECU message time-slice comments (30ms) to DBC, MD, and code

### Planned Steps
- Add `30ms` time-slice notes for `0x280`, `0x289`, `0x298`, `0x4E0`, `0x7A0` in DBC `CM_ BO_` comments.
- Add matching time-slice notes in `documentation/can_signals.md` sections.
- Add matching source comments in `src/986_ecu_messages.cpp` for the same message handlers.
- Run build validation.

### Changes Made
- `documentation/porsche_986_996_compiled_working.dbc`: Added `Time slice: 30ms` to `CM_ BO_` comments for `0x280`, `0x289`, `0x298`, `0x4E0`, `0x7A0`.
- `documentation/can_signals.md`: Added `Time slice: 30 ms` note in the sections for `CAN ID 0x280`, `0x289`, `0x298`, `0x4E0`, and `0x7A0`.
- `src/986_ecu_messages.cpp`: Added source comments marking nominal `30 ms` time slice for message handlers `0x280`, `0x289`, `0x298`, `0x4E0`, and `0x7A0`.

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.

### Result
- `time_slice_comments_added_in_dbc: true`
- `time_slice_comments_added_in_md: true`
- `time_slice_comments_added_in_code: true`
- `build_status: success`

### Artifacts
- [986 dbc](documentation/porsche_986_996_compiled_working.dbc)
- [can signals md](documentation/can_signals.md)
- [ecu codec source](src/986_ecu_messages.cpp)

### Notes
- No blockers.

---

## 2026-02-25 20:55:34 +01:00 - Interpret standard ECU defaults into EcuSignals fields

### Planned Steps
- Map the provided standard frame patterns to physical DBC signal defaults for `0x288`, `0x280`, `0x298`, and `0x4E0`.
- Update `src/986_ecu_messages.h` field initializers so the listed `EcuSignals` values directly reflect those interpreted defaults.
- Set the `0x288` multiplexer byte sequence to start with `0x00`.
- Run a build validation.

### Changes Made
- `src/986_ecu_messages.h`: Interpreted the provided standard frame patterns into explicit DBC-physical defaults on `EcuSignals` fields.
- `src/986_ecu_messages.h`: Updated `0x288` standard byte0 sequence to start at multiplexer `00` (`{0x00, 0x40, 0x80, 0xF3}`).
- `src/986_ecu_messages.h`: Set `motor2_*` defaults from the supplied standard `0x288` bytes (`00 00 14 7D 88 7B 66 61` plus rotating byte0), including `motor2_max_torque_nm = 510.0f`, `motor2_coolant_temperature = -48.0f`, `motor2_cruise_target_speed_kmh = 174.08f`, `motor2_idle_target_speed_rpm = 1230.0f`, `motor2_limited_torque_percent = 39.78f`, `motor2_min_torque_ignition_retard_percent = 37.83f`.
- `src/986_ecu_messages.h`: Set `0x280` defaults to the interpreted standard values from fixed-bit pattern (`throttle_position_280 = 37.5f`, `pedal_position_1 = 1.0f`, `pedal_position_2 = 28.0f`).
- `src/986_ecu_messages.h`: Set `0x298` defaults to interpreted standard values (`inlet_air_temperature = -47.25f`, `throttle_position_1_298 = 37.890625f`, `throttle_position_2_298 = 48.828125f`).
- `src/986_ecu_messages.h`: Set `0x4E0` defaults to interpreted standard behavior (`check_engine_light_bit1 = true`, `oil_temperature = -48.0f`).

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.

### Result
- `standard_defaults_interpreted_into_struct_fields: true`
- `motor2_multiplexer_start_value: 0x00`
- `build_status: success`

### Artifacts
- [ecu signal defaults](src/986_ecu_messages.h)

### Notes
- Assumption: “standard signals” refers to the previously supplied standard byte/bit patterns, including `0x288` rotating byte0 values.

---

## 2026-02-25 20:53:23 +01:00 - Add setup LED blink sequence at half brightness (white then green)

### Planned Steps
- Add a small LED-off helper in `src/utils.h` so setup can blink cleanly.
- Update `setup()` in `src/main.cpp` to run a half-brightness startup blink sequence (white, then green), then restore configured brightness.
- Build the firmware and log the result.

### Changes Made
- `src/utils.h`: Added `fastledOff()` helper to explicitly switch the RGB LED off between setup blink steps.
- `src/main.cpp`: Updated `setup()` with startup LED sequence at half brightness: white on, off, green on; then restores `RGB_BRIGHTNESS`.

### Automated Tests Run
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.

### Result
- `setup_startup_led_sequence_added: true`
- `startup_led_order: white_then_green`
- `startup_led_brightness: RGB_BRIGHTNESS/2`
- `build_status: success`

### Artifacts
- [LED utilities](src/utils.h)
- [setup sequence](src/main.cpp)

### Notes
- No blockers.

---

## 2026-02-25 20:48:32 +01:00 - Remove non-986 signal fields and boost pressure

### Planned Steps
- Remove non-requested signals from `src/986_vehicle_messages.*` (`wheel_speed_*` 16-bit set, `dcdc_*`, `pack_*`, `inverter_temp`, `motor_temp`) and remove `boost_pressure` from `src/986_ecu_messages.*`.
- Remove related decode/encode handling and dispatcher entries for removed messages/signals.
- Update `documentation/porsche_986_996_compiled_working.dbc` and `documentation/can_signals.md` to match the reduced set.
- Run build validation and record results.

### Changes Made
- `src/986_vehicle_messages.h`: Removed non-legacy wheel-speed fields (`wheel_speed_fl`, `wheel_speed_fr`, `wheel_speed_rl`, `wheel_speed_rr`) and removed `dcdc_command_byte0`, `dcdc_target_voltage_raw`, `pack_current`, `pack_voltage`, `inverter_temp`, `motor_temp`.
- `src/986_vehicle_messages.h`: Removed declarations for `decode/encode986Vehicle0x1F4`, `decode/encode986Vehicle0x125`, and `decode/encode986Vehicle0x126`.
- `src/986_vehicle_messages.cpp`: Removed 16-bit wheel-speed decode/encode path for `0x2A8` (kept legacy 15-bit signals only).
- `src/986_vehicle_messages.cpp`: Removed implementations and dispatcher entries for `0x1F4`, `0x125`, `0x126`.
- `src/986_ecu_messages.h`: Removed `boost_pressure` from `EcuSignals`.
- `src/986_ecu_messages.cpp`: Removed `boost_pressure` handling from `decode986Ecu0x4E0` and `encode986Ecu0x4E0`.
- `src/main.cpp`: Removed global `boost_pressure` and `boost_pressure_override_mbar`, removed `0x4E0` boost decode/override write, and removed boost output from status print.
- `src/webinterface.cpp`: Removed boost extern and JSON `boost` field.
- `documentation/porsche_986_996_compiled_working.dbc`: Removed `WheelSpeed_*_16b` signals from `0x2A8`, removed `BoostPressure` from `0x4E0`, removed `BO_ 500/293/294` messages and related comments.
- `documentation/can_signals.md`: Removed `0x1F4`, `0x125`, `0x126` rows/sections, removed 16-bit wheel-speed rows, removed boost-pressure row, and updated coverage summary counts.

### Automated Tests Run
- `rg --line-number "wheel_speed_fl\\b|wheel_speed_fr\\b|wheel_speed_rl\\b|wheel_speed_rr\\b|dcdc_command_byte0|dcdc_target_voltage_raw|pack_current|pack_voltage|inverter_temp\\b|motor_temp\\b|BoostPressure|boost_pressure\\b|0x1F4|0x125|0x126|WheelSpeed_.._16b" src documentation/can_signals.md documentation/porsche_986_996_compiled_working.dbc`: PASS (no matches).
- `& "$env:USERPROFILE\\.platformio\\penv\\Scripts\\platformio.exe" run -e esp32s3`: PASS.

### Result
- `removed_nonlegacy_wheel_speed_fields: true`
- `removed_dcdc_pack_inverter_fields_and_handlers: true`
- `removed_boost_pressure_from_code_and_docs: true`
- `build_status: success`

### Artifacts
- [986 vehicle header](src/986_vehicle_messages.h)
- [986 vehicle implementation](src/986_vehicle_messages.cpp)
- [986 ecu header](src/986_ecu_messages.h)
- [986 ecu implementation](src/986_ecu_messages.cpp)
- [main](src/main.cpp)
- [webinterface](src/webinterface.cpp)
- [986 compiled dbc](documentation/porsche_986_996_compiled_working.dbc)
- [can signals reference](documentation/can_signals.md)

### Notes
- Aligning to your latest scope, even though these signals/messages exist in the current local compiled DBC.

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

## 2026-02-23 22:13:00 +01:00 - iPhone Safari chrome suppression tweaks

### Planned Steps
- Add iOS standalone/PWA meta tags to the web UI HTML.
- Add dynamic viewport height (`100dvh`) and iOS scrolling behavior tweaks to reduce Safari bar movement impact.
- Mirror the HTML changes into `data_spiffs/index.html`.
- Run a quick structural verification command.

### Changes Made
- `data/index.html`: Added iOS standalone/PWA meta tags:
  - `apple-mobile-web-app-capable=yes`
  - `apple-mobile-web-app-status-bar-style=black-translucent`
  - `apple-mobile-web-app-title=eBoxster`
  - `mobile-web-app-capable=yes`
- `data/index.html`: Added viewport/layout behavior tweaks to reduce Safari navigation bar impact:
  - `html, body` use `min-height: 100dvh`
  - `.app` uses `min-height: 100dvh`
  - `body` uses `overscroll-behavior: none` and `-webkit-overflow-scrolling: touch`
- `data_spiffs/index.html`: Mirrored the updated HTML/CSS from `data/index.html`.

### Automated Tests Run
- `powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_ios_safari_bar_tweaks' -Paths @('data/index.html','data_spiffs/index.html','IMPLEMENTATION_LOG.md') }"`: PASS
- `rg --line-number "apple-mobile-web-app-capable|apple-mobile-web-app-status-bar-style|apple-mobile-web-app-title|mobile-web-app-capable|min-height:100dvh|overscroll-behavior:none" data/index.html data_spiffs/index.html`: PASS

### Result
- `ios_standalone_meta_added: true`
- `dynamic_viewport_height_enabled: true`
- `spiffs_ui_mirrored: true`

### Artifacts
- [web ui source](data/index.html)
- [spiffs web ui](data_spiffs/index.html)
- [stage snapshot manifest](archives/stages/20260223_221301_before_ios_safari_bar_tweaks/manifest.json)

### Notes
- Safari browser chrome cannot be fully force-hidden in regular browser mode; these changes optimize fullscreen behavior and Home Screen mode.

---

## 2026-02-23 22:13:58 +01:00 - Upload SPIFFS and flash firmware

### Planned Steps
- Upload SPIFFS image so updated web UI is available on-device.
- Flash firmware to the ESP32-S3 target.
- Record command outcomes and target port details.

### Changes Made
- Uploaded SPIFFS image to ESP32-S3 on `COM15`.
- Uploaded firmware image to ESP32-S3 on `COM15` after retries.

### Automated Tests Run
- `powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_uploadfs_and_flash' -Paths @('IMPLEMENTATION_LOG.md','data/index.html','data_spiffs/index.html','src/webinterface.cpp','src/main.cpp') }"`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs`: FAIL (`COM16` unavailable)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: FAIL (`COM15` busy/permission denied)
- `Start-Sleep -Seconds 2; & "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: FAIL (PlatformIO lock file permission error)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS

### Result
- `spiffs_upload: success`
- `firmware_upload: success`
- `final_target_port: COM15`

### Artifacts
- [spiffs web ui](data_spiffs/index.html)
- [ui source](data/index.html)
- [stage snapshot manifest](archives/stages/20260223_221358_before_uploadfs_and_flash/manifest.json)

### Notes
- No blockers at planning stage.

---

## 2026-02-25 20:39:40 +01:00 - Add linked 986 coverage table at top of can_signals.md

### Planned Steps
- Move/add the 986 coverage table to the beginning of `documentation/can_signals.md`.
- Add per-message links in each row to the corresponding description section in the same markdown.
- Add links in each row to decoder and encoder function definitions in source files.
- Add missing short message sections so every row has a description target.

### Changes Made
- `documentation/can_signals.md`: Added `## Porsche 986 Coverage (Linked)` at top with all 14 986 rows.
- `documentation/can_signals.md`: Added description links per row using in-file anchors (for example `#can-0x280`, `#can-0x4a1`, `#can-0x7a0`).
- `documentation/can_signals.md`: Added decode/encode links per row pointing to function definitions in:
  - `src/986_ecu_messages.cpp`
  - `src/986_vehicle_messages.cpp`
- `documentation/can_signals.md`: Added explicit anchors before existing sections and added short sections for IDs that previously had no section (`0x4A1`, `0x510`, `0x560`, `0x440`, `0x4F8`, `0x7A0`).
- `documentation/can_signals.md`: Removed the old duplicate coverage table block from the bottom to keep a single canonical table at the beginning.

### Automated Tests Run
- `Get-Content documentation/can_signals.md -TotalCount 120`: PASS.
- `rg -n "^## Porsche 986 Coverage \\(Linked\\)" documentation/can_signals.md`: PASS.
- `rg -n "decode986Ecu0x280\\]\\(\\.\\./src/986_ecu_messages.cpp#L9\\)" documentation/can_signals.md`: PASS.
- `rg -n "encode986Vehicle0x00C0\\]\\(\\.\\./src/986_vehicle_messages.cpp#L137\\)" documentation/can_signals.md`: PASS.
- `rg -n "can-0x4a1|can-0x7a0" documentation/can_signals.md`: PASS.

### Result
- `coverage_table_moved_to_top: true`
- `rows_with_description_links: 14`
- `rows_with_decoder_links: 14`
- `rows_with_encoder_links: 14`
- `all_rows_have_description_anchor_targets: true`

### Artifacts
- [can signals reference](documentation/can_signals.md)
- [986 ecu implementation](src/986_ecu_messages.cpp)
- [986 vehicle implementation](src/986_vehicle_messages.cpp)

### Notes
- Decoder/encoder links use current source line anchors and may shift if source files change later.

---

## 2026-02-25 20:42:38 +01:00 - Restore full 986 DBC signal implementation coverage

### Planned Steps
- Reconcile `src/986_ecu_messages.*` and `src/986_vehicle_messages.*` with all signals/messages listed in `documentation/porsche_986_996_compiled_working.dbc`.
- Re-add previously removed 986 DBC messages/signals (`0x1F4`, `0x125`, `0x126`, 0x2A8 16-bit, 0x4E0 boost, remaining 0x1A0 flags).
- Update `documentation/can_signals.md` coverage counts/rows and wheel-speed section to match full DBC implementation.
- Build to validate.

### Changes Made
- `src/986_ecu_messages.cpp`: Verified all ECU-sender DBC messages/signals are implemented (`0x280`, `0x289`, `0x298`, `0x4E0`, `0x4F8`, `0x7A0`) including `BoostPressure` in `0x4E0`.
- `src/986_vehicle_messages.cpp`: Verified all non-ECU DBC messages/signals are implemented (`0x2A8`, `0x1A0`, `0x4A1`, `0x510`, `0x520`, `0x440`, `0x00C0`, `0x1F4`, `0x125`, `0x126`, `0x560`), including both 16-bit and 15-bit legacy wheel-speed signal variants for `0x2A8`.
- `documentation/can_signals.md`: Updated top coverage table to include all 17 DBC messages with decoder/encoder links; added missing detailed sections for `0x1F4`, `0x125`, `0x126`; expanded `0x2A8` section with both 16-bit and legacy 15-bit signals; updated coverage summary counts to `17` total and `11/11` non-ECU in vehicle codec.

### Automated Tests Run
- `Get-Content documentation/porsche_986_996_compiled_working.dbc -TotalCount 220`: PASS.
- `rg --line-number "^bool decode986Ecu0x|^void encode986Ecu0x|^bool decode986Vehicle0x|^void encode986Vehicle0x" src/986_ecu_messages.cpp src/986_vehicle_messages.cpp`: PASS.
- `powershell DBC-vs-decoder coverage check (BO_ IDs vs decode986* dispatch targets)`: PASS (`DBC_IDS=17`, `MISSING=0`, `MISSING_IDS=none`).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `dbc_messages_total: 17`
- `dbc_messages_missing_decoder: 0`
- `ecu_sender_messages_in_ecu_codec: 6/6`
- `non_ecu_sender_messages_in_vehicle_codec: 11/11`
- `build_status: success`

### Artifacts
- [porsche dbc](documentation/porsche_986_996_compiled_working.dbc)
- [986 ecu codec](src/986_ecu_messages.cpp)
- [986 vehicle codec](src/986_vehicle_messages.cpp)
- [can signals reference](documentation/can_signals.md)

### Notes
- This aligns to full DBC coverage, even for messages with sender `TEENSY`/`UNKNOWN` present in the compiled 986 DBC file.

---

## 2026-02-25 19:56:37 +01:00 - Add DBC comments and enums to PQ markdown signal tables

### Planned Steps
- Inspect current `PQ_messages.md` generation/output and confirm missing `CM_ SG_` and `VAL_` data.
- Regenerate `PQ_messages.md` and `documentation/PQ_messages.md` so each signal row includes DBC comment text and enum/value mapping where present.
- Validate output on known examples (e.g. `BSK_HD_Hauptraste`) and record command results.

### Changes Made
- `tools/generate_pq_messages_md.py`: Added a reusable generator that parses `documentation/vw_pq.dbc` and emits:
  - per-message sections
  - full signal tables
  - `DBC Comment` from `CM_ SG_`
  - `Enum Values` from `VAL_`
  - cycle time from `BA_ "GenMsgCycleTime"` where available
- `PQ_messages.md`: Regenerated with the new columns and populated DBC comments/enums.
- `documentation/PQ_messages.md`: Regenerated with the same enriched content.

### Automated Tests Run
- `python tools/generate_pq_messages_md.py`: PASS (`messages=86`, `signals=1331`).
- `rg -n "BSK_HD_Hauptraste" documentation/PQ_messages.md -A 2 -B 2`: PASS (contains comment `Status of trunk lid main detent`).
- `rg -n "GE1_Wahl_Pos|MO5_Mp_Code" documentation/PQ_messages.md -A 1 -B 1`: PASS (contains populated enum mappings).
- `Get-Content documentation/PQ_messages.md -TotalCount 36`: PASS (header shows `DBC Comment` and `Enum Values` columns).

### Result
- `pq_messages_total: 86`
- `pq_signals_total: 1331`
- `dbc_signal_comments_included: true`
- `signal_enum_mappings_included: true`
- `output_files_updated: 2`

### Artifacts
- [PQ generator script](tools/generate_pq_messages_md.py)
- [PQ messages (root)](PQ_messages.md)
- [PQ messages (documentation)](documentation/PQ_messages.md)

### Notes
- No blockers.

---

## 2026-02-25 20:32:07 +01:00 - Keep only legacy wheel speed interpretation and remove non-legacy references

### Planned Steps
- Remove non-legacy wheel speed fields and decode/encode paths from `src/986_vehicle_messages.*`.
- Remove non-legacy wheel speed globals/decoding/status output from `src/main.cpp`.
- Remove non-legacy wheel speed API exports and UI bindings from `src/webinterface.cpp`, `data/index.html`, and `data_spiffs/index.html`.
- Update wheel speed section in `documentation/can_signals.md` to legacy-only interpretation.
- Build to validate.

### Changes Made
- `src/986_vehicle_messages.h`: Removed non-legacy wheel speed fields (`wheel_speed_fl/fr/rl/rr`), keeping only legacy wheel speed fields.
- `src/986_vehicle_messages.cpp`: Removed 16-bit/non-legacy wheel speed decode and encode path in `0x2A8`; kept only legacy 15-bit interpretation and encoding.
- `src/main.cpp`: Removed non-legacy wheel speed globals and 16-bit/non-legacy `0x2A8` interpretation; status output now reports legacy wheel speeds only.
- `src/webinterface.cpp`: Removed non-legacy wheel speed externs and JSON keys (`wfl_kmh`, `wfr_kmh`, `wrl_kmh`, `wrr_kmh`), keeping legacy keys only.
- `data/index.html`: Removed "Wheel New" tiles and corresponding JS bindings.
- `data_spiffs/index.html`: Removed "Wheel New" tiles and corresponding JS bindings.
- `documentation/can_signals.md`: Updated `0x2A8` section to legacy-only wheel speed entries; removed non-legacy 16-bit rows.

### Automated Tests Run
- `rg -n "wheel_speed_fl\\b|wheel_speed_fr\\b|wheel_speed_rl\\b|wheel_speed_rr\\b|wfl_kmh|wfr_kmh|wrl_kmh|wrr_kmh|0\\.005218|1\\.6338" src data data_spiffs documentation/can_signals.md`: PASS (no matches found; `rg` exit code 1 is expected for zero matches).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `legacy_wheel_speed_only_in_code_and_project_docs: true`
- `non_legacy_wheel_speed_references_removed: true`
- `build_status: success`

### Artifacts
- [986 vehicle header](src/986_vehicle_messages.h)
- [986 vehicle implementation](src/986_vehicle_messages.cpp)
- [main source](src/main.cpp)
- [web interface](src/webinterface.cpp)
- [web ui source](data/index.html)
- [spiffs web ui](data_spiffs/index.html)
- [can signal reference](documentation/can_signals.md)

### Notes
- DBC source files are not rewritten; cleanup is applied to implementation and project reference docs.

---

## 2026-02-25 20:37:15 +01:00 - Remove non-986 TEENSY/UNKNOWN frames from 986 vehicle references

### Planned Steps
- Remove `0x1F4`, `0x125`, and `0x126` signal fields/declarations/implementations from `src/986_vehicle_messages.*`.
- Remove dispatcher cases for those IDs from `decode986Vehicle(...)`.
- Remove those IDs from `documentation/can_signals.md` sender mapping coverage.
- Build to validate.

### Changes Made
- `src/986_vehicle_messages.h`: Removed TEENSY/UNKNOWN-related fields (`dcdc_command_byte0`, `dcdc_target_voltage_raw`, `pack_current`, `pack_voltage`, `inverter_temp`, `motor_temp`) and removed declarations for `decode/encode986Vehicle0x1F4`, `0x125`, `0x126`.
- `src/986_vehicle_messages.cpp`: Removed implementations of `decode/encode986Vehicle0x1F4`, `decode/encode986Vehicle0x125`, and `decode/encode986Vehicle0x126`.
- `src/986_vehicle_messages.cpp`: Removed dispatcher cases for `0x1F4`, `0x125`, and `0x126`.
- `documentation/can_signals.md`: Removed coverage rows for `0x1F4`, `0x125`, and `0x126`; updated coverage summary to reflect 986-only tracked set.

### Automated Tests Run
- `rg -n "decode986Vehicle0x1F4|encode986Vehicle0x1F4|decode986Vehicle0x125|encode986Vehicle0x125|decode986Vehicle0x126|encode986Vehicle0x126|0x1F4|0x125|0x126|dcdc_command_byte0|pack_current|pack_voltage|inverter_temp|motor_temp" src documentation/can_signals.md`: PASS (no matches found; `rg` exit code 1 is expected for zero matches).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `removed_vehicle_ids: [0x1F4, 0x125, 0x126]`
- `removed_non_986_signal_fields_from_vehicle_struct: true`
- `coverage_doc_aligned_to_986_only_scope: true`
- `build_status: success`

### Artifacts
- [986 vehicle header](src/986_vehicle_messages.h)
- [986 vehicle implementation](src/986_vehicle_messages.cpp)
- [can signal reference](documentation/can_signals.md)

### Notes
- Scope intentionally limited to implementation and reference docs; no DBC file edits.

---

## 2026-02-25 20:08:16 +01:00 - Add PQ Motor_2 (0x288) support to 986 ECU codec with clearer signal names

### Planned Steps
- Add `0x288` signal fields with readable names to `params::EcuSignals` in `src/986_ecu_messages.h`.
- Implement `decode986Ecu0x288` and `encode986Ecu0x288` in `src/986_ecu_messages.cpp` (including multiplexed byte-0 handling).
- Wire `0x288` into `decode986Ecu(...)` dispatcher and declarations.
- Run build validation and record results.

### Changes Made
- `src/986_ecu_messages.h`: Added descriptive `params::EcuSignals` fields for PQ `Motor_2` (`0x288`) signals, including multiplex byte-0 content and decoded physical values.
- `src/986_ecu_messages.h`: Added `decode986Ecu0x288(...)` and `encode986Ecu0x288(...)` declarations.
- `src/986_ecu_messages.cpp`: Implemented `decode986Ecu0x288(...)` for:
  - multiplex selector (`bits 7..6`) and payload (`bits 5..0`) decoding
  - multiplexer-dependent byte-0 meanings (CAN version, engine code, transmission code, max torque)
  - all fixed signals in bytes 1..7 with proper scaling.
- `src/986_ecu_messages.cpp`: Implemented `encode986Ecu0x288(...)` with matching multiplexer handling and inverse scaling.
- `src/986_ecu_messages.cpp`: Added `0x288` case to `decode986Ecu(...)` dispatcher.

### Automated Tests Run
- `rg -n "decode986Ecu0x288|encode986Ecu0x288|motor2_" src/986_ecu_messages.h src/986_ecu_messages.cpp`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `new_ecu_message_added: 0x288`
- `existing_0x289_behavior_changed: false`
- `multiplexed_byte0_support: true`
- `build_status: success`

### Artifacts
- [986 ecu header](src/986_ecu_messages.h)
- [986 ecu implementation](src/986_ecu_messages.cpp)

### Notes
- Assumption: Existing `0x289` implementation remains unchanged; `0x288` is added as an additional ECU frame.

---

## 2026-02-25 20:10:26 +01:00 - Auto-cycle PQ 0x288 byte0 standard sequence every 4 frames

### Planned Steps
- Implement automatic byte-0 cycling in `encode986Ecu0x288(...)` with standard values `0x40`, `0x80`, `0xF3`, `0x00`.
- Hold each value for 4 consecutive encoded messages, then advance to the next value.
- Keep other `0x288` signal encoding unchanged.
- Run build validation.

### Changes Made
- `src/986_ecu_messages.cpp`: Updated `encode986Ecu0x288(...)` to enforce standard byte-0 sequence `0x40`, `0x80`, `0xF3`, `0x00`.
- `src/986_ecu_messages.cpp`: Implemented 4-frame hold per sequence value using internal frame counter (`(counter/4)%4`), then automatic advance to next value.
- `src/986_ecu_messages.cpp`: Kept all non-byte0 signal encoding unchanged; only byte0 source changed from free parameters to controlled standard sequence.
- `src/986_ecu_messages.cpp`: Mirrored selected byte0 state back into `params::ecu.motor2_*` fields for consistency.

### Automated Tests Run
- `rg -n "kByte0Sequence|s_muxFrameCounter|0x40, 0x80, 0xF3, 0x00|encode986Ecu0x288" src/986_ecu_messages.cpp`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `byte0_standard_sequence_enabled: true`
- `byte0_sequence_values: [0x40, 0x80, 0xF3, 0x00]`
- `frames_per_sequence_value: 4`
- `applied_in_function: encode986Ecu0x288`
- `build_status: success`

### Artifacts
- [986 ecu implementation](src/986_ecu_messages.cpp)

### Notes
- Assumption: Request text says `0x289`, but multiplexed byte-0 behavior applies to PQ `0x288 (Motor_2)`, so implementation is done in `encode986Ecu0x288(...)`.

---

## 2026-02-25 20:24:14 +01:00 - Remove boost pressure from 986 ECU modular implementation

### Planned Steps
- Remove `boost_pressure` signal field from `src/986_ecu_messages.h`.
- Remove boost pressure decode/encode handling from `decode986Ecu0x4E0` and `encode986Ecu0x4E0` in `src/986_ecu_messages.cpp`.
- Run build validation to ensure no remaining references in modular ECU codec.

### Changes Made
- `src/986_ecu_messages.h`: Removed `boost_pressure` from `params::EcuSignals`.
- `src/986_ecu_messages.cpp`: Removed boost pressure decode in `decode986Ecu0x4E0(...)` (previously byte 4 / bits `32..39`).
- `src/986_ecu_messages.cpp`: Removed boost pressure encode in `encode986Ecu0x4E0(...)` and removed `rawBoost` calculation/write.

### Automated Tests Run
- `rg -n "boost_pressure|rawBoost|BoostPressure" src/986_ecu_messages.h src/986_ecu_messages.cpp`: PASS (no matches found; command exit code 1 from `rg` expected for zero matches).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `boost_pressure_removed_from_modular_ecu_codec: true`
- `affected_message: 0x4E0`
- `build_status: success`

### Artifacts
- [986 ecu header](src/986_ecu_messages.h)
- [986 ecu implementation](src/986_ecu_messages.cpp)

### Notes
- Scope limited to modular ECU codec (`src/986_ecu_messages.*`) as requested.

---

## 2026-02-25 20:26:06 +01:00 - Apply standard default bit pattern for 0x280 encode

### Planned Steps
- Add fixed standard-bit enforcement in `encode986Ecu0x280(...)` based on the provided bit map.
- Keep existing dynamic signal encoding for RPM/throttle/pedal values.
- Build to verify no regression.

### Changes Made
- `src/986_ecu_messages.cpp`: Updated `encode986Ecu0x280(...)` to force standard bits to `1` on every encoded frame at indices:
  - `8, 11, 13, 14, 32, 37, 38, 50, 52, 54, 58, 59, 60`
- Existing dynamic writes for RPM, throttle, pedal1, pedal2 remain in place.

### Automated Tests Run
- `rg -n "Enforce standard fixed bit pattern|writeBitsLE\\(frame\\.data, (8|11|13|14|32|37|38|50|52|54|58|59|60), 1, 1U\\)" src/986_ecu_messages.cpp`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `message_id: 0x280`
- `forced_standard_bits_count: 13`
- `build_status: success`

### Artifacts
- [986 ecu implementation](src/986_ecu_messages.cpp)

### Notes
- Assumption: black cells in the provided bit map represent bits that must be forced to `1` on each encoded 0x280 frame.

---

## 2026-02-25 20:27:17 +01:00 - Apply standard default bit pattern for 0x298 encode

### Planned Steps
- Add fixed standard-bit enforcement in `encode986Ecu0x298(...)` based on the provided bit map.
- Keep existing dynamic signal encoding for IAT/throttle/barometric pressure/speed unchanged.
- Build to verify no regression.

### Changes Made
- `src/986_ecu_messages.cpp`: Updated `encode986Ecu0x298(...)` to force standard bits to `1` on every encoded frame at indices:
  - `8, 16, 21, 22, 24, 32, 34, 35, 36, 37, 38`
- Existing dynamic writes for inlet air temp, throttle1/2, barometric pressure, and speed raw remain in place.

### Automated Tests Run
- `rg -n "Enforce standard fixed bit pattern for 0x298|writeBitsLE\\(frame\\.data, (8|16|21|22|24|32|34|35|36|37|38), 1, 1U\\)" src/986_ecu_messages.cpp`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `message_id: 0x298`
- `forced_standard_bits_count: 11`
- `build_status: success`

### Artifacts
- [986 ecu implementation](src/986_ecu_messages.cpp)

### Notes
- Assumption: black cells in the provided bit map represent bits that must be forced to `1` on each encoded 0x298 frame.

---

## 2026-02-25 20:28:22 +01:00 - Apply standard default bit pattern for 0x4E0 encode

### Planned Steps
- Add fixed standard-bit enforcement in `encode986Ecu0x4E0(...)` based on the provided bit map.
- Keep existing dynamic signal encoding unchanged.
- Build to verify no regression.

### Changes Made
- `src/986_ecu_messages.cpp`: Updated `encode986Ecu0x4E0(...)` to force standard bit `1` to `1` on every encoded frame.
- Existing dynamic writes for MIL/reduced-power/fan/fuel-used/oil-temp remain in place.

### Automated Tests Run
- `rg -n "0x4E0 defaults|writeBitsLE\\(frame\\.data, 1, 1, 1U\\)" src/986_ecu_messages.cpp`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `message_id: 0x4E0`
- `forced_standard_bits_count: 1`
- `build_status: success`

### Artifacts
- [986 ecu implementation](src/986_ecu_messages.cpp)

### Notes
- Assumption: black cells in the provided bit map represent bits that must be forced to `1` on each encoded 0x4E0 frame.

---

## 2026-02-25 20:30:26 +01:00 - Move ECU standard/default controls into EcuSignals struct

### Planned Steps
- Add standard/default control fields to `params::EcuSignals` for `0x280`, `0x288`, `0x298`, and `0x4E0`.
- Remove hardcoded standard constants/static state from encoder functions.
- Update encoder functions to read only from `params::ecu` standard/default fields.
- Run build validation.

### Changes Made
- `src/986_ecu_messages.h`: Added standard/default control fields directly into `EcuSignals`:
  - `standard_280_force_one_bits[]`, `standard_280_force_one_count`
  - `standard_298_force_one_bits[]`, `standard_298_force_one_count`
  - `standard_4e0_force_bit1`
  - `standard_288_byte0_sequence[]`, `standard_288_byte0_sequence_len`, `standard_288_frames_per_step`, `standard_288_frame_counter`
- `src/986_ecu_messages.cpp`: Replaced hardcoded `0x280` forced-bit writes with loop over `params::ecu.standard_280_force_one_bits`.
- `src/986_ecu_messages.cpp`: Replaced hardcoded/static `0x288` byte0 sequence state with struct-driven sequence and counter fields from `params::ecu`.
- `src/986_ecu_messages.cpp`: Replaced hardcoded `0x298` forced-bit writes with loop over `params::ecu.standard_298_force_one_bits`.
- `src/986_ecu_messages.cpp`: Replaced hardcoded `0x4E0` bit1 enforcement with `params::ecu.standard_4e0_force_bit1` check.

### Automated Tests Run
- `rg -n "standard_280_force_one_bits|standard_298_force_one_bits|standard_4e0_force_bit1|standard_288_byte0_sequence|static uint32_t s_muxFrameCounter|kByte0Sequence" src/986_ecu_messages.h src/986_ecu_messages.cpp`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `standard_controls_moved_to_struct: true`
- `hardcoded_encoder_standard_patterns_removed: true`
- `build_status: success`

### Artifacts
- [986 ecu header](src/986_ecu_messages.h)
- [986 ecu implementation](src/986_ecu_messages.cpp)

### Notes
- No blockers.

---

## 2026-02-23 22:27:16 +01:00 - Use served asset icons and improve nav/swipe UX

### Planned Steps
- Replace inline bottom-nav SVGs with served `/assets/*.svg` icons.
- Rename page titles to `Vehicle Data`, `ECU Data`, and `Electric Engine`.
- Pin navbar to the bottom for web-app layout consistency.
- Improve horizontal swipe gesture logic to avoid accidental vertical-lock failures.
- Mirror updated HTML to `data_spiffs/index.html` and verify.

### Changes Made
- `data/index.html`: Replaced inline bottom-nav SVG icons with served asset files:
  - Page 1 icon: `/assets/car.svg`
  - Page 2 icon: `/assets/tachometer.svg`
  - Page 3 icon: `/assets/car-battery.svg`
- `data/index.html`: Updated page titles to:
  - `Vehicle Data`
  - `ECU Data`
  - `Electric Engine`
- `data/index.html`: Anchored app layout/nav to bottom using fixed viewport container (`.app { position: fixed; inset: 0; height: 100dvh; }`) and `nav` bottom flex behavior.
- `data/index.html`: Improved swipe handling with axis locking (`swipeAxis`) so vertical moves no longer block intended horizontal page swipes unpredictably.
- `data/index.html`: Added image icon styling for inactive/active tab coloring.
- `data_spiffs/index.html`: Mirrored updated UI from `data/index.html`.
- `data_spiffs/assets/tachometer.svg`: Added required icon file for ECU tab.
- `data_spiffs/assets/car-battery.svg`: Added required icon file for Electric Engine tab.

### Automated Tests Run
- `rg --line-number "\/assets\/car\.svg|\/assets\/tachometer\.svg|\/assets\/car-battery\.svg|ECU Data|Electric Engine|position:fixed;inset:0|swipeAxis|tab img|tab.active img" data/index.html data_spiffs/index.html`: PASS
- `@( 'car.svg','tachometer.svg','car-battery.svg' ) | ForEach-Object { if (Test-Path (Join-Path 'data_spiffs/assets' $_)) { "OK $_" } else { "MISSING $_" } }`: PASS

### Result
- `served_asset_icons_in_nav: true`
- `page_titles_updated: true`
- `navbar_bottom_pinned: true`
- `swipe_axis_lock_added: true`
- `spiffs_assets_present: true`

### Artifacts
- [web ui source](data/index.html)
- [spiffs web ui](data_spiffs/index.html)
- [ecu tab icon](data_spiffs/assets/tachometer.svg)
- [electric engine tab icon](data_spiffs/assets/car-battery.svg)

### Notes
- Icon selection constrained to currently available asset set.

---

## 2026-02-23 22:29:23 +01:00 - Move bit read/write helpers to can_utils

### Planned Steps
- Add generic `readBitsLE` and `writeBitsLE` helpers to `src/can_utils.h`.
- Update codec implementations to use `canutils::readBitsLE` / `canutils::writeBitsLE`.
- Remove duplicated `readBitsLE` / `writeBitsLE` from `src/986_can_codec.h`.
- Run a build to validate no regressions.

### Changes Made
- `src/can_utils.h`: Added generic `canutils::readBitsLE(...)` and `canutils::writeBitsLE(...)` helper functions.
- `src/986_can_codec.h`: Removed duplicated `can986::readBitsLE(...)` and `can986::writeBitsLE(...)`; kept `can986::signExtend(...)`.
- `src/986_ecu_messages.cpp`: Switched all bitfield read/write calls from `can986::...` to `canutils::...`.
- `src/986_vehicle_messages.cpp`: Switched all bitfield read/write calls from `can986::...` to `canutils::...`.
- `src/tesla_sdu_messages.cpp`: Switched all bitfield read/write calls from `can986::...` to `canutils::...` and added `#include "can_utils.h"`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `bit_helpers_moved_to_canutils: true`
- `codec_callsites_updated: true`
- `build_status: success`

### Artifacts
- [generic can utils](src/can_utils.h)
- [986 codec header](src/986_can_codec.h)
- [986 ecu messages](src/986_ecu_messages.cpp)
- [986 vehicle messages](src/986_vehicle_messages.cpp)
- [tesla sdu messages](src/tesla_sdu_messages.cpp)

### Notes
- No blockers at planning stage.

---

## 2026-02-23 22:35:10 +01:00 - Remove can_utils namespace usage

### Planned Steps
- Remove the `canutils` namespace wrapper from `src/can_utils.h`.
- Update codec callsites to use unqualified helper names (`readBitsLE`, `writeBitsLE`, `clampU8`, `clampU16`).
- Run a full build validation.

### Changes Made
- `src/can_utils.h`: Removed `namespace canutils` wrapper so helpers are global inline functions.
- `src/986_ecu_messages.cpp`: Updated helper calls from `canutils::...` to unqualified `readBitsLE`, `writeBitsLE`, `clampU8`, `clampU16`.
- `src/986_vehicle_messages.cpp`: Updated helper calls from `canutils::...` to unqualified `readBitsLE`, `writeBitsLE`, `clampU8`, `clampU16`.
- `src/tesla_sdu_messages.cpp`: Updated helper calls from `canutils::...` to unqualified `readBitsLE`, `writeBitsLE`.

### Automated Tests Run
- `powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_remove_canutils_namespace' -Paths @('src/can_utils.h','src/986_ecu_messages.cpp','src/986_vehicle_messages.cpp','src/tesla_sdu_messages.cpp','IMPLEMENTATION_LOG.md') }"`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS

### Result
- `can_utils_namespace_removed: true`
- `codec_helpers_unqualified: true`
- `build_status: success`

### Artifacts
- [generic can utils](src/can_utils.h)
- [ecu messages](src/986_ecu_messages.cpp)
- [vehicle messages](src/986_vehicle_messages.cpp)
- [tesla sdu messages](src/tesla_sdu_messages.cpp)
- [stage snapshot manifest](archives/stages/20260223_223511_before_remove_canutils_namespace/manifest.json)

### Notes
- No blockers at planning stage.

---

## 2026-02-23 22:36:24 +01:00 - Flash SPIFFS and firmware

### Planned Steps
- Upload SPIFFS image to ESP32-S3.
- Upload firmware image to ESP32-S3.
- Record command results and final upload port.

### Changes Made
- Uploaded SPIFFS image to ESP32-S3 on `COM15`.
- Uploaded firmware image to ESP32-S3 on `COM15` after retry due to transient port/lock contention.

### Automated Tests Run
- `powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_flash_spiffs_and_program' -Paths @('IMPLEMENTATION_LOG.md','data/index.html','data_spiffs/index.html','src/webinterface.cpp','src/main.cpp') }"`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: FAIL (COM15 busy/permission denied)
- `Start-Sleep -Seconds 2; & "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: FAIL (PlatformIO lock file permission denied)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS

### Result
- `spiffs_upload: success`
- `firmware_upload: success`
- `final_target_port: COM15`

### Artifacts
- [spiffs web ui](data_spiffs/index.html)
- [web ui source](data/index.html)
- [stage snapshot manifest](archives/stages/20260223_223624_before_flash_spiffs_and_program/manifest.json)

### Notes
- No blockers at planning stage.

---

## 2026-02-23 22:40:38 +01:00 - Fix iPhone webapp navbar bottom anchoring

### Planned Steps
- Make the bottom navbar viewport-fixed (`position: fixed; bottom: 0`) instead of flow-based.
- Add sufficient bottom padding to page content so scroll/tiles are not hidden behind fixed nav.
- Mirror changes to `data_spiffs/index.html`.
- Validate expected CSS selectors are present.

### Changes Made
- `data/index.html`: Kept navbar hard-fixed at viewport bottom and tightened safe-area spacing:
  - `.nav { position: fixed; left: 0; right: 0; bottom: 0; z-index: 50; ... }`
  - `.nav` bottom padding changed to `calc(6px + var(--safeB))`
- `data/index.html`: Added iOS viewport-height sync variable and switched layout sizing to it:
  - `:root { --vh: 100dvh; }`
  - `html, body, .app` now use `var(--vh)` based sizing.
- `data/index.html`: Added `syncViewportHeight()` JS using `window.visualViewport.height` fallback to `window.innerHeight`, bound on resize events.
- `data/index.html`: Rebalanced content spacing for fixed nav:
  - `.app` bottom padding changed to `calc(80px + var(--safeB))`
  - `.page` bottom padding changed to `calc(76px + var(--safeB))`
- `data_spiffs/index.html`: Mirrored all changes from `data/index.html`.

### Automated Tests Run
- `rg --line-number -- "--vh:100dvh|height:var\\(--vh\\)|syncViewportHeight|visualViewport|\\.nav\\{position:fixed;left:0;right:0;bottom:0" data/index.html data_spiffs/index.html`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS

### Result
- `navbar_viewport_fixed_bottom: true`
- `ios_viewport_sync_added: true`
- `content_bottom_spacing_adjusted: true`
- `spiffs_ui_mirrored: true`
- `spiffs_upload_to_com15: success`

### Artifacts
- [web ui source](data/index.html)
- [spiffs web ui](data_spiffs/index.html)

### Notes
- iOS standalone viewport behavior varies; fixed positioning is used for strongest bottom anchoring.

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

## 2026-02-25 18:49:08 +01:00 - Add VW PQ CAN codec modules from DBC

### Planned Steps
- Extract valid DBC text from `documentation/vw_pq.dbc` (currently stored as a GitHub HTML blob page) into a local parsed DBC source file.
- Generate `src/vw_pq_messages.h` and `src/vw_pq_messages.cpp` with:
- a global `params` struct holding all VW PQ signal values
- per-message `decode0x...` and `encode0x...` functions for every `BO_` frame
- message/signal comments and cycle-time metadata from DBC attributes where available
- Run a build check to confirm the new modules compile.

### Changes Made
- `documentation/vw_pq_extracted.dbc`: Added extracted plain DBC text reconstructed from the embedded `rawLines` payload inside `documentation/vw_pq.dbc`.
- `src/vw_pq_messages.h`: Added VW PQ signal container `params::VwPqSignals` with all parsed signals and declarations for per-message decode/encode functions.
- `src/vw_pq_messages.cpp`: Added generated decode/encode implementations for all VW PQ `BO_` messages.
- `src/vw_pq_messages.cpp`: Added message-level metadata comments for each frame (name, ID, DLC, TX, and DBC attributes like cycle/delay/send type where present), plus signal-level comments.
- `src/vw_pq_messages.cpp`: Included DBC signal comments and value-table text in code comments where available.
- `src/vw_pq_messages.h` and `src/vw_pq_messages.cpp`: Used unique function prefixes `decodeVwPq0x...` / `encodeVwPq0x...` to avoid symbol collisions with existing `986_*` and `tesla_*` modules that share CAN IDs.

### Automated Tests Run
- `python` extraction script reading `documentation/vw_pq.dbc` embedded `rawLines` and writing `documentation/vw_pq_extracted.dbc`: PASS (`lines=1964`, `messages=86`)
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3` (first attempt): FAIL (linker collisions with existing global `decode0x.../encode0x...` symbols on shared IDs)
- Rename pass for VW PQ functions from `decode0x.../encode0x...` to `decodeVwPq0x.../encodeVwPq0x...`: PASS
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3` (after rename): PASS
- Message coverage check script comparing DBC `BO_` count vs generated declarations: PASS (`dbc_messages=86`, `decode_decls=86`, `encode_decls=86`)

### Result
- `vw_pq_messages_generated: true`
- `dbc_messages_parsed: 86`
- `generated_decode_functions: 86`
- `generated_encode_functions: 86`
- `all_dbc_messages_covered: true`
- `build_status: success`

### Artifacts
- [extracted vw pq dbc](documentation/vw_pq_extracted.dbc)
- [vw pq header](src/vw_pq_messages.h)
- [vw pq implementation](src/vw_pq_messages.cpp)
- [stage snapshot manifest](archives/stages/20260225_185032_before_add_vw_pq_messages/manifest.json)

### Notes
- `documentation/vw_pq.dbc` is an HTML page export; actual DBC lines are reconstructed from the embedded `rawLines` JSON payload.

---

## 2026-02-25 19:02:00 +01:00 - Add favicon and PWA icon set from assets/icon.png

### Planned Steps
- Add favicon/apple/PWA link tags to `data/index.html`.
- Generate required icon variants and manifest from `data/assets/icon.png` using a Python script.
- Mirror generated assets and HTML updates to `data_spiffs`.
- Validate generated files and references.

### Changes Made
- `tools/generate_web_icons.py`: Added Python generator to build favicon/PWA assets from `data/assets/icon.png`, including:
- `favicon-16x16.png`
- `favicon-32x32.png`
- `favicon.ico` (ICO container generated from PNG bytes)
- `apple-touch-icon.png`
- `android-chrome-192x192.png`
- `android-chrome-512x512.png`
- `site.webmanifest`
- `safari-pinned-tab.svg`
- `data/assets/*`: Generated icon outputs from source `icon.png`.
- `data_spiffs/assets/*`: Mirrored/generated icon outputs for SPIFFS deployment.
- `data/index.html`: Added favicon/apple/PWA/pinned-tab tags referencing generated `/assets/*` files and aligned iOS meta style/title.
- `data_spiffs/index.html`: Mirrored updated HTML head tags from `data/index.html`.

### Automated Tests Run
- `python tools\generate_web_icons.py`: PASS
- `rg --line-number "favicon-32x32|favicon-16x16|favicon\.ico|apple-touch-icon|site\.webmanifest|safari-pinned-tab|apple-mobile-web-app-status-bar-style" data/index.html data_spiffs/index.html`: PASS
- Asset existence check for generated files in both `data/assets` and `data_spiffs/assets`: PASS
- `Get-Content data_spiffs\assets\site.webmanifest`: PASS (manifest content verified)

### Result
- `source_icon_used: data/assets/icon.png`
- `favicon_pwa_links_added: true`
- `generated_asset_variants: 8`
- `spiffs_assets_ready: true`

### Artifacts
- [icon generator script](tools/generate_web_icons.py)
- [web html head tags](data/index.html)
- [spiffs html head tags](data_spiffs/index.html)
- [generated spiffs manifest](data_spiffs/assets/site.webmanifest)
- [generated spiffs favicon ico](data_spiffs/assets/favicon.ico)

### Notes
- Source icon provided by user at `data/assets/icon.png`.

---

## 2026-02-25 18:58:14 +01:00 - Align iOS/PWA metadata to modern standalone pattern

### Planned Steps
- Update HTML head tags to use root-level icon/manifest paths and white theme color.
- Update generated manifest values (`display`, `background_color`) and icon URLs to root-level paths.
- Add server routes to serve root-level icon/manifest paths from SPIFFS assets.
- Mirror HTML updates to `data_spiffs/index.html` and validate references.

### Changes Made
- `data/index.html`: Updated metadata and icon links to modern standalone pattern with root-level paths:
- `<link rel="apple-touch-icon" href="/apple-touch-icon.png">`
- `<meta name="theme-color" content="#ffffff">`
- `<meta name="apple-mobile-web-app-capable" content="yes">`
- Root-level favicon/manifest links (`/favicon-*.png`, `/favicon.ico`, `/site.webmanifest`, `/safari-pinned-tab.svg`).
- `data_spiffs/index.html`: Mirrored updated head tags from `data/index.html`.
- `tools/generate_web_icons.py`: Updated generated manifest to:
- root-level icon `src` paths (`/android-chrome-192x192.png`, `/android-chrome-512x512.png`)
- `"display": "standalone"`
- `"background_color": "#ffffff"`
- `"theme_color": "#ffffff"`
- `data/assets/site.webmanifest` and `data_spiffs/assets/site.webmanifest`: Regenerated with the new values.
- `src/webinterface.cpp`: Added SPIFFS `serveStatic` aliases for root-level icon/manifest URLs, mapped to files under `/assets/`.

### Automated Tests Run
- `python tools\generate_web_icons.py`: PASS
- `rg --line-number "theme-color|apple-touch-icon|manifest|favicon-32x32|favicon-16x16|favicon\.ico|safari-pinned-tab|apple-mobile-web-app-capable" data/index.html data_spiffs/index.html`: PASS
- `Get-Content data_spiffs\assets\site.webmanifest`: PASS
- `rg --line-number "favicon-16x16|favicon-32x32|favicon\.ico|apple-touch-icon|android-chrome-192x192|android-chrome-512x512|site\.webmanifest|safari-pinned-tab" src/webinterface.cpp`: PASS

### Result
- `modern_ios_metadata_applied: true`
- `manifest_display_standalone: true`
- `manifest_background_color_white: true`
- `root_icon_urls_served: true`
- `spiffs_assets_regenerated: true`

### Artifacts
- [html head metadata](data/index.html)
- [spiffs html metadata](data_spiffs/index.html)
- [icon generator](tools/generate_web_icons.py)
- [spiffs manifest](data_spiffs/assets/site.webmanifest)
- [root asset routes](src/webinterface.cpp)

### Notes
- Root-level asset URLs are backed by explicit `serveStatic` aliases to files stored in `/assets/`.

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

---

## 2026-02-25 19:02:31 +01:00 - Standardize CAN encode/decode function naming across message modules

### Planned Steps
- Rename decode/encode function declarations and definitions in `986_ecu_messages`, `986_vehicle_messages`, `tesla_sdu_messages`, and `vw_pq_messages` to a consistent prefixed scheme.
- Update all in-repo call sites to the renamed function identifiers.
- Run a build validation command to ensure no naming regressions.

### Changes Made
- `src/986_ecu_messages.h`: Renamed function declarations to `decode986Ecu0x...` and `encode986Ecu0x...`.
- `src/986_ecu_messages.cpp`: Renamed function definitions to `decode986Ecu0x...` and `encode986Ecu0x...`.
- `src/986_vehicle_messages.h`: Renamed function declarations to `decode986Vehicle0x...` and `encode986Vehicle0x...`.
- `src/986_vehicle_messages.cpp`: Renamed function definitions to `decode986Vehicle0x...` and `encode986Vehicle0x...`.
- `src/tesla_sdu_messages.h`: Renamed function declarations to `decodeTeslaSdu0x...` and `encodeTeslaSdu0x...`.
- `src/tesla_sdu_messages.cpp`: Renamed function definitions to `decodeTeslaSdu0x...` and `encodeTeslaSdu0x...`.
- `src/main.cpp`: Updated Tesla SDU decode call sites to the new function names.
- `src/vw_pq_messages.h` + `src/vw_pq_messages.cpp`: No rename required; these files already used prefixed naming (`decodeVwPq0x...` / `encodeVwPq0x...`).

### Automated Tests Run
- `rg -n "\bdecode0x|\bencode0x" src/main.cpp src/986_ecu_messages.h src/986_vehicle_messages.h src/tesla_sdu_messages.h`: PASS (no matches).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `naming_standard_applied_986_ecu: true`
- `naming_standard_applied_986_vehicle: true`
- `naming_standard_applied_tesla_sdu: true`
- `vw_pq_prefix_naming_already_compliant: true`
- `build_status: success`

### Artifacts
- [986 ECU header](src/986_ecu_messages.h)
- [986 ECU implementation](src/986_ecu_messages.cpp)
- [986 vehicle header](src/986_vehicle_messages.h)
- [986 vehicle implementation](src/986_vehicle_messages.cpp)
- [Tesla SDU header](src/tesla_sdu_messages.h)
- [Tesla SDU implementation](src/tesla_sdu_messages.cpp)
- [main decode call sites](src/main.cpp)

### Notes
- No blockers.

---

## 2026-02-25 19:05:09 +01:00 - Add module-level decoder dispatch functions

### Planned Steps
- Add one dispatcher declaration per message module header.
- Implement dispatcher definitions in each corresponding `.cpp` with `switch (frame.id)` routing to the specific `decode...0x...` function.
- Build the project to validate symbol and compile integrity.

### Changes Made
- `src/986_ecu_messages.h`: Added dispatcher declaration `bool decode986Ecu(const CANMessage &frame);`.
- `src/986_ecu_messages.cpp`: Added dispatcher implementation with `switch (frame.id)` routing to `decode986Ecu0x280/289/298/4E0`.
- `src/986_vehicle_messages.h`: Added dispatcher declaration `bool decode986Vehicle(const CANMessage &frame);`.
- `src/986_vehicle_messages.cpp`: Added dispatcher implementation with `switch (frame.id)` routing to `decode986Vehicle0x2A8/1A0/520/00C0`.
- `src/tesla_sdu_messages.h`: Added dispatcher declaration `bool decodeTeslaSdu(const CANMessage &frame);`.
- `src/tesla_sdu_messages.cpp`: Added dispatcher implementation with `switch (frame.id)` routing to `decodeTeslaSdu0x320/321/322/323/324`.
- `src/vw_pq_messages.h`: Added dispatcher declaration `bool decodeVwPq(const CANMessage &frame);`.
- `src/vw_pq_messages.cpp`: Added dispatcher implementation with `switch (frame.id)` routing to all `decodeVwPq0x...` decoders declared in the module.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `dispatchers_added_986_ecu: true`
- `dispatchers_added_986_vehicle: true`
- `dispatchers_added_tesla_sdu: true`
- `dispatchers_added_vw_pq: true`
- `build_status: success`

### Artifacts
- [986 ECU header](src/986_ecu_messages.h)
- [986 ECU implementation](src/986_ecu_messages.cpp)
- [986 vehicle header](src/986_vehicle_messages.h)
- [986 vehicle implementation](src/986_vehicle_messages.cpp)
- [Tesla SDU header](src/tesla_sdu_messages.h)
- [Tesla SDU implementation](src/tesla_sdu_messages.cpp)
- [VW PQ header](src/vw_pq_messages.h)
- [VW PQ implementation](src/vw_pq_messages.cpp)

### Notes
- No blockers.

---

## 2026-02-25 19:07:51 +01:00 - Generate PQ messages markdown overview

### Planned Steps
- Parse `documentation/vw_pq.dbc` to extract every `BO_` message entry and metadata.
- Extract optional cycle time metadata (`GenMsgCycleTime`) and compute per-message signal counts.
- Generate `documentation/PQ_messages.md` with a full Markdown table overview of all messages.

### Changes Made
- `documentation/PQ_messages.md`: Generated a complete VW PQ message overview table (all messages) with columns for ID, name, DLC, transmitter, signal count, and cycle time placeholder.
- `PQ_messages.md`: Added the same overview at repo root as requested.
- `documentation/vw_pq.dbc`: Inspected and found this file is currently HTML content, so metadata extraction was done from `src/vw_pq_messages.cpp` (the generated VW PQ implementation source).

### Automated Tests Run
- `Get-Content documentation\\vw_pq.dbc -TotalCount 80`: PASS (revealed HTML content, not raw DBC text).
- `(Select-String -Path PQ_messages.md -Pattern '^\\| 0x').Count`: PASS (`86` message rows found).
- `Get-Content PQ_messages.md -TotalCount 22`: PASS (table header and first rows verified).

### Result
- `pq_messages_md_generated: true`
- `messages_documented: 86`
- `cycle_time_values_available_from_source: false`
- `output_paths: documentation/PQ_messages.md, PQ_messages.md`

### Artifacts
- [PQ messages overview (root)](PQ_messages.md)
- [PQ messages overview (documentation)](documentation/PQ_messages.md)

### Notes
- Assumption: `src/vw_pq_messages.cpp` is the canonical available source for VW PQ metadata in this workspace, since `documentation/vw_pq.dbc` is not raw DBC text.

---

## 2026-02-25 19:11:20 +01:00 - Validation build and dual flash after latest changes

### Planned Steps
- Run validation build for `esp32s3`.
- Flash firmware to `COM15`.
- Flash SPIFFS filesystem image to `COM15`.

### Changes Made
- No source code changes.
- Executed validation build and dual flash commands on `esp32s3` / `COM15`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PARTIAL PASS (PlatformIO reports `SUCCESS`, but trailing SPIFFS pack message reports `SPIFFS_write error(-10010)` and `Error for adding content from assets`).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15` (re-run): PARTIAL PASS (same trailing SPIFFS asset add error).
- `Get-ChildItem -LiteralPath data_spiffs\assets -Force`: PASS (asset files enumerated for diagnosis).

### Result
- `build_status: success`
- `firmware_flash_com15: success`
- `spiffs_flash_com15: success_with_asset_pack_warning`
- `spiffs_warning_reproducible: true`

### Artifacts
- [implementation log](documentation/IMPLEMENTATION_LOG.md)

### Notes
- Blocker: SPIFFS image creation/upload step emits `Error for adding content from assets` after `SUCCESS`; likely SPIFFS file/path naming constraint in current asset set.

---

## 2026-02-25 19:12:29 +01:00 - Add FastLED RGB utility and wire blink task

### Planned Steps
- Add `src/utils.h` with an enum-based color API and FastLED helper for ESP32-S3 built-in RGB LED.
- Integrate `main.cpp` blink task to use the new helper instead of `digitalWrite` toggling.
- Add required FastLED dependency/config and run a validation build.

### Changes Made
- `src/utils.h`: Added FastLED utility with enum-based color API (`LedColor`) and functions `fastledInit()` + `fastled(LedColor)`.
- `src/main.cpp`: Included `utils.h`, switched `blinkLED()` from GPIO toggle to enum-driven FastLED color cycling (red, green, blue, yellow, white), and initialized LED via `fastledInit()` in `setup()`.
- `platformio.ini`: Added FastLED dependency (`FastLED/FastLED@^3.9.15`).

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3` (initial run): FAIL (FastLED pin resolved as invalid pin `97`).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3` (after pin/config fix in `utils.h`): PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: FAIL (`Could not open COM15`, permission denied / busy).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PARTIAL PASS (upload succeeds; trailing SPIFFS `assets` add-file warning persists).

### Result
- `fastled_utils_added: true`
- `blink_task_drives_enum_colors: true`
- `build_status: success_after_fix`
- `firmware_flash_com15: failed_port_busy`
- `spiffs_flash_com15: success_with_known_assets_warning`

### Artifacts
- [FastLED utility](src/utils.h)
- [main integration](src/main.cpp)
- [PlatformIO dependency update](platformio.ini)

### Notes
- Blocker: `COM15` was busy during firmware upload; serial monitor or another process likely holds the port.

---

## 2026-02-25 19:15:30 +01:00 - Verify VW PQ DBC message implementation coverage

### Planned Steps
- Extract all message IDs (`BO_`) from VW PQ DBC source.
- Extract all implemented VW PQ decode/encode message IDs from `src/vw_pq_messages.h/.cpp`.
- Compare sets and report missing/extra IDs.

### Changes Made
- No source-code changes.
- Verified VW PQ message coverage by comparing DBC `BO_` IDs against implemented `decodeVwPq0x...` / `encodeVwPq0x...` IDs in both header and source.
- Verified dispatcher `decodeVwPq(const CANMessage&)` switch-case coverage against all decode functions.

### Automated Tests Run
- `Get-Content documentation\vw_pq.dbc -TotalCount 20`: PASS (confirmed raw DBC content is present).
- Coverage script comparing DBC IDs vs `src/vw_pq_messages.h/.cpp` decode+encode IDs: PASS.
- Dispatcher switch coverage script (`case 0x... -> decodeVwPq0x...`) vs decode function IDs: PASS.

### Result
- `dbc_message_count: 86`
- `decode_cpp_count: 86`
- `encode_cpp_count: 86`
- `decode_header_count: 86`
- `encode_header_count: 86`
- `missing_decode_ids: 0`
- `missing_encode_ids: 0`
- `extra_decode_ids: 0`
- `extra_encode_ids: 0`
- `dispatcher_switch_missing_ids: 0`

### Artifacts
- [vw pq dbc](documentation/vw_pq.dbc)
- [vw pq implementation header](src/vw_pq_messages.h)
- [vw pq implementation source](src/vw_pq_messages.cpp)

### Notes
- No blockers.

---

## 2026-02-25 19:18:10 +01:00 - Consolidate CAN bit helpers into can_utils

### Planned Steps
- Ensure `readBitsLE`, `writeBitsLE`, `signExtend`, and `copyFrame` are defined in `src/can_utils.h`.
- Remove duplicate helper definitions from `src/main.cpp` and rely on shared include.
- Build to verify no missing symbols or regressions.

### Changes Made
- `src/can_utils.h`: Added shared helpers `signExtend(...)` and `copyFrame(...)`; included `<ACAN2515.h>` for `CANMessage`.
- `src/main.cpp`: Added `#include "can_utils.h"` and removed duplicated local helper implementations for `readBitsLE`, `signExtend`, `writeBitsLE`, `copyFrame`, `clampU8`, and `clampU16`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `can_helper_deduplication_complete: true`
- `main_cpp_local_helper_duplicates_removed: true`
- `build_status: success`

### Artifacts
- [shared can utilities](src/can_utils.h)
- [main source](src/main.cpp)

### Notes
- No blockers.

---

## 2026-02-25 19:20:30 +01:00 - Ensure clamp helpers are only in can_utils

### Planned Steps
- Check whether `clampU8`/`clampU16` still exist in `src/main.cpp`.
- If present, remove local duplicates and rely on `src/can_utils.h`.
- Run build validation.

### Changes Made
- No code changes required.
- Confirmed `clampU8` and `clampU16` are defined only in `src/can_utils.h` and used from `src/main.cpp`.

### Automated Tests Run
- `rg -n "static inline uint8_t clampU8|static inline uint16_t clampU16|\bclampU8\(|\bclampU16\(" src/main.cpp src/can_utils.h`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `main_local_clamp_definitions_present: false`
- `can_utils_clamp_definitions_present: true`
- `build_status: success`

### Artifacts
- [shared can utilities](src/can_utils.h)
- [main source](src/main.cpp)

### Notes
- No blockers.

---

## 2026-02-25 19:22:40 +01:00 - Expand PQ markdown with per-message signal details

### Planned Steps
- Inspect existing message markdown format used in project for per-message signal sections.
- Regenerate `PQ_messages.md` with message-level sections and signal tables for each VW PQ message.
- Validate output structure and update implementation log.

### Changes Made
- `PQ_messages.md`: Rebuilt from `documentation/vw_pq.dbc` into per-message sections with full signal tables (same style as existing CAN signal markdown format).
- `documentation/PQ_messages.md`: Updated to the same regenerated content for consistency.
- Included per message: CAN ID heading, DLC/TX/Cycle metadata line, and a signal table with columns `Signal`, `Start`, `Len`, `Endian`, `Type`, `Factor`, `Offset`, `Unit`, `Source`, `Notes`.

### Automated Tests Run
- Regeneration script (`tools/_regen_pq_md.ps1` temporary run): PASS (`messages=86`, `signals=1331`).
- `Get-Content PQ_messages.md -TotalCount 80`: PASS (verified section/table format).
- `(Select-String -Path PQ_messages.md -Pattern '^## CAN ID ').Count`: PASS (`86` sections).
- `(Select-String -Path PQ_messages.md -Pattern '^\\| Signal \\| Start').Count`: PASS (`86` signal tables).

### Result
- `pq_md_contains_message_sections: true`
- `pq_md_contains_signal_tables: true`
- `message_sections_count: 86`
- `total_signals_documented: 1331`
- `format_matches_project_signal_md_style: true`

### Artifacts
- [PQ message reference](PQ_messages.md)
- [PQ message reference (documentation)](documentation/PQ_messages.md)

### Notes
- No blockers.

---

## 2026-02-25 19:27:10 +01:00 - Restore webapp nav icons and iPhone app icon visibility

### Planned Steps
- Analyze when icons disappeared by checking SPIFFS upload/build logs and current asset serving paths.
- Restore nav icons and app icon delivery using SPIFFS-safe filenames.
- Rebuild, flash firmware, and upload SPIFFS to verify icon availability.

### Changes Made
- `src/webinterface.cpp`: Updated static routes to serve icon/PWA URLs from short root SPIFFS files (`/fav16.png`, `/fav32.png`, `/fav.ico`, `/app.png`, `/a192.png`, `/a512.png`, `/mask.svg`, `/i_car.svg`, `/i_ecu.svg`, `/i_sdu.svg`).
- `data_spiffs/index.html`: Switched nav icon image paths from `/assets/*.svg` to `/i_car.svg`, `/i_ecu.svg`, `/i_sdu.svg`.
- `data/index.html`: Mirrored the same nav icon path updates.
- `data_spiffs/`: Added short root-level icon files (`i_car.svg`, `i_ecu.svg`, `i_sdu.svg`, `app.png`, `fav16.png`, `fav32.png`, `fav.ico`, `a192.png`, `a512.png`, `mask.svg`, `site.webmanifest`).
- `data_spiffs/assets/`: Removed all files except `.gitkeep` to eliminate SPIFFS pack failures from long/complex asset paths.
- `data/`: Added mirrored short root-level icon files for local parity with `data_spiffs`.

### Automated Tests Run
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15`: PASS (no trailing SPIFFS add-file errors; all short icon files listed in image build).
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15`: PASS.

### Result
- `root_cause_identified: true`
- `root_cause_summary: SPIFFS packaging failed on assets directory in prior runs, so nav/app icon files were not consistently present on device`
- `nav_icons_restored_paths: true`
- `iphone_app_icon_route_restored: true`
- `spiffs_pack_error_resolved: true`
- `firmware_flashed_com15: true`
- `spiffs_uploaded_com15: true`

### Artifacts
- [web server static mapping](src/webinterface.cpp)
- [SPIFFS index](data_spiffs/index.html)
- [dev index mirror](data/index.html)
- [SPIFFS root icons](data_spiffs)

### Notes
- Analysis indicates icon disappearance started after introducing many files under `data_spiffs/assets`; this correlates with repeated `SPIFFS_write error(-10010)` lines seen in prior uploadfs logs.

---

## 2026-02-25 19:31:20 +01:00 - Align 986 ECU vs vehicle message implementation with Porsche DBC sender

### Planned Steps
- Parse `documentation/porsche_986_996_compiled_working.dbc` to classify all messages by sender (`ECU` vs non-ECU).
- Compare classified IDs against current implementations in `src/986_ecu_messages.*` and `src/986_vehicle_messages.*`.
- Implement/add/move message decode+encode functions so all ECU-sender messages are in ECU files and all others in vehicle files.
- Update markdown documentation with implementation coverage and sender-based placement status.
- Build and run upload validation as feasible.

### Changes Made
- `src/986_ecu_messages.h`: Added ECU-only DBC frames `0x4F8` and `0x7A0` with explicit decode/encode declarations and backing signal storage.
- `src/986_ecu_messages.cpp`: Implemented `decode986Ecu0x4F8` / `encode986Ecu0x4F8` and `decode986Ecu0x7A0` / `encode986Ecu0x7A0`; updated ECU dispatcher to include both IDs.
- `src/986_vehicle_messages.h`: Added non-ECU DBC declarations and signal fields for `0x4A1`, `0x510`, `0x440`, `0x1F4`, `0x125`, `0x126`, `0x560`.
- `src/986_vehicle_messages.cpp`: Implemented non-ECU decoders/encoders and dispatcher wiring for `0x4A1`, `0x510`, `0x440`, `0x1F4`, `0x125`, `0x126`, `0x560`.
- `src/986_vehicle_messages.h` and `src/986_vehicle_messages.cpp`: Corrected DCDC command CAN ID from `0x1D4` to DBC-correct `0x1F4` (`BO_ 500`).
- `documentation/can_signals.md`: Added `Porsche 986 DBC Sender Mapping Coverage` table with all 17 DBC messages, sender, target codec file, decode/encode function names, and implementation status.

### Automated Tests Run
- `Get-Content documentation/porsche_986_996_compiled_working.dbc -TotalCount 260`: PASS.
- `rg -n "decode986(Ecu|Vehicle)0x|encode986(Ecu|Vehicle)0x" src/986_ecu_messages.h src/986_ecu_messages.cpp src/986_vehicle_messages.h src/986_vehicle_messages.cpp`: PASS.
- `powershell coverage check (DBC BO_ sender split vs decoder dispatchers)`: PASS (`TOTAL_DBC=17`, `MISSING_ECU=0`, `MISSING_VEH=0`, `MISPLACED_IN_ECU=0`, `MISPLACED_IN_VEH=0`).
- `rg -n "decode986Vehicle0x1F4|encode986Vehicle0x1F4|case 0x1F4|decode986Vehicle0x1D4|encode986Vehicle0x1D4|case 0x1D4" src/986_vehicle_messages.h src/986_vehicle_messages.cpp documentation/can_signals.md`: PASS.
- `& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32s3`: PASS.

### Result
- `dbc_total_messages: 17`
- `ecu_sender_messages_expected: 6`
- `non_ecu_sender_messages_expected: 11`
- `ecu_sender_messages_in_ecu_codec: 6/6`
- `non_ecu_sender_messages_in_vehicle_codec: 11/11`
- `misplaced_messages: 0`
- `remaining_missing_messages: 0`
- `build_status: success`

### Artifacts
- [porsche 986 dbc](documentation/porsche_986_996_compiled_working.dbc)
- [986 ecu messages header](src/986_ecu_messages.h)
- [986 ecu messages implementation](src/986_ecu_messages.cpp)
- [986 vehicle messages header](src/986_vehicle_messages.h)
- [986 vehicle messages implementation](src/986_vehicle_messages.cpp)
- [can signals reference](documentation/can_signals.md)

### Notes
- No blockers at planning stage.

---

## 2026-02-25 21:19:05 +01:00 - Remove MONITOR_PORT macro and use GVRET_PORT directly

### Planned Steps
- Locate all MONITOR_PORT references in source.
- Replace logging calls to use GVRET_PORT directly and remove the MONITOR_PORT macro definition.
- Run a PlatformIO build to verify no regressions.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: Removed #define MONITOR_PORT GVRET_PORT.
- src/canmanager.h: Replaced all MONITOR_PORT log calls with GVRET_PORT.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- monitor_port_macro_removed: true
- logging_uses_gvret_port_directly: true
- uild_status: success

### Artifacts
- [main.cpp](src/main.cpp)
- [canmanager.h](src/canmanager.h)

### Notes
- Existing inline variable warnings in src/canmanager.h are unchanged and pre-existing.

---

## 2026-02-25 21:20:10 +01:00 - Decode only vehicle frames in pollVehicleCAN

### Planned Steps
- Update pollVehicleCAN() to call only decode986Vehicle(frame).
- Keep USB forwarding unchanged.
- Build esp32s3 to validate.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: In pollVehicleCAN(), removed decode986Ecu(frame) so only decode986Vehicle(frame) is called.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- poll_vehicle_can_decodes_only_vehicle: true
- uild_status: success

### Artifacts
- [main.cpp](src/main.cpp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:21:05 +01:00 - Schedule emulateECUCAN as 30ms task

### Planned Steps
- Add a TaskScheduler task that calls emulateECUCAN() every 30 ms.
- Remove direct emulateECUCAN() call from pollCAN() to avoid duplicate execution.
- Build esp32s3 to validate.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: Added Task taskEmulateECUCAN(30, TASK_FOREVER, &emulateECUCAN, &runner, true);.
- src/main.cpp: Removed direct emulateECUCAN(); call from pollCAN().

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- emulate_ecu_can_runs_via_scheduler: true
- emulate_ecu_can_period_ms: 30
- duplicate_direct_call_removed: true
- uild_status: success

### Artifacts
- [main.cpp](src/main.cpp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:23:00 +01:00 - Remap 986 CAN 0x289 to Motor_2 and remove original EngineTemp 0x289

### Planned Steps
- Move Motor_2 payload handling from  x288 to  x289 in 986 ECU codec and remove  x288 encoder/decoder entry points.
- Update main ECU emulation scheduling to stop sending  x288 and keep  x289 at 30 ms.
- Remove original EngineTemp  x289 references from implementation and docs; update 986 DBC and documentation/can_signals.md to describe  x289 as Motor_2 with 30 ms timeslice.
- Build esp32s3 to validate.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/986_ecu_messages.h:
  - Removed  x288 API declarations (decode986Ecu0x288, encode986Ecu0x288).
  - Remapped standard sequence state from standard_288_* to standard_289_*.
  - Removed legacy original- x289 signal fields (coolant_temperature, coolant_level_switch, cruise_control_active).
  - Updated Motor_2 comment to explicit  x289 mapping.
- src/986_ecu_messages.cpp:
  - Replaced old decode986Ecu0x289/encode986Ecu0x289 (EngineTemp) with Motor_2 codec logic previously used for  x288.
  - Updated frame ID checks/writes from  x288 to  x289 for Motor_2 encoding/decoding.
  - Updated state usage from standard_288_* to standard_289_*.
  - Removed  x288 case from decode986Ecu(...) dispatcher.
- src/main.cpp:
  - Removed periodic  x288 transmission path from emulateECUCAN().
  - Kept  x289 at 30 ms and remapped coolant override to params::ecu.motor2_coolant_temperature.
- src/webinterface.cpp:
  - Removed old original- x289 field usage and mapped live coolant/cruise indicators to Motor_2-backed fields.
  - Removed /api/set-ecu writes for deleted original- x289 fields.
- documentation/porsche_986_996_compiled_working.dbc:
  - Replaced BO_ 649 EngineTemp with BO_ 649 Motor_2 using MO2 multiplexed signal set.
  - Updated CM_ BO_ 649 comment to Motor_2 mapping with retained 30ms time slice.
- documentation/can_signals.md:
  - Updated  x289 mapping row and detailed section from EngineTemp to Motor_2 signal table.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- motor2_payload_mapped_to_0x289: true
- legacy_enginetemp_0x289_codec_removed: true
-  x288_removed_from_986_ecu_implementation: true
- emulation_uses_0x289_timeslice_30ms: true
- dbc_649_updated_to_motor2: true
- can_signals_md_0x289_updated: true
- uild_status: success

### Artifacts
- [986 ECU header](src/986_ecu_messages.h)
- [986 ECU implementation](src/986_ecu_messages.cpp)
- [main loop/emulation](src/main.cpp)
- [web interface](src/webinterface.cpp)
- [986 DBC](documentation/porsche_986_996_compiled_working.dbc)
- [CAN signals reference](documentation/can_signals.md)

### Notes
- Historical references to old  x289 behavior remain in prior entries of documentation/IMPLEMENTATION_LOG.md by design (audit trail).

---

## 2026-02-25 21:26:11 +01:00 - Remove override globals and use params only

### Planned Steps
- Remove override globals from src/main.cpp and related override logic in emulateECUCAN().
- Remove override extern declarations from src/webinterface.cpp.
- Update /api/set and live JSON fields to read/write directly from params::ecu.
- Build esp32s3 and record result.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: Removed all override globals (engine_speed_override_*, engine_temp_override_*, engine_stat_override_*) so only params state is used.
- src/main.cpp: Removed override application block from emulateECUCAN().
- src/webinterface.cpp: Removed override extern declarations.
- src/webinterface.cpp: Updated live JSON set_* fields to read directly from params::ecu.
- src/webinterface.cpp: Updated /api/set handler to write directly to params::ecu.engine_speed_rpm, params::ecu.motor2_coolant_temperature, and params::ecu.oil_temperature.

### Automated Tests Run
- g -n "engine_speed_override_active|engine_speed_override_rpm|engine_temp_override_active|coolant_temperature_override_c|engine_stat_override_active|oil_temperature_override_c" src/main.cpp src/webinterface.cpp: PASS (no matches)
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- override_globals_removed: true
- untime_state_source: params_only
- pi_set_writes_params_directly: true
- uild_status: success

### Artifacts
- [main.cpp](src/main.cpp)
- [webinterface.cpp](src/webinterface.cpp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:27:31 +01:00 - Evenly distribute ECU frame sends without millis

### Planned Steps
- Replace millis()-based burst sending in emulateECUCAN() with slot-based round-robin frame transmission.
- Change ECU emulation task period to 6 ms so five ECU frames are spread across a 30 ms cycle.
- Build esp32s3 to validate.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: Changed ECU emulation task period from 30 ms to 6 ms (	askEmulateECUCAN).
- src/main.cpp: Replaced millis()-based per-message timing in emulateECUCAN() with slot-based round-robin scheduler.
- src/main.cpp: emulateECUCAN() now sends exactly one frame per invocation in sequence:  x280,  x289,  x298,  x4E0,  x7A0.
- src/main.cpp: Added fixed-cycle slot wrap ( ..4) so each frame is emitted once every 30 ms without burst peaks.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- millis_dependency_removed_from_emulate_ecu_can: true
- ecu_frames_evenly_spread_over_30ms_cycle: true
- per_call_frames_sent: 1
- emulation_task_period_ms: 6
- uild_status: success

### Artifacts
- [main.cpp](src/main.cpp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:29:03 +01:00 - Set runtime LED to green blink at 30 percent brightness

### Planned Steps
- Replace multi-color LED cycle in linkLED() with green on/off blink only.
- Set FastLED brightness to 30% during runtime blink handling.
- Build esp32s3 to validate.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: Replaced multi-color LED cycle in linkLED() with green on/off blink only.
- src/main.cpp: Set runtime LED brightness to 30% in linkLED() via FastLED.setBrightness((255*30)/100).

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- unning_led_mode: green_blink
- unning_led_brightness_percent: 30
- uild_status: success

### Artifacts
- [main.cpp](src/main.cpp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:41:46 +01:00 - Set serial baud to 1000000 and flash firmware

### Planned Steps
- Change Serial.begin(...) in src/main.cpp from 115200 to 1000000.
- Flash firmware to ESP32-S3 on COM15.
- Record command result and artifacts.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: Changed Serial.begin(115200) to Serial.begin(1000000).

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15: PASS

### Result
- serial_baud_configured: 1000000
- irmware_flashed: true
- lash_port: COM15

### Artifacts
- [main.cpp](src/main.cpp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:43:08 +01:00 - Remove pollCAN and printStatus wrappers from main

### Planned Steps
- Remove pollCAN() and printStatus() prototypes and definitions from src/main.cpp.
- Update loop() to call pollVehicleCAN() and pollTeslaCAN() directly.
- Build esp32s3 to validate.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: Removed pollCAN() prototype and definition.
- src/main.cpp: Removed printStatus() prototype and definition.
- src/main.cpp: Updated loop() to call pollVehicleCAN() and pollTeslaCAN() directly.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- pollcan_wrapper_removed: true
- printstatus_wrapper_removed: true
- loop_calls_direct_poll_functions: true
- uild_status: success

### Artifacts
- [main.cpp](src/main.cpp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:47:28 +01:00 - Restore ECU page slider/toggle control style from archive

### Planned Steps
- Compare current ECU page control rendering with archived slider/toggle style.
- Update data_spiffs/index.html ECU control renderer to use slider/toggle tiles while preserving full /api/set-ecu mapping.
- Mirror the same update to data/index.html for parity.
- Build esp32s3 to validate firmware compiles with unchanged web server code.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- data_spiffs/index.html: Restored ECU page control rendering to styled slider/toggle tiles (archive style) while keeping full current /api/set-ecu control coverage.
- data_spiffs/index.html: Replaced plain checkbox/number control generator with:
  - toggle switches for bool fields using existing .toggle / .toggle-ui styling,
  - slider tiles for numeric fields using existing .slider / .slider-tile styling,
  - live value labels per control (ecv_*).
- data_spiffs/index.html: Removed obsolete ECU control keys from page-2 control defs that no longer exist in current backend (coolant_temperature, coolant_level_switch, cruise_control_active).
- data/index.html: Mirrored the same ECU control renderer updates for parity.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- rchive_root_cause_identified: true
- oot_cause: ecu_page_controls_were_replaced_with_plain_generated_inputs
- ecu_page_slider_toggle_style_restored: true
- ull_api_set_ecu_mapping_preserved: true
- uild_status: success

### Artifacts
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)

### Notes
- To see the restored controls on device, uploadfs is required in addition to firmware flash.

---

## 2026-02-25 21:49:07 +01:00 - Set runtime LED blink brightness to 10 percent

### Planned Steps
- Update linkLED() brightness scaling in src/main.cpp from 30% to 10%.
- Build esp32s3 to validate.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/main.cpp: Updated linkLED() runtime brightness from 30% to 10% (FastLED.setBrightness((255*10)/100)).

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- unning_led_brightness_percent: 10
- uild_status: success

### Artifacts
- [main.cpp](src/main.cpp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:50:30 +01:00 - Flash SPIFFS and firmware to COM15

### Planned Steps
- Upload SPIFFS image to COM15.
- Upload firmware image to COM15.
- Record command results.

### Changes Made
- No file changes.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- None.

### Notes
- None.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15: PASS

### Result
- spiffs_uploaded: true
- irmware_uploaded: true
- upload_port: COM15

### Artifacts
- None.

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 21:58:25 +01:00 - Resume mandatory stage snapshots and enforce in runbook

### Planned Steps
- Create a stage snapshot immediately before any further edits.
- Update 	ools/upload_ui_post_test_runbook_986.md to make stage snapshot mandatory before every code/doc change.
- Record stage id/path and validation in this implementation log entry.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- 	ools/upload_ui_post_test_runbook_986.md: Added mandatory requirement to create a snapshot before any code/doc change and to log stage_id + stage_path in IMPLEMENTATION_LOG.md.
- 	ools/upload_ui_post_test_runbook_986.md: Added Mandatory gate section under Stage Archiving Workflow.
- 	ools/upload_ui_post_test_runbook_986.md: Added robust snapshot command form using powershell -Command and -Paths @(...).

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_enforce_archive_in_runbook' -Paths @('src/main.cpp','src/webinterface.cpp','data/index.html','data_spiffs/index.html','tools/upload_ui_post_test_runbook_986.md','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "Before any code/doc change|Mandatory gate|SNAPSHOT_OK|robust form" tools/upload_ui_post_test_runbook_986.md: PASS

### Result
- snapshot_created_before_edit: true
- snapshot_stage_id: 20260225_215834_before_enforce_archive_in_runbook
- snapshot_stage_path: archives/stages/20260225_215834_before_enforce_archive_in_runbook
- unbook_snapshot_gate_added: true

### Artifacts
- [runbook](tools/upload_ui_post_test_runbook_986.md)
- [snapshot stage](archives/stages/20260225_215834_before_enforce_archive_in_runbook)

### Notes
- None.

---

## 2026-02-25 22:00:02 +01:00 - Enforce archive ignore and untrack archived files

### Planned Steps
- Create a stage snapshot before changing ignore/index state.
- Tighten .gitignore archive rules to explicitly ignore staged archive content.
- Remove already-tracked archive files from Git index (--cached) while keeping files on disk.
- Verify git ls-files archives is empty.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- .gitignore: Replaced broad archive ignore with explicit recursive rules:
  - rchives/**
  - !archives/.gitkeep
  - !archives/stages/.gitkeep
- Git index: Removed already-tracked archive files via git rm -r --cached archives (files remain on disk).

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_archive_gitignore_enforce' -Paths @('.gitignore','documentation/IMPLEMENTATION_LOG.md') }": PASS
- git rm -r --cached archives: PASS
- git ls-files archives: PASS (no output; none tracked)
- git status --short .gitignore archives: PASS (shows .gitignore modified and archive files staged for index removal)

### Result
- snapshot_created_before_edit: true
- snapshot_stage_id: 20260225_220009_before_archive_gitignore_enforce
- snapshot_stage_path: archives/stages/20260225_220009_before_archive_gitignore_enforce
- rchive_ignore_rules_explicit: true
- 	racked_archive_files_remaining: 0

### Artifacts
- [gitignore](.gitignore)
- [snapshot stage](archives/stages/20260225_220009_before_archive_gitignore_enforce)

### Notes
- Archive files are now ignored for future changes and removed from Git index; local files are preserved.

---

## 2026-02-25 22:03:14 +01:00 - Confirm full 986 vehicle signal UI coverage and fix acceleration unit label

### Planned Steps
- Validate that the vehicle page includes all requested 986 vehicle signals.
- Correct the acceleration unit text rendering in UI (m/s� display issue).
- Mirror the text fix in both data_spiffs/index.html and data/index.html.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- data_spiffs/index.html: Confirmed vehicle page ehicleSignals list already includes the complete requested Porsche 986 vehicle signal set.
- data_spiffs/index.html: Fixed acceleration unit text from malformed m/s² to m/s^2.
- data/index.html: Mirrored the same acceleration unit text fix.
- src/main.cpp: Moved #define GVRET_PORT Serial above #include "gvret.h" / #include "canmanager.h" to restore compile correctness for validation.

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_vehicle_page_unit_fix' -Paths @('data_spiffs/index.html','data/index.html','documentation/IMPLEMENTATION_LOG.md') }": PASS
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_fix_gvret_port_define_order' -Paths @('src/main.cpp','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "Longitudinal Acceleration|m/s\^2|m/s²" data_spiffs/index.html data/index.html: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- ehicle_page_contains_all_requested_986_vehicle_signals: true
- cceleration_unit_label_fixed: true
- snapshot_stage_id_1: 20260225_220321_before_vehicle_page_unit_fix
- snapshot_stage_path_1: archives/stages/20260225_220321_before_vehicle_page_unit_fix
- snapshot_stage_id_2: 20260225_220412_before_fix_gvret_port_define_order
- snapshot_stage_path_2: archives/stages/20260225_220412_before_fix_gvret_port_define_order
- uild_status: success

### Artifacts
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)
- [main.cpp](src/main.cpp)
- [snapshot 1](archives/stages/20260225_220321_before_vehicle_page_unit_fix)
- [snapshot 2](archives/stages/20260225_220412_before_fix_gvret_port_define_order)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 22:05:54 +01:00 - Enforce ECU page tile width convention and document it

### Planned Steps
- Create a stage snapshot before edits.
- Update UI CSS/renderer so slider controls are 2x1 (full row) and boolean/value tiles are 1x1.
- Mirror changes in both data_spiffs/index.html and data/index.html.
- Document the convention in runbook for future changes.
- Validate with a build.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- data_spiffs/index.html: Enforced ECU control tile layout rule in CSS:
  - .slider-tile spans full row (2x1) via grid-column: 1 / -1.
  - .control-tile remains 1x1 via grid-column: span 1.
  - Added .value-tile as explicit 1x1 class.
- data_spiffs/index.html: Updated ECU renderer to support def.ui === "value" and render alue-tile (1x1).
- data/index.html: Mirrored the same layout and renderer changes.
- 	ools/upload_ui_post_test_runbook_986.md: Added persistent UI Layout Convention section documenting:
  - sliders 2x1
  - booleans 1x1
  - value tiles 1x1

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_ecu_tile_width_rule' -Paths @('data_spiffs/index.html','data/index.html','tools/upload_ui_post_test_runbook_986.md','documentation/IMPLEMENTATION_LOG.md') }": PASS
- Select-String -Path data_spiffs/index.html,data/index.html -Pattern '\.slider-tile\{|\.control-tile\{|\.value-tile\{|def\.ui === "value"': PASS
- Select-String -Path tools/upload_ui_post_test_runbook_986.md -Pattern 'UI Layout Convention|Slider visual controls must be 2x1|Boolean controls must be 1x1|Value tiles must be 1x1': PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- snapshot_stage_id: 20260225_220602_before_ecu_tile_width_rule
- snapshot_stage_path: archives/stages/20260225_220602_before_ecu_tile_width_rule
- slider_tiles_are_2x1: true
- oolean_tiles_are_1x1: true
- alue_tiles_are_1x1: true
- layout_rule_documented_for_future: true
- uild_status: success

### Artifacts
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)
- [runbook](tools/upload_ui_post_test_runbook_986.md)
- [snapshot stage](archives/stages/20260225_220602_before_ecu_tile_width_rule)

### Notes
- None.

---

## 2026-02-25 22:07:50 +01:00 - Flash SPIFFS and firmware to COM15 after ECU tile layout update

### Planned Steps
- Upload SPIFFS image to COM15.
- Upload firmware image to COM15.
- Record command results.

### Changes Made
- No file changes.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- None.

### Notes
- None.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15: PASS

### Result
- spiffs_uploaded: true
- irmware_uploaded: true
- upload_port: COM15

### Artifacts
- None.

### Notes
- None.

---

## 2026-02-25 22:11:49 +01:00 - Fix slider swipe conflict and iPhone home-screen icon metadata

### Planned Steps
- Create stage snapshot before UI edits.
- Update touch-swipe logic to ignore drag start on interactive controls (range/inputs/toggles/buttons).
- Add stricter Apple touch icon link metadata for iPhone home-screen icon detection.
- Mirror changes in both data_spiffs/index.html and data/index.html.
- Validate build.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- data_spiffs/index.html: Added iOS icon metadata hardening (pple-touch-icon with explicit sizes=180x180 and pple-touch-icon-precomposed).
- data/index.html: Mirrored the same iOS icon metadata changes.
- data_spiffs/index.html: Updated swipe handler to ignore drag start on interactive controls (input/select/textarea/button/label) so slider drags do not trigger page swipes.
- data/index.html: Mirrored the swipe-handler fix.
- src/webinterface.cpp: Added iOS-friendly icon fallback routes:
  - /apple-touch-icon-180x180.png -> /app.png
  - /apple-touch-icon-precomposed.png -> /app.png

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_swipe_and_ios_icon_fix' -Paths @('data_spiffs/index.html','data/index.html','src/webinterface.cpp','documentation/IMPLEMENTATION_LOG.md') }": PASS
- @' ... png header check ... '@ | python -: PASS (pp.png, av16.png, av32.png, 192.png, 512.png currently all 200x200)
- Select-String -Path data_spiffs/index.html,data/index.html -Pattern 'apple-touch-icon|pointerdown': PASS
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_add_ios_icon_fallback_routes' -Paths @('src/webinterface.cpp','documentation/IMPLEMENTATION_LOG.md') }": PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- slider_swipe_conflict_mitigated: true
- ios_icon_metadata_strengthened: true
- ios_icon_fallback_routes_added: true
- snapshot_stage_id_1: 20260225_221156_before_swipe_and_ios_icon_fix
- snapshot_stage_path_1: archives/stages/20260225_221156_before_swipe_and_ios_icon_fix
- snapshot_stage_id_2: 20260225_221314_before_add_ios_icon_fallback_routes
- snapshot_stage_path_2: archives/stages/20260225_221314_before_add_ios_icon_fallback_routes
- uild_status: success

### Artifacts
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)
- [web interface server](src/webinterface.cpp)
- [snapshot 1](archives/stages/20260225_221156_before_swipe_and_ios_icon_fix)
- [snapshot 2](archives/stages/20260225_221314_before_add_ios_icon_fallback_routes)

### Notes
- iOS may cache old home-screen icons; remove existing home-screen shortcut and add it again after uploadfs.

---

## 2026-02-25 22:20:02 +01:00 - Add 0x4E0 boolean bits 2/5/6/7 to ECU params, codec, and UI

### Planned Steps
- Create stage snapshot before edits.
- Add new ECU params fields for  x4E0 bits 2, 5, 6, 7.
- Update decode986Ecu0x4E0 and encode986Ecu0x4E0 to read/write these bits.
- Expose these fields in web JSON/API (/api/live, /api/set-ecu) and add ECU page controls.
- Build esp32s3 to validate.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.

### Changes Made
- src/986_ecu_messages.h: Added new global ECU params for  x4E0 boolean bits:
  - engine_stat_bool2
  - engine_stat_bool5
  - engine_stat_bool6
  - engine_stat_bool7
- src/986_ecu_messages.cpp:
  - decode986Ecu0x4E0(...) now decodes bits 2, 5, 6, 7 into the new params fields.
  - encode986Ecu0x4E0(...) now encodes bits 2, 5, 6, 7 from the new params fields.
- src/webinterface.cpp:
  - Added new booleans to live JSON output.
  - Added new booleans to ECU page JSON payload.
  - Added /api/set-ecu parsing/writing for the new booleans.
- data_spiffs/index.html:
  - Added ECU controls for the new booleans:
    - engine_stat_bool2
    - engine_stat_bool5
    - engine_stat_bool6
    - engine_stat_bool7
- data/index.html: Mirrored the same ECU control additions.

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_add_4e0_bits_2_5_6_7' -Paths @('src/986_ecu_messages.h','src/986_ecu_messages.cpp','src/webinterface.cpp','data_spiffs/index.html','data/index.html','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "engine_stat_bool2|engine_stat_bool5|engine_stat_bool6|engine_stat_bool7" src/986_ecu_messages.h src/986_ecu_messages.cpp src/webinterface.cpp data_spiffs/index.html data/index.html: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- snapshot_stage_id: 20260225_222011_before_add_4e0_bits_2_5_6_7
- snapshot_stage_path: archives/stages/20260225_222011_before_add_4e0_bits_2_5_6_7
- 
ew_4e0_bit_booleans_added_to_params: true
- 
ew_4e0_bit_booleans_added_to_decode_encode: true
- 
ew_4e0_bit_booleans_added_to_ui: true
- uild_status: success

### Artifacts
- [ECU params header](src/986_ecu_messages.h)
- [ECU codec](src/986_ecu_messages.cpp)
- [web API server](src/webinterface.cpp)
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)
- [snapshot stage](archives/stages/20260225_222011_before_add_4e0_bits_2_5_6_7)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-25 22:22:50 +01:00 - Double flash to COM15 (SPIFFS + firmware)

### Planned Steps
- Upload SPIFFS image to COM15.
- Upload firmware image to COM15.
- Record command results.

### Changes Made
- No file changes.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- None.

### Notes
- None.

---

## 2026-02-25 22:24:29 +01:00 - Double flash to COM15 (SPIFFS + firmware) - Result

### Planned Steps
- Record SPIFFS upload command result.
- Record firmware upload command result.
- Confirm overall double-flash status.

### Changes Made
- No file changes.

### Automated Tests Run
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t uploadfs --upload-port COM15: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3 -t upload --upload-port COM15: PASS

### Result
- uploadfs_status: success
- firmware_upload_status: success
- target_port: COM15
- double_flash_status: success

### Artifacts
- [SPIFFS image build output](.pio/build/esp32s3/spiffs.bin)
- [Firmware image](.pio/build/esp32s3/firmware.bin)

### Notes
- None.

---

## 2026-02-26 00:01:33 +01:00 - Complete 0x2A8 legacy wheel-speed + free bits in code/DBC/MD

### Planned Steps
- Create stage snapshot before modifications.
- Add all 0x2A8 legacy signals (4 wheel speeds + 4 free bits) to vehicle params and codec.
- Align Porsche 986 DBC 0x2A8 signal list to the same layout.
- Update 986 can_signals.md 0x2A8 table accordingly.
- Run validation build and record results.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.


---

## 2026-02-26 00:03:33 +01:00 - Complete 0x2A8 legacy wheel-speed + free bits in code/DBC/MD - Result

### Planned Steps
- Create stage snapshot before modifications.
- Add all 0x2A8 legacy signals (4 wheel speeds + 4 free bits) to vehicle params and codec.
- Align Porsche 986 DBC 0x2A8 signal list to the same layout.
- Update 986 can_signals.md 0x2A8 table accordingly.
- Run validation build and record results.

### Changes Made
- src/986_vehicle_messages.h: Added rei_bremse_3_1..4 to params::VehicleSignals.
- src/986_vehicle_messages.cpp: decode986Vehicle0x2A8 now decodes bits  /16/32/48 into rei_bremse_3_1..4; encode986Vehicle0x2A8 now writes those four bits.
- documentation/porsche_986_996_compiled_working.dbc: Added SG_ Frei_Bremse_3_1..4 to BO_ 680 Vehicle_Speed at start bits  /16/32/48.
- documentation/can_signals.md: Expanded  x2A8 table with Frei_Bremse_3_1..4 so all 8 legacy fields are documented.

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_complete_0x2a8_legacy_bits' -Paths @('src/986_vehicle_messages.h','src/986_vehicle_messages.cpp','documentation/porsche_986_996_compiled_working.dbc','documentation/can_signals.md','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "frei_bremse_3_[1-4]|Frei_Bremse_3_[1-4]" src/986_vehicle_messages.h src/986_vehicle_messages.cpp documentation/porsche_986_996_compiled_working.dbc documentation/can_signals.md: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- snapshot_stage_id: 20260226_000143_before_complete_0x2a8_legacy_bits
- message_0x2a8_legacy_complete_in_codec: 	rue
- message_0x2a8_legacy_complete_in_986_dbc: 	rue
- message_0x2a8_legacy_complete_in_986_md: 	rue
- build_status: success

### Artifacts
- [Vehicle signal struct](src/986_vehicle_messages.h)
- [Vehicle codec implementation](src/986_vehicle_messages.cpp)
- [Porsche 986 DBC](documentation/porsche_986_996_compiled_working.dbc)
- [986 CAN signal reference](documentation/can_signals.md)
- [Stage snapshot](archives/stages/20260226_000143_before_complete_0x2a8_legacy_bits)

### Notes
- Existing C++17 inline-variable warnings from src/canmanager.h are unchanged.

---

## 2026-02-26 00:05:43 +01:00 - Adopt RB Lenkwinkel_1 layout for 0x00C0 across codec/DBC/MD/UI

### Planned Steps
- Create stage snapshot before changing files.
- Replace 0x00C0 decode/encode mapping in 986 vehicle codec with RB bit layout (DLC 2).
- Update 986 DBC BO_ 192 and comments to RB fields.
- Update 986 can_signals.md section for 0x00C0.
- Expose new 0x00C0 RB fields in /api/live and vehicle page UI.
- Run validation build and record results.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.


---

## 2026-02-26 00:08:13 +01:00 - Adopt RB Lenkwinkel_1 layout for 0x00C0 across codec/DBC/MD/UI - Result

### Planned Steps
- Create stage snapshot before changing files.
- Replace 0x00C0 decode/encode mapping in 986 vehicle codec with RB bit layout (DLC 2).
- Update 986 DBC BO_ 192 and comments to RB fields.
- Update 986 can_signals.md section for 0x00C0.
- Expose new 0x00C0 RB fields in /api/live and vehicle page UI.
- Run validation build and record results.

### Changes Made
- src/986_vehicle_messages.h: Replaced old steering-angle fields with RB fields for  x00C0: orzeichen_rb, lenkwinkel_rb, lws_ok_rb, lws_abgleich_rb, rei_lenkwinkel_1_1_rb, ine_check_rb, coarse_check_rb.
- src/986_vehicle_messages.cpp: Reworked decode986Vehicle0x00C0 and encode986Vehicle0x00C0 to RB mapping (DLC=2, bits  ..15 as provided).
- src/webinterface.cpp: /api/live now exports the RB steering keys and no longer exports steer_old_deg / steer_deg.
- data_spiffs/index.html: Vehicle page signal list switched from old steering entries to RB entries.
- data/index.html: Mirrored same vehicle page RB signal updates.
- documentation/porsche_986_996_compiled_working.dbc: BO_ 192 SWA updated to DLC=2 with RB signals and updated BO comment.
- documentation/can_signals.md: CAN ID 0x00C0 table replaced with RB signal table.

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_adopt_0x00c0_rb_layout' -Paths @('src/986_vehicle_messages.h','src/986_vehicle_messages.cpp','src/webinterface.cpp','data_spiffs/index.html','data/index.html','documentation/porsche_986_996_compiled_working.dbc','documentation/can_signals.md','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "steer_old_deg|steer_deg|vorzeichen_rb|lenkwinkel_rb|lws_ok_rb|lws_abgleich_rb|frei_lenkwinkel_1_1_rb|fine_check_rb|coarse_check_rb|BO_ 192 SWA: 2|Lenkwinkel__RB_" src/986_vehicle_messages.h src/986_vehicle_messages.cpp src/webinterface.cpp data_spiffs/index.html data/index.html documentation/porsche_986_996_compiled_working.dbc documentation/can_signals.md: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- snapshot_stage_id: 20260226_000543_before_adopt_0x00c0_rb_layout
- rb_0x00c0_layout_in_codec: 	rue
- rb_0x00c0_layout_in_986_dbc: 	rue
- rb_0x00c0_layout_in_986_md: 	rue
- rb_0x00c0_layout_in_web_ui: 	rue
- build_status: success

### Artifacts
- [986 vehicle header](src/986_vehicle_messages.h)
- [986 vehicle implementation](src/986_vehicle_messages.cpp)
- [web interface backend](src/webinterface.cpp)
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)
- [986 DBC](documentation/porsche_986_996_compiled_working.dbc)
- [986 CAN signals MD](documentation/can_signals.md)
- [stage snapshot](archives/stages/20260226_000543_before_adopt_0x00c0_rb_layout)

### Notes
- Existing C++17 inline-variable warnings from src/canmanager.h are unchanged.

---

## 2026-02-26 00:19:30 +01:00 - Replace 986 0x4E0 with MO5 definition across codec/UI/DBC/MD

### Planned Steps
- Create stage snapshot before modifications.
- Replace 0x4E0 ECU signals in struct + decode/encode with MO5 fields.
- Update web API and ECU page controls to the new MO5 keys.
- Update Porsche 986 DBC BO_1248 signal definitions for MO5.
- Update documentation/can_signals.md 0x4E0 section to MO5 list.
- Run build validation and record results.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.


---

## 2026-02-26 00:22:46 +01:00 - Replace 986 0x4E0 with MO5 definition across codec/UI/DBC/MD - Result

### Planned Steps
- Create stage snapshot before modifications.
- Replace 0x4E0 ECU signals in struct + decode/encode with MO5 fields.
- Update web API and ECU page controls to the new MO5 keys.
- Update Porsche 986 DBC BO_1248 signal definitions for MO5.
- Update documentation/can_signals.md 0x4E0 section to MO5 list.
- Run build validation and record results.

### Changes Made
- src/986_ecu_messages.h: Replaced old  x4E0 fields with MO5 fields (mo5_vorgluehen, mo5_e_gas, mo5_obd_2, mo5_heissl, mo5_klimakompr, mo5_feld_kuehl, mo5_kliko_red, mo5_verbrauch_ul, mo5_ueberlverb).
- src/986_ecu_messages.cpp: Reimplemented decode986Ecu0x4E0/encode986Ecu0x4E0 for MO5 mapping: bits 9..15, 16..30 (U15), bit 31.
- src/webinterface.cpp: Updated /api/live and /api/set-ecu to expose/set MO5 keys; removed old 4E0 keys from API payload; mil now mirrors mo5_obd_2.
- data_spiffs/index.html: Updated ECU page controls from old EngineStat booleans/oil/fuel to MO5 controls.
- data/index.html: Mirrored same ECU control updates.
- documentation/porsche_986_996_compiled_working.dbc: Replaced BO_ 1248 EngineStat signals with MO5 signals and updated BO comment.
- documentation/can_signals.md: Replaced  x4E0 signal table with MO5 definition rows.

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_replace_0x4e0_with_mo5' -Paths @('src/986_ecu_messages.h','src/986_ecu_messages.cpp','src/webinterface.cpp','data_spiffs/index.html','data/index.html','documentation/porsche_986_996_compiled_working.dbc','documentation/can_signals.md','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "mo5_vorgluehen|mo5_e_gas|mo5_obd_2|mo5_heissl|mo5_klimakompr|mo5_feld_kuehl|mo5_kliko_red|mo5_verbrauch_ul|mo5_ueberlverb|MO5_Vorgluehen|MO5_Verbrauch" src/986_ecu_messages.h src/986_ecu_messages.cpp src/webinterface.cpp data_spiffs/index.html data/index.html documentation/porsche_986_996_compiled_working.dbc documentation/can_signals.md: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- snapshot_stage_id: 20260226_001931_before_replace_0x4e0_with_mo5
- message_0x4e0_mo5_codec_applied: 	rue
- message_0x4e0_mo5_ui_applied: 	rue
- message_0x4e0_mo5_dbc_applied: 	rue
- message_0x4e0_mo5_md_applied: 	rue
- build_status: success

### Artifacts
- [ECU signals header](src/986_ecu_messages.h)
- [ECU codec implementation](src/986_ecu_messages.cpp)
- [Web API backend](src/webinterface.cpp)
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)
- [986 DBC](documentation/porsche_986_996_compiled_working.dbc)
- [986 CAN signals doc](documentation/can_signals.md)
- [Stage snapshot](archives/stages/20260226_001931_before_replace_0x4e0_with_mo5)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-26 00:38:29 +01:00 - Restore 0x4E0 byte0 interpretation while keeping MO5 mapping

### Planned Steps
- Create stage snapshot before modifications.
- Re-add byte0 fields (bits 0..7) to ECU struct and 0x4E0 decode/encode.
- Re-expose byte0 fields in web API and ECU controls.
- Update 986 DBC + can_signals.md 0x4E0 section to include byte0 + MO5 signals.
- Run validation build and record results.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.


---

## 2026-02-26 00:40:26 +01:00 - Restore 0x4E0 byte0 interpretation while keeping MO5 mapping - Result

### Planned Steps
- Create stage snapshot before modifications.
- Re-add byte0 fields (bits 0..7) to ECU struct and 0x4E0 decode/encode.
- Re-expose byte0 fields in web API and ECU controls.
- Update 986 DBC + can_signals.md 0x4E0 section to include byte0 + MO5 signals.
- Run validation build and record results.

### Changes Made
- src/986_ecu_messages.h: Re-added byte0 booleans for  x4E0 (check_engine_light, check_engine_light_bit1, engine_stat_bool2, educed_power, an_error, engine_stat_bool5, engine_stat_bool6, engine_stat_bool7) while keeping MO5 fields.
- src/986_ecu_messages.cpp: decode986Ecu0x4E0 and encode986Ecu0x4E0 now process both byte0 (its 0..7) and MO5 (its 9..31) mappings.
- src/webinterface.cpp: Re-added byte0 fields to /api/live, ECU field export, and /api/set-ecu setter handling.
- data_spiffs/index.html: Re-added byte0 controls on ECU page.
- data/index.html: Mirrored same byte0 control additions.
- documentation/porsche_986_996_compiled_working.dbc: Added byte0 SG fields for BO_ 1248 EngineStat and adjusted BO comment to state byte0 + MO5 coexistence.
- documentation/can_signals.md: Updated  x4E0 table to include byte0 rows plus MO5 rows.

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_restore_4e0_byte0_interp' -Paths @('src/986_ecu_messages.h','src/986_ecu_messages.cpp','src/webinterface.cpp','data_spiffs/index.html','data/index.html','documentation/porsche_986_996_compiled_working.dbc','documentation/can_signals.md','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "check_engine_light|check_engine_light_bit1|engine_stat_bool2|reduced_power|fan_error|engine_stat_bool5|engine_stat_bool6|engine_stat_bool7|MO5_Vorgluehen|EngineStat_Bool2|EngineStat_Bool5|EngineStat_Bool6|EngineStat_Bool7" src/986_ecu_messages.h src/986_ecu_messages.cpp src/webinterface.cpp data_spiffs/index.html data/index.html documentation/porsche_986_996_compiled_working.dbc documentation/can_signals.md: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- snapshot_stage_id: 20260226_003829_before_restore_4e0_byte0_interp
- message_0x4e0_byte0_interpretation_restored: 	rue
- message_0x4e0_mo5_mapping_preserved: 	rue
- build_status: success

### Artifacts
- [ECU signals header](src/986_ecu_messages.h)
- [ECU codec implementation](src/986_ecu_messages.cpp)
- [Web API backend](src/webinterface.cpp)
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)
- [986 DBC](documentation/porsche_986_996_compiled_working.dbc)
- [986 CAN signals doc](documentation/can_signals.md)
- [Stage snapshot](archives/stages/20260226_003829_before_restore_4e0_byte0_interp)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-26 00:47:34 +01:00 - Replace 986 0x1A0 with Bremse_1 definition across code/DBC/MD/UI

### Planned Steps
- Create stage snapshot before modifications.
- Update vehicle signal struct + decode/encode for full Bremse_1 bit mapping.
- Update 986 DBC BO_416 to match Bremse_1 signals and scaling.
- Update can_signals.md 0x1A0 table to the new definition.
- Update web live JSON and vehicle UI tiles to show Bremse_1 fields.
- Run build validation and record results.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.


---

## 2026-02-26 00:51:19 +01:00 - Replace 986 0x1A0 with Bremse_1 definition across code/DBC/MD/UI - Result

### Planned Steps
- Create stage snapshot before modifications.
- Update vehicle signal struct + decode/encode for full Bremse_1 bit mapping.
- Update 986 DBC BO_416 to match Bremse_1 signals and scaling.
- Update can_signals.md 0x1A0 table to the new definition.
- Update web live JSON and vehicle UI tiles to show Bremse_1 fields.
- Run build validation and record results.

### Changes Made
- src/986_vehicle_messages.h: Extended VehicleSignals with missing Bremse_1 fields: msr_torque_inverse, r1_counter, r1_asr_esp, r1_espasr_passive, r1_sta_esp, r1_ersatz_kmh.
- src/986_vehicle_messages.cpp: Updated decode986Vehicle0x1A0/encode986Vehicle0x1A0 to Bremse_1 semantics:
  - BR1_Rad_kmh corrected to 17|15 with  .01 factor.
  - Added dual interpretation for byte 5 (BR1_ASRMo_fa and BR1_MSR_Mo_inv) via msr_torque_inverse.
  - Added bits 56..63 handling (BR1_Zaehler, BR1_ASR_ESP, BR1_ESPASR_passive, BR1_Sta_ESP, BR1_Ersatz_Kmh).
- src/webinterface.cpp: Added live JSON fields for new Bremse_1 values (msr_inv_pct, r1_counter, r1_asr_esp, r1_espasr_passive, r1_sta_esp, r1_ersatz_kmh).
- data_spiffs/index.html: Updated Vehicle page labels to BR1 naming and added the new BR1 tiles.
- data/index.html: Mirrored same Vehicle page BR1 updates.
- documentation/porsche_986_996_compiled_working.dbc: Replaced BO_ 416 ABS_1 SG list with full Bremse_1 BR1 mapping and updated message comment.
- documentation/can_signals.md: Replaced  x1A0 section table with full Bremse_1 signal list.

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_replace_0x1a0_bremse1' -Paths @('src/986_vehicle_messages.h','src/986_vehicle_messages.cpp','src/webinterface.cpp','data_spiffs/index.html','data/index.html','documentation/porsche_986_996_compiled_working.dbc','documentation/can_signals.md','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "msr_torque_inverse|br1_counter|br1_asr_esp|br1_espasr_passive|br1_sta_esp|br1_ersatz_kmh|BR1_Rad_kmh|BR1_Zaehler|msr_inv_pct" src/986_vehicle_messages.h src/986_vehicle_messages.cpp src/webinterface.cpp data_spiffs/index.html data/index.html documentation/porsche_986_996_compiled_working.dbc documentation/can_signals.md: PASS
- & "C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe" run -e esp32s3: PASS

### Result
- snapshot_stage_id: 20260226_004734_before_replace_0x1a0_bremse1
- message_0x1a0_bremse1_codec_applied: 	rue
- message_0x1a0_bremse1_ui_applied: 	rue
- message_0x1a0_bremse1_dbc_applied: 	rue
- message_0x1a0_bremse1_md_applied: 	rue
- build_status: success

### Artifacts
- [986 vehicle header](src/986_vehicle_messages.h)
- [986 vehicle codec](src/986_vehicle_messages.cpp)
- [web API](src/webinterface.cpp)
- [SPIFFS UI](data_spiffs/index.html)
- [data UI mirror](data/index.html)
- [986 DBC](documentation/porsche_986_996_compiled_working.dbc)
- [986 signal doc](documentation/can_signals.md)
- [stage snapshot](archives/stages/20260226_004734_before_replace_0x1a0_bremse1)

### Notes
- Existing C++17 inline-variable warnings in src/canmanager.h are unchanged.

---

## 2026-02-26 20:24:34 +01:00 - Enforce archives gitignore policy and document in runbook

### Planned Steps
- Snapshot .gitignore + runbook + implementation log before edits.
- Harden .gitignore rules for archives at repo root.
- Add explicit archives-ignore requirement to canonical runbook.
- Verify with quick grep and record results.

### Changes Made
- Pending.

### Automated Tests Run
- Pending.

### Result
- Pending.

### Artifacts
- Pending.

### Notes
- None.


---

## 2026-02-26 20:25:09 +01:00 - Enforce archives gitignore policy and document in runbook - Result

### Planned Steps
- Snapshot .gitignore + runbook + implementation log before edits.
- Harden .gitignore rules for archives at repo root.
- Add explicit archives-ignore requirement to canonical runbook.
- Verify with quick grep and record results.

### Changes Made
- .gitignore: Added explicit root-level ignore rules /archives/ and /archives/** (existing keep-file exceptions retained).
- 	ools/upload_ui_post_test_runbook_986.md: Added Git tracking rule section under Stage Archiving Workflow requiring archive contents to remain untracked, with only .gitkeep placeholders allowed.

### Automated Tests Run
- powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_enforce_archives_gitignore_policy' -Paths @('.gitignore','tools/upload_ui_post_test_runbook_986.md','documentation/IMPLEMENTATION_LOG.md') }": PASS
- g -n "^/archives/|^/archives/\*\*|archives/\.gitkeep|Git tracking rule|Keep all generated stage archive contents out of git" .gitignore tools/upload_ui_post_test_runbook_986.md: PASS
- git check-ignore -v archives/stages/test_dummy.txt: PASS

### Result
- snapshot_stage_id: 20260226_202434_before_enforce_archives_gitignore_policy
- archives_root_ignore_explicit: 	rue
- runbook_archives_rule_added: 	rue
- git_ignore_verification: rchives/stages/* ignored

### Artifacts
- [gitignore](.gitignore)
- [runbook](tools/upload_ui_post_test_runbook_986.md)
- [stage snapshot](archives/stages/20260226_202434_before_enforce_archives_gitignore_policy)

### Notes
- None.

---

## 2026-04-11 01:31:07 +02:00 - Consolidate duplicated SPIFFS web asset folders

### Planned Steps
- Point PlatformIO at a single web asset directory.
- Remove remaining tooling references to `data_spiffs/`.
- Delete the duplicated tracked files under `data_spiffs/`.
- Verify the project still builds and the filesystem image resolves from `data/`.

### Changes Made
- `platformio.ini`: Switched `data_dir` from `data_spiffs` to `data`.
- `tools/generate_web_icons.py`: Removed the duplicate `data_spiffs` output so generated assets target only `data/`.
- `data_spiffs/*`: Removed the duplicated tracked SPIFFS web asset files; `data/` is now the single source of truth.

### Automated Tests Run
- `rg -n "\bdata_spiffs\b" -S --glob '!documentation/IMPLEMENTATION_LOG.md' --glob '!**/.git/**'`: PASS (no live references remain outside the historical log)
- `git ls-files data_spiffs`: PASS (no tracked files remain)
- `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run`: PASS
- `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run -t buildfs`: PASS

### Result
- single_web_asset_dir: `data/`
- firmware_build: success
- spiffs_image_build: success
- duplicate_folder_removed: true

### Artifacts
- [platformio config](platformio.ini)
- [web ui](data/index.html)
- [icon generator](tools/generate_web_icons.py)

### Notes
- Historical log entries still mention `data_spiffs/`; those references are now legacy path names only.

---

## 2026-04-11 02:13:08 +02:00 - Integrate separate touch UI at /touch.html and retire reference folder

### Planned Steps
- Add a dedicated touch UI entry point and asset subtree without changing the existing mobile UI.
- Convert the provided local reference prototype into a live `/api/live` dashboard and route it from SPIFFS.
- Bring the reference validation tools into the repo, retarget them to the integrated page, and run them.
- Build firmware and SPIFFS, then remove the temporary reference folder after successful validation.

### Changes Made
- `src/webinterface.cpp`: Added a reusable SPIFFS HTML streamer, registered `/touch.html`, and exposed `/touch/` as a separate static subtree.
- `data/touch.html`: Added the new touch-oriented entry page while leaving `/index.html` unchanged.
- `data/touch/css/app.css`: Added the 1024x600 touch layout styling based on the provided reference look and adapted for the integrated dashboard.
- `data/touch/js/app.js`: Replaced the reference demo script with a live `/api/live` dashboard using project telemetry groups for Home, Vehicle, ECU, SDU, and Cluster views.
- `data/touch/assets/*`: Added the touch-specific icon and hero assets copied from the provided reference.
- `tools/touch_ui_test_support.py`: Added a local mock `/api/live` server helper for deterministic touch UI validation outside the ESP32.
- `tools/touch_ui_local_server_test.py`: Added fetch-based validation for `/touch.html`, touch CSS/JS assets, and the mocked `/api/live` endpoint.
- `tools/touch_ui_screenshot_validate.py`: Added Playwright-based 1024x600 screenshot validation for the integrated touch UI.
- `refrence/`: Removed the temporary reference folder after the integrated page and validation tools passed.

### Automated Tests Run
- `python -m py_compile tools\touch_ui_test_support.py tools\touch_ui_local_server_test.py tools\touch_ui_screenshot_validate.py`: PASS
- `python tools\touch_ui_local_server_test.py`: PASS
- `python tools\touch_ui_screenshot_validate.py`: PASS
- `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run`: PASS
- `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run -t buildfs`: PASS

### Result
- touch_route: `/touch.html`
- existing_mobile_ui_preserved: true
- touch_asset_subtree: `/touch/`
- local_touch_validation: success
- screenshot_validation_1024x600: success
- firmware_build: success
- spiffs_image_build: success
- reference_folder_removed: true

### Artifacts
- [touch entry page](data/touch.html)
- [touch stylesheet](data/touch/css/app.css)
- [touch script](data/touch/js/app.js)
- [touch UI local test](tools/touch_ui_local_server_test.py)
- [touch UI screenshot validator](tools/touch_ui_screenshot_validate.py)
- [touch screenshot](archives/touch_ui_1024x600/layout_1024x600.png)

### Notes
- The local validation scripts serve `data/` with a mocked `/api/live` response so the integrated touch UI can be tested on the PC without flashing hardware.

---

## 2026-04-11 12:12:13 +02:00 - Add 1-second fullscreen intro splash to touch UI and optimize asset for SPIFFS

### Planned Steps
- Show the provided intro image full-screen for 1 second on `/touch.html` load.
- Keep the splash isolated to the touch UI so the existing mobile UI remains unchanged.
- Validate the touch UI locally and confirm the SPIFFS image still builds.
- If the raw image is too large for SPIFFS, preserve the original source and generate an optimized deployed asset.

### Changes Made
- `data/touch.html`: Added an intro splash overlay and preloaded the deployed intro image for immediate display on page open.
- `data/touch/css/app.css`: Added full-screen splash overlay styling and hide transition behavior.
- `data/touch/js/app.js`: Added a 1-second timer that dismisses the splash after page load and removes the overlay node.
- `img/touch-source/intro.png`: Preserved the original provided intro source image outside the SPIFFS payload.
- `img/touch-source/front.png`: Preserved the additional large touch source image outside the SPIFFS payload.
- `img/touch-source/vent.png`: Preserved the additional large touch source image outside the SPIFFS payload.
- `data/touch/img/intro.jpg`: Added an optimized 1024x600 deployed intro image for the touch splash so SPIFFS packaging succeeds.

### Automated Tests Run
- `python tools\touch_ui_local_server_test.py`: PASS
- `python tools\touch_ui_screenshot_validate.py --screenshot artifacts\touch_ui_1024x600\layout_1024x600_intro.png`: PASS
- `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run -t buildfs`: PASS

### Result
- touch_intro_splash: enabled
- splash_duration_ms: 1000
- deployed_intro_asset: `data/touch/img/intro.jpg`
- deployed_intro_size_bytes: 67097
- original_intro_preserved: true
- local_touch_validation: success
- spiffs_image_build: success

### Artifacts
- [touch entry page](data/touch.html)
- [touch stylesheet](data/touch/css/app.css)
- [touch script](data/touch/js/app.js)
- [deployed intro image](data/touch/img/intro.jpg)
- [source intro image](img/touch-source/intro.png)
- [touch screenshot with intro change](artifacts/touch_ui_1024x600/layout_1024x600_intro.png)

### Notes
- The initial raw intro image plus the other large touch images exceeded the SPIFFS partition size, so the originals were preserved under `img/touch-source/` and a device-sized deployed asset was generated for runtime use.

---

## 2026-04-11 12:20:16 +02:00 - Fix intro validation timing and keep splash over UI until initial ready state

### Planned Steps
- Investigate why the existing screenshot artifact did not show the intro splash.
- Keep the splash in front until the first live-data render and window load complete, while still enforcing the 1-second minimum duration.
- Extend the screenshot validator so it can capture both intro and ready states explicitly.
- Re-run local validation and confirm the SPIFFS image still builds.

### Changes Made
- `data/touch/js/app.js`: Replaced the fixed splash-hide timer with a gated hide flow that waits for minimum elapsed time, first live-data completion, and window load before dismissing the intro.
- `tools/touch_ui_screenshot_validate.py`: Added `--phase intro|ready` support so the validator can capture and verify either the visible splash state or the ready UI state.

### Automated Tests Run
- `python -m py_compile tools\touch_ui_screenshot_validate.py`: PASS
- `python tools\touch_ui_local_server_test.py`: PASS
- `python tools\touch_ui_screenshot_validate.py --phase intro --screenshot artifacts\touch_ui_1024x600\intro_1024x600.png`: PASS
- `python tools\touch_ui_screenshot_validate.py --phase ready --screenshot artifacts\touch_ui_1024x600\ready_1024x600.png`: PASS
- `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run -t buildfs`: PASS

### Result
- intro_validator_phase: available
- ready_validator_phase: available
- intro_screenshot_proves_visibility: true
- splash_hidden_until_initial_ready: true
- spiffs_image_build: success

### Artifacts
- [touch script](data/touch/js/app.js)
- [touch screenshot validator](tools/touch_ui_screenshot_validate.py)
- [intro splash screenshot](artifacts/touch_ui_1024x600/intro_1024x600.png)
- [ready-state screenshot](artifacts/touch_ui_1024x600/ready_1024x600.png)

### Notes
- The previous validator always waited long enough for the splash to disappear, so its screenshot could not prove the intro overlay was working.

---

## 2026-04-11 15:52:47 +02:00 - Replace touch intro image splash with intro MP4

### Planned Steps
- Swap the touch intro overlay from the deployed still image to the provided `intro.mp4`.
- Keep the splash fullscreen and in front of the UI until the page is ready and the intro media is complete.
- Update validation so intro-phase screenshots assert the presence of the video-based splash.
- Re-run local touch validation and SPIFFS filesystem packaging.

### Changes Made
- `data/touch.html`: Replaced the intro splash media from the preloaded image to a preloaded `intro.mp4` video with the existing intro JPG as poster fallback.
- `data/touch/css/app.css`: Switched the splash media selector from image to video while preserving fullscreen stretch behavior.
- `data/touch/js/app.js`: Added intro-video completion gating, autoplay retry on load, and a safety timeout so the UI can still recover if video playback fails.
- `tools/touch_ui_screenshot_validate.py`: Updated intro-phase checks to validate the presence and readiness of the intro video instead of the previous image element.

### Automated Tests Run
- `python -m py_compile tools\touch_ui_screenshot_validate.py`: PASS
- `python tools\touch_ui_local_server_test.py`: PASS
- `python tools\touch_ui_screenshot_validate.py --phase intro --screenshot artifacts\touch_ui_1024x600\intro_video_1024x600.png`: PASS
- `python tools\touch_ui_screenshot_validate.py --phase ready --screenshot artifacts\touch_ui_1024x600\ready_video_1024x600.png`: PASS
- `C:\Users\Win11 Pro\.platformio\penv\Scripts\platformio.exe run -t buildfs`: PASS

### Result
- touch_intro_media: `data/touch/img/intro.mp4`
- intro_video_size_bytes: 735490
- intro_video_phase_validation: success
- ready_phase_validation: success
- spiffs_image_build: success

### Artifacts
- [touch entry page](data/touch.html)
- [touch stylesheet](data/touch/css/app.css)
- [touch script](data/touch/js/app.js)
- [touch screenshot validator](tools/touch_ui_screenshot_validate.py)
- [intro video splash screenshot](artifacts/touch_ui_1024x600/intro_video_1024x600.png)
- [ready-state screenshot after video intro](artifacts/touch_ui_1024x600/ready_video_1024x600.png)

### Notes
- The local server on port `8080` was restarted after the change so browser testing can pick up the MP4-based splash instead of the previous image-based version.
