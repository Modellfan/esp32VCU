#!/usr/bin/env python3
"""Build pwa/src into pwa/dist for the VCU SPIFFS partition.

* Text files (html/css/js/json/svg) are stored gzip-only (`x.js.gz`); the ESP32
  WebServer serves `x.js.gz` with Content-Encoding gzip when `x.js` is requested.
* sw.js gets the build version and the precache list.
* Checks: SPIFFS name length, allowed characters, every referenced asset exists,
  total size within budget. Any violation fails the build (exit code 1).
"""

from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import re
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
DIST = ROOT / "dist"

SPIFFS_PARTITION_BYTES = 0x180000          # default_8MB.csv
BUDGET_BYTES = 1_100_000                   # leave room for SPIFFS metadata and wear levelling
SPIFFS_MAX_NAME = 31                       # CONFIG_SPIFFS_OBJ_NAME_LEN 32 incl. terminator
TEXT_SUFFIXES = {".html", ".css", ".js", ".json", ".svg"}
NAME_PATTERN = re.compile(r"^/[a-z0-9._/-]+$")
REF_PATTERN = re.compile(r"""["'(]((?:a|i|e|css|js)/[A-Za-z0-9._/-]+\.[a-z0-9]+)["')]""")
EXCLUDE = {".DS_Store", "Thumbs.db"}


def source_files() -> list[Path]:
    return sorted(p for p in SRC.rglob("*") if p.is_file() and p.name not in EXCLUDE)


def check_references(files: list[Path]) -> list[str]:
    existing = {p.relative_to(SRC).as_posix() for p in files}
    errors = []
    for path in files:
        if path.suffix not in {".html", ".css", ".js", ".json"}:
            continue
        text = path.read_text(encoding="utf-8")
        for ref in REF_PATTERN.findall(text):
            if ref not in existing:
                errors.append(f"{path.relative_to(SRC).as_posix()}: missing asset '{ref}'")
        if path.name == "layout.json":
            layout = json.loads(text)
            base = layout.get("assetsBasePath", "")
            for image in layout.get("images", []):
                if not image.get("file"):
                    continue  # vector items (e.g. coolantPipePath) have no image
                ref = base + image["file"]
                if ref not in existing:
                    errors.append(f"e/layout.json: missing energy-flow image '{ref}'")
        if "/touch/" in text:
            errors.append(f"{path.relative_to(SRC).as_posix()}: legacy '/touch/' path left")
    return errors


def build(budget: int) -> dict:
    files = source_files()
    errors = check_references(files)

    precache = ["./"] + [p.relative_to(SRC).as_posix() for p in files if p.name != "sw.js"]
    digest = hashlib.sha256()
    for path in files:
        digest.update(path.relative_to(SRC).as_posix().encode())
        digest.update(path.read_bytes())
    version = digest.hexdigest()[:12]

    if DIST.exists():
        shutil.rmtree(DIST)
    DIST.mkdir(parents=True)

    report = {"version": version, "files": [], "total_bytes": 0}
    for path in files:
        rel = path.relative_to(SRC).as_posix()
        data = path.read_bytes()
        if path.name == "sw.js":
            text = data.decode("utf-8")
            text = re.sub(r'const VERSION = .*?/\* @build:version \*/',
                          f'const VERSION = "{version}"; /* @build:version */', text)
            text = re.sub(r'const PRECACHE = .*?/\* @build:precache \*/',
                          "const PRECACHE = " + json.dumps(precache) + "; /* @build:precache */", text)
            if version not in text:
                errors.append("sw.js: build placeholders not found")
            data = text.encode("utf-8")
        stored = rel
        if path.suffix in TEXT_SUFFIXES:
            data = gzip.compress(data, compresslevel=9, mtime=0)
            stored = rel + ".gz"
        spiffs_name = "/" + stored
        if len(spiffs_name) > SPIFFS_MAX_NAME:
            errors.append(f"{spiffs_name}: name longer than {SPIFFS_MAX_NAME} characters")
        if not NAME_PATTERN.match(spiffs_name):
            errors.append(f"{spiffs_name}: only a-z, 0-9, '.', '_', '-' and '/' allowed")
        target = DIST / stored
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)
        report["files"].append({"path": spiffs_name, "bytes": len(data)})
        report["total_bytes"] += len(data)

    report["budget_bytes"] = budget
    report["partition_bytes"] = SPIFFS_PARTITION_BYTES
    if report["total_bytes"] > budget:
        errors.append(f"total {report['total_bytes']} B exceeds budget {budget} B")
    report["errors"] = errors
    (ROOT / "build-report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    return report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--budget", type=int, default=BUDGET_BYTES)
    args = parser.parse_args()
    report = build(args.budget)
    largest = sorted(report["files"], key=lambda f: f["bytes"], reverse=True)[:5]
    print(f"version {report['version']}: {len(report['files'])} files, "
          f"{report['total_bytes']} B of budget {report['budget_bytes']} B "
          f"(partition {report['partition_bytes']} B)")
    for item in largest:
        print(f"  {item['bytes']:>8} B  {item['path']}")
    if report["errors"]:
        print("BUILD FAILED:")
        for error in report["errors"]:
            print("  - " + error)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
