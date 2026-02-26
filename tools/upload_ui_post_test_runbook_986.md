# Upload UI + POST Test Runbook

This is the single canonical runbook for ESP32 upload UI tests.

## Implementation Log Usage

Before and after each task in this runbook, update [`IMPLEMENTATION_LOG.md`](../IMPLEMENTATION_LOG.md).

Required flow:
- Create a new timestamped entry title: `YYYY-MM-DD HH:MM:SS +/-TZ - Task Title`.
- Fill `Planned Steps` before changing files.
- Before any code/doc change, create a stage snapshot and record `stage_id` + `stage_path` in the same log entry.
- After implementation, fill `Changes Made`, `Automated Tests Run`, `Result`, and `Artifacts`.
- Run a validation test after each implementation step, then record that command/result immediately.
- Put only blockers/assumptions in `Notes` (do not place metrics or artifact lists in `Notes`).
- If no test is run, explicitly state why in `Automated Tests Run` or `Notes`.

## Feature Testability Rule

For every new feature request, define and implement a test automation path before closing the task.

Required for each feature:
- Add or update automation in `<UPLOAD_UI_TESTER>` so the feature can be verified non-interactively.
- Add a dedicated serial test command in firmware with predictable naming: `test_<feature_name>`.
- Ensure `help` output documents the new serial command.
- Run the serial command at least once during validation and record command + outcome.

Serial command example:

```text
test_queue_pipeline
```

## Self-Test Placement Convention

For readability, keep all firmware self-test function implementations at the end of each source file.

Required format:
- Add a dedicated section divider before self-tests:
  - `// ====================================================================================================`
  - `// Self Tests`
  - `// ====================================================================================================`
- Place all `run_*selftest` implementations under that divider.

## UI Layout Convention

For ECU page controls, keep tile widths consistent:
- Slider visual controls must be `2x1` (span full row across 2 columns).
- Boolean controls must be `1x1`.
- Value tiles must be `1x1`.

Implementation expectation:
- Encode this in CSS classes (`.slider-tile`, `.control-tile`, `.value-tile`) so it remains enforced for future edits.

## Stage Archiving Workflow

Use `tools/stage_archive.ps1` to keep restore points before code changes.

Mandatory gate:
- Do not edit code/docs until a snapshot is created for the exact files you are about to modify.
- Record `SNAPSHOT_OK stage=<stage_id>` and `stage_path=...` in `IMPLEMENTATION_LOG.md` before proceeding.

Git tracking rule:
- Keep all generated stage archive contents out of git at all times.
- Enforce root-level ignore for `archives/` in `.gitignore`.
- Only placeholder keep-files are allowed (`archives/.gitkeep`, `archives/stages/.gitkeep`).

### Create a snapshot

```powershell
powershell -ExecutionPolicy Bypass -File tools\stage_archive.ps1 `
  -Action snapshot `
  -Label "before_<change_name>" `
  -Paths src/dev/upload_ui_test_v5 src/dev/sd_http_upload_ui_test_v5.cpp tools/upload_ui_tester_v5.py
```

If `-Paths` parsing fails in your shell, use this robust form:

```powershell
powershell -ExecutionPolicy Bypass -Command "& { .\tools\stage_archive.ps1 -Action snapshot -Label 'before_<change_name>' -Paths @('path1','path2','path3') }"
```

Output includes:
- stage id
- stage path under `archives/stages/<timestamp>_<label>`

Each stage stores:
- copied files (same relative paths as repo)
- `manifest.json` with timestamp + git head + git status

### List snapshots

```powershell
powershell -ExecutionPolicy Bypass -File tools\stage_archive.ps1 -Action list
```

### Restore a snapshot

```powershell
powershell -ExecutionPolicy Bypass -File tools\stage_archive.ps1 -Action restore -Stage "<stage_id>"
```

Dry run:

```powershell
powershell -ExecutionPolicy Bypass -File tools\stage_archive.ps1 -Action restore -Stage "<stage_id>" -DryRun
```

## Version References

Use these variables in commands below:

- `<UPLOAD_UI_ENV>`: PlatformIO upload UI env (example: `sd_http_upload_ui_test_v5`)
- `<UPLOAD_UI_TESTER>`: tester script path (example: `tools/upload_ui_tester_v5.py`)
- `<RUN_PREFIX>`: output prefix (example: `upload_ui_v5_run`)

## Scope

- Main target: `<UPLOAD_UI_ENV>`
- Baseline target: `sd_http_post_speed_test`
- Mock upload server example: `http://192.168.0.37:8000/edit`

## Required Config

Set these files before running:

1. `include/dev/sd_http_upload_ui_secrets.h`
- `SD_HTTP_UPLOAD_TEST_SSID`
- `SD_HTTP_UPLOAD_TEST_PASSWORD`
- `SD_HTTP_UPLOAD_TEST_URL`

