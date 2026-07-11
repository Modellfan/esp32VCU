#!/usr/bin/env python3
"""Shared local server helpers for validating the integrated touch UI."""

from __future__ import annotations

import contextlib
import json
import math
import socket
import socketserver
import threading
import time
from http.server import SimpleHTTPRequestHandler
from pathlib import Path
from urllib.parse import parse_qs, urlsplit

REPO_ROOT = Path(__file__).resolve().parents[1]
WEB_ROOT = REPO_ROOT / "data"
DEFAULT_SCREENSHOT = REPO_ROOT / "archives" / "touch_ui_1024x600" / "layout_1024x600.png"

MOCK_LIVE = {
    "ap_ssid": "eboxster",
    "ap_ip": "192.168.4.1",
    "vehicle_speed_510": 87.4,
    "vref_kmh": 86.9,
    "acceleration": 0.164,
    "prnd_pos": 3,
    "pedal_position_1": 34.2,
    "pedal_position_2": 34.0,
    "throttle_position_280": 22.8,
    "brake_normal_or_offset": 1.25,
    "brake_sw": False,
    "brake_sw_inv": True,
    "handbrake": False,
    "brake_lamp": False,
    "abs_lamp": False,
    "driver_braking": False,
    "no_actively_built_up_brake_pressure": False,
    "br1_counter": 17,
    "tcs": False,
    "msr": False,
    "abs": False,
    "abd": False,
    "fdr": False,
    "asr_mode": 1,
    "tcs_slow_pct": 0.0,
    "tcs_fast_pct": 0.0,
    "msr_inv_pct": 0.0,
    "itq_pct": 62.5,
    "yaw_rate": 0.0214,
    "lenkwinkel_rb": 12.8,
    "lws_ok_rb": True,
    "fine_check_rb": True,
    "coarse_check_rb": True,
    "indicator_left": False,
    "indicator_right": True,
    "tempomat_button": False,
    "steeringwheel_button_up": False,
    "steeringwheel_button_down": False,
    "lat_g": 0.114,
    "engine_speed_rpm": 3180.0,
    "motor2_limited_torque_percent": 74.2,
    "motor2_min_torque_ignition_retard_percent": 8.5,
    "reduced_power": False,
    "motor2_cruise_control_status": 2,
    "motor2_cruise_control_regulating": True,
    "motor2_speed_limiter_selected": False,
    "motor2_cruise_target_speed_kmh": 96.0,
    "motor2_idle_target_speed_rpm": 1020.0,
    "motor2_offroad_mode_active": False,
    "motor2_normal_operation_active": True,
    "motor2_drivetrain_coordinator_active": True,
    "motor2_coolant_temperature": 51.7,
    "motor2_coolant_temp_not_ok": False,
    "inlet_air_temperature": 27.5,
    "barometric_pressure_mbar": 991.0,
    "motor2_ac_compressor_active": False,
    "motor2_brake_light_switch": False,
    "motor2_brake_test_switch": False,
    "motor2_sport_error": False,
    "check_engine_light": False,
    "check_engine_light_bit1": False,
    "fan_error": False,
    "coolant_fan_level": 0,
    "coolant_fan_low_output": False,
    "coolant_fan_high_output": False,
    "mo5_vorgluehen": False,
    "mo5_e_gas": True,
    "mo5_obd_2": False,
    "mo5_klimakompr": False,
    "mo5_heissl": False,
    "mo5_verbrauch_ul": 2250,
    "mo5_ueberlverb": False,
    "ambient_c": 18.8,
    "oil_pressure_bar": 2.46,
    "sdu_udc": 356.7,
    "sdu_idc": 112.4,
    "sdu_il1": 93.2,
    "sdu_il2": -88.6,
    "sdu_fstat": 82.4,
    "sdu_uaux": 13.8,
    "sdu_turns": 18432,
    "sdu_amp": 116.0,
    "sdu_angle": 18.5,
    "sdu_regenpresent": 28.5,
    "sdu_regenpreset": 28.5,
    "sdu_regenpresent32": 24.0,
    "sdu_lasterr": 0,
    "sdu_opmode": 1,
    "sdu_status": 64,
    "sdu_speed": 4210,
    "sdu_cruisespeed": 4000,
    "sdu_seldir": 1,
    "sdu_rotordir": 1,
    "sdu_seldir43": 1,
    "sdu_pot": 1040,
    "sdu_pot2": 1024,
    "sdu_potnom": 31.5,
    "sdu_pwmio": 142,
    "sdu_canio": 43,
    "sdu_din_cruise": 1,
    "sdu_din_start": 1,
    "sdu_din_brake": 0,
    "sdu_din_mprot": 1,
    "sdu_din_forward": 1,
    "sdu_din_reverse": 0,
    "sdu_din_emcystop": 1,
    "sdu_din_ocur": 1,
    "sdu_din_ocur51": 0,
    "sdu_din_desat": 1,
    "sdu_din_bms": 1,
    "sdu_temp_heatsink": 46.3,
    "sdu_temp_heatsink74": 46.7,
    "sdu_tmphs": 47.1,
    "sdu_tmpm": 58.2,
    "sdu_uptime": 352600,
    "sdu_cpuload": 34.0,
    "sdu_ilmax": 168.0,
    "sdu_uac": 228.0,
    "sdu_il1rms": 67.0,
    "sdu_il2rms": 64.0,
    "sdu_boostcalc": 0.94,
    "sdu_fweakcalc": 96.0,
    "cluster_activated": True,
    "cluster_needle_position": 118,
    "cluster_total_fuel_l": 7.214,
    "cluster_heatsink_temp_critical": False,
    "cluster_motor_temp_critical": False,
    "cluster_power_percent_max": 88.6,
    "cluster_power_percent_dyn": 63.4,
    "target_charge_pct": 80,
    "ac_current_limit_a": 16,
    "light_dimmer": 73,
    "cluster_counter": 142,
    "fdr_lamp": False,
    "psm_button_lamp": False,
    "rough_road_suppression": False,
    "receipt_for_acc_message": True,
}
MOCK_STATE = dict(MOCK_LIVE)
MOCK_LOCK = threading.Lock()
MOCK_START_MONOTONIC = time.monotonic()


