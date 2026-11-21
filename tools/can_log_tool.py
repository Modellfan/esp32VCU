#!/usr/bin/env python3
"""Evaluate SavvyCAN/GVRET logs of the VCU for reverse engineering and fault finding.

Logs live in documentation/can_logs/ (*.csv or *.csv.gz, SavvyCAN "GVRET" CSV export).
Bus numbers of the VCU bridge (src/services/gvret_bridge.h): 0 = CAN0 battery/Zombie,
1 = CAN1 vehicle (Porsche 986), 2 = CAN2 Tesla SDU, 3 = spare, 4 = sent by the VCU.
Frames of bus 4 are assigned to their physical bus by ID (CAN0 DBC, otherwise CAN1).

Decoding:
  CAN1  documentation/porsche_986_996_compiled_working.dbc (master)
  CAN0  documentation/ZombieSlave_teensybms_dcdc_mvcu.dbc
  CAN2  OpenInverter tables in documentation/can_messages.md
  --ref documentation/vw_pq.dbc (VW PQ reference, only for comparing interpretations)

Commands (log = file name, stem or unique part of it, e.g. "fahrt2"):
  import                      move new SavvyCAN CSVs from the OneDrive root here (gzip)
  list                        all logs with duration and frame count
  summary  LOG                per bus and ID: name, count, cycle time, gaps, DLC, payloads
  bits     LOG --id 0x440     per bit: constant / toggling / counter, rare changes with times
  events   LOG                timeline of anomalies: timeouts, DLC changes, rare bit changes,
                              status/fault changes of decoded signals
  signals  LOG --id 0x41C     decoded signals: range, changes; --sig NAME for one signal
  changes  LOG --sig NAME     every value change of one signal (or --id X --bit N)
  snapshot LOG --t 123.4      last decoded value of every signal at time t
  dump     LOG --id 0x440 --from 10 --to 11   raw frames
  plot     LOG --sig A,B,0x440:b12,0x440:B3 --out x.png [--from --to]
  check    LOG                verify the DBCs: counters, CRCs, values outside the DBC range

Times are seconds from the start of the log (32-bit microsecond wrap of GVRET removed).
Parsed logs are cached in documentation/can_logs/.cache/ (Parquet, not versioned).
"""

from __future__ import annotations

import argparse
import gzip
import re
import shutil
import sys
from datetime import datetime
from pathlib import Path

import numpy as np
import pandas as pd

ROOT = Path(__file__).resolve().parents[1]
DOC = ROOT / "documentation"
LOGS = DOC / "can_logs"
CACHE = LOGS / ".cache"
ONEDRIVE = Path.home() / "OneDrive"
SAVVY_HEADER = "Time Stamp,ID,Extended,Dir,Bus,LEN"

BUS_NAMES = {0: "CAN0", 1: "CAN1", 2: "CAN2", 3: "CAN3"}
GVRET_TX_BUS = 4


# --------------------------------------------------------------------------- logs

def log_files() -> list[Path]:
    files = sorted(list(LOGS.glob("*.csv")) + list(LOGS.glob("*.csv.gz")))
    return [f for f in files if f.is_file()]


def stem_of(path: Path) -> str:
    name = path.name
    return name[:-7] if name.endswith(".csv.gz") else name[:-4]


def find_log(query: str) -> Path:
    path = Path(query)
    if path.is_file():
        return path
    hits = [f for f in log_files() if stem_of(f) == query]
    if not hits:
        hits = [f for f in log_files() if query.lower() in f.name.lower()]
    if len(hits) != 1:
        names = ", ".join(stem_of(f) for f in (hits or log_files()))
        sys.exit(f"log '{query}' {'ambiguous' if hits else 'not found'}: {names}")
    return hits[0]


def is_savvy_csv(path: Path) -> bool:
    opener = gzip.open if path.name.endswith(".gz") else open
    try:
        with opener(path, "rt", encoding="utf-8", errors="replace") as fh:
            return fh.readline().startswith(SAVVY_HEADER)
    except OSError:
        return False


