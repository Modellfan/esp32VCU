"""Component tests of the Kombi calculation updateCluster() (src/app/cluster_calc.cpp).

Application level only: inputs and outputs are signal-table parameters, no CAN layer.
Test ids TC-SIL-xx are listed in sil/README.md. Run: python -m pytest sil/tests -q
"""

from __future__ import annotations

import pytest

import oracle
from vcu_sil import DriverProfile, EnvironmentModel, Phase, Simulation, VcuSil

# Direction values as the current firmware interprets sdu_seldir (see TC-SIL-02):
# > 0 is treated as reverse, < 0 as drive. OpenInverter defines -1 reverse / 1 forward.
FW_DRIVE = -1
FW_NEUTRAL = 0
FW_REVERSE = 1

OI_FORWARD = 1    # OpenInverter / touch UI enum
OI_REVERSE = -1


@pytest.fixture()
def vcu() -> VcuSil:
    """Fresh instance of the component under test."""
    return VcuSil("cluster")


def apply(vcu: VcuSil, cycles: int = 1, **values) -> None:
    vcu.set_many(values)
    vcu.step(cycles)


# --------------------------------------------------------------------------- needle mapping


def test_tc_sil_01_neutral_fixed_needle(vcu):
    apply(vcu, sdu_udc=348, sdu_idc=150, sdu_seldir=FW_NEUTRAL)
    assert vcu.get("cluster_needle_position") == oracle.needle("neutral")
    assert vcu.get("engine_speed_rpm") == oracle.needle("neutral")


def test_tc_sil_02a_direction_characterisation(vcu):
    """Pins the current behaviour so a change of the direction logic is noticed."""
    apply(vcu, sdu_udc=348, sdu_idc=150, sdu_seldir=1)
    assert vcu.get("cluster_needle_position") == oracle.NEEDLE_REVERSE
    apply(vcu, sdu_udc=348, sdu_idc=150, sdu_seldir=-1)
    assert vcu.get("cluster_needle_position") == oracle.needle("drive", 348, 150)


@pytest.mark.xfail(strict=True, reason="BEFUND: Firmware wertet sdu_seldir=+1 (OpenInverter 'Forward') als "
                                      "Rückwärtsgang. Klären: Richtung im Fahrzeug invertiert oder Fehler in "
                                      "needleFromPercent().")
def test_tc_sil_02b_openinverter_forward_shows_power(vcu):
    apply(vcu, sdu_udc=348, sdu_idc=150, sdu_seldir=OI_FORWARD)
    assert vcu.get("cluster_needle_position") == oracle.needle("drive", 348, 150)


@pytest.mark.xfail(strict=True, reason="BEFUND wie TC-SIL-02b: OpenInverter 'Reverse' (-1) zeigt Leistung "
                                      "statt der festen Rückwärts-Stellung.")
def test_tc_sil_02c_openinverter_reverse_fixed_needle(vcu):
    apply(vcu, sdu_udc=348, sdu_idc=150, sdu_seldir=OI_REVERSE)
    assert vcu.get("cluster_needle_position") == oracle.NEEDLE_REVERSE


@pytest.mark.parametrize("udc,idc", [
    (348, 0), (348, 10), (348, 18), (348, 30), (348, 75), (348, 150), (348, 225), (348, 300), (348, 450),
    (300, 300), (380, 200), (348, -3), (348, -10), (348, -25), (348, -50), (348, -100), (348, -200),
])
def test_tc_sil_03_power_scale(vcu, udc, idc):
    """Drive: zero mark 4100 to full scale 7950 (+lag); regeneration down to 1600; ±5 % dead band."""
    apply(vcu, sdu_udc=udc, sdu_idc=idc, sdu_seldir=FW_DRIVE)
    assert vcu.get("cluster_power_percent_max") == pytest.approx(oracle.power_fraction(udc, idc), abs=1e-3)
    assert abs(vcu.get("cluster_needle_position") - oracle.needle("drive", udc, idc)) <= 1


