#!/usr/bin/env python3
"""Mirrors DBC files into readable Markdown tables with the same layout.

Masters (edit these, never the Markdown):
  documentation/porsche_986_996_compiled_working.dbc -> documentation/porsche_986_messages.md
  documentation/vw_pq.dbc                            -> documentation/vw_pq_messages.md

Reverse engineering results go into the DBC (signals, CM_ comments, "OPEN (986 #n)" notes,
GenMsgCycleTime); then run:

    python tools/generate_can_md.py

Layout of every file: overview (each message links to its signal table), open points,
then one section per message.
"""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DOC = ROOT / "documentation"

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
OPEN_RE = re.compile(r"OPEN \((\w+) #(\d+)\):\s*(.*?)(?=\s*OPEN \(\w+ #|$)")


def parse(dbc: Path):
    messages, order = {}, []
    current = None
    network = ""
    for line in dbc.read_text(encoding="utf-8", errors="replace").splitlines():
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
            if int(m.group(1)) in messages:
                messages[int(m.group(1))]["comment"] = m.group(2)
        elif m := CM_SG_RE.match(line):
            for sig in messages.get(int(m.group(1)), {"signals": []})["signals"]:
                if sig["name"] == m.group(2):
                    sig["comment"] = m.group(3)
        elif m := VAL_RE.match(line):
            pairs = ", ".join(f"{k} = {v}" for k, v in PAIR_RE.findall(m.group(3)))
            for sig in messages.get(int(m.group(1)), {"signals": []})["signals"]:
                if sig["name"] == m.group(2):
                    sig["values"] = pairs
        elif m := CYCLE_RE.match(line):
            if int(m.group(1)) in messages:
                messages[int(m.group(1))]["cycle"] = int(m.group(2))
    return [messages[i] for i in order], network


def cell(text: str) -> str:
    return (text or "-").replace("|", "\\|")


def anchor(msg) -> str:
    return f"msg-0x{msg['id']:03x}"


def sig_type(sig) -> str:
    if sig["len"] == 1 and not sig["signed"]:
        return "Bool"
    return ("S" if sig["signed"] else "U") + str(sig["len"])


def mux(sig) -> str:
    if sig["mux"] == "M":
        return " (Mux)"
    if sig["mux"].startswith("m") and sig["mux"] != "m":
        return f" ({sig['mux']})"
    return ""


def opens_of(text: str):
    return [(tag, int(num), body.strip()) for tag, num, body in OPEN_RE.findall(text)]


def overview(groups) -> list[str]:
    lines = ["## Übersicht", "",
             "| ID | Botschaft | Sender | Rolle | Zyklus | Signale | Offen |",
             "| --- | --- | --- | --- | --- | ---: | --- |"]
    for role, msgs in groups:
        for msg in msgs:
            opens = ", ".join(f"#{n}" for _, n, _ in opens_of(msg["comment"])) or "-"
            cycle = f"{msg['cycle']} ms" if msg["cycle"] else "-"
            lines.append(f"| [0x{msg['id']:03X}](#{anchor(msg)}) | [{msg['name']}](#{anchor(msg)}) | "
                         f"{msg['sender']} | {role or '-'} | {cycle} | {len(msg['signals'])} | {opens} |")
    return lines + [""]


def open_list(msgs, network) -> list[str]:
    items = [(n, "allgemein", t) for _, n, t in opens_of(network)]
    for msg in msgs:
        items += [(n, f"[0x{msg['id']:03X}](#{anchor(msg)})", t) for _, n, t in opens_of(msg["comment"])]
    if not items:
        return []
    lines = ["## Offene Punkte", "", "Arbeitsliste fürs Reverse Engineering (aus den `OPEN`-Kommentaren der DBC).", ""]
    for num, where, text in sorted(items, key=lambda x: x[0]):
        lines.append(f"- **#{num}** ({where}): {text}")
    return lines + [""]


