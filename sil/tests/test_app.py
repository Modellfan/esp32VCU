"""Component tests of the application step appStep() (src/app/vcu_app.cpp).

appStep() runs the Kombi calculation and then applies the ECU overrides (VCU-DEC-005).
Application level only: inputs and outputs are signal-table parameters.
"""

from __future__ import annotations

import pytest

import oracle
from vcu_sil import VcuSil

FW_DRIVE = -1   # current firmware convention, see test_cluster.py TC-SIL-02


@pytest.fixture()
def app() -> VcuSil:
    return VcuSil("app")


def test_tc_sil_07_override_beats_calculation(app):
    """A UI override wins over the Kombi calculation; releasing it restores the calculation."""
    app.set_many({"sdu_udc": 348, "sdu_idc": 150, "sdu_seldir": FW_DRIVE})
    app.step()
    assert app.get("engine_speed_rpm") == oracle.needle("drive", 348, 150)
    app.override_engine_speed(3000)
    app.step(5)
    assert app.get("engine_speed_rpm") == 3000
    assert app.get("cluster_needle_position") == oracle.needle("drive", 348, 150)  # calculation keeps running
    app.override_engine_speed(None)
    app.step()
    assert app.get("engine_speed_rpm") == oracle.needle("drive", 348, 150)


def test_tc_sil_18_override_also_when_cluster_off(app):
    app.set_many({"cluster_activated": 0, "engine_speed_rpm": 1000})
    app.override_engine_speed(2500)
    app.step(3)
    assert app.get("engine_speed_rpm") == 2500
