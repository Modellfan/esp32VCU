"""Hardening tests for the VCU touch PWA (pwa/dist served like the VCU).

Run: python -m pytest pwa/tests -q
"""

from __future__ import annotations

import gzip
import json
import re
import time
import urllib.request

import pytest
from PIL import Image

from conftest import PWA, VIEWPORT, enable_engineering, open_app, screenshot

import build

SECTIONS = ["home", "vehicle", "coolant", "styleTest", "ventilation", "stopwatch", "cluster", "adjust"]

VIEW_CHECK_JS = """
() => {
  const root = document.getElementById('screenRoot');
  const visible = el => !!(el.offsetWidth || el.offsetHeight || el.getClientRects().length)
      && getComputedStyle(el).visibility !== 'hidden';
  const text = root.innerText || '';
  const bad = (text.match(/undefined|\\bNaN\\b|\\[object Object\\]|Infinity/g) || []);
  const brokenImages = Array.from(root.querySelectorAll('img'))
      .filter(img => visible(img) && !img.hidden && img.complete && img.naturalWidth === 0)
      .map(img => img.getAttribute('src'));
  const se = document.scrollingElement;
  return {
    bad: bad,
    brokenImages: brokenImages,
    scrollW: se.scrollWidth, scrollH: se.scrollHeight,
    section: (document.querySelector('.l1-icon.active') || {}).dataset?.section || ''
  };
}
"""


def visible_tabs(page):
    return page.evaluate(
        "() => Array.from(document.querySelectorAll('.tab')).map((t, i) => "
        "({i: Number(t.dataset.tabIndex), shown: !!(t.offsetWidth || t.offsetHeight) && !t.hidden && !t.disabled}))"
        ".filter(t => t.shown).map(t => t.i)"
    )


def goto_section(page, section):
    page.click(f".l1-icon[data-section='{section}']")
    page.wait_for_timeout(250)


def wait_until(predicate, timeout_s=10.0, step_s=0.2):
    end = time.time() + timeout_s
    while time.time() < end:
        value = predicate()
        if value:
            return value
        time.sleep(step_s)
    return predicate()


def is_offline_marked(page):
    return page.evaluate("() => document.getElementById('topbarTime').classList.contains('is-api-offline')")


# --------------------------------------------------------------------------- build


def test_build_within_spiffs_budget(build_report):
    assert build_report["total_bytes"] <= build.BUDGET_BYTES
    assert build_report["total_bytes"] < build_report["partition_bytes"] * 0.75


def test_spiffs_names_valid(build_report):
    for item in build_report["files"]:
        assert len(item["path"]) <= build.SPIFFS_MAX_NAME, item
        assert re.match(r"^/[a-z0-9._/-]+$", item["path"]), item


def test_sw_precache_matches_dist(build_report):
    sw = gzip.decompress((build.DIST / "sw.js.gz").read_bytes()).decode("utf-8")
    assert build_report["version"] in sw
    precache = json.loads(re.search(r"const PRECACHE = (\[.*?\]);", sw).group(1))
    for url in precache:
        if url == "./":
            continue
        rel = url.split("?", 1)[0]
        assert (build.DIST / rel).exists() or (build.DIST / (rel + ".gz")).exists(), url


def test_manifest_and_icons():
    manifest = json.loads((PWA / "src" / "manifest.json").read_text(encoding="utf-8"))
    for key in ("name", "short_name", "start_url", "scope", "display", "icons", "background_color", "theme_color"):
        assert manifest.get(key), key
    assert manifest["display"] in ("fullscreen", "standalone")
    purposes = set()
    for icon in manifest["icons"]:
        with Image.open(PWA / "src" / icon["src"]) as im:
            assert f"{im.width}x{im.height}" == icon["sizes"], icon
        purposes.add(icon.get("purpose", "any"))
    assert {"any", "maskable"} <= purposes
    sizes = {icon["sizes"] for icon in manifest["icons"]}
    assert {"192x192", "512x512"} <= sizes



# --------------------------------------------------------------------------- serving


def test_static_served_gzip_like_vcu(server, build_report):
    for item in build_report["files"]:
        path = item["path"][:-3] if item["path"].endswith(".gz") else item["path"]
        request = urllib.request.Request(server + path, headers={"Accept-Encoding": "gzip"})
        with urllib.request.urlopen(request, timeout=5) as response:
            assert response.status == 200, path
            if item["path"].endswith(".gz"):
                assert response.headers.get("Content-Encoding") == "gzip", path