2. `include/dev/sd_http_post_speed_secrets.h`
- `SD_HTTP_TEST_SSID`
- `SD_HTTP_TEST_PASSWORD`
- `SD_HTTP_POST_TEST_URL`

Both URLs should point to the same server endpoint during comparison.

## Tools Used

- Firmware build/flash: PlatformIO
- UI test runner: `<UPLOAD_UI_TESTER>`
- Speed baseline serial runner: `tools/serial-run.ps1`

## Execution Procedure

### A) Flash and verify UI firmware

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e <UPLOAD_UI_ENV> -t upload
```

Find ESP IP via serial boot (`Wi-Fi IP: ...`) or reuse known DHCP lease.

### B) Webservice availability check (fast health check)

```powershell
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --check-only --request-timeout 10
```

Expected: `check_ok: ...`

### C) Reset uploaded-state before a new run (recommended)

```powershell
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --reset-upload-state --request-timeout 10
```

Expected: `reset_upload_state_ok: True state_file_removed: True`

### D) Execute one upload run (UI + upload in parallel)

```powershell
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --start --request-timeout 10 --timeout 180 --prefix <RUN_PREFIX>
```

Artifacts:
- JSONL samples: `logs/<RUN_PREFIX>_*.jsonl`
- Summary: `logs/<RUN_PREFIX>_*.summary.txt`

### E) Baseline run with original speed test

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -t upload -e sd_http_post_speed_test
powershell -ExecutionPolicy Bypass -File tools\serial-run.ps1 -EnvName sd_http_post_speed_test -NoFlash -TimeoutSec 240
```

Artifacts:
- Serial log: `logs/sd_http_post_speed_*.log`
- Summary: `logs/sd_http_post_speed_*.summary.txt`

## Feature Self-Checks (HTTP + Serial)

Uploaded-state bookkeeping:

```powershell
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --uploaded-state-check --request-timeout 10
```

Serial command:

```text
test_uploaded_state_bookkeeping
```

SD recovery:

```powershell
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --recover-sd-check --request-timeout 10
```

Serial command:

```text
test_recover_sd
```

Reachability probe + cached resolution:

```powershell
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --reachability-probe-check --request-timeout 10
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --reachability-cache-check --request-timeout 10
```

Serial commands:

```text
test_reachability_probe
test_reachability_cache
```

## Pass/Fail Thresholds

Apply these thresholds to each `<UPLOAD_UI_ENV>` run.

### 1) Functional pass (mandatory)

All must be true:

- `result_state: DONE` in `logs/<RUN_PREFIX>_*.summary.txt`
- `error_code: 0`
- `sent_bytes == total_bytes`
- Serial log contains end line similar to:
  - `UPLOAD_LOG tag=end ... state=DONE ... http=201 ... error=0`

If any fail: run is **FAIL**.

### 2) Throughput pass (network-dependent but required)

- `avg_upload_mb_s >= 0.25`

Note: recent validated runs were around `0.29-0.37 MB/s` on this setup.

### 3) UI responsiveness pass while upload is active

Measure with `/status` polling during upload. Pass criteria:

- success rate `>= 99%` (timeouts/failures <= 1%)
- p95 latency `<= 100 ms`
- max latency `<= 1500 ms`

If p95 exceeds 100 ms consistently or failures exceed 1%, mark **DEGRADED**.

### 4) Stability over repeated runs

Run at least 3 consecutive runs:

- minimum 3/3 functional pass (or 2/3 only acceptable with explicit note)
- no unhandled firmware reset/panic

## Recommended 3-run sequence

```powershell
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --start --request-timeout 10 --timeout 180 --prefix upload_ui_r1
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --start --request-timeout 10 --timeout 180 --prefix upload_ui_r2
python <UPLOAD_UI_TESTER> --base-url http://<ESP_IP> --start --request-timeout 10 --timeout 180 --prefix upload_ui_r3
```

## Known Failure Signatures

1. Early connection reset (server rejects request format)
- Serial: `write(): ... errno: 104, "Connection reset by peer"`
- Usually indicates wrong POST wire format.

2. Poller misses terminal state
- UI falls back to `IDLE` too quickly.
- Firmware now keeps terminal state briefly; still verify via serial `UPLOAD_LOG tag=end`.

3. Python connectivity failures with proxy env vars
- Environment may set `HTTP_PROXY` to invalid localhost.
- `<UPLOAD_UI_TESTER>` should bypass proxies explicitly.

## Reporting Template

For each run, report:

1. Firmware env + git ref
2. Target URL
3. `result_state`, `error_code`, `avg_upload_mb_s`
4. `sent_bytes/total_bytes`
5. UI responsiveness stats (`success`, `p95`, `max`)
6. Paths to summary/log files
