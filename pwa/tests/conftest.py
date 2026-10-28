from __future__ import annotations

import json
import sys
import urllib.request
from pathlib import Path

import pytest
from playwright.sync_api import sync_playwright

PWA = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PWA / "tools"))

import build  # noqa: E402
import serve  # noqa: E402

RESULTS = PWA / "test-results"
VIEWPORT = {"width": 1024, "height": 600}


@pytest.fixture(scope="session")
def build_report():
    report = build.build(build.BUDGET_BYTES)
    assert not report["errors"], report["errors"]
    return report


@pytest.fixture(scope="session")
def server(build_report):
    srv, base = serve.start(build.DIST)
    yield base
    srv.shutdown()


class Api:
    def __init__(self, base: str):
        self.base = base

    def _get(self, path: str):
        with urllib.request.urlopen(self.base + path, timeout=5) as response:
            return json.loads(response.read().decode("utf-8"))

    def fault(self, mode: str, delay_ms: int = 0):
        return self._get(f"/__test/fault?mode={mode}&delay_ms={delay_ms}")

    def stats(self):
        return self._get("/__test/stats")

    def reset(self):
        return self._get("/__test/reset")


@pytest.fixture()
def api(server):
    client = Api(server)
    client.reset()
    yield client
    client.reset()


@pytest.fixture(scope="session")
def browser():
    with sync_playwright() as p:
        b = p.chromium.launch()
        yield b
        b.close()


class PageProbe:
    """Collects console errors and uncaught exceptions of a page."""

    def __init__(self, page):
        self.page = page
        self.errors: list[str] = []
        self.console_errors: list[str] = []
        page.on("pageerror", lambda exc: self.errors.append(str(exc)))
        page.on("console", self._console)

    def _console(self, msg):
        if msg.type == "error":
            text = msg.text
            # Expected while the mock VCU is deliberately unreachable.
            if "Failed to load resource" in text or "ERR_" in text:
                return
            self.console_errors.append(text)


@pytest.fixture()
def page(browser, server, api):
    context = browser.new_context(viewport=VIEWPORT, device_scale_factor=1, service_workers="block")
    pg = context.new_page()
    probe = PageProbe(pg)
    pg.probe = probe
    yield pg
    context.close()


def open_app(page, base: str, query: str = ""):
    page.goto(base + "/" + query)
    page.wait_for_selector("#screenRoot:not([hidden])", timeout=8000)
    return page


def enable_engineering(page):
    page.evaluate("window.localStorage.setItem('touchEngineeringMode','1')")
    page.reload()
    page.wait_for_selector("#screenRoot:not([hidden])", timeout=8000)


def screenshot(page, name: str):
    RESULTS.mkdir(exist_ok=True)
    page.screenshot(path=str(RESULTS / f"{name}.png"))
