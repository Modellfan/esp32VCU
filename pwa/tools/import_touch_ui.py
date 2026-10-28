#!/usr/bin/env python3
"""Import the touch UI from data/ into pwa/src/ (one-way, re-runnable).

data/touch.html and data/touch/ stay untouched; they remain the master copy of
the legacy UI and of the full-resolution images. This script

* copies HTML/CSS/JS/SVG into short, SPIFFS-safe paths,
* converts the large PNG/JPG images to WebP at display resolution,
* rewrites every asset reference to the new relative paths.

Hand-made PWA changes live in files this script does NOT overwrite
(index.html, js/app.js, js/net.js, sw.js, manifest.json) unless --force is given.
"""

from __future__ import annotations

import argparse
import json
import re
import shutil
from pathlib import Path

from PIL import Image

REPO = Path(__file__).resolve().parents[2]
DATA = REPO / "data"
TOUCH = DATA / "touch"
SRC = REPO / "pwa" / "src"

# (source path relative to data/touch, target path relative to pwa/src, max edge px or None)
IMAGES = [
    ("img/half_front.png", "i/front.webp", None),
    ("img/intro.jpg", "i/intro.webp", None),
    ("img/battery-reference.png", "i/battery.webp", 1024),
    ("img/charger-reference.png", "i/charger.webp", 1024),
    ("img/drive-unit-reference.png", "i/drive.webp", 1024),
    ("energy-flow/img/background car 2.png", "e/bg-car-2.webp", 1280),
    ("energy-flow/img/background car.png", "e/bg-car.webp", 1280),
    ("energy-flow/img/battery.png", "e/battery.webp", 512),
    ("energy-flow/img/charge port.png", "e/charge-port.webp", 512),
    ("energy-flow/img/charger.png", "e/charger.webp", 512),
    ("energy-flow/img/dcdc.png", "e/dcdc.webp", 512),
    ("energy-flow/img/msd.png", "e/msd.webp", 512),
    ("energy-flow/img/tesla.png", "e/tesla.webp", None),
]

SVG_RENAMES = {"active-cabin-ventilation.svg": "vent.svg"}

# Literal string replacements applied to HTML/CSS/JS (order matters: specific first).
PATH_REWRITES = [
    ("/touch/energy-flow/energy-flow-layout.json", "e/layout.json"),
    ("/touch/energy-flow/img/", "e/"),
    ("/touch/img/half_front.png", "i/front.webp"),
    ("/touch/img/intro.jpg", "i/intro.webp"),
    ("/touch/img/battery-reference.png", "i/battery.webp"),
    ("/touch/img/charger-reference.png", "i/charger.webp"),
    ("/touch/img/drive-unit-reference.png", "i/drive.webp"),
    ("/touch/assets/active-cabin-ventilation.svg", "a/vent.svg"),
    ("/touch/assets/", "a/"),
    ("/touch/css/", "css/"),
    ("/touch/js/", "js/"),
]

ENERGY_FILE_RENAMES = {
    "background car 2.png": "bg-car-2.webp",
    "background car.png": "bg-car.webp",
    "battery.png": "battery.webp",
    "charge port.png": "charge-port.webp",
    "charger.png": "charger.webp",
    "dcdc.png": "dcdc.webp",
    "msd.png": "msd.webp",
    "tesla.png": "tesla.webp",
}

PROTECTED = {"index.html", "js/app.js", "js/net.js", "sw.js", "manifest.json"}


def rewrite_paths(text: str) -> str:
    for old, new in PATH_REWRITES:
        text = text.replace(old, new)
    return text


def write_text(target: Path, text: str, force: bool) -> None:
    rel = target.relative_to(SRC).as_posix()
    if rel in PROTECTED and target.exists() and not force:
        print(f"skip (hand-edited): {rel}")
        return
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text(text, encoding="utf-8", newline="\n")
    print(f"text  {rel}")


def convert_image(source: Path, target: Path, max_edge: int | None) -> None:
    target.parent.mkdir(parents=True, exist_ok=True)
    with Image.open(source) as im:
        im.load()
        if max_edge and max(im.size) > max_edge:
            scale = max_edge / max(im.size)
            im = im.resize((round(im.width * scale), round(im.height * scale)), Image.LANCZOS)
        has_alpha = im.mode in ("RGBA", "LA") or "transparency" in im.info
        im = im.convert("RGBA" if has_alpha else "RGB")
        im.save(target, "WEBP", quality=78, method=6)
    print(f"image {target.relative_to(SRC).as_posix()}  {source.stat().st_size} -> {target.stat().st_size} B")


def make_icons() -> None:
    with Image.open(DATA / "app.png") as im:
        im = im.convert("RGBA")
        for size, name, pad in [(192, "a/icon-192.png", 0), (512, "a/icon-512.png", 0),
                                (512, "a/icon-mask.png", 0.12), (180, "a/apple-touch.png", 0)]:
            canvas = Image.new("RGBA", (size, size), (0, 0, 0, 255))
            inner = round(size * (1 - 2 * pad))
            scaled = im.resize((inner, inner), Image.LANCZOS)
            offset = (size - inner) // 2
            canvas.alpha_composite(scaled, (offset, offset))
            target = SRC / name
            target.parent.mkdir(parents=True, exist_ok=True)
            canvas.convert("RGB").quantize(colors=128, method=Image.Quantize.MEDIANCUT).save(target, "PNG", optimize=True)
            print(f"icon  {name}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--force", action="store_true", help="also overwrite hand-edited PWA files")
    args = parser.parse_args()

    write_text(SRC / "index.html", rewrite_paths((DATA / "touch.html").read_text(encoding="utf-8")), args.force)
    for sub in ("css", "js"):
        for source in sorted((TOUCH / sub).glob("*.*")):
            write_text(SRC / sub / source.name, rewrite_paths(source.read_text(encoding="utf-8")), args.force)

    for source in sorted((TOUCH / "assets").glob("*.svg")):
        target = SRC / "a" / SVG_RENAMES.get(source.name, source.name)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
        print(f"svg   {target.relative_to(SRC).as_posix()}")

    layout = json.loads((TOUCH / "energy-flow" / "energy-flow-layout.json").read_text(encoding="utf-8"))
    layout["assetsBasePath"] = "e/"
    for image in layout.get("images", []):
        image["file"] = ENERGY_FILE_RENAMES.get(image.get("file", ""), image.get("file", ""))
    write_text(SRC / "e" / "layout.json", json.dumps(layout, indent=2, ensure_ascii=False) + "\n", args.force)

    for rel_source, rel_target, max_edge in IMAGES:
        convert_image(TOUCH / rel_source, SRC / rel_target, max_edge)
    make_icons()

    leftovers = []
    for path in SRC.rglob("*"):
        if path.suffix in {".html", ".css", ".js", ".json"}:
            text = path.read_text(encoding="utf-8")
            leftovers += [f"{path.relative_to(SRC)}: {m}" for m in re.findall(r"/touch/[^\"'\s)]+", text)]
    if leftovers:
        print("WARNING unresolved /touch/ references:")
        print("\n".join(leftovers))


if __name__ == "__main__":
    main()