def cmd_import(args) -> None:
    src = Path(args.src)
    moved = 0
    for f in sorted(src.glob("*.csv")):
        if not is_savvy_csv(f):
            continue
        date = datetime.fromtimestamp(f.stat().st_mtime).strftime("%Y%m%d")
        stem = f.stem if re.match(r"^\d{8}_", f.stem) else f"{date}_{f.stem}"
        target = LOGS / f"{stem}.csv.gz"
        if target.exists() or (LOGS / f"{stem}.csv").exists():
            print(f"skip {f.name}: {target.name} exists")
            continue
        with open(f, "rb") as fin, gzip.open(target, "wb", compresslevel=6) as fout:
            shutil.copyfileobj(fin, fout)
        if not args.keep:
            f.unlink()
        moved += 1
        print(f"{f.name} -> {target.relative_to(ROOT)}")
    print(f"{moved} log(s) imported")


# --------------------------------------------------------------------------- decoding tables

class Sig:
    __slots__ = ("name", "start", "length", "little", "signed", "scale", "offset",
                 "minimum", "maximum", "unit", "choices", "mux_value", "is_mux")

    def __init__(self, name, start, length, little=True, signed=False, scale=1.0, offset=0.0,
                 minimum=None, maximum=None, unit="", choices=None, mux_value=None, is_mux=False):
        self.name, self.start, self.length = name, start, length
        self.little, self.signed, self.scale, self.offset = little, signed, scale, offset
        self.minimum, self.maximum, self.unit = minimum, maximum, unit
        self.choices, self.mux_value, self.is_mux = choices or {}, mux_value, is_mux


class Msg:
    def __init__(self, frame_id, name, bus, signals, cycle_ms=None, source=""):
        self.frame_id, self.name, self.bus, self.signals = frame_id, name, bus, signals
        self.cycle_ms, self.source = cycle_ms, source


def _from_cantools(path: Path, bus: int, label: str) -> dict[int, Msg]:
    import cantools
    db = cantools.database.load_file(str(path), strict=False)
    out = {}
    for m in db.messages:
        sigs = []
        for s in m.signals:
            mux_ids = s.multiplexer_ids
            choices = {int(k): str(v) for k, v in (s.choices or {}).items()}
            sigs.append(Sig(s.name, s.start, s.length, s.byte_order == "little_endian",
                            s.is_signed, float(s.scale), float(s.offset), s.minimum, s.maximum,
                            s.unit or "", choices, mux_ids[0] if mux_ids else None,
                            s.is_multiplexer))
        cycle = m.cycle_time
        out[m.frame_id] = Msg(m.frame_id, m.name, bus, sigs, cycle, label)
    return out


def _from_can_messages_md() -> dict[int, Msg]:
    """OpenInverter tables of documentation/can_messages.md (CAN2)."""
    text = (DOC / "can_messages.md").read_text(encoding="utf-8")
    enums: dict[str, dict[int, str]] = {}
    for m in re.finditer(r"`(\w+)` (?:enum|bitfield):\s*\n\s*\n\|[^\n]*\n\|[^\n]*\n((?:\|[^\n]*\n)+)", text):
        table = {}
        for row in m.group(2).strip().splitlines():
            cells = [c.strip() for c in row.strip("|").split("|")]
            try:
                table[int(cells[0])] = cells[1]
            except ValueError:
                pass
        enums[m.group(1)] = table
    out = {}
    for m in re.finditer(r"### CAN ID 0x([0-9A-Fa-f]+) / \d+ - ([^\n]+)\n(.*?)(?=\n### |\n## |\Z)", text, re.S):
        frame_id = int(m.group(1), 16)
        sigs = []
        for row in m.group(3).splitlines():
            cells = [c.strip() for c in row.strip().strip("|").split("|")]
            if len(cells) < 7 or not cells[1].isdigit():
                continue
            name, start, length, _endian, typ, factor, offset = cells[:7]
            unit = cells[7] if len(cells) > 7 else ""
            sigs.append(Sig("SDU_" + name, int(start), int(length), True, typ.startswith("S"),
                            float(factor), float(offset), unit=unit, choices=enums.get(unit)))
        out[frame_id] = Msg(frame_id, "SDU_" + re.sub(r"\W+", "_", m.group(2)).strip("_"), 2, sigs,
                            None, "can_messages.md")
    return out


_TABLES = None


