from __future__ import annotations

import sys
from pathlib import Path

import pytest

SIL = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SIL))

from vcu_sil import VcuSil  # noqa: E402


@pytest.fixture()
def vcu() -> VcuSil:
    """Fresh firmware instance (own library copy, static state reset)."""
    return VcuSil()