def test_tc_sil_04_needle_monotonic_over_power(vcu):
    """A smooth power ramp never makes the needle step backwards (drive side)."""
    last = None
    for idc in range(0, 301, 5):
        apply(vcu, sdu_udc=348, sdu_idc=idc, sdu_seldir=FW_DRIVE)
        value = vcu.get("cluster_needle_position")
        if last is not None:
            assert value >= last, (idc, last, value)
        last = value


# --------------------------------------------------------------------------- temperature gauge and warning


@pytest.mark.parametrize("motor_c", [0, 20, 90, 143, 160, 200, 255])
def test_tc_sil_05_coolant_gauge_shows_motor_temperature(vcu, motor_c):
    apply(vcu, sdu_tmpm=motor_c, sdu_tmphs=20)
    assert vcu.get("motor2_coolant_temperature") == pytest.approx(oracle.coolant_gauge(motor_c), abs=0.01)


@pytest.mark.parametrize("heatsink_c,motor_c", [
    (20, 20), (79, 159), (80, 20), (20, 160), (95, 170), (79, 160), (80, 159),
])
def test_tc_sil_06_coolant_warning_thresholds(vcu, heatsink_c, motor_c):
    apply(vcu, sdu_tmphs=heatsink_c, sdu_tmpm=motor_c)
    assert bool(vcu.get("motor2_coolant_temp_not_ok")) == oracle.coolant_warning(heatsink_c, motor_c)
    assert bool(vcu.get("cluster_heatsink_temp_critical")) == (heatsink_c >= oracle.HEATSINK_CRITICAL_C)
    assert bool(vcu.get("cluster_motor_temp_critical")) == (motor_c >= oracle.MOTOR_CRITICAL_C)


# --------------------------------------------------------------------------- activation switch


def test_tc_sil_08_cluster_switched_off_keeps_ecu_values(vcu):
    apply(vcu, cluster_activated=0, engine_speed_rpm=1234, motor2_coolant_temperature=55,
          sdu_udc=348, sdu_idc=300, sdu_seldir=FW_DRIVE, sdu_tmpm=150)
    vcu.step(50)
    assert vcu.get("engine_speed_rpm") == 1234
    assert vcu.get("motor2_coolant_temperature") == 55
    assert vcu.get("mo5_verbrauch_ul") == 0


# --------------------------------------------------------------------------- closed loop with the environment model


def drive_model(*phases: Phase, **kwargs) -> EnvironmentModel:
    # The model speaks OpenInverter; direction_sign=-1 matches the current firmware convention.
    return EnvironmentModel(DriverProfile(list(phases)), direction_sign=-1, **kwargs)


def true_run_time(trace, start_kmh: float, target_kmh: float, search_from: int = 0) -> float:
    t0 = trace.crossing_time("speed_kmh_true", start_kmh, search_from)
    t1 = trace.crossing_time("speed_kmh_true", target_kmh, search_from)
    return t1 - t0


def test_tc_sil_11_acceleration_times_match_model(vcu):
    """VCU-REQ-007: 0-50 and 0-100 km/h within 0.1 s of the model's exact times."""
    sim = Simulation(vcu, drive_model(Phase(1.0), Phase(14.0, pedal=1.0)))
    trace = sim.run()
    assert trace.last("cluster_acceleration_0_50_valid") == 1
    assert trace.last("cluster_acceleration_0_100_valid") == 1
    assert trace.last("cluster_acceleration_0_50_last_s") == pytest.approx(true_run_time(trace, 1, 50), abs=0.1)
    assert trace.last("cluster_acceleration_0_100_last_s") == pytest.approx(true_run_time(trace, 1, 100), abs=0.1)