def tables(ref: bool = False) -> dict[tuple[int, int], Msg]:
    """(bus, id) -> Msg. With ref the VW PQ DBC replaces the 986 DBC on CAN1."""
    global _TABLES
    key = "ref" if ref else "main"
    if _TABLES is None:
        _TABLES = {}
    if key not in _TABLES:
        t = {}
        can1 = DOC / ("vw_pq.dbc" if ref else "porsche_986_996_compiled_working.dbc")
        for fid, m in _from_cantools(can1, 1, can1.name).items():
            t[(1, fid)] = m
        for fid, m in _from_cantools(DOC / "ZombieSlave_teensybms_dcdc_mvcu.dbc", 0,
                                     "ZombieSlave DBC").items():
            t[(0, fid)] = m
        for fid, m in _from_can_messages_md().items():
            t[(2, fid)] = m
        _TABLES[key] = t
    return _TABLES[key]


def find_signal(name: str, ref: bool = False) -> tuple[Msg, Sig]:
    hits = [(m, s) for m in tables(ref).values() for s in m.signals if s.name == name]
    if not hits:
        hits = [(m, s) for m in tables(ref).values() for s in m.signals if name.lower() in s.name.lower()]
    if len(hits) != 1:
        sys.exit(f"signal '{name}': {len(hits)} matches " + ", ".join(s.name for _, s in hits[:20]))
    return hits[0]


# --------------------------------------------------------------------------- frames

def load(path: Path) -> pd.DataFrame:
    """Columns: t (s), raw_bus, bus (physical), tx, id, dlc, b0..b7, le (uint64), be (uint64)."""
    CACHE.mkdir(parents=True, exist_ok=True)
    stat = path.stat()
    cached = CACHE / f"{stem_of(path)}_{stat.st_size}_{int(stat.st_mtime)}.parquet"
    if cached.exists():
        return pd.read_parquet(cached)
    for old in CACHE.glob(f"{stem_of(path)}_*.parquet"):
        old.unlink()
    df = pd.read_csv(path, dtype=str, usecols=range(14), on_bad_lines="skip")
    df.columns = ["ts", "id", "ext", "dir", "bus", "len"] + [f"d{i}" for i in range(8)]
    ts = df["ts"].astype(np.int64).to_numpy()
    step = np.diff(ts)
    back = step < 0
    # Backward step: 32-bit wrap of micros() (old value near 2^32) or a restart of the time base
    # (VCU reset, new GVRET time sync); a restart continues 1 ms after the last frame.
    wrap = back & (ts[:-1] > (1 << 32) - 60_000_000)
    step = np.where(wrap, step + (1 << 32), np.where(back, 1000, step))
    ts = np.concatenate([[0], np.cumsum(step)])
    seg = np.concatenate([[0], np.cumsum(back & ~wrap)]).astype(np.int16)
    out = pd.DataFrame({
        "t": ts / 1e6,
        "seg": seg,
        "raw_bus": df["bus"].astype(np.int8).to_numpy(),
        "id": df["id"].map(lambda v: int(v, 16)).astype(np.int32).to_numpy(),
        "dlc": df["len"].astype(np.int8).to_numpy(),
    })
    le = np.zeros(len(df), dtype=np.uint64)
    be = np.zeros(len(df), dtype=np.uint64)
    for i in range(8):
        col = df[f"d{i}"].fillna("00").replace("", "00").map(lambda v: int(v, 16) if v else 0)
        b = col.astype(np.uint8).to_numpy()
        b = np.where(out["dlc"].to_numpy() > i, b, 0).astype(np.uint8)
        out[f"b{i}"] = b
        le |= b.astype(np.uint64) << np.uint64(8 * i)
        be |= b.astype(np.uint64) << np.uint64(8 * (7 - i))
    out["le"], out["be"] = le, be
    out["tx"] = out["raw_bus"] == GVRET_TX_BUS
    can0_ids = {fid for (bus, fid) in tables() if bus == 0}
    phys = out["raw_bus"].to_numpy().copy()
    tx = out["tx"].to_numpy()
    phys[tx] = np.where(np.isin(out["id"].to_numpy()[tx], list(can0_ids)), 0, 1)
    out["bus"] = phys.astype(np.int8)
    out = out.sort_values("t", kind="stable").reset_index(drop=True)
    out.to_parquet(cached)
    return out


def select(df, ident=None, bus=None, tx=None, t_from=None, t_to=None):
    m = np.ones(len(df), dtype=bool)
    if ident is not None:
        m &= df["id"].to_numpy() == ident
    if bus is not None:
        m &= df["bus"].to_numpy() == bus
    if tx is not None:
        m &= df["tx"].to_numpy() == tx
    if t_from is not None:
        m &= df["t"].to_numpy() >= t_from
    if t_to is not None:
        m &= df["t"].to_numpy() <= t_to
    return df[m]


