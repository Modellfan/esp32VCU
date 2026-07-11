import json
import struct
from pathlib import Path


def png_size(png_bytes: bytes):
    if len(png_bytes) < 24 or png_bytes[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError("icon.png is not a valid PNG")
    width = struct.unpack(">I", png_bytes[16:20])[0]
    height = struct.unpack(">I", png_bytes[20:24])[0]
    return width, height


def write_ico_from_png(png_bytes: bytes, dst: Path):
    width, height = png_size(png_bytes)
    w = 0 if width >= 256 else width
    h = 0 if height >= 256 else height

    # ICO header + one directory entry + PNG payload
    header = struct.pack("<HHH", 0, 1, 1)
    entry = struct.pack(
        "<BBBBHHII",
        w,
        h,
        0,
        0,
        1,
        32,
        len(png_bytes),
        6 + 16,
    )
    dst.write_bytes(header + entry + png_bytes)


def ensure_assets(icon_src: Path, assets_dir: Path):
    assets_dir.mkdir(parents=True, exist_ok=True)
    png = icon_src.read_bytes()

    # Keep all names expected by favicon/PWA ecosystems.
    copies = [
        "favicon-16x16.png",
        "favicon-32x32.png",
        "apple-touch-icon.png",
        "android-chrome-192x192.png",
        "android-chrome-512x512.png",
    ]
    for name in copies:
        (assets_dir / name).write_bytes(png)

    write_ico_from_png(png, assets_dir / "favicon.ico")

    # Safari pinned tab expects monochrome SVG.
    (assets_dir / "safari-pinned-tab.svg").write_text(
        """<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
  <rect x="8" y="8" width="84" height="84" rx="20" ry="20" fill="black"/>
  <circle cx="50" cy="50" r="20" fill="white"/>
</svg>
""",
        encoding="utf-8",
    )

    manifest = {
        "name": "eBoxster",
        "short_name": "eBoxster",
        "icons": [
            {"src": "/android-chrome-192x192.png", "sizes": "192x192", "type": "image/png"},
            {"src": "/android-chrome-512x512.png", "sizes": "512x512", "type": "image/png"},
        ],
        "theme_color": "#ffffff",
        "background_color": "#ffffff",
        "display": "standalone",
    }
    (assets_dir / "site.webmanifest").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")


def main():
    root = Path(__file__).resolve().parents[1]
    src_icon = root / "data" / "assets" / "icon.png"
    if not src_icon.exists():
        raise FileNotFoundError(f"Missing source icon: {src_icon}")

    # Keep data/ as the single source of truth for filesystem web assets.
    ensure_assets(src_icon, root / "data" / "assets")
    print("ICON_GEN_OK")


if __name__ == "__main__":
    main()
