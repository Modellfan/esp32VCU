#!/usr/bin/env python3
"""Builds sil/architecture.html from sil/architecture.json with the viewer of
documentation/architecture.html (one viewer, two data sets)."""

from __future__ import annotations

import json
import re
from pathlib import Path

SIL = Path(__file__).resolve().parent
VIEWER = SIL.parent / "documentation" / "architecture.html"
MARK = '<script type="application/json" id="arch-data">'


def main() -> None:
    data = (SIL / "architecture.json").read_text(encoding="utf-8")
    json.loads(data)  # fail early on invalid JSON
    src = VIEWER.read_text(encoding="utf-8")
    start = src.index(MARK) + len(MARK)
    end = src.index("</script>", start)
    out = src[:start] + "\n" + data + src[end:]
    out = out.replace("<title>VCU Architektur</title>", "<title>SIL Architektur</title>")
    out = re.sub(r"<!--\n  Interaktive Architekturkarte der esp32VCU.*?-->",
                 "<!--\n  Architekturkarte der SIL-Umgebung. Erzeugt von sil/make_architecture.py aus\n"
                 "  sil/architecture.json mit dem Viewer aus documentation/architecture.html.\n"
                 "  Details in sil/architecture.json pflegen, dann das Skript ausführen.\n-->",
                 out, flags=re.S)
    out = out.replace('/^(src|lib|pwa|documentation)\\//', '/^(src|lib|pwa|documentation|sil)\\//')
    (SIL / "architecture.html").write_text(out, encoding="utf-8", newline="\n")
    print(SIL / "architecture.html")


if __name__ == "__main__":
    main()
