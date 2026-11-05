"""Closed loop: environment model -> VCU parameters -> firmware step -> recorded outputs."""

from __future__ import annotations

from dataclasses import dataclass, field

from .model import EnvironmentModel
from .vcu import VcuSil

DEFAULT_OUTPUTS = [
    "cluster_power_percent_max", "cluster_needle_position", "engine_speed_rpm",
    "motor2_coolant_temperature", "motor2_coolant_temp_not_ok",
    "cluster_heatsink_temp_critical", "cluster_motor_temp_critical",
    "mo5_verbrauch_ul", "mo5_ueberlverb", "cluster_total_fuel_l",
    "cluster_acceleration_run_active", "cluster_acceleration_run_time_s",
    "cluster_acceleration_0_50_last_s", "cluster_acceleration_0_50_best_s", "cluster_acceleration_0_50_valid",
    "cluster_acceleration_0_100_last_s", "cluster_acceleration_0_100_best_s", "cluster_acceleration_0_100_valid",
]


@dataclass
class Trace:
    rows: list[dict] = field(default_factory=list)

    def column(self, name: str) -> list[float]:
        return [row[name] for row in self.rows]

    def last(self, name: str) -> float:
        return self.rows[-1][name]

    def crossing_time(self, name: str, threshold: float, start_index: int = 0) -> float | None:
        """Linear-interpolated time when column `name` first reaches `threshold`."""
        rows = self.rows
        for i in range(max(1, start_index), len(rows)):
            a, b = rows[i - 1][name], rows[i][name]
            if a < threshold <= b:
                ta, tb = rows[i - 1]["t"], rows[i]["t"]
                return ta + (threshold - a) / (b - a) * (tb - ta)
        return None


class Simulation:
    """Runs model and VCU in lock-step with the 6 ms real-time cycle.

    substeps: model integration steps per VCU cycle (finer physics, same coupling rate).
    """

    def __init__(self, vcu: VcuSil, model: EnvironmentModel, outputs: list[str] | None = None,
                 substeps: int = 6, mark_rx: bool = True) -> None:
        self.vcu = vcu
        self.model = model
        self.outputs = outputs or DEFAULT_OUTPUTS
        self.substeps = substeps
        self.mark_rx = mark_rx
        self.trace = Trace()

    def step(self) -> dict:
        dt = self.vcu.cycle_s / self.substeps
        for _ in range(self.substeps):
            self.model.step(dt)
        inputs = self.model.vcu_inputs()
        self.vcu.set_many(inputs)
        if self.mark_rx:
            for bus, can_id in self.model.RX_MESSAGES:
                self.vcu.mark_rx(bus, can_id)
        self.vcu.step()
        row = {"t": self.vcu.now_s, "speed_kmh_true": self.model.speed_kmh,
               "mech_power_kw": self.model.s.mech_power_w / 1000.0,
               "motor_temp_true": self.model.s.motor_temp_c,
               "heatsink_temp_true": self.model.s.heatsink_temp_c}
        row.update(inputs)
        row.update({name: self.vcu.get(name) for name in self.outputs})
        self.trace.rows.append(row)
        return row

    def run(self, seconds: float | None = None) -> Trace:
        duration = seconds if seconds is not None else self.model.profile.duration_s
        for _ in range(max(1, round(duration / self.vcu.cycle_s))):
            self.step()
        return self.trace
