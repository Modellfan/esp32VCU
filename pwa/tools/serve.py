#!/usr/bin/env python3
"""Serve the PWA like the VCU does, plus a mock /api with fault injection.

Static files behave like the ESP32 WebServer + SPIFFS:
  `x.js` missing but `x.js.gz` present -> serve the .gz with Content-Encoding gzip,
  directory requests -> index.html.

Mock API: /api/live, /api/signals, /api/set, /api/set-ecu, /api/set-cluster (dynamic values from
pwa/tools/mock_api.py). Test control endpoints (not on the VCU):
  GET /__test/fault?mode=<mode>&delay_ms=<n>   modes: ok, down, slow, http500, badjson,
        array, empty, garbage, stale_set, static_down, all_down
  GET /__test/stats    request counters, max concurrent /api/live, recorded sets
  GET /__test/reset    reset faults and counters

Usage: python pwa/tools/serve.py [--root pwa/dist] [--port 8090]
"""

from __future__ import annotations

import argparse
import json
import mimetypes
import socket
import sys
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, unquote, urlsplit

PWA_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(Path(__file__).resolve().parent))

import mock_api as mock  # noqa: E402

MIME = {
    ".html": "text/html", ".css": "text/css", ".js": "application/javascript",
    ".json": "application/json", ".svg": "image/svg+xml", ".png": "image/png",
    ".webp": "image/webp", ".jpg": "image/jpeg", ".ico": "image/x-icon",
}

LOCK = threading.Lock()
FAULT = {"mode": "ok", "delay_ms": 0}
STATS = {"live": 0, "set": 0, "static": 0, "concurrent_live": 0, "max_concurrent_live": 0,
         "sets": [], "live_times": []}
STALE_VALUES: dict[str, object] = {}


def reset() -> None:
    with LOCK:
        FAULT.update(mode="ok", delay_ms=0)
        STATS.update(live=0, set=0, static=0, concurrent_live=0, max_concurrent_live=0, sets=[], live_times=[])
        STALE_VALUES.clear()
    with mock.MOCK_LOCK:
        mock.MOCK_STATE.clear()
        mock.MOCK_STATE.update(mock.MOCK_LIVE)
        mock.MOCK_STATE.setdefault("coolant_fan_level", 0)
        mock.MOCK_STATE.setdefault("digital_pot_pct", 0)
        mock.MOCK_START_MONOTONIC = time.monotonic()
        mock.update_dynamic_mock_state()


GARBAGE = ["", "abc", None, True, -1e308, 1e308, "NaN", "Infinity", -999999, [1, 2], {"x": 1}, "<b>x</b>"]


def garbage_payload(snapshot: dict) -> dict:
    out = {}
    for index, key in enumerate(snapshot):
        out[key] = GARBAGE[index % len(GARBAGE)]
    return out


