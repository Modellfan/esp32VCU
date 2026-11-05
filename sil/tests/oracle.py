"""Test oracle: expected Kombi values written from the requirement (VCU-REQ-005), not from
the firmware code. Constants are the documented calibration values of src/cluster_calc.cpp.
"""

from __future__ import annotations

REF_VOLTAGE_V = 348.0
DRIVE_CURRENT_A = 300.0
REGEN_CURRENT_A = 100.0
NEEDLE_REVERSE = 35
NEEDLE_NEUTRAL = 822
NEEDLE_ZERO = 4100
NEEDLE_FULL_DRIVE = 7950
NEEDLE_FULL_REGEN = 1600
NEEDLE_LAG = 30
DEADBAND = 0.05
HEATSINK_CRITICAL_C = 80.0
MOTOR_CRITICAL_C = 160.0
COOLANT_GAUGE_MIN_C = -48.0
COOLANT_GAUGE_MAX_C = 255 * 0.75 - 48.0   # 143.25 °C, MO2_Kuehlm_T range


def power_fraction(udc: float, idc: float) -> float:
    """Signed share of the reference power (drive > 0, regeneration < 0), clamped to ±1."""
    limit = DRIVE_CURRENT_A if idc >= 0 else REGEN_CURRENT_A
    fraction = abs(udc * idc) / (REF_VOLTAGE_V * limit)
    fraction = min(fraction, 1.0)
    return fraction if idc >= 0 else -fraction


def round_half_away(x: float) -> int:
    return int(x + 0.5) if x >= 0 else -int(-x + 0.5)


def needle(direction: str, udc: float = 0.0, idc: float = 0.0) -> int:
    """direction: 'reverse' | 'neutral' | 'drive' (the gear, not the CAN encoding)."""
    if direction == "reverse":
        return NEEDLE_REVERSE
    if direction == "neutral":
        return NEEDLE_NEUTRAL
    p = power_fraction(udc, idc)
    if abs(p) < DEADBAND:
        return NEEDLE_ZERO
    if p > 0:
        return min(NEEDLE_FULL_DRIVE, round_half_away(NEEDLE_ZERO + (NEEDLE_FULL_DRIVE - NEEDLE_ZERO) * p) + NEEDLE_LAG)
    return round_half_away(NEEDLE_ZERO + (NEEDLE_FULL_REGEN - NEEDLE_ZERO) * -p)


def coolant_gauge(motor_temp_c: float) -> float:
    return max(COOLANT_GAUGE_MIN_C, min(COOLANT_GAUGE_MAX_C, motor_temp_c))


def coolant_warning(heatsink_c: float, motor_c: float) -> bool:
    return heatsink_c >= HEATSINK_CRITICAL_C or motor_c >= MOTOR_CRITICAL_C


def decode_0x280_rpm(data: bytes) -> float:
    """Motor_1 engine speed: bits 16..31 little endian, 0.25 rpm/bit (DBC)."""
    return (data[2] | (data[3] << 8)) * 0.25


def encode_0x500(udc_v: float, idc_a: float) -> bytes:
    """OpenInverter 0x500: udc 0.1 V/bit bits 0..15, idc signed 0.1 A/bit bits 16..31."""
    udc = round(udc_v * 10) & 0xFFFF
    idc = round(idc_a * 10) & 0xFFFF
    return bytes([udc & 0xFF, udc >> 8, idc & 0xFF, idc >> 8, 0, 0, 0, 0])
