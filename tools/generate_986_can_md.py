#!/usr/bin/env python3
"""Mirrors the Porsche 986 DBC into a readable Markdown table.

The DBC (documentation/porsche_986_996_compiled_working.dbc) is the master. Reverse
engineering results go into the DBC (signals, CM_ comments, "OPEN (986 #n)" notes,
GenMsgCycleTime); then run:

    python tools/generate_986_can_md.py

Output: documentation/porsche_986_can.md (never edit it by hand).
"""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DBC = ROOT / "documentation" / "porsche_986_996_compiled_working.dbc"
OUT = ROOT / "documentation" / "porsche_986_can.md"

BO_RE = re.compile(r"^BO_\s+(\d+)\s+(\w+)\s*:\s*(\d+)\s+(\w+)")
SG_RE = re.compile(
    r'^\s*SG_\s+(\w+)\s*(M|m\d*)?\s*:\s*(\d+)\|(\d+)@([01])([+-])\s+\(([^,]+),([^)]+)\)\s+'
    r'\[([^|]*)\|([^\]]*)\]\s+"([^"]*)"\s*(.*)$')
CM_NET_RE = re.compile(r'^CM_\s+"(.*)";\s*$')
CM_BO_RE = re.compile(r'^CM_\s+BO_\s+(\d+)\s+"(.*)";\s*$')
CM_SG_RE = re.compile(r'^CM_\s+SG_\s+(\d+)\s+(\w+)\s+"(.*)";\s*$')
VAL_RE = re.compile(r"^VAL_\s+(\d+)\s+(\w+)\s+(.*);\s*$")
CYCLE_RE = re.compile(r'^BA_\s+"GenMsgCycleTime"\s+BO_\s+(\d+)\s+(\d+);')
PAIR_RE = re.compile(r'(-?\d+)\s+"([^"]*)"')
OPEN_RE = re.compile(r"OPEN \(986 #(\d+)\):\s*(.*?)(?=\s*OPEN \(986 #|$)")

VCU_SENDS = {"ECU"}  # the VCU replaces the DME on CAN1


def parse():
    messages, order = {}, []
    current = None
    network = ""
    for line in DBC.read_text(encoding="ascii").splitlines():
        if m := BO_RE.match(line):
            mid = int(m.group(1))
            current = {"id": mid, "name": m.group(2), "dlc": int(m.group(3)), "sender": m.group(4),
                       "signals": [], "comment": "", "cycle": None}
            messages[mid] = current
            order.append(mid)
        elif (m := SG_RE.match(line)) and current is not None:
            current["signals"].append({
                "name": m.group(1), "mux": m.group(2) or "", "start": int(m.group(3)),
                "len": int(m.group(4)), "intel": m.group(5) == "1", "signed": m.group(6) == "-",
                "factor": m.group(7).strip(), "offset": m.group(8).strip(),
                "min": m.group(9).strip(), "max": m.group(10).strip(), "unit": m.group(11),
                "rx": m.group(12).strip().replace(",", ", "), "comment": "", "values": ""})
        elif not line.strip():
            current = None
        if m := CM_NET_RE.match(line):
            network = m.group(1)
        elif m := CM_BO_RE.match(line):
            messages[int(m.group(1))]["comment"] = m.group(2)
        elif m := CM_SG_RE.match(line):
            for sig in messages[int(m.group(1))]["signals"]:
                if sig["name"] == m.group(2):
                    sig["comment"] = m.group(3)
        elif m := VAL_RE.match(line):
            pairs = ", ".join(f"{k} = {v}" for k, v in PAIR_RE.findall(m.group(3)))
            for sig in messages[int(m.group(1))]["signals"]:
                if sig["name"] == m.group(2):
                    sig["values"] = pairs
        elif m := CYCLE_RE.match(line):
            messages[int(m.group(1))]["cycle"] = int(m.group(2))
    return [messages[i] for i in order], network


def cell(text: str) -> str:
    return (text or "-").replace("|", "\\|")


def sig_type(sig) -> str:
    if sig["len"] == 1:
        return "Bool"
    return ("S" if sig["signed"] else "U") + str(sig["len"])


def mux(sig) -> str:
    if sig["mux"] == "M":
        return " (Mux)"
    if sig["mux"].startswith("m") and sig["mux"] != "m":
        return f" ({sig['mux']})"
    return ""