def test_tc_sil_12_best_time_keeps_the_faster_run(vcu):
    """Two runs (full and half throttle): last = latest run, best = faster run."""
    sim = Simulation(vcu, drive_model(
        Phase(1.0), Phase(12.0, pedal=1.0), Phase(25.0, brake=1.0), Phase(2.0),
        Phase(30.0, pedal=0.5)))
    trace = sim.run()
    fast = true_run_time(trace, 1, 100)
    second_start = next(i for i, r in enumerate(trace.rows) if r["t"] > 40.0)
    slow = true_run_time(trace, 1, 100, second_start)
    assert slow > fast + 0.5
    assert trace.last("cluster_acceleration_0_100_last_s") == pytest.approx(slow, abs=0.1)
    assert trace.last("cluster_acceleration_0_100_best_s") == pytest.approx(fast, abs=0.1)


def test_tc_sil_13_rolling_start_is_not_timed(vcu):
    """A run that did not start from standstill must not produce a time."""
    sim = Simulation(vcu, drive_model(Phase(12.0, pedal=1.0), initial_speed_kmh=30.0))
    trace = sim.run()
    assert trace.last("cluster_acceleration_0_50_valid") == 0
    assert trace.last("cluster_acceleration_0_100_valid") == 0


def test_tc_sil_14_drive_cycle_needle_follows_power(vcu):
    """Mixed cycle: the needle always equals the oracle for the current inverter values."""
    sim = Simulation(vcu, drive_model(
        Phase(2.0), Phase(6.0, pedal=0.6), Phase(4.0, pedal=0.2), Phase(5.0, brake=0.8),
        Phase(2.0, direction=0), Phase(3.0, pedal=1.0)))
    trace = sim.run()
    for row in trace.rows:
        gear = "neutral" if row["sdu_seldir"] == 0 else "drive"
        assert abs(row["cluster_needle_position"] - oracle.needle(gear, row["sdu_udc"], row["sdu_idc"])) <= 1, row
    needles = trace.column("cluster_needle_position")
    assert max(needles) > 6000 and min(needles) < 4100   # drive and regeneration both seen


def test_tc_sil_15_overheating_raises_warning(vcu):
    """Thermal scenario: sustained full power with poor cooling heats the heat sink past 80 °C."""
    model = drive_model(Phase(1.0), Phase(60.0, pedal=1.0), initial_temp_c=60.0)
    model.p.heatsink_thermal_resistance = 0.05   # degraded cooling
    sim = Simulation(vcu, model)
    trace = sim.run()
    hot = next((r for r in trace.rows if r["sdu_tmphs"] >= 80), None)
    assert hot is not None, "model did not reach 80 °C"
    first = trace.rows.index(hot)
    assert trace.rows[first]["motor2_coolant_temp_not_ok"] == 1
    assert all(r["motor2_coolant_temp_not_ok"] == 0 for r in trace.rows[:first] if r["sdu_tmpm"] < 160)


# --------------------------------------------------------------------------- fuel signal (VCU-REQ-006)


def test_tc_sil_16_fuel_counter_wraps_with_overflow_flag(vcu):
    """MO5 consumption: 15-bit µl counter wraps at 32768 and raises the overflow flag for 5 updates."""
    seen_flag = 0
    wraps = 0
    last = vcu.get("mo5_verbrauch_ul")
    for _ in range(3000):   # 18 s
        vcu.step()
        value = vcu.get("mo5_verbrauch_ul")
        assert 0 <= value <= 0x7FFF
        if value < last:
            wraps += 1
        seen_flag += int(vcu.get("mo5_ueberlverb"))
        last = value
    assert wraps >= 1
    assert seen_flag > 0


@pytest.mark.xfail(strict=True, reason="BEFUND/Lücke VCU-REQ-006: Der Verbrauch ist noch simuliert und steigt "
                                      "auch im Stand ohne Leistung (16,7 ml/s).")
def test_tc_sil_17_no_consumption_at_standstill(vcu):
    apply(vcu, 500, sdu_udc=360, sdu_idc=0, sdu_seldir=FW_NEUTRAL)
    assert vcu.get("cluster_total_fuel_l") == 0