def clamp(value: float, minimum: float, maximum: float) -> float:
    return max(minimum, min(maximum, value))


def wave(elapsed_s: float, period_s: float, phase_rad: float = 0.0) -> float:
    return math.sin((elapsed_s / period_s) * (2.0 * math.pi) + phase_rad)


def compose_canio_flags(state: dict[str, int]) -> int:
    flags = 0
    if state["sdu_din_cruise"] == 1:
        flags |= 1
    if state["sdu_din_start"] == 1:
        flags |= 2
    if state["sdu_din_brake"] == 1:
        flags |= 4
    if state["sdu_din_forward"] == 1:
        flags |= 8
    if state["sdu_din_reverse"] == 1:
        flags |= 16
    if state["sdu_din_bms"] == 1:
        flags |= 32
    return flags


def compose_status_flags(
    dc_voltage: float,
    dc_current: float,
    heatsink_temp: float,
    pot: int,
    din_start: int,
    din_brake: int,
    din_emcystop: int,
    din_mprot: int,
) -> int:
    status = 0
    if dc_voltage < 338.0:
        status |= 1
    if dc_voltage > 370.0:
        status |= 2
    if dc_voltage < 346.0:
        status |= 4
    if abs(dc_current) > 145.0:
        status |= 8
    if din_emcystop == 0:
        status |= 16
    if din_mprot == 0:
        status |= 32
    if pot > 1080:
        status |= 64
    if heatsink_temp > 50.0:
        status |= 128
    if din_start != 1:
        status |= 256
    if din_brake == 1:
        status |= 512
    return status


