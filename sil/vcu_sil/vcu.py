"""ctypes wrapper around build/vcu_sil.dll (unchanged application components + shim).

A VcuSil instance runs one component on the application level:
    "cluster"  updateCluster()   Kombi calculation (src/cluster_calc.cpp)
    "app"      appStep(now)      cluster + ECU overrides + fan logic (src/vcu_app.cpp)
Inputs and outputs are parameters of the signal table, set and read by name.

Each instance loads its own copy of the library, so static state inside the firmware
(e.g. acceleration timing, fuel counter) starts fresh for every test.
"""

from __future__ import annotations

import ctypes
import shutil
import sys
import tempfile
import uuid
from pathlib import Path

SIL = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SIL))

import build as _build  # noqa: E402

COMPONENTS = {"cluster": 1, "app": 2}


class SignalError(KeyError):
    pass


class VcuSil:
    """One application component, driven by the simulation clock (6 ms per step)."""

    def __init__(self, component: str = "cluster") -> None:
        if component not in COMPONENTS:
            raise ValueError(f"unknown component {component!r}, choose {sorted(COMPONENTS)}")
        self.component = component
        self._component_id = COMPONENTS[component]
        library = _build.build()
        self._dir = Path(tempfile.mkdtemp(prefix="vcu_sil_"))
        copy = self._dir / f"vcu_sil_{uuid.uuid4().hex[:8]}{library.suffix}"
        shutil.copyfile(library, copy)
        self._lib = ctypes.CDLL(str(copy))
        lib = self._lib
        lib.sil_get.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_double)]
        lib.sil_get.restype = ctypes.c_int
        lib.sil_set.argtypes = [ctypes.c_char_p, ctypes.c_double]
        lib.sil_set.restype = ctypes.c_int
        lib.sil_signal_name.argtypes = [ctypes.c_int]
        lib.sil_signal_name.restype = ctypes.c_char_p
        lib.sil_now_ms.restype = ctypes.c_uint32
        lib.sil_cycle_ms.restype = ctypes.c_uint32
        lib.sil_step.argtypes = [ctypes.c_int]
        lib.sil_step.restype = ctypes.c_int
        lib.sil_override_engine_speed.argtypes = [ctypes.c_int, ctypes.c_double]
        self.cycle_s = lib.sil_cycle_ms() / 1000.0
        self._value = ctypes.c_double()

    # ------------------------------------------------------------ parameters
    def signals(self) -> list[str]:
        return [self._lib.sil_signal_name(i).decode() for i in range(self._lib.sil_signal_count())]

    def get(self, name: str) -> float:
        if self._lib.sil_get(name.encode(), ctypes.byref(self._value)) != 0:
            raise SignalError(name)
        return self._value.value

    def set(self, name: str, value: float) -> None:
        if self._lib.sil_set(name.encode(), float(value)) != 0:
            raise SignalError(name)

    def set_many(self, values: dict[str, float]) -> None:
        for name, value in values.items():
            self.set(name, value)

    # ------------------------------------------------------------ time
    @property
    def now_s(self) -> float:
        return self._lib.sil_now_ms() / 1000.0

    def step(self, cycles: int = 1) -> None:
        for _ in range(cycles):
            self._lib.sil_step(self._component_id)

    def run_for(self, seconds: float) -> None:
        self.step(max(1, round(seconds / self.cycle_s)))

    # ------------------------------------------------------------ overrides (component "app")
    def override_engine_speed(self, rpm: float | None) -> None:
        self._lib.sil_override_engine_speed(0 if rpm is None else 1, 0.0 if rpm is None else rpm)
