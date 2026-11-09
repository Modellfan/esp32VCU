r"""Read the VCU text log on UART0 (COM10) without resetting the board.

    .pio\canivore-py-test-tool-venv\Scripts\python.exe tools\vcu_log.py --port COM10 --duration 10 [--diag-only]

The log is only active while GVRET is not in binary mode (after a reset, before SavvyCAN or
tools/gvret_listen.py sent 0xE7). Every second the firmware prints one [DIAG] line with the
ECU task timing and, per bus, rx frame count, TEC, REC, EFLG and bus-off.
"""
from __future__ import annotations

import argparse
import time

import serial


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--port", default="COM10")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--duration", type=float, default=10.0)
    parser.add_argument("--diag-only", action="store_true", help="print only [DIAG] lines")
    args = parser.parse_args()

    ser = serial.Serial()
    ser.port = args.port
    ser.baudrate = args.baud
    ser.timeout = 0.2
    ser.dtr = False
    ser.rts = False
    ser.open()
    end = time.time() + args.duration
    buf = b""
    while time.time() < end:
        buf += ser.read(1024)
        while b"\n" in buf:
            line, buf = buf.split(b"\n", 1)
            text = line.decode("utf-8", errors="replace").rstrip("\r")
            if not args.diag_only or text.startswith("[DIAG]"):
                print(text, flush=True)
    ser.close()


if __name__ == "__main__":
    main()
