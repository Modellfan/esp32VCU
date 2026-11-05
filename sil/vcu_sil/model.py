"""Python model of the VCU environment: driver, drive unit, battery, thermal, vehicle.

The model is deliberately simple and independent of the firmware: it only produces the
signals the VCU receives (inverter and vehicle CAN values, by signal-table name) and
keeps ground truth (exact speed and power) for the test oracles.

Direction convention: OpenInverter `seldir` (-1 reverse, 0 neutral, 1 forward), as in
the touch UI enum. `direction_sign` lets a test emulate an inverted installation.
"""

from __future__ import annotations

import math
from dataclasses import dataclass, field


@dataclass
class Phase:
    """One segment of a driver profile."""

    duration_s: float
    pedal: float = 0.0          # 0..1 drive demand
    brake: float = 0.0          # 0..1 regenerative braking demand
    direction: int = 1          # -1 reverse, 0 neutral, 1 forward


@dataclass
class DriverProfile:
    phases: list[Phase]

    def at(self, t: float) -> Phase:
        for phase in self.phases:
            if t < phase.duration_s:
                return phase
            t -= phase.duration_s
        return self.phases[-1]

    @property
    def duration_s(self) -> float:
        return sum(p.duration_s for p in self.phases)


@dataclass
class VehicleParameters:
    mass_kg: float = 1450.0
    cda_m2: float = 0.62
    crr: float = 0.012
    air_density: float = 1.2
    wheel_radius_m: float = 0.31
    gear_ratio: float = 9.73
    max_torque_nm: float = 420.0      # at the motor
    max_power_kw: float = 110.0       # mechanical, drive
    max_regen_kw: float = 35.0
    max_current_a: float = 300.0      # battery current limit, drive
    max_regen_current_a: float = 100.0
    efficiency: float = 0.92
    battery_ocv_v: float = 360.0
    battery_resistance_ohm: float = 0.08
    aux_voltage_v: float = 13.6
    ambient_c: float = 20.0
    # thermal: first order, heat capacity [J/K] and resistance to coolant [K/W]
    motor_heat_capacity: float = 18000.0
    motor_thermal_resistance: float = 0.012
    heatsink_heat_capacity: float = 2500.0
    heatsink_thermal_resistance: float = 0.010
    heatsink_loss_share: float = 0.35     # share of losses in the inverter


@dataclass
class ModelState:
    t_s: float = 0.0
    speed_mps: float = 0.0
    motor_temp_c: float = 20.0
    heatsink_temp_c: float = 20.0
    battery_voltage_v: float = 360.0
    battery_current_a: float = 0.0
    mech_power_w: float = 0.0
    direction: int = 0
    motor_rpm: float = 0.0
    history: list = field(default_factory=list)


class EnvironmentModel:
    """Integrates the vehicle with a fixed step and returns VCU input signals."""

    def __init__(self, profile: DriverProfile, params: VehicleParameters | None = None,
                 initial_speed_kmh: float = 0.0, initial_temp_c: float | None = None,
                 direction_sign: int = 1) -> None:
        self.p = params or VehicleParameters()
        self.profile = profile
        self.direction_sign = direction_sign
        self.s = ModelState(speed_mps=initial_speed_kmh / 3.6,
                            motor_temp_c=initial_temp_c if initial_temp_c is not None else self.p.ambient_c,
                            heatsink_temp_c=initial_temp_c if initial_temp_c is not None else self.p.ambient_c,
                            battery_voltage_v=self.p.battery_ocv_v)

    # ------------------------------------------------------------ physics
    def _battery_current(self, dc_power_w: float) -> tuple[float, float]:
        """Solve P = (OCV - R*I) * I for I; returns (current, terminal voltage)."""
        ocv, r = self.p.battery_ocv_v, self.p.battery_resistance_ohm
        disc = ocv * ocv - 4.0 * r * dc_power_w
        if disc < 0:  # beyond the maximum transferable power
            disc = 0.0
        current = (ocv - math.sqrt(disc)) / (2.0 * r)
        return current, ocv - r * current

    def step(self, dt: float) -> None:
        p, s = self.p, self.s
        phase = self.profile.at(s.t_s)
        s.direction = phase.direction
        speed = s.speed_mps
        wheel_rpm = speed / p.wheel_radius_m * 60.0 / (2.0 * math.pi)
        s.motor_rpm = wheel_rpm * p.gear_ratio
        omega = s.motor_rpm * 2.0 * math.pi / 60.0

        # Motor torque request from the pedal (drive) or the brake (regeneration).
        torque = 0.0
        if phase.direction != 0 and phase.pedal > 0:
            torque = phase.pedal * p.max_torque_nm
            if omega > 1.0:
                torque = min(torque, p.max_power_kw * 1000.0 / omega)
        elif phase.brake > 0 and omega > 1.0:
            torque = -min(phase.brake * p.max_torque_nm, p.max_regen_kw * 1000.0 / omega)

        mech_power = torque * omega
        dc_power = mech_power / p.efficiency if mech_power >= 0 else mech_power * p.efficiency
        current, voltage = self._battery_current(dc_power)
        # Battery current limits (the inverter derates torque).
        limit = p.max_current_a if current >= 0 else p.max_regen_current_a
        if abs(current) > limit:
            scale = limit / abs(current)
            torque *= scale
            mech_power *= scale
            dc_power = mech_power / p.efficiency if mech_power >= 0 else mech_power * p.efficiency
            current, voltage = self._battery_current(dc_power)

        # Longitudinal dynamics (forward or reverse, magnitude only).
        drive_force = torque * p.gear_ratio / p.wheel_radius_m
        resist = 0.5 * p.air_density * p.cda_m2 * speed * speed + (p.crr * p.mass_kg * 9.81 if speed > 0.01 else 0.0)
        accel = (drive_force - resist) / p.mass_kg
        s.speed_mps = max(0.0, speed + accel * dt)

        # Thermal model: losses split between motor and inverter heat sink.
        losses = abs(mech_power) * (1.0 - p.efficiency) / p.efficiency
        q_hs = losses * p.heatsink_loss_share
        q_m = losses - q_hs
        s.motor_temp_c += dt * (q_m - (s.motor_temp_c - p.ambient_c) / p.motor_thermal_resistance) / p.motor_heat_capacity
        s.heatsink_temp_c += dt * (q_hs - (s.heatsink_temp_c - p.ambient_c) / p.heatsink_thermal_resistance) / p.heatsink_heat_capacity

        s.mech_power_w = mech_power
        s.battery_current_a = current
        s.battery_voltage_v = voltage
        s.t_s += dt

    # ------------------------------------------------------------ VCU interface
    @property
    def speed_kmh(self) -> float:
        return self.s.speed_mps * 3.6

    def vcu_inputs(self) -> dict[str, float]:
        """Values the VCU would decode from CAN 1 (vehicle) and CAN 2 (inverter)."""
        s = self.s
        return {
            "sdu_udc": round(s.battery_voltage_v, 1),
            "sdu_idc": round(s.battery_current_a, 1),
            "sdu_seldir": s.direction * self.direction_sign,
            "sdu_speed": round(s.motor_rpm),
            "sdu_tmpm": max(0, min(255, round(s.motor_temp_c))),
            "sdu_tmphs": max(0, min(255, round(s.heatsink_temp_c))),
            "sdu_uaux": self.p.aux_voltage_v,
            "vehicle_speed_510": round(self.speed_kmh, 2),
        }

    # Messages the inputs above belong to (bus 2 = drive, bus 1 = vehicle).
    RX_MESSAGES = [(2, 0x500), (2, 0x501), (2, 0x504), (1, 0x510)]