def update_dynamic_mock_state() -> None:
    elapsed_s = time.monotonic() - MOCK_START_MONOTONIC
    slow = wave(elapsed_s, 9.5, 0.15)
    medium = wave(elapsed_s, 5.75, 1.1)
    fast = wave(elapsed_s, 3.1, -0.55)
    pulse = (wave(elapsed_s, 6.8, 0.45) + 1.0) * 0.5
    tick = int(elapsed_s)

    vehicle_speed = round(86.5 + 9.8 * slow + 2.6 * fast, 1)
    dc_current = round(111.0 + 34.0 * medium + 9.5 * fast, 3)
    dc_voltage = round(356.5 + 14.2 * slow + 3.6 * fast, 3)
    heatsink_temp = round(46.2 + 5.8 * pulse + 0.7 * slow, 2)
    motor_temp = round(57.0 + 7.4 * pulse + 1.2 * medium, 2)
    sdu_speed = int(round(4210 + 780 * medium + 130 * fast))
    sdu_cruisespeed = int(round(4000 + 185 * slow))
    sdu_pot = int(round(980 + 180 * pulse + 35 * medium))
    sdu_pot2 = int(round(968 + 168 * pulse + 28 * medium))
    sdu_regenpreset = round(clamp(27.5 + 7.5 * max(0.0, -medium), 0.0, 100.0), 2)
    sdu_seldir = 1 if tick % 18 < 11 else (-1 if tick % 18 in (14, 15) else 0)
    sdu_rotordir = 1 if sdu_speed > 40 else (-1 if sdu_speed < -40 else 0)
    sdu_din_cruise = 1 if tick % 9 in (1, 2, 3, 4) else 0
    sdu_din_start = 1 if tick % 12 in (0, 1, 2, 3) else 0
    sdu_din_brake = 1 if fast < -0.38 else 0
    sdu_din_mprot = 0 if motor_temp >= 63.0 else 1
    sdu_din_forward = 1 if sdu_seldir == 1 else 0
    sdu_din_reverse = 1 if sdu_seldir == -1 else 0
    sdu_din_emcystop = 0 if tick % 37 == 0 else 1
    sdu_din_ocur = 1 if medium > -0.15 else 0
    sdu_din_ocur51 = 1 if fast > 0.35 else 0
    sdu_din_desat = 0 if tick % 29 == 0 else 1
    sdu_din_bms = 1 if slow > -0.55 else 0
    sdu_canio = compose_canio_flags(
        {
            "sdu_din_cruise": sdu_din_cruise,
            "sdu_din_start": sdu_din_start,
            "sdu_din_brake": sdu_din_brake,
            "sdu_din_forward": sdu_din_forward,
            "sdu_din_reverse": sdu_din_reverse,
            "sdu_din_bms": sdu_din_bms,
        }
    )
    sdu_status = compose_status_flags(
        dc_voltage,
        dc_current,
        heatsink_temp,
        sdu_pot,
        sdu_din_start,
        sdu_din_brake,
        sdu_din_emcystop,
        sdu_din_mprot,
    )
    sdu_il1 = round(dc_current * 0.84 + 8.5 * fast, 2)
    sdu_il2 = round(-dc_current * 0.79 + 7.0 * slow, 2)
    sdu_fstat = round(78.0 + 18.0 * medium + 2.5 * fast, 2)
    sdu_turns = int(round(elapsed_s * sdu_speed / 60.0))
    sdu_amp = round(abs(dc_current) * 1.08, 2)
    sdu_angle = round(18.0 + 22.0 * slow, 1)
    sdu_potnom = round(clamp((((sdu_pot + sdu_pot2) * 0.5) - 1024.0) / 5.2, -100.0, 100.0), 2)
    sdu_uptime = int(round(elapsed_s * 100.0))
    sdu_cpuload = round(clamp(31.0 + 13.0 * pulse + 3.5 * fast, 0.0, 100.0), 1)
    sdu_ilmax = round(165.0 + 12.0 * slow, 2)
    sdu_uac = round(228.0 + 7.0 * slow + 1.5 * medium, 2)
    sdu_il1rms = round(abs(sdu_il1) * 0.707, 2)
    sdu_il2rms = round(abs(sdu_il2) * 0.707, 2)
    sdu_boostcalc = round(0.88 + 0.22 * max(0.0, medium), 2)
    sdu_fweakcalc = round(95.0 + 6.0 * slow + 1.2 * fast, 2)

    MOCK_STATE.update(
        {
            "vehicle_speed_510": vehicle_speed,
            "vref_kmh": round(vehicle_speed - 0.7 + 0.4 * fast, 1),
            "acceleration": round(0.18 * fast, 3),
            "pedal_position_1": round(clamp(31.0 + 18.0 * pulse + 2.4 * medium, 0.0, 100.0), 1),
            "pedal_position_2": round(clamp(30.4 + 17.0 * pulse + 2.0 * medium, 0.0, 100.0), 1),
            "throttle_position_280": round(clamp(20.0 + 19.0 * pulse + 2.8 * fast, 0.0, 100.0), 1),
            "brake_normal_or_offset": round(1.05 + 0.5 * max(0.0, -fast), 2),
            "br1_counter": 12 + (tick % 20),
            "tcs_slow_pct": round(max(0.0, 7.5 * -fast), 2),
            "tcs_fast_pct": round(max(0.0, 10.5 * -medium), 2),
            "msr_inv_pct": round(max(0.0, 4.8 * fast), 2),
            "itq_pct": round(clamp(58.0 + 14.0 * medium + 4.0 * fast, 0.0, 100.0), 2),
            "yaw_rate": round(0.012 + 0.036 * fast, 4),
            "lenkwinkel_rb": round(11.5 + 6.0 * slow, 2),
            "indicator_left": tick % 6 in (0, 1),
            "indicator_right": tick % 6 in (3, 4),
            "tempomat_button": tick % 10 == 0,
            "steeringwheel_button_up": tick % 12 == 4,
            "steeringwheel_button_down": tick % 12 == 8,
            "lat_g": round(0.08 + 0.09 * fast, 3),
            "engine_speed_rpm": round(3150.0 + 420.0 * medium + 140.0 * fast, 1),
            "motor2_limited_torque_percent": round(clamp(72.0 + 10.0 * medium, 0.0, 100.0), 1),
            "motor2_min_torque_ignition_retard_percent": round(clamp(7.0 + 3.4 * fast, 0.0, 100.0), 1),
            "motor2_cruise_control_status": 2 + (tick % 2),
            "motor2_cruise_target_speed_kmh": round(94.0 + 5.0 * slow, 1),
            "motor2_idle_target_speed_rpm": round(1015.0 + 25.0 * fast, 1),
            "motor2_coolant_temperature": round(51.0 + 3.8 * pulse, 1),
            "inlet_air_temperature": round(27.0 + 1.9 * slow, 1),
            "barometric_pressure_mbar": round(991.0 + 2.0 * slow, 1),
            "coolant_fan_level": tick % 3,
            "coolant_fan_low_output": (tick % 3) >= 1,
            "coolant_fan_high_output": (tick % 3) >= 2,
            "mo5_verbrauch_ul": int(round(2140 + 190 * pulse + 40 * medium)),
            "ambient_c": round(18.6 + 1.4 * slow, 1),
            "oil_pressure_bar": round(2.35 + 0.35 * pulse, 2),
            "sdu_udc": dc_voltage,
            "sdu_idc": dc_current,
            "sdu_il1": sdu_il1,
            "sdu_il2": sdu_il2,
            "sdu_fstat": sdu_fstat,
            "sdu_uaux": round(13.7 + 0.45 * slow, 2),
            "sdu_turns": sdu_turns,
            "sdu_amp": sdu_amp,
            "sdu_angle": sdu_angle,
            "sdu_regenpreset": sdu_regenpreset,
            "sdu_regenpresent": sdu_regenpreset,
            "sdu_regenpresent32": round(clamp(sdu_regenpreset - 4.5 + 2.0 * max(0.0, -fast), 0.0, 100.0), 2),
            "sdu_lasterr": [0, 0, 3, 7, 10, 17][tick % 6],
            "sdu_opmode": [1, 1, 2, 3, 4, 5, 6, 1][tick % 8],
            "sdu_status": sdu_status,
            "sdu_speed": sdu_speed,
            "sdu_cruisespeed": sdu_cruisespeed,
            "sdu_seldir": sdu_seldir,
            "sdu_rotordir": sdu_rotordir,
            "sdu_seldir43": sdu_rotordir,
            "sdu_pot": sdu_pot,
            "sdu_pot2": sdu_pot2,
            "sdu_potnom": sdu_potnom,
            "sdu_pwmio": 140 + (tick % 18),
            "sdu_canio": sdu_canio,
            "sdu_din_cruise": sdu_din_cruise,
            "sdu_din_start": sdu_din_start,
            "sdu_din_brake": sdu_din_brake,
            "sdu_din_mprot": sdu_din_mprot,
            "sdu_din_forward": sdu_din_forward,
            "sdu_din_reverse": sdu_din_reverse,
            "sdu_din_emcystop": sdu_din_emcystop,
            "sdu_din_ocur": sdu_din_ocur,
            "sdu_din_ocur51": sdu_din_ocur51,
            "sdu_din_desat": sdu_din_desat,
            "sdu_din_bms": sdu_din_bms,
            "sdu_temp_heatsink": round(heatsink_temp - 0.8, 2),
            "sdu_temp_heatsink74": round(motor_temp - 1.1, 2),
            "sdu_tmphs": heatsink_temp,
            "sdu_tmpm": motor_temp,
            "sdu_uptime": sdu_uptime,
            "sdu_cpuload": sdu_cpuload,
            "sdu_ilmax": sdu_ilmax,
            "sdu_uac": sdu_uac,
            "sdu_il1rms": sdu_il1rms,
            "sdu_il2rms": sdu_il2rms,
            "sdu_boostcalc": sdu_boostcalc,
            "sdu_fweakcalc": sdu_fweakcalc,
            "cluster_needle_position": int(round(clamp(114 + 10 * medium + 3 * fast, 0.0, 255.0))),
            "cluster_total_fuel_l": round(7.18 + 0.06 * slow, 3),
            "cluster_heatsink_temp_critical": heatsink_temp >= 52.0,
            "cluster_motor_temp_critical": motor_temp >= 63.0,
            "cluster_power_percent_max": round(clamp(86.0 + 7.0 * pulse + 2.0 * slow, 0.0, 100.0), 3),
            "cluster_power_percent_dyn": round(clamp(61.0 + 10.0 * medium + 3.0 * fast, 0.0, 100.0), 3),
            "light_dimmer": int(round(clamp(70.0 + 9.0 * slow, 0.0, 100.0))),
            "cluster_counter": 120 + ((tick * 3) % 80),
            "fdr_lamp": medium < -0.72,
            "psm_button_lamp": fast < -0.58,
            "rough_road_suppression": tick % 14 in (5, 6),
        }
    )


class TouchUiHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(WEB_ROOT), **kwargs)

    def log_message(self, format: str, *args) -> None:
        return

    def end_headers(self) -> None:
        self.send_header("Cache-Control", "no-store, max-age=0")
        self.send_header("Pragma", "no-cache")
        self.send_header("Expires", "0")
        super().end_headers()

    def do_GET(self) -> None:
        parsed = urlsplit(self.path)
        if parsed.path == "/api/set":
            params = parse_qs(parsed.query)
            with MOCK_LOCK:
                if "target_charge_pct" in params:
                    value = float(params["target_charge_pct"][0])
                    MOCK_STATE["target_charge_pct"] = max(50, min(100, round(value)))
                if "ac_current_limit_a" in params:
                    value = float(params["ac_current_limit_a"][0])
                    MOCK_STATE["ac_current_limit_a"] = max(6, min(32, round(value)))
                update_dynamic_mock_state()
                payload = json.dumps(MOCK_STATE).encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Cache-Control", "no-store")
            self.send_header("Content-Length", str(len(payload)))
            self.end_headers()
            self.wfile.write(payload)
            return
        if parsed.path == "/api/live":
            with MOCK_LOCK:
                update_dynamic_mock_state()
                payload = json.dumps(MOCK_STATE).encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Cache-Control", "no-store")
            self.send_header("Content-Length", str(len(payload)))
            self.end_headers()
            self.wfile.write(payload)
            return
        super().do_GET()


def find_free_port(host: str) -> int:
    with contextlib.closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as sock:
        sock.bind((host, 0))
        return int(sock.getsockname()[1])


def start_server(host: str, port: int) -> tuple[socketserver.TCPServer, threading.Thread]:
    global MOCK_START_MONOTONIC
    with MOCK_LOCK:
        MOCK_STATE.clear()
        MOCK_STATE.update(MOCK_LIVE)
        MOCK_START_MONOTONIC = time.monotonic()
        update_dynamic_mock_state()
    server = socketserver.TCPServer((host, port), TouchUiHandler)
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    return server, thread
