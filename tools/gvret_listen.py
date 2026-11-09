r"""Read what the VCU receives and sends, via GVRET on UART0, and count frames per bus.

    .pio\canivore-py-test-tool-venv\Scripts\python.exe tools\gvret_listen.py --port COM10 --duration 20 [--pattern-id 611]

Buses as reported by the firmware: 0 = sent by the VCU (ECU emulation), 1 = CAN1 (vehicle,
MCP2515), 2 = CAN2 (drive, MCP2515), 3 = battery bus (TWAI).

--pattern-id checks frames of canivore-py-test-tool tx-pattern (0xCA, stream, 32-bit sequence,
CRC-16-CCITT) per bus: count, missing and duplicate sequence numbers, CRC errors.

Opening the port keeps DTR/RTS low so the ESP32 is not reset. Sends 0xE7 0xE7 once, which
switches GVRET to binary output (the same thing SavvyCAN does on connect).
"""
from __future__ import annotations

import argparse
import binascii
import collections
import time

import serial

BUS_NAMES = {0: "VCU-TX", 1: "CAN1", 2: "CAN2", 3: "CAN0/TWAI"}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--port", default="COM10")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--duration", type=float, default=10.0)
    parser.add_argument("--pattern-id", default="", help="hex ID of the tx-pattern stream to verify")
    args = parser.parse_args()
    pattern_id = int(args.pattern_id, 16) if args.pattern_id else None

    ser = serial.Serial()
    ser.port = args.port
    ser.baudrate = args.baud
    ser.timeout = 0.05
    ser.dtr = False
    ser.rts = False
    ser.open()
    ser.reset_input_buffer()
    ser.write(bytes([0xE7, 0xE7]))

    counts: dict[int, collections.Counter] = collections.defaultdict(collections.Counter)
    seqs: dict[int, list[int]] = collections.defaultdict(list)
    crc_errors: collections.Counter = collections.Counter()
    buf = bytearray()
    end = time.time() + args.duration
    while time.time() < end:
        buf += ser.read(4096)
        while True:
            start = buf.find(0xF1)
            if start < 0:
                buf.clear()
                break
            if start:
                del buf[:start]
            if len(buf) < 11:
                break
            if buf[1] != 0x00:  # not a CAN frame (command reply): skip the marker
                del buf[:1]
                continue
            length = buf[10] & 0x0F
            bus = buf[10] >> 4
            if length > 8 or bus > 3:
                del buf[:1]
                continue
            total = 12 + length
            if len(buf) < total:
                break
            can_id = int.from_bytes(buf[6:10], "little") & 0x1FFFFFFF
            data = bytes(buf[11:11 + length])
            del buf[:total]
            counts[bus][can_id] += 1
            if pattern_id is not None and can_id == pattern_id and length == 8 and data[0] == 0xCA:
                if binascii.crc_hqx(data[:6], 0xFFFF) != int.from_bytes(data[6:8], "big"):
                    crc_errors[bus] += 1
                seqs[bus].append(int.from_bytes(data[2:6], "big"))
    ser.close()

    for bus in sorted(counts):
        ids = ", ".join(f"{i:03X}:{n}" for i, n in sorted(counts[bus].items()))
        print(f"{BUS_NAMES.get(bus, bus):10s} {sum(counts[bus].values()):7d} frames  {ids}")
    if pattern_id is not None:
        print(f"\nPattern 0x{pattern_id:03X}:")
        for bus in (1, 2):
            s = seqs.get(bus, [])
            if not s:
                print(f"  {BUS_NAMES[bus]}: none received")
                continue
            unique = set(s)
            missing = (max(unique) - min(unique) + 1) - len(unique)
            print(f"  {BUS_NAMES[bus]}: {len(s)} frames, seq {min(unique)}..{max(unique)}, "
                  f"missing {missing}, duplicates {len(s) - len(unique)}, CRC errors {crc_errors[bus]}")


if __name__ == "__main__":
    main()