class Handler(BaseHTTPRequestHandler):
    server_version = "VCU-mock/1.0"
    root: Path = PWA_ROOT / "dist"

    def log_message(self, *args) -> None:  # quiet
        return

    def _send(self, status: int, body: bytes, ctype: str, extra: dict | None = None) -> None:
        extra = dict(extra or {})
        self.send_response(status)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", extra.pop("Cache-Control", "no-store"))
        for key, value in extra.items():
            self.send_header(key, value)
        self.end_headers()
        if self.command != "HEAD":
            self.wfile.write(body)

    def _drop(self) -> None:
        """Simulate an unreachable VCU: close the socket without a response."""
        self.close_connection = True
        try:
            self.connection.shutdown(socket.SHUT_RDWR)
        except OSError:
            pass

    def _json(self, payload: object, status: int = 200) -> None:
        self._send(status, json.dumps(payload).encode("utf-8"), "application/json")

    def do_HEAD(self) -> None:
        self.do_GET()

    def do_GET(self) -> None:
        url = urlsplit(self.path)
        path = unquote(url.path)
        query = parse_qs(url.query)
        if path.startswith("/__test/"):
            return self._test_control(path, query)
        if path.startswith("/api/"):
            return self._api(path, query)
        with LOCK:
            STATS["static"] += 1
            mode = FAULT["mode"]
        if mode in ("static_down", "all_down"):
            return self._drop()
        return self._static(path, versioned="v" in query)

    def _test_control(self, path: str, query: dict) -> None:
        if path == "/__test/fault":
            with LOCK:
                FAULT["mode"] = query.get("mode", ["ok"])[0]
                FAULT["delay_ms"] = int(query.get("delay_ms", ["0"])[0])
            return self._json({"ok": True, "fault": FAULT})
        if path == "/__test/stats":
            with LOCK:
                return self._json(dict(STATS))
        if path == "/__test/reset":
            reset()
            return self._json({"ok": True})
        return self._json({"error": "unknown"}, 404)

    def _api(self, path: str, query: dict) -> None:
        with LOCK:
            mode = FAULT["mode"]
            delay = FAULT["delay_ms"]
        if mode in ("down", "all_down"):
            return self._drop()
        if mode == "slow":
            time.sleep((delay or 5000) / 1000)
        elif delay:
            time.sleep(delay / 1000)
        if mode == "http500":
            return self._send(500, b"internal error", "text/plain")
        if mode == "badjson":
            return self._send(200, b'{"rpm": 12,,', "application/json")
        if mode == "array":
            return self._send(200, b"[1,2,3]", "application/json")

        if path == "/api/live":
            return self._live(query, mode)
        if path == "/api/signals":
            return self._json({"timeout_ms": 500, "signals": mock.SIGNAL_CATALOG})
        if path in ("/api/set", "/api/set-ecu", "/api/set-cluster"):
            return self._set(path, query)
        return self._send(404, b"Not found", "text/plain")

    def _live(self, query: dict, mode: str) -> None:
        with LOCK:
            STATS["live"] += 1
            STATS["concurrent_live"] += 1
            STATS["max_concurrent_live"] = max(STATS["max_concurrent_live"], STATS["concurrent_live"])
            STATS["live_times"].append(round(time.monotonic(), 3))
            STATS["live_times"] = STATS["live_times"][-200:]
        try:
            with mock.MOCK_LOCK:
                mock.update_dynamic_mock_state()
                snapshot = {**mock.signal_defaults(), **mock.MOCK_STATE}
            if mode == "stale_set":
                with LOCK:
                    snapshot.update(STALE_VALUES)
            if "fields" in query and query["fields"][0]:
                wanted = [f for f in query["fields"][0].split(",") if f]
                snapshot = {f: snapshot[f] for f in wanted if f in snapshot}
            if mode == "empty":
                snapshot = {}
            elif mode == "garbage":
                snapshot = garbage_payload(snapshot)
            self._json(snapshot)
        finally:
            with LOCK:
                STATS["concurrent_live"] -= 1

    def _set(self, path: str, query: dict) -> None:
        limits = {"target_charge_pct": (50, 100), "ac_current_limit_a": (6, 32),
                  "digital_pot_pct": (0, 100), "coolant_fan_level": (0, 2)}
        with mock.MOCK_LOCK:
            for key, values in query.items():
                raw = values[0]
                with LOCK:
                    STATS["set"] += 1
                    STATS["sets"].append({"path": path, "key": key, "value": raw, "t": round(time.monotonic(), 3)})
                    if key not in STALE_VALUES:
                        STALE_VALUES[key] = mock.MOCK_STATE.get(key)
                try:
                    number = float(raw)
                except ValueError:
                    continue
                if key in limits:
                    low, high = limits[key]
                    mock.MOCK_STATE[key] = int(max(low, min(high, round(number))))
                else:
                    mock.MOCK_STATE[key] = number
            mock.update_dynamic_mock_state()
            snapshot = dict(mock.MOCK_STATE)
        self._json(snapshot)

    def _static(self, path: str, versioned: bool = False) -> None:
        rel = path.lstrip("/")
        if rel == "" or rel.endswith("/"):
            rel += "index.html"
        target = (self.root / rel).resolve()
        if self.root.resolve() not in target.parents and target != self.root.resolve():
            return self._send(403, b"forbidden", "text/plain")
        ctype = MIME.get(Path(rel).suffix, mimetypes.guess_type(rel)[0] or "application/octet-stream")
        shell = rel.endswith(".html") or rel.endswith("sw.js") or rel.endswith("manifest.json")
        # Same cache policy as the firmware (webinterface.cpp serveStaticFile).
        extra = {"Cache-Control": "no-cache" if shell else
                 "public, max-age=31536000, immutable" if versioned else "max-age=300"}
        if not target.is_file():
            gz = target.with_name(target.name + ".gz")
            if gz.is_file():
                target = gz
                extra["Content-Encoding"] = "gzip"
            else:
                return self._send(404, b"Not found", "text/plain")
        self._send(200, target.read_bytes(), ctype, extra)


def start(root: Path, host: str = "127.0.0.1", port: int = 0) -> tuple[ThreadingHTTPServer, str]:
    reset()
    Handler.root = root
    server = ThreadingHTTPServer((host, port), Handler)
    server.daemon_threads = True
    threading.Thread(target=server.serve_forever, daemon=True).start()
    return server, f"http://{host}:{server.server_address[1]}"


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--root", default=str(PWA_ROOT / "dist"))
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8090)
    args = parser.parse_args()
    server, base = start(Path(args.root), args.host, args.port)
    print(f"serving {args.root} at {base}/")
    try:
        threading.Event().wait()
    except KeyboardInterrupt:
        server.shutdown()


if __name__ == "__main__":
    main()
