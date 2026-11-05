"""ctypes wrapper around build/vcu_sil.dll (unchanged firmware code + shim).

Each VcuSil instance loads its own copy of the library, so the static state inside
the firmware (e.g. acceleration timing, fuel counter) starts fresh for every test.
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

BUS_VEHICLE = 1
BUS_DRIVE = 2


class SignalError(KeyError):
    pass


class VcuSil:
    """The VCU application step, driven by the simulation clock (6 ms per step)."""

    def __init__(self) -> None:
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
        lib.sil_mark_rx.argtypes = [ctypes.c_int, ctypes.c_uint32]
        lib.sil_override_engine_speed.argtypes = [ctypes.c_int, ctypes.c_double]
        lib.sil_encode_ecu.argtypes = [ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint8)]
        lib.sil_encode_ecu.restype = ctypes.c_int
        lib.sil_receive.argtypes = [ctypes.c_int, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint8), ctypes.c_int]
        lib.sil_receive.restype = ctypes.c_int
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
            self._lib.sil_step()

    def run_for(self, seconds: float) -> None:
        self.step(max(1, round(seconds / self.cycle_s)))

    # ------------------------------------------------------------ CAN
    def mark_rx(self, bus: int, can_id: int) -> None:
        self._lib.sil_mark_rx(bus, can_id)

    def encode_ecu(self, can_id: int) -> bytes:
        buf = (ctypes.c_uint8 * 8)()
        length = self._lib.sil_encode_ecu(can_id, buf)
        if length < 0:
            raise ValueError(f"0x{can_id:X} is not an ECU emulation frame")
        return bytes(buf)[:length]

    def receive(self, bus: int, can_id: int, data: bytes) -> bool:
        buf = (ctypes.c_uint8 * 8)(*data[:8])
        return self._lib.sil_receive(bus, can_id, buf, len(data)) == 1

    # ------------------------------------------------------------ overrides
    def override_engine_speed(self, rpm: float | None) -> None:
        self._lib.sil_override_engine_speed(0 if rpm is None else 1, 0.0 if rpm is None else rpm)