# --------------------------------------------------------------------------- UI


def test_boot_without_errors(page, server):
    start = time.time()
    open_app(page, server)
    assert time.time() - start < 4.0, "splash/boot too slow"
    page.wait_for_timeout(1500)
    assert not page.probe.errors
    assert not page.probe.console_errors
    assert page.evaluate("() => document.title") == "eBoxster VCU"
    assert page.evaluate("() => !!document.querySelector('link[rel=manifest]')")


def test_every_section_and_tab_renders(page, server):
    open_app(page, server)
    enable_engineering(page)
    page.wait_for_timeout(1200)
    for section in SECTIONS:
        goto_section(page, section)
        tabs = visible_tabs(page) or [None]
        for tab in tabs:
            if tab is not None:
                page.click(f".tab[data-tab-index='{tab}']")
                page.wait_for_timeout(1300)  # at least one poll with this view's fields
            result = page.evaluate(VIEW_CHECK_JS)
            label = f"{section}-{tab}"
            screenshot(page, f"view-{label}")
            assert result["section"] == section, label
            assert not result["bad"], (label, result["bad"])
            assert not result["brokenImages"], (label, result["brokenImages"])
            assert result["scrollW"] <= VIEWPORT["width"], (label, result)
            assert result["scrollH"] <= VIEWPORT["height"], (label, result)
    assert not page.probe.errors, page.probe.errors
    assert not page.probe.console_errors, page.probe.console_errors


def test_polling_rate_about_one_per_second(page, server, api):
    open_app(page, server)
    before = api.stats()["live"]
    page.wait_for_timeout(5000)
    count = api.stats()["live"] - before
    assert 3 <= count <= 7, count
    assert api.stats()["max_concurrent_live"] <= 1


@pytest.mark.parametrize("mode", ["down", "http500", "badjson", "array"])
def test_connection_faults_show_warning_and_recover(page, server, api, mode):
    open_app(page, server)
    page.wait_for_timeout(1500)
    assert not is_offline_marked(page)
    api.fault(mode)
    assert wait_until(lambda: is_offline_marked(page), 12), f"no offline warning for {mode}"
    status_ok = page.evaluate("() => document.getElementById('screenRoot').hidden === false")
    assert status_ok
    api.fault("ok")
    assert wait_until(lambda: not is_offline_marked(page), 12), f"no recovery after {mode}"
    assert not page.probe.errors, page.probe.errors


def test_single_failure_does_not_flicker(page, server, api):
    open_app(page, server)
    page.wait_for_timeout(1500)
    api.fault("http500")
    page.wait_for_timeout(700)  # roughly one failed poll
    api.fault("ok")
    flagged = False
    for _ in range(15):
        flagged = flagged or is_offline_marked(page)
        page.wait_for_timeout(100)
    assert not flagged


def test_vcu_down_at_startup(page, server, api):
    api.fault("down")
    open_app(page, server)
    assert wait_until(lambda: is_offline_marked(page), 8)
    api.fault("ok")
    assert wait_until(lambda: not is_offline_marked(page), 12)
    assert not page.probe.errors


def test_slow_vcu_times_out_without_pileup(page, server, api):
    open_app(page, server)
    page.wait_for_timeout(1200)
    api.fault("slow", 6000)
    assert wait_until(lambda: is_offline_marked(page), 15)
    page.wait_for_timeout(4000)
    assert api.stats()["max_concurrent_live"] <= 2  # one hung + at most one new after timeout
    api.fault("ok")
    assert wait_until(lambda: not is_offline_marked(page), 20)
    assert not page.probe.errors


def test_backoff_while_unreachable(page, server, api):
    open_app(page, server)
    page.wait_for_timeout(1200)
    api.fault("down")
    page.wait_for_timeout(2000)
    start = api.stats()["live"]
    page.wait_for_timeout(10000)
    attempts = api.stats()["live"] - start
    assert attempts <= 5, attempts  # 1 s, 2 s, 4 s, 5 s ... instead of 10