def message_block(msg, cycle_note: str) -> list[str]:
    cycle = f"{msg['cycle']} ms{cycle_note}" if msg["cycle"] else "-"
    comment = OPEN_RE.sub("", msg["comment"]).strip()
    lines = [f'<a id="{anchor(msg)}"></a>', f"### 0x{msg['id']:03X} {msg['name']}", "",
             f"Sender: {msg['sender']} · DLC {msg['dlc']} · Zyklus {cycle} · [zur Übersicht](#übersicht)", ""]
    if comment:
        lines += [comment, ""]
    opens = opens_of(msg["comment"])
    for _, num, text in opens:
        lines.append(f"- **Offen (#{num}):** {text}")
    if opens:
        lines.append("")
    if not msg["signals"]:
        return lines + ["Keine Signale bekannt (nur Rohbytes).", ""]
    lines += ["| Signal | Start | Len | Byteorder | Typ | Faktor | Offset | Bereich | Einheit | Empfänger | Kommentar | Werte |",
              "| --- | ---: | ---: | --- | --- | ---: | ---: | --- | --- | --- | --- | --- |"]
    for sig in sorted(msg["signals"], key=lambda s: s["start"]):
        lines.append("| " + " | ".join([
            f"`{sig['name']}`{mux(sig)}", str(sig["start"]), str(sig["len"]),
            "Intel" if sig["intel"] else "Motorola", sig_type(sig), sig["factor"], sig["offset"],
            f"{sig['min']} … {sig['max']}", cell(sig["unit"]), cell(sig["rx"]),
            cell(sig["comment"]), cell(sig["values"])]) + " |")
    return lines + [""]


def write(out: Path, title: str, intro: list[str], groups, network: str, cycle_note: str = "") -> None:
    msgs = [m for _, ms in groups for m in ms]
    lines = [f"# {title}", ""] + intro + [
        f"{len(msgs)} Botschaften, {sum(len(m['signals']) for m in msgs)} Signale.", ""]
    lines += overview(groups)
    lines += open_list(msgs, network)
    for i, (role, ms) in enumerate(groups, 1):
        if role:
            lines += [f"## {i}. {role[0].upper()}{role[1:]}", ""]
        else:
            lines += ["## Botschaften", ""]
        for msg in ms:
            lines += message_block(msg, cycle_note)
    out.write_text("\n".join(lines).rstrip() + "\n", encoding="utf-8", newline="\n")
    print(f"{out.relative_to(ROOT)}: {len(msgs)} messages, {sum(len(m['signals']) for m in msgs)} signals")


def mirror_note(dbc: str) -> str:
    return (f"> **Gespiegelt aus der DBC, nicht von Hand ändern.** Masterquelle: [{dbc}]({dbc}). "
            "Änderungen zuerst in die DBC eintragen, dann `python tools/generate_can_md.py` ausführen.")


def main() -> int:
    msgs, network = parse(DOC / "porsche_986_996_compiled_working.dbc")
    sent = [m for m in msgs if m["sender"] == "ECU"]  # the VCU replaces the DME on CAN1
    received = [m for m in msgs if m["sender"] != "ECU"]
    write(DOC / "porsche_986_messages.md", "Porsche 986 CAN (CAN1)", [
        mirror_note("porsche_986_996_compiled_working.dbc") +
        " Erkenntnisse aus dem Reverse Engineering: Signale, `CM_`-Kommentare, `OPEN (986 #n)`, "
        "`GenMsgCycleTime`. Zuordnung zu VCU-Feldern und UI: [signalliste.md](signalliste.md).", "",
        "Zykluszeiten aus dem Standmitschnitt `can_logs/986_log_stand.csv` (Originalfahrzeug).", ""],
        [("von der VCU gesendet (statt DME)", sent), ("vom Fahrzeug empfangen", received)], network,
        " (Original)")

    msgs, network = parse(DOC / "vw_pq.dbc")
    write(DOC / "vw_pq_messages.md", "VW PQ CAN (Referenz)", [
        mirror_note("vw_pq.dbc") +
        " Nur Hintergrund zum Abgleich mit den 986-Botschaften (VCU-DEC-013); die VCU nutzt diese Botschaften nicht.", ""],
        [("", sorted(msgs, key=lambda m: m["id"]))], network)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
