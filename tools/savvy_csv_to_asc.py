"""Convert a SavvyCAN/GVRET CSV log into a Vector ASC file for canivore-py-test-tool playback.

CSV columns: Time Stamp (µs), ID (hex), Extended, Dir, Bus, LEN, D1..D8.

    python tools/savvy_csv_to_asc.py <in.csv> <out.asc> [--exclude 280,289,298,4E0,7A0]
        [--start-s 0] [--duration-s 0]

--exclude drops IDs the VCU sends itself (ECU emulation), so a playback on the bench
does not put a second sender on the same IDs.
"""
from __future__ import annotations

import argparse
import csv
from pathlib import Path

import can

VCU_TX_IDS = "280,289,298,4E0,7A0"


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("csv_file", type=Path)
    parser.add_argument("asc_file", type=Path)
    parser.add_argument("--exclude", default="", help=f"comma-separated hex IDs to drop, e.g. {VCU_TX_IDS}")
    parser.add_argument("--start-s", type=float, default=0.0, help="skip the first seconds of the log")
    parser.add_argument("--duration-s", type=float, default=0.0, help="keep only this many seconds (0 = all)")
    args = parser.parse_args()

    excluded = {int(x, 16) for x in args.exclude.split(",") if x.strip()}
    written = skipped = 0
    first_us = None
    with args.csv_file.open(newline="", encoding="utf-8", errors="replace") as src, \
            can.ASCWriter(str(args.asc_file)) as asc:
        for row in csv.DictReader(src):
            try:
                t_us = int(row["Time Stamp"])
                can_id = int(row["ID"], 16)
                length = int(row["LEN"])
            except (KeyError, ValueError):
                skipped += 1
                continue
            if first_us is None:
                first_us = t_us
            t_s = (t_us - first_us) / 1e6
            if t_s < args.start_s or (args.duration_s > 0 and t_s > args.start_s + args.duration_s):
                continue
            if can_id in excluded:
                continue
            data = bytes(int(row[f"D{i}"], 16) for i in range(1, length + 1))
            asc.on_message_received(can.Message(
                timestamp=t_s - args.start_s,
                arbitration_id=can_id,
                is_extended_id=row["Extended"].strip().lower() == "true",
                data=data,
                channel=0,
            ))
            written += 1
    print(f"{args.asc_file}: {written} frames, {skipped} unreadable rows skipped")


if __name__ == "__main__":
    main()
