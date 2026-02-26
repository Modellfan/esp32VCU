#!/usr/bin/env python3
import re
from pathlib import Path


BO_RE = re.compile(r"^BO_\s+(\d+)\s+([^:]+):\s+(\d+)\s+(\S+)\s*$")
SG_RE = re.compile(
    r'^\s*SG_\s+(.+?)\s*:\s*(\d+)\|(\d+)@([01])([+-])\s+\(([^,]+),([^)]+)\)\s+\[([^\|]*)\|([^\]]*)\]\s+"([^"]*)"\s+(.*)$'
)
CM_SG_RE = re.compile(r'^CM_\s+SG_\s+(\d+)\s+(\S+)\s+"(.*)";\s*$')
VAL_RE = re.compile(r"^VAL_\s+(\d+)\s+(\S+)\s+(.*);\s*$")
CYCLE_RE = re.compile(r'^BA_\s+"GenMsgCycleTime"\s+BO_\s+(\d+)\s+(\d+);\s*$')
ENUM_PAIR_RE = re.compile(r'(-?\d+)\s+"([^"]*)"')


def parse_signal_name_and_mux(raw_name: str) -> tuple[str, str]:
    parts = raw_name.strip().split()
    if len(parts) >= 2 and re.fullmatch(r"[mM][0-9]*", parts[-1]):
        return " ".join(parts[:-1]), parts[-1]
    return raw_name.strip(), ""


def format_can_id(msg_id: int) -> str:
    return f"0x{msg_id:03X}"


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    dbc_path = root / "documentation" / "vw_pq.dbc"
    out_paths = [root / "PQ_messages.md", root / "documentation" / "PQ_messages.md"]

    text = dbc_path.read_text(encoding="utf-8", errors="replace").splitlines()

    comments: dict[tuple[int, str], str] = {}
    enums: dict[tuple[int, str], list[tuple[str, str]]] = {}
    cycles: dict[int, int] = {}
    messages: list[dict] = []
    msg_by_id: dict[int, dict] = {}
    current = None

    for line in text:
        m_cycle = CYCLE_RE.match(line)
        if m_cycle:
            cycles[int(m_cycle.group(1))] = int(m_cycle.group(2))
            continue

        m_comment = CM_SG_RE.match(line)
        if m_comment:
            key = (int(m_comment.group(1)), m_comment.group(2))
            comments[key] = m_comment.group(3).strip()
            continue

        m_val = VAL_RE.match(line)
        if m_val:
            msg_id = int(m_val.group(1))
            sig_name = m_val.group(2)
            pairs = ENUM_PAIR_RE.findall(m_val.group(3))
            enums[(msg_id, sig_name)] = pairs
            continue

        m_bo = BO_RE.match(line)
        if m_bo:
            msg_id = int(m_bo.group(1))
            current = {
                "id": msg_id,
                "name": m_bo.group(2).strip(),
                "dlc": int(m_bo.group(3)),
                "tx": m_bo.group(4).strip(),
                "signals": [],
            }
            messages.append(current)
            msg_by_id[msg_id] = current
            continue

        if current is not None:
            m_sg = SG_RE.match(line)
            if m_sg:
                raw_name = m_sg.group(1).strip()
                sig_name, mux = parse_signal_name_and_mux(raw_name)
                current["signals"].append(
                    {
                        "raw_name": raw_name,
                        "sig_name": sig_name,
                        "mux": mux,
                        "start": int(m_sg.group(2)),
                        "length": int(m_sg.group(3)),
                        "endian": "Intel" if m_sg.group(4) == "1" else "Motorola",
                        "sign": m_sg.group(5),
                        "factor": m_sg.group(6).strip(),
                        "offset": m_sg.group(7).strip(),
                        "unit": m_sg.group(10).strip() or "-",
                    }
                )

    for msg in messages:
        if msg["id"] in cycles:
            msg["cycle"] = f'{cycles[msg["id"]]} ms'
        else:
            msg["cycle"] = "-"

    out: list[str] = []
    out.append("# VW PQ Signal Reference")
    out.append("")
    out.append("Source: documentation/vw_pq.dbc")
    out.append("")
    out.append(f"- Total messages: **{len(messages)}**")
    out.append(f"- Total signals: **{sum(len(m['signals']) for m in messages)}**")
    out.append("")

    for msg in messages:
        out.append(f"## CAN ID {format_can_id(msg['id'])} - {msg['name']}")
        out.append("")
        out.append(f"- DLC: {msg['dlc']}  TX: {msg['tx']}  Cycle: {msg['cycle']}")
        out.append("")
        out.append("| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | DBC Comment | Enum Values |")
        out.append("| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- |")
        for s in msg["signals"]:
            stype = f"U{s['length']}" if s["sign"] == "+" else f"I{s['length']}"
            if s["length"] == 1:
                stype = "Bool" if s["sign"] == "+" else "I1"
            c = comments.get((msg["id"], s["sig_name"]), "-")
            enum_vals = enums.get((msg["id"], s["sig_name"]), [])
            enum_str = (
                ", ".join([f'{k}="{v}"' for k, v in enum_vals]) if enum_vals else "-"
            )
            display_name = s["raw_name"]
            out.append(
                f"| {display_name} | {s['start']} | {s['length']} | {s['endian']} | {stype} | {s['factor']} | {s['offset']} | {s['unit']} | {c} | {enum_str} |"
            )
        out.append("")

    content = "\n".join(out).rstrip() + "\n"
    for p in out_paths:
        p.write_text(content, encoding="utf-8")

    print(
        f"OK messages={len(messages)} signals={sum(len(m['signals']) for m in messages)} files={', '.join(str(p) for p in out_paths)}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