@pytest.mark.parametrize("mode", ["garbage", "empty"])
def test_bad_values_never_break_rendering(page, server, api, mode):
    open_app(page, server)
    enable_engineering(page)
    api.fault(mode)
    for section in SECTIONS:
        goto_section(page, section)
        for tab in visible_tabs(page) or [None]:
            if tab is not None:
                page.click(f".tab[data-tab-index='{tab}']")
            page.wait_for_timeout(1100)
            result = page.evaluate(VIEW_CHECK_JS)
            assert result["scrollW"] <= VIEWPORT["width"], (section, tab, result)
    assert not page.probe.errors, page.probe.errors


def test_markup_in_values_is_escaped(page, server, api):
    open_app(page, server)
    api.fault("garbage")
    page.wait_for_timeout(2500)
    injected = page.evaluate("() => document.querySelectorAll('#screenRoot b').length")
    assert injected == 0


def find_control_row(page, key):
    for section in SECTIONS:
        goto_section(page, section)
        for tab in visible_tabs(page) or [None]:
            if tab is not None:
                page.click(f".tab[data-tab-index='{tab}']")
                page.wait_for_timeout(150)
            row = page.query_selector(f"tr[data-control-key='{key}']")
            if row and row.is_visible():
                return row
    return None


def set_slider(page, value):
    page.evaluate(
        "v => { const s = document.getElementById('touchBatterySettingSlider');"
        " s.value = String(v); s.dispatchEvent(new Event('input', {bubbles: true})); }",
        value,
    )


def test_set_command_reaches_vcu(page, server, api):
    open_app(page, server)
    enable_engineering(page)
    row = find_control_row(page, "target_charge_pct")
    assert row, "target charge control not found"
    row.click()
    page.wait_for_selector("#touchBatterySettingSlider", state="visible")
    set_slider(page, 85)
    assert wait_until(lambda: any(s["key"] == "target_charge_pct" for s in api.stats()["sets"]), 5)
    sets = [s for s in api.stats()["sets"] if s["key"] == "target_charge_pct"]
    assert sets[-1]["value"] == "85"
    assert not page.probe.errors


def test_rapid_slider_changes_coalesce(page, server, api):
    open_app(page, server)
    enable_engineering(page)
    row = find_control_row(page, "digital_pot_pct")
    assert row
    row.click()
    page.wait_for_selector("#touchBatterySettingSlider", state="visible")
    api.fault("ok", 300)  # slow VCU makes queued values pile up
    for value in range(10, 70, 2):
        set_slider(page, value)
        page.wait_for_timeout(30)
    set_slider(page, 70)
    assert wait_until(lambda: any(s["key"] == "digital_pot_pct" and s["value"] == "70"
                                  for s in api.stats()["sets"]), 8)
    sets = [s for s in api.stats()["sets"] if s["key"] == "digital_pot_pct"]
    assert len(sets) <= 6, sets
    assert sets[-1]["value"] == "70"


def test_set_value_does_not_jump_back(page, server, api):
    open_app(page, server)
    page.wait_for_timeout(1200)
    goto_section(page, "coolant")
    page.wait_for_timeout(800)
    api.fault("stale_set")  # /api/live keeps reporting the old value
    page.click(".coolant-fan-button[data-fan-level='2']")
    samples = []
    for _ in range(10):
        samples.append(page.evaluate(
            "() => (document.querySelector('.coolant-fan-button.is-active, .coolant-fan-button[aria-pressed=true]')"
            " || {}).dataset?.fanLevel || ''"))
        page.wait_for_timeout(100)
    assert any(s["key"] == "coolant_fan_level" and s["value"] == "2" for s in api.stats()["sets"])
    assert all(s in ("2", "") for s in samples), samples


def test_set_failure_is_reported(page, server, api):
    warnings = []
    page.on("console", lambda msg: warnings.append(msg.text) if msg.type == "warning" else None)
    open_app(page, server)
    page.wait_for_timeout(1200)
    goto_section(page, "coolant")
    page.wait_for_timeout(500)
    api.fault("http500")
    page.click(".coolant-fan-button[data-fan-level='1']")
    for _ in range(30):
        if any("set failed" in w for w in warnings):
            break
        page.wait_for_timeout(200)
    assert any("set failed" in w for w in warnings), warnings
    assert not page.probe.errors


