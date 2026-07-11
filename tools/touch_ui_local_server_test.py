#!/usr/bin/env python3
"""Serve the integrated touch UI locally and verify its primary resources."""

from __future__ import annotations

import argparse
import json
import time
import urllib.request

from touch_ui_test_support import WEB_ROOT, find_free_port, start_server


def fetch_text(url: str, timeout_s: float = 3.0) -> str:
    with urllib.request.urlopen(url, timeout=timeout_s) as response:
        return response.read().decode("utf-8", errors="replace")


def fetch_json(url: str, timeout_s: float = 3.0) -> dict:
    with urllib.request.urlopen(url, timeout=timeout_s) as response:
        return json.loads(response.read().decode("utf-8", errors="replace"))


def run_fetch_checks(base_url: str) -> dict:
    checks = {
        "touch_html": {"path": "/touch.html", "contains": "Touchscreen dashboard"},
        "touch_css": {"path": "/touch/css/app.css", "contains": ".hero-metrics"},
        "touch_js": {"path": "/touch/js/app.js", "contains": "const sections"},
        "touch_home_battery_js": {"path": "/touch/js/home-battery.js", "contains": "window.TouchHomeBattery"},
        "touch_battery_modal_js": {"path": "/touch/js/bset.js", "contains": "window.TouchBatterySettingsModal"},
        "touch_coolant_stage_js": {"path": "/touch/js/coolant-stage.js", "contains": "window.TouchCoolantStage"},
        "touch_api": {"path": "/api/live", "contains": "\"vehicle_speed_510\""},
    }

    report = {"base_url": base_url, "ok": True, "checks": {}}
    for name, spec in checks.items():
        url = base_url + spec["path"]
        body = fetch_text(url)
        matched = spec["contains"] in body
        report["checks"][name] = {
            "url": url,
            "contains": spec["contains"],
            "matched": matched,
            "bytes": len(body.encode("utf-8")),
        }
        if not matched:
            report["ok"] = False

    first_live = fetch_json(base_url + "/api/live")
    time.sleep(0.35)
    second_live = fetch_json(base_url + "/api/live")
    dynamic_keys = [
        "sdu_udc",
        "sdu_idc",
        "sdu_speed",
        "sdu_pot",
        "sdu_tmpm",
    ]
    changed = {key: first_live.get(key) != second_live.get(key) for key in dynamic_keys}
    dynamic_ok = all(changed.values())
    report["checks"]["touch_dynamic_api"] = {
        "url": base_url + "/api/live",
        "keys": dynamic_keys,
        "changed": changed,
        "matched": dynamic_ok,
    }
    if not dynamic_ok:
        report["ok"] = False
    return report


def main() -> int:
    parser = argparse.ArgumentParser(description="Serve and fetch-test the integrated touch UI")
    parser.add_argument("--host", default="127.0.0.1", help="Bind host (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=0, help="Bind port (default: auto)")
    parser.add_argument("--duration", type=float, default=0.0, help="Seconds to keep server alive after checks")
    args = parser.parse_args()

    if not WEB_ROOT.exists():
        raise FileNotFoundError(f"Missing web root: {WEB_ROOT}")

    port = args.port or find_free_port(args.host)
    server, thread = start_server(args.host, port)
    base_url = f"http://{args.host}:{port}"

    try:
        report = run_fetch_checks(base_url)
        print(json.dumps(report, indent=2))
        if args.duration > 0:
            time.sleep(args.duration)
        return 0 if report["ok"] else 2
    finally:
        server.shutdown()
        server.server_close()
        thread.join(timeout=1.0)


if __name__ == "__main__":
    raise SystemExit(main())
