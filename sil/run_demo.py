#!/usr/bin/env python3
"""Demo: drive cycle through the SIL loop + test run -> sil/results/report.html.

python sil/run_demo.py            (needs only the Python standard library and pytest)
"""

from __future__ import annotations

import html
import subprocess
import sys
import xml.etree.ElementTree as ET
from datetime import datetime
from pathlib import Path

SIL = Path(__file__).resolve().parent
sys.path.insert(0, str(SIL))

from vcu_sil import DriverProfile, EnvironmentModel, Phase, Simulation, VcuSil  # noqa: E402

RESULTS = SIL / "results"

CYCLE = [
    Phase(2.0, direction=1),
    Phase(9.0, pedal=1.0, direction=1),
    Phase(6.0, pedal=0.25, direction=1),
    Phase(8.0, brake=0.9, direction=1),
    Phase(3.0, direction=0),
    Phase(6.0, pedal=0.5, direction=1),
    Phase(6.0, brake=0.5, direction=1),
]


def svg_chart(title: str, t: list[float], series: list[tuple[str, list[float], str]],
              y_min: float | None = None, y_max: float | None = None) -> str:
    w, h, pad_l, pad_r, pad_t, pad_b = 900, 190, 56, 12, 24, 26
    values = [v for _, ys, _ in series for v in ys]
    lo = min(values) if y_min is None else y_min
    hi = max(values) if y_max is None else y_max
    if hi == lo:
        hi = lo + 1
    t0, t1 = t[0], t[-1]

    def x(v):
        return pad_l + (v - t0) / (t1 - t0) * (w - pad_l - pad_r)

    def y(v):
        return pad_t + (hi - v) / (hi - lo) * (h - pad_t - pad_b)

    parts = [f'<svg viewBox="0 0 {w} {h}" role="img" aria-label="{html.escape(title)}">',
             f'<text x="{pad_l}" y="15" class="ct">{html.escape(title)}</text>']
    for frac in (0, 0.5, 1):
        v = lo + (hi - lo) * frac
        parts.append(f'<line x1="{pad_l}" x2="{w - pad_r}" y1="{y(v):.1f}" y2="{y(v):.1f}" class="grid"/>'
                     f'<text x="{pad_l - 6}" y="{y(v) + 4:.1f}" class="ax" text-anchor="end">{v:.4g}</text>')
    step = max(1, round((t1 - t0) / 8))
    tick = 0.0
    while tick <= t1:
        parts.append(f'<text x="{x(tick):.1f}" y="{h - 6}" class="ax" text-anchor="middle">{tick:.0f} s</text>')
        tick += step
    stride = max(1, len(t) // 900)
    for i, (name, ys, color) in enumerate(series):
        pts = " ".join(f"{x(t[j]):.1f},{y(ys[j]):.1f}" for j in range(0, len(t), stride))
        parts.append(f'<polyline points="{pts}" fill="none" stroke="{color}" stroke-width="1.6"/>')
        parts.append(f'<text x="{w - pad_r - 6}" y="{pad_t + 12 + i * 14}" class="lg" text-anchor="end" fill="{color}">{html.escape(name)}</text>')
    parts.append("</svg>")
    return "".join(parts)


def run_tests() -> list[dict]:
    junit = RESULTS / "junit.xml"
    subprocess.run([sys.executable, "-m", "pytest", str(SIL / "tests"), "-q", "-p", "no:cacheprovider",
                    f"--junitxml={junit}"], capture_output=True, text=True)
    rows = []
    for case in ET.parse(junit).getroot().iter("testcase"):
        status = "passed"
        message = ""
        for child in case:
            if child.tag == "skipped":
                status = "xfail" if "xfail" in (child.get("type", "") + child.get("message", "")).lower() or "BEFUND" in child.get("message", "") else "skipped"
                message = child.get("message", "")
            elif child.tag in ("failure", "error"):
                status = "FAILED"
                message = (child.get("message") or "")[:200]
        rows.append({"name": case.get("name"), "status": status, "message": message,
                     "time": float(case.get("time", 0))})
    return rows


def main() -> None:
    RESULTS.mkdir(exist_ok=True)
    model = EnvironmentModel(DriverProfile(CYCLE), direction_sign=-1)
    trace = Simulation(VcuSil(), model).run()
    t = trace.column("t")
    charts = [
        svg_chart("Geschwindigkeit [km/h]", t, [("Modell", trace.column("speed_kmh_true"), "#0f6e56")], 0),
        svg_chart("Leistungsanteil [−1…1]", t, [("cluster_power_percent_max", trace.column("cluster_power_percent_max"), "#534ab7")], -1, 1),
        svg_chart("Drehzahlzeiger [Rohwert]", t, [("cluster_needle_position", trace.column("cluster_needle_position"), "#c2410c")], 0, 8000),
        svg_chart("Batterie", t, [("sdu_udc [V]", trace.column("sdu_udc"), "#185fa5"),
                                  ("sdu_idc [A]", trace.column("sdu_idc"), "#a66300")]),
        svg_chart("Temperaturen [°C]", t, [("Motor", trace.column("sdu_tmpm"), "#b42318"),
                                          ("Kühlkörper", trace.column("sdu_tmphs"), "#a66300"),
                                          ("Kühlmittelanzeige", trace.column("motor2_coolant_temperature"), "#534ab7")]),
        svg_chart("MO5-Verbrauch [µl, 15 bit]", t, [("mo5_verbrauch_ul", trace.column("mo5_verbrauch_ul"), "#5f5e5a")], 0, 32768),
    ]
    tests = run_tests()
    counts = {s: sum(1 for r in tests if r["status"] == s) for s in ("passed", "xfail", "FAILED", "skipped")}
    test_rows = "".join(
        f'<tr class="{r["status"]}"><td>{html.escape(r["name"])}</td><td>{r["status"]}</td>'
        f'<td>{html.escape(r["message"])}</td></tr>' for r in tests)
    acc = (f'0–50 km/h: {trace.last("cluster_acceleration_0_50_last_s"):.2f} s · '
           f'0–100 km/h: {trace.last("cluster_acceleration_0_100_last_s"):.2f} s')
    page = f"""<!doctype html><html lang="de"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1"><title>SIL Kombi</title>
<style>
:root{{--bg:#f6f5f1;--ink:#1d1d1b;--muted:#6b6a64;--line:#d6d3ca;--card:#fff}}
@media (prefers-color-scheme:dark){{:root{{--bg:#161614;--ink:#ecebe6;--muted:#a3a29b;--line:#3a3935;--card:#20201e}}}}
body{{margin:0;background:var(--bg);color:var(--ink);font:14px/1.45 system-ui,"Segoe UI",sans-serif}}
main{{max-width:960px;margin:0 auto;padding:20px 16px}} h1{{font-size:20px;margin:0}} .sub{{color:var(--muted)}}
.card{{background:var(--card);border:1px solid var(--line);border-radius:12px;padding:12px;margin:12px 0}}
svg{{width:100%;height:auto}} .ct{{font-size:13px;font-weight:600;fill:var(--ink)}} .ax{{font-size:11px;fill:var(--muted)}}
.lg{{font-size:12px;font-weight:600}} .grid{{stroke:var(--line)}}
table{{border-collapse:collapse;width:100%;font-size:13px}} td{{border-top:1px solid var(--line);padding:4px 6px;vertical-align:top}}
tr.xfail td:nth-child(2){{color:#a66300;font-weight:600}} tr.FAILED td:nth-child(2){{color:#b42318;font-weight:700}}
tr.passed td:nth-child(2){{color:#2f7d32}}
</style></head><body><main>
<h1>SIL Kombi-Funktion</h1>
<div class="sub">Erzeugt {datetime.now():%Y-%m-%d %H:%M} · Firmware-Code src/cluster_calc.cpp über vcu_app.cpp · Fahrzyklus {model.profile.duration_s:.0f} s · {acc}</div>
<div class="card"><b>Tests:</b> {counts["passed"]} bestanden · {counts["xfail"]} bekannte Befunde (xfail) · {counts["FAILED"]} fehlgeschlagen
<table>{test_rows}</table></div>
{"".join(f'<div class="card">{c}</div>' for c in charts)}
</main></body></html>"""
    out = RESULTS / "report.html"
    out.write_text(page, encoding="utf-8")
    print(f"{out}  ({counts})")


if __name__ == "__main__":
    main()