def test_polling_pauses_when_hidden(page, server, api):
    open_app(page, server)
    page.wait_for_timeout(1500)
    page.evaluate(
        "() => { Object.defineProperty(document, 'hidden', {configurable: true, get: () => true});"
        " document.dispatchEvent(new Event('visibilitychange')); }"
    )
    page.wait_for_timeout(1500)
    start = api.stats()["live"]
    page.wait_for_timeout(4000)
    assert api.stats()["live"] - start == 0
    page.evaluate(
        "() => { Object.defineProperty(document, 'hidden', {configurable: true, get: () => false});"
        " document.dispatchEvent(new Event('visibilitychange')); }"
    )
    assert wait_until(lambda: api.stats()["live"] > start, 4)


def test_long_run_is_stable(page, server, api):
    """60 s of polling: no errors, bounded DOM and heap growth, steady request rate."""
    open_app(page, server)
    goto_section(page, "vehicle")
    page.wait_for_timeout(3000)
    probe = "() => ({nodes: document.getElementsByTagName('*').length," \
            " heap: performance.memory ? performance.memory.usedJSHeapSize : 0})"
    first = page.evaluate(probe)
    start = api.stats()["live"]
    for index in range(6):
        page.wait_for_timeout(10000)
        if index == 2:
            goto_section(page, "ventilation")
        if index == 4:
            goto_section(page, "vehicle")
    last = page.evaluate(probe)
    requests = api.stats()["live"] - start
    assert 45 <= requests <= 75, requests
    assert last["nodes"] <= first["nodes"] * 1.3 + 50, (first, last)
    if first["heap"]:
        assert last["heap"] <= first["heap"] * 3 + 5_000_000, (first, last)
    assert not page.probe.errors


# --------------------------------------------------------------------------- service worker


def test_service_worker_offline_reload(browser, server, api):
    context = browser.new_context(viewport=VIEWPORT)
    page = context.new_page()
    errors = []
    page.on("pageerror", lambda exc: errors.append(str(exc)))
    open_app(page, server)
    assert wait_until(lambda: page.evaluate("() => window.VcuPwa && window.VcuPwa.registered"), 8)
    if not page.evaluate("() => !!navigator.serviceWorker.controller"):
        page.reload()
        page.wait_for_selector("#screenRoot:not([hidden])", timeout=8000)
    page.wait_for_function("() => navigator.serviceWorker.controller !== null", timeout=10000)
    page.wait_for_timeout(1500)  # let runtime caching finish
    api.fault("all_down")  # VCU completely gone (static files and API)
    context.set_offline(True)
    page.reload()
    page.wait_for_selector("#screenRoot:not([hidden])", timeout=10000)
    assert wait_until(lambda: is_offline_marked(page), 10)
    broken = page.evaluate(
        "() => Array.from(document.querySelectorAll('#screenRoot img'))"
        ".filter(i => i.offsetWidth && i.complete && i.naturalWidth === 0).map(i => i.src)")
    assert not broken, broken
    context.set_offline(False)
    api.fault("ok")
    assert wait_until(lambda: not is_offline_marked(page), 15)
    assert not errors, errors
    context.close()


def test_insecure_origin_skips_service_worker(browser, server, api):
    """On plain HTTP from a non-local address (like the VCU AP) the app must still run."""
    import socket
    host_ip = socket.gethostbyname(socket.gethostname())
    if host_ip.startswith("127."):
        pytest.skip("no LAN address to emulate an insecure origin")
    import serve
    srv, base = serve.start(build.DIST, host="0.0.0.0")
    port = srv.server_address[1]
    context = browser.new_context(viewport=VIEWPORT)
    page = context.new_page()
    errors = []
    page.on("pageerror", lambda exc: errors.append(str(exc)))
    try:
        page.goto(f"http://{host_ip}:{port}/")
        page.wait_for_selector("#screenRoot:not([hidden])", timeout=8000)
        page.wait_for_timeout(1500)
        assert page.evaluate("() => window.VcuPwa.reason") == "insecure-context"
        assert not is_offline_marked(page)
        assert not errors
    finally:
        context.close()
        srv.shutdown()


# --------------------------------------------------------------------------- offline cache (Android radio)

ASSET_REF = re.compile(r"""["'(]((?:a|i|e|css|js)/[A-Za-z0-9._/-]+\.(?:svg|png|webp|css|js|json))(\?v=[0-9a-f]+)?["')]""")


