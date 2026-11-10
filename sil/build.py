#!/usr/bin/env python3
"""Build the SIL library: unchanged application components + stubs + shim -> build/vcu_sil.dll.

Compiler: Clang from the PyPI package `ziglang` (python -m pip install --user ziglang),
so no system-wide compiler or Visual Studio is needed.
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

SIL = Path(__file__).resolve().parent
REPO = SIL.parent
BUILD = SIL / "build"
LIBRARY = BUILD / ("vcu_sil.dll" if sys.platform == "win32" else "libvcu_sil.so")

# Application components under test (not modified for SIL). No CAN codecs; of the drivers only
# the fan automatic (GPIO stubbed). Signal groups of modules left out are in sil/shim/params_storage.cpp.
FIRMWARE_SOURCES = [
    "src/app/cluster_calc.cpp",
    "src/drivers/coolant_fan_control.cpp",
    "src/drivers/canmanager_api.cpp",
    "src/services/signal_table.cpp",
    "src/services/params_lock.cpp",
]
SRC_LAYERS = ["app", "comms", "drivers", "services"]
SHIM_SOURCES = ["sil/shim/params_storage.cpp", "sil/shim/sil_api.cpp"]


def sources() -> list[Path]:
    return [REPO / s for s in FIRMWARE_SOURCES + SHIM_SOURCES]


def up_to_date() -> bool:
    if not LIBRARY.exists():
        return False
    newest = max(p.stat().st_mtime for p in
                 sources() + list((REPO / "src").rglob("*.h")) + list((SIL / "stubs").rglob("*.h")))
    return LIBRARY.stat().st_mtime >= newest


def build(force: bool = False) -> Path:
    if not force and up_to_date():
        return LIBRARY
    BUILD.mkdir(exist_ok=True)
    cmd = [
        sys.executable, "-m", "ziglang", "c++",
        "-shared", "-std=gnu++17", "-O1", "-g0",
        "-Wall", "-Wno-unused-function", "-Wno-invalid-offsetof", "-DVCU_LOG_ENABLED=0",
        "-I", str(SIL / "stubs"), "-I", str(REPO / "src"),
        *[a for d in SRC_LAYERS for a in ("-I", str(REPO / "src" / d))],
        "-o", str(LIBRARY),
        *[str(p) for p in sources()],
    ]
    if sys.platform != "win32":
        cmd.insert(5, "-fPIC")
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError("SIL build failed:\n" + result.stderr)
    if result.stderr.strip():
        print(result.stderr)
    return LIBRARY


if __name__ == "__main__":
    print(build(force="--force" in sys.argv))