def message_block(msg) -> list[str]:
    cycle = f"{msg['cycle']} ms (Original)" if msg["cycle"] else "nicht im Mitschnitt"
    comment = OPEN_RE.sub("", msg["comment"]).strip() or "-"
    lines = [f"### 0x{msg['id']:03X} {msg['name']}", "",
             f"Sender: {msg['sender']} · DLC {msg['dlc']} · Zyklus {cycle}", "", comment, ""]
    opens = OPEN_RE.findall(msg["comment"])
    for num, text in opens:
        lines.append(f"- **Offen (#{num}):** {text.strip()}")
    if opens:
        lines.append("")
    if not msg["signals"]:
        lines += ["Keine Signale bekannt (nur Rohbytes).", ""]
        return lines
    lines += ["| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |",
              "| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |"]
    for sig in sorted(msg["signals"], key=lambda s: s["start"]):
        lines.append("| " + " | ".join([
            f"`{sig['name']}`{mux(sig)}", str(sig["start"]), str(sig["len"]),
            "Intel" if sig["intel"] else "Motorola", sig_type(sig), sig["factor"], sig["offset"],
            f"{sig['min']} … {sig['max']}", cell(sig["unit"]), cell(sig["rx"]),
            cell(sig["comment"]), cell(sig["values"])]) + " |")
    lines.append("")
    return lines


def main() -> int:
    msgs, network = parse()
    sent = [m for m in msgs if m["sender"] in VCU_SENDS]
    received = [m for m in msgs if m["sender"] not in VCU_SENDS]
    out = [
        "# Porsche 986 CAN (CAN1)",
        "",
        "> **Gespiegelt aus der DBC, nicht von Hand ändern.** Masterquelle: "
        "[porsche_986_996_compiled_working.dbc](porsche_986_996_compiled_working.dbc). "
        "Erkenntnisse aus dem Reverse Engineering (Signale, Kommentare, `OPEN (986 #n)`, "
        "`GenMsgCycleTime`) zuerst in die DBC eintragen, dann `python tools/generate_986_can_md.py` "
        "ausführen. Zuordnung zu VCU-Feldern und UI: [signalliste.md](signalliste.md).",
        "",
        f"{len(msgs)} Botschaften, {sum(len(m['signals']) for m in msgs)} Signale. "
        "Zykluszeiten aus dem Standmitschnitt `can_logs/986_log_stand.csv` (Originalfahrzeug); "
        "die VCU sendet jede DME-Botschaft alle 30 ms.",
        "",
        "## Übersicht",
        "",
        "| ID | Botschaft | Sender | VCU | Zyklus | Signale | Offen |",
        "| --- | --- | --- | --- | --- | ---: | --- |",
    ]
    for msg in sent + received:
        role = "sendet (statt DME)" if msg in sent else "empfängt"
        opens = ", ".join(f"#{n}" for n, _ in OPEN_RE.findall(msg["comment"])) or "-"
        cycle = f"{msg['cycle']} ms" if msg["cycle"] else "-"
        out.append(f"| 0x{msg['id']:03X} | {msg['name']} | {msg['sender']} | {role} | {cycle} | "
                   f"{len(msg['signals'])} | {opens} |")
    out += ["", "## Offene Punkte", "",
            "Arbeitsliste fürs Reverse Engineering (aus den `OPEN`-Kommentaren der DBC).", ""]
    items = [(int(n), "allgemein", t) for n, t in OPEN_RE.findall(network)]
    for msg in msgs:
        items += [(int(n), f"0x{msg['id']:03X}", t) for n, t in OPEN_RE.findall(msg["comment"])]
    for num, where, text in sorted(items):
        out.append(f"- **#{num}** ({where}): {text.strip()}")
    out += ["", "## 1. Von der VCU zum Fahrzeug gesendet (DME-Emulation)", ""]
    for msg in sent:
        out += message_block(msg)
    out += ["## 2. Vom Fahrzeug empfangen", ""]
    for msg in received:
        out += message_block(msg)
    OUT.write_text("\n".join(out).rstrip() + "\n", encoding="utf-8", newline="\n")
    print(f"{OUT.relative_to(ROOT)}: {len(msgs)} messages")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