def test_all_asset_references_are_versioned(build_report):
    """Every asset URL carries its content hash, so the browser may cache it forever."""
    versions = build_report["asset_versions"]
    for path in build.DIST.rglob("*.gz"):
        if path.name == "sw.js.gz":
            continue
        text = gzip.decompress(path.read_bytes()).decode("utf-8", "replace")
        for ref, version in ASSET_REF.findall(text):
            assert version == "?v=" + versions[ref], (path.name, ref, version)
    layout = json.loads(gzip.decompress((build.DIST / "e" / "layout.json.gz").read_bytes()))
    for image in layout["images"]:
        if image.get("file"):
            assert "?v=" in image["file"], image


def test_versioned_assets_cached_for_a_year(server, build_report):
    ref, version = next(iter(build_report["asset_versions"].items()))
    with urllib.request.urlopen(f"{server}/{ref}?v={version}", timeout=5) as response:
        assert "immutable" in response.headers.get("Cache-Control", "")
    with urllib.request.urlopen(server + "/", timeout=5) as response:
        assert response.headers.get("Cache-Control") == "no-cache"


def lan_ip():
    import socket
    ip = socket.gethostbyname(socket.gethostname())
    return None if ip.startswith("127.") else ip


def wait_cache_complete(page, timeout_s=30):
    end = time.time() + timeout_s
    status = None
    while time.time() < end:
        status = page.evaluate(
            "() => { window.VcuPwa && window.VcuPwa.refreshCache(); return window.VcuPwa && window.VcuPwa.cache; }")
        if status and status.get("total") and status["cached"] == status["total"]:
            return status
        page.wait_for_timeout(500)
    return status


def test_cache_status_shown_in_network_popover(browser, server, api):
    context = browser.new_context(viewport=VIEWPORT)
    page = context.new_page()
    open_app(page, server)
    status = wait_cache_complete(page)
    assert status and status["cached"] == status["total"] > 30, status
    goto_section(page, "vehicle")
    page.click("#topbarTime")
    page.wait_for_selector("[data-offline-cache]")
    text = page.inner_text("[data-offline-cache]")
    assert text.startswith("aktiv"), text
    context.close()


def test_android_radio_chrome_flag_offline(browser, tmp_path, api):
    """Radio scenario: Chrome with 'Insecure origins treated as secure' for the VCU address.

    Plain http on a LAN address, cache filled once, then the VCU disappears completely:
    the app must start from the cache with every image present.
    """
    ip = lan_ip()
    if not ip:
        pytest.skip("no LAN address to emulate the VCU access point")
    import serve
    srv, _ = serve.start(build.DIST, host="0.0.0.0")
    origin = f"http://{ip}:{srv.server_address[1]}"
    try:
        if True:
            context = browser.browser_type.launch_persistent_context(
                str(tmp_path / "profile"), viewport=VIEWPORT, channel="chromium",
                args=[f"--unsafely-treat-insecure-origin-as-secure={origin}"])
            page = context.pages[0] if context.pages else context.new_page()
            errors = []
            page.on("pageerror", lambda exc: errors.append(str(exc)))
            page.goto(origin + "/")
            page.wait_for_selector("#screenRoot:not([hidden])", timeout=10000)
            assert page.evaluate("() => window.isSecureContext")
            status = wait_cache_complete(page, 40)
            assert status and status["cached"] == status["total"], status

            serve.FAULT["mode"] = "all_down"  # VCU switched off: no files, no API
            page.reload()
            page.wait_for_selector("#screenRoot:not([hidden])", timeout=15000)
            page.evaluate("localStorage.setItem('touchEngineeringMode','1')")
            page.reload()
            page.wait_for_selector("#screenRoot:not([hidden])", timeout=15000)
            assert wait_until(lambda: is_offline_marked(page), 10)

            broken = {}
            for section in SECTIONS:
                page.click(f".l1-icon[data-section='{section}']")
                for tab in visible_tabs(page) or [None]:
                    if tab is not None:
                        page.click(f".tab[data-tab-index='{tab}']")
                    page.wait_for_timeout(600)
                    missing = page.evaluate(
                        "() => Array.from(document.querySelectorAll('#screenRoot img'))"
                        ".filter(i => i.getAttribute('src') && i.complete && i.naturalWidth === 0)"
                        ".map(i => i.getAttribute('src'))")
                    if missing:
                        broken[f"{section}-{tab}"] = missing
            screenshot(page, "offline-radio-last-view")
            assert not broken, broken
            assert not errors, errors
            context.close()
    finally:
        serve.FAULT["mode"] = "ok"
        srv.shutdown()
