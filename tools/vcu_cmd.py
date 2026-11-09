r"""Send a text command to the VCU on UART0 (COM10) and print the answer.

    .pio\canivore-py-test-tool-venv\Scripts\python.exe tools\vcu_cmd.py version
    .pio\canivore-py-test-tool-venv\Scripts\python.exe tools\vcu_cmd.py wifi-status
    .pio\canivore-py-test-tool-venv\Scripts\python.exe tools\vcu_cmd.py reset --duration 6
    .pio\canivore-py-test-tool-venv\Scripts\python.exe tools\vcu_cmd.py --hard-reset --duration 8

Commands (src/services/serial_console.h): help, version, wifi-status, reset.
They work only while GVRET is not in binary mode (after a reset, before SavvyCAN or
tools/gvret_listen.py). --hard-reset pulses RTS (EN of the board, like after flashing) and
works in every state; it prints the boot log. The port is opened without a DTR/RTS reset.
[DIAG] lines are hidden unless --diag is given.
"""
from __future__ import annotations

import argparse
import sys
import time

import serial


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("command", nargs="?", help="help, version, wifi-status, reset")
    parser.add_argument("--port", default="COM10")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--duration", type=float, default=1.5, help="seconds to read the answer")
    parser.add_argument("--hard-reset", action="store_true", help="reset via RTS instead of a command")
    parser.add_argument("--diag", action="store_true", help="also print [DIAG] lines")
    args = parser.parse_args()
    if not args.command and not args.hard_reset:
        parser.error("command or --hard-reset required")
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")

    ser = serial.Serial()
    ser.port = args.port
    ser.baudrate = args.baud
    ser.timeout = 0.1
    ser.dtr = False
    ser.rts = False
    ser.open()
    ser.reset_input_buffer()
    if args.hard_reset:
        ser.rts = True
        time.sleep(0.1)
        ser.rts = False
    if args.command:
        ser.write(b"\n" + args.command.encode("ascii") + b"\n")
        ser.flush()

    end = time.time() + args.duration
    buf = b""
    while time.time() < end:
        buf += ser.read(1024)
        while b"\n" in buf:
            line, buf = buf.split(b"\n", 1)
            text = line.decode("utf-8", errors="replace").rstrip("\r")
            if args.diag or not text.startswith("[DIAG]"):
                print(text, flush=True)
    ser.close()


if __name__ == "__main__":
    main()