def raw_values(frames: pd.DataFrame, s: Sig) -> np.ndarray:
    mask = np.uint64((1 << s.length) - 1)
    if s.little:
        raw = (frames["le"].to_numpy() >> np.uint64(s.start)) & mask
    else:  # Motorola: DBC start bit = MSB
        p = 8 * (s.start // 8) + (7 - s.start % 8)
        raw = (frames["be"].to_numpy() >> np.uint64(64 - (p + s.length))) & mask
    raw = raw.astype(np.int64)
    if s.signed:
        raw = np.where(raw >= (1 << (s.length - 1)), raw - (1 << s.length), raw)
    return raw


def decode(frames: pd.DataFrame, msg: Msg, s: Sig) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """-> times, raw, physical (only frames where the multiplexer selects the signal)."""
    keep = np.ones(len(frames), dtype=bool)
    if s.mux_value is not None:
        mux = next((x for x in msg.signals if x.is_mux), None)
        if mux is not None:
            keep = raw_values(frames, mux) == s.mux_value
    f = frames[keep]
    raw = raw_values(f, s)
    return f["t"].to_numpy(), raw, raw * s.scale + s.offset


def parse_id(text: str) -> int:
    return int(text, 16) if text.lower().startswith("0x") else int(text, 16 if re.search("[a-fA-F]", text) else 10)


def fmt_t(t: float) -> str:
    return f"{t:9.3f}"


def msg_name(bus: int, fid: int) -> str:
    m = tables().get((bus, fid))
    return m.name if m else "?"


def label(s: Sig, raw: int, phys: float) -> str:
    if s.choices:
        if raw in s.choices:
            return f"{raw} ({s.choices[raw]})"
        flags = [v for k, v in s.choices.items() if k > 0 and (k & (k - 1)) == 0 and raw & k]
        if flags:
            return f"{raw} ({'|'.join(flags)})"
    if s.scale == 1.0 and s.offset == 0.0:
        return str(raw)
    return f"{phys:.4g}"


# --------------------------------------------------------------------------- analysis helpers

def id_groups(df):
    return df.groupby(["bus", "tx", "id"], sort=True)


def cycle_stats(t: np.ndarray):
    if len(t) < 3:
        return None, None, None
    d = np.diff(t) * 1000.0
    return float(np.median(d)), float(np.percentile(d, 99)), float(d.max())


def bit_matrix(frames: pd.DataFrame) -> np.ndarray:
    le = frames["le"].to_numpy()
    return ((le[:, None] >> np.arange(64, dtype=np.uint64)[None, :]) & np.uint64(1)).astype(np.uint8)


def counter_fields(frames: pd.DataFrame, dlc: int) -> list[str]:
    """Nibbles / bytes that count up by a constant step most of the time."""
    found = []
    le = frames["le"].to_numpy()
    for start, length in [(8 * b, 8) for b in range(dlc)] + [(4 * n, 4) for n in range(2 * dlc)]:
        v = ((le >> np.uint64(start)) & np.uint64((1 << length) - 1)).astype(np.int64)
        if len(v) < 20 or len(np.unique(v)) < 4:
            continue
        d = np.diff(v) % (1 << length)
        vals, counts = np.unique(d, return_counts=True)
        best = counts.argmax()
        if vals[best] != 0 and counts[best] / len(d) > 0.9:
            found.append(f"bits {start}-{start + length - 1} step {vals[best]}")
    return found


# --------------------------------------------------------------------------- commands

def cmd_list(args) -> None:
    for f in log_files():
        df = load(f)
        buses = ", ".join(f"{BUS_NAMES.get(b, b)} {n}" for b, n in df[~df.tx].groupby("bus").size().items())
        restarts = int(df.seg.iloc[-1])
        print(f"{stem_of(f):28s} {df.t.iloc[-1]:8.1f} s {len(df):9d} frames  rx: {buses}  tx: {int(df.tx.sum())}"
              + (f"  time base restarts: {restarts}" if restarts else ""))


def cmd_summary(args) -> None:
    df = load(find_log(args.log))
    print(f"{find_log(args.log).name}: {len(df)} frames, {df.t.iloc[-1]:.1f} s")
    print(f"{'bus':4s} {'dir':2s} {'id':>5s} {'name':28s} {'n':>7s} {'cyc ms':>7s} {'p99':>7s} {'max':>8s} "
          f"{'gap>3x':>6s} {'dlc':5s} {'payl':>5s} {'first':>8s} {'last':>8s}")
    for (bus, tx, fid), g in id_groups(df):
        med, p99, mx = cycle_stats(g.t.to_numpy())
        gaps = int((np.diff(g.t.to_numpy()) * 1000 > 3 * med).sum()) if med else 0
        dlcs = ",".join(str(x) for x in sorted(g.dlc.unique()))
        print(f"{BUS_NAMES.get(bus, bus):4s} {'tx' if tx else 'rx':2s} {fid:#05x} {msg_name(bus, fid)[:28]:28s} "
              f"{len(g):7d} {med or 0:7.1f} {p99 or 0:7.1f} {mx or 0:8.1f} {gaps:6d} {dlcs:5s} "
              f"{g["le"].nunique():5d} {g.t.iloc[0]:8.2f} {g.t.iloc[-1]:8.2f}")


def cmd_bits(args) -> None:
    df = load(find_log(args.log))
    ident = parse_id(args.id)
    frames = select(df, ident, args.bus, False if not args.tx else True, args.t_from, args.t_to)
    if frames.empty:
        sys.exit("no frames")
    dlc = int(frames.dlc.max())
    bits = bit_matrix(frames)
    t = frames.t.to_numpy()
    msg = tables().get((int(frames.bus.iloc[0]), ident))
    owner = {}
    if msg:
        for s in msg.signals:
            if s.little:
                for b in range(s.start, s.start + s.length):
                    owner.setdefault(b, []).append(s.name)
    print(f"{ident:#05x} {msg.name if msg else '?'}: {len(frames)} frames, DLC {dlc}")
    for c in counter_fields(frames, dlc):
        print("  counter-like:", c)
    for byte in range(dlc):
        vals = frames[f"b{byte}"]
        print(f"  byte {byte}: {vals.nunique():3d} values, range {vals.min():#04x}..{vals.max():#04x}")
    print("  bit  value       changes  first changes (s)                         signal")
    for b in range(8 * dlc):
        col = bits[:, b]
        ch = np.flatnonzero(np.diff(col)) + 1
        state = "const " + str(col[0]) if len(ch) == 0 else f"{col.mean():.2f} on "
        times = " ".join(f"{t[i]:.2f}{'↑' if col[i] else '↓'}" for i in ch[:args.show])
        if len(ch) > args.show:
            times += " …"
        print(f"  {b:3d}  {state:10s} {len(ch):8d}  {times[:45]:45s} {','.join(owner.get(b, []))}")


STATUS_RE = r"(err|fault|fehler|warn|lamp|lampe|state|status|mode|dtc|_ok|aktiv|active)"


def rare_bit_events(frames, max_changes):
    bits = bit_matrix(frames)
    t = frames.t.to_numpy()
    out = []
    for b in range(64):
        col = bits[:, b]
        ch = np.flatnonzero(np.diff(col)) + 1
        if 0 < len(ch) <= max_changes:
            out += [(t[i], b, int(col[i])) for i in ch]
    return out


def cmd_events(args) -> None:
    path = find_log(args.log)
    df = load(path)
    events = []
    for (bus, tx, fid), g in id_groups(df):
        if len(g) < 5:
            events.append((g.t.iloc[0], bus, tx, fid, f"only {len(g)} frames"))
            continue
        t = g.t.to_numpy()
        med, _, _ = cycle_stats(t)
        d = np.diff(t) * 1000
        limit = max(3 * med, args.min_gap_ms)
        for i in np.flatnonzero(d > limit):
            events.append((t[i], bus, tx, fid, f"gap {d[i]:.0f} ms (cycle {med:.0f} ms) until {t[i + 1]:.3f}"))
        if g.dlc.nunique() > 1:
            ch = np.flatnonzero(np.diff(g.dlc.to_numpy())) + 1
            for i in ch[:5]:
                events.append((t[i], bus, tx, fid, f"DLC {g.dlc.iloc[i - 1]} -> {g.dlc.iloc[i]}"))
        msg = tables().get((bus, fid))
        status = lambda s: bool(s.choices) or s.length == 1 or re.search(STATUS_RE, s.name, re.I)
        # Bits of decoded numeric values (temperatures, voltages, ...) change rarely when the value
        # drifts slowly; they are reported only through the decoded signal, not bit by bit.
        numeric_bits = set()
        if msg and not args.all_bits:
            for s in msg.signals:
                if s.little:  # status signals are reported decoded below
                    numeric_bits.update(range(s.start, s.start + s.length))
        if not tx:
            for when, bit, val in rare_bit_events(g, args.max_changes):
                if bit not in numeric_bits:
                    events.append((when, bus, tx, fid, f"bit {bit} -> {val}"))
        if msg and not tx:
            for s in msg.signals:
                if not status(s):
                    continue
                tt, raw, phys = decode(g, msg, s)
                ch = np.flatnonzero(np.diff(raw)) + 1
                if 0 < len(ch) <= args.max_changes:
                    for i in ch:
                        events.append((tt[i], bus, tx, fid, f"{s.name}: {label(s, raw[i - 1], phys[i - 1])} -> {label(s, raw[i], phys[i])}"))
    for i in np.flatnonzero(np.diff(df.seg.to_numpy())) + 1:
        events.append((df.t.iloc[i], -1, False, 0, "time base restarted (VCU reset or new GVRET time sync)"))
    first = df.groupby(["bus", "tx", "id"]).t.min()
    late = first[first > args.late_s]
    for (bus, tx, fid), when in late.items():
        events.append((when, bus, tx, fid, "first frame (appears late)"))
    events.sort(key=lambda e: e[0])
    print(f"{path.name}: {len(events)} events (rare = at most {args.max_changes} changes)")
    for when, bus, tx, fid, text in events:
        if args.t_from is not None and when < args.t_from or args.t_to is not None and when > args.t_to:
            continue
        if bus < 0:
            print(f"{fmt_t(when)}  ---- {text}")
            continue
        print(f"{fmt_t(when)}  {BUS_NAMES.get(bus, bus)} {'tx' if tx else 'rx'} {fid:#05x} {msg_name(bus, fid)[:24]:24s} {text}")


def cmd_signals(args) -> None:
    df = load(find_log(args.log))
    if args.sig:
        msg, s = find_signal(args.sig, args.ref)
        pairs = [(msg, [s])]
    else:
        ident = parse_id(args.id)
        msgs = [m for (b, fid), m in tables(args.ref).items() if fid == ident and (args.bus is None or b == args.bus)]
        if not msgs:
            sys.exit(f"{ident:#x} not in the decoding tables")
        pairs = [(m, m.signals) for m in msgs]
    for msg, sigs in pairs:
        frames = select(df, msg.frame_id, msg.bus, None, args.t_from, args.t_to)
        print(f"{msg.frame_id:#05x} {msg.name} ({msg.source}): {len(frames)} frames")
        if frames.empty:
            continue
        for s in sigs:
            t, raw, phys = decode(frames, msg, s)
            if len(raw) == 0:
                continue
            ch = int((np.diff(raw) != 0).sum())
            vals = np.unique(raw)
            shown = ", ".join(label(s, int(v), v * s.scale + s.offset) for v in vals[:8]) + (" …" if len(vals) > 8 else "")
            mux = f" [mux {s.mux_value}]" if s.mux_value is not None else ""
            print(f"  {s.name[:34]:34s}{mux} {phys.min():10.4g} .. {phys.max():<10.4g} {s.unit[:6]:6s} "
                  f"changes {ch:6d}  values {len(vals):4d}: {shown[:70]}")


def cmd_changes(args) -> None:
    df = load(find_log(args.log))
    if args.sig:
        msg, s = find_signal(args.sig, args.ref)
        frames = select(df, msg.frame_id, msg.bus, None, args.t_from, args.t_to)
        t, raw, phys = decode(frames, msg, s)
        lab = lambda i: label(s, int(raw[i]), phys[i])
        title = f"{msg.name}.{s.name}"
    else:
        ident = parse_id(args.id)
        frames = select(df, ident, args.bus, None, args.t_from, args.t_to)
        t = frames.t.to_numpy()
        raw = ((frames["le"].to_numpy() >> np.uint64(args.bit)) & np.uint64((1 << args.len) - 1)).astype(np.int64)
        lab = lambda i: str(int(raw[i]))
        title = f"{ident:#05x} bits {args.bit}..{args.bit + args.len - 1}"
    if len(raw) == 0:
        sys.exit("no frames")
    ch = np.flatnonzero(np.diff(raw)) + 1
    print(f"{title}: start {lab(0)} at {t[0]:.3f}, {len(ch)} changes")
    for i in ch[:args.limit]:
        print(f"{fmt_t(t[i])}  {lab(i - 1)} -> {lab(i)}")
    if len(ch) > args.limit:
        print(f"… {len(ch) - args.limit} more")


def cmd_snapshot(args) -> None:
    df = load(find_log(args.log))
    before = df[df.t <= args.t]
    for (bus, fid), msg in sorted(tables().items()):
        frames = before[(before.bus == bus) & (before.id == fid)]
        if frames.empty:
            continue
        last = frames.iloc[[-1]]
        parts = []
        for s in msg.signals:
            if s.mux_value is not None:
                continue
            raw = raw_values(last, s)[0]
            parts.append(f"{s.name}={label(s, int(raw), raw * s.scale + s.offset)}")
        age = args.t - last.t.iloc[0]
        print(f"{BUS_NAMES[bus]} {fid:#05x} {msg.name} (age {age * 1000:.0f} ms): " + "  ".join(parts))


def cmd_dump(args) -> None:
    df = load(find_log(args.log))
    frames = select(df, parse_id(args.id) if args.id else None, args.bus, None, args.t_from, args.t_to)
    for _, r in frames.head(args.limit).iterrows():
        data = " ".join(f"{int(r[f'b{i}']):02X}" for i in range(int(r.dlc)))
        print(f"{fmt_t(r.t)}  {BUS_NAMES.get(r.bus, r.bus)} {'tx' if r.tx else 'rx'} {int(r.id):#05x} [{int(r.dlc)}] {data}")


def series(df, spec: str, ref: bool, t_from, t_to):
    m = re.match(r"^(0x[0-9a-fA-F]+|\d+):([bB])(\d+)$", spec)
    if m:
        frames = select(df, parse_id(m.group(1)), None, None, t_from, t_to)
        shift = int(m.group(3)) * (8 if m.group(2) == "B" else 1)
        width = 0xFF if m.group(2) == "B" else 1
        return frames.t.to_numpy(), ((frames["le"].to_numpy() >> np.uint64(shift)) & np.uint64(width)).astype(float), spec
    msg, s = find_signal(spec, ref)
    frames = select(df, msg.frame_id, msg.bus, None, t_from, t_to)
    t, _raw, phys = decode(frames, msg, s)
    return t, phys, f"{s.name} [{s.unit}]"


def cmd_plot(args) -> None:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    df = load(find_log(args.log))
    specs = [x for x in args.sig.split(",") if x]
    fig, axes = plt.subplots(len(specs), 1, sharex=True, figsize=(14, 2.0 * len(specs) + 0.6), squeeze=False)
    for ax, spec in zip(axes[:, 0], specs):
        t, v, name = series(df, spec, args.ref, args.t_from, args.t_to)
        ax.step(t, v, where="post", lw=0.8)
        ax.set_ylabel(name, rotation=0, ha="right", fontsize=8)
        ax.grid(alpha=0.3)
    axes[-1, 0].set_xlabel("t [s]")
    fig.suptitle(find_log(args.log).name)
    fig.tight_layout()
    out = Path(args.out)
    fig.savefig(out, dpi=110)
    print(out)


def stm32_crc8(data: bytes) -> int:
    """Low byte of the STM32 CRC-32 over two little-endian words, byte 7 = 0 (can_messages.md)."""
    buf = bytearray(data[:8].ljust(8, b"\0"))
    buf[7] = 0
    crc = 0xFFFFFFFF
    for w in range(2):
        word = int.from_bytes(buf[4 * w:4 * w + 4], "little")
        crc ^= word
        for _ in range(32):
            crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF if crc & 0x80000000 else (crc << 1) & 0xFFFFFFFF
    return crc & 0xFF


def cmd_check(args) -> None:
    df = load(find_log(args.log))
    for (bus, fid), msg in sorted(tables().items()):
        frames = df[(df.bus == bus) & (df.id == fid)]
        if frames.empty:
            continue
        notes = []
        for s in msg.signals:
            t, raw, phys = decode(frames, msg, s)
            if len(raw) == 0:
                continue
            lo, hi = s.minimum, s.maximum
            if lo is not None and hi is not None and hi > lo:
                bad = (phys < lo - 1e-9) | (phys > hi + 1e-9)
                if bad.any():
                    notes.append(f"{s.name}: {int(bad.sum())} values outside [{lo}, {hi}] (e.g. {phys[bad][0]:.4g})")
            if re.search(r"(zaehler|counter|alive)", s.name, re.I) and len(raw) > 10:
                d = np.diff(raw) % (1 << s.length)
                vals, counts = np.unique(d, return_counts=True)
                step = vals[counts.argmax()]
                share = counts.max() / len(d)
                if share < 0.95 or step == 0:
                    notes.append(f"{s.name}: not a clean counter (step {step} in {share:.0%})")
            if s.choices and not s.is_mux:
                unknown = sorted({int(v) for v in np.unique(raw)} - set(s.choices))
                bitflags = all(k == 0 or (k & (k - 1)) == 0 for k in s.choices)
                if unknown and not bitflags:
                    notes.append(f"{s.name}: raw values without a value table entry {unknown[:8]}")
        if bus == 0 and not frames.tx.any() and fid in (0x41A, 0x41B, 0x41C, 0x41D, 0x41E, 0x437, 0x439, 0x43A):
            data = frames[[f"b{i}" for i in range(8)]].to_numpy().astype(np.uint8)
            bad = sum(1 for row in data if stm32_crc8(bytes(row)) != row[7])
            if bad:
                notes.append(f"CRC byte 7 wrong in {bad} of {len(data)} frames")
        print(f"{BUS_NAMES[bus]} {fid:#05x} {msg.name}: {len(frames)} frames" + ("" if notes else "  ok"))
        for n in notes:
            print("    " + n)
    known = {(b, f) for (b, f) in tables()}
    rx = df[~df.tx]
    for (bus, fid), g in rx.groupby(["bus", "id"]):
        if (bus, fid) not in known:
            print(f"{BUS_NAMES.get(bus, bus)} {fid:#05x} not in any DBC: {len(g)} frames, {g["le"].nunique()} payloads")


# --------------------------------------------------------------------------- main

def main() -> None:
    sys.stdout.reconfigure(encoding="utf-8")
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)

    def add(name, func, log=True):
        sp = sub.add_parser(name)
        sp.set_defaults(func=func)
        if log:
            sp.add_argument("log")
            sp.add_argument("--from", dest="t_from", type=float)
            sp.add_argument("--to", dest="t_to", type=float)
            sp.add_argument("--bus", type=int)
            sp.add_argument("--ref", action="store_true", help="decode CAN1 with the VW PQ DBC")
        return sp

    sp = add("import", cmd_import, log=False)
    sp.add_argument("--src", default=str(ONEDRIVE))
    sp.add_argument("--keep", action="store_true", help="copy instead of move")
    add("list", cmd_list, log=False)
    add("summary", cmd_summary)
    sp = add("bits", cmd_bits)
    sp.add_argument("--id", required=True)
    sp.add_argument("--tx", action="store_true")
    sp.add_argument("--show", type=int, default=6)
    sp = add("events", cmd_events)
    sp.add_argument("--max-changes", type=int, default=20)
    sp.add_argument("--min-gap-ms", type=float, default=50.0)
    sp.add_argument("--late-s", type=float, default=2.0)
    sp.add_argument("--all-bits", action="store_true", help="also bits of decoded numeric values")
    sp = add("signals", cmd_signals)
    sp.add_argument("--id")
    sp.add_argument("--sig")
    sp = add("changes", cmd_changes)
    sp.add_argument("--sig")
    sp.add_argument("--id")
    sp.add_argument("--bit", type=int, default=0)
    sp.add_argument("--len", type=int, default=1)
    sp.add_argument("--limit", type=int, default=60)
    sp = add("snapshot", cmd_snapshot)
    sp.add_argument("--t", type=float, required=True)
    sp = add("dump", cmd_dump)
    sp.add_argument("--id")
    sp.add_argument("--limit", type=int, default=50)
    sp = add("plot", cmd_plot)
    sp.add_argument("--sig", required=True)
    sp.add_argument("--out", default=str(CACHE / "plot.png"))
    add("check", cmd_check)
    args = p.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
