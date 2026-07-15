#!/usr/bin/env python3
"""Capture and validate a 1024x600 screenshot of the integrated touch UI."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from touch_ui_test_support import DEFAULT_SCREENSHOT, WEB_ROOT, find_free_port, start_server


def run_layout_capture(base_url: str, screenshot_path: Path, phase: str, capture_ms: int | None) -> dict:
    try:
        from playwright.sync_api import sync_playwright
    except ImportError:
        return {
            "ok": False,
            "error": "Playwright is not installed. Run: python -m pip install playwright && python -m playwright install chromium",
        }

    screenshot_path.parent.mkdir(parents=True, exist_ok=True)

    with sync_playwright() as playwright:
        browser = playwright.chromium.launch(headless=True)
        context = browser.new_context(
            viewport={"width": 1024, "height": 600},
            screen={"width": 1024, "height": 600},
            device_scale_factor=1,
            has_touch=True,
        )
        page = context.new_page()
        if phase == "intro":
            page.goto(base_url + "/touch.html", wait_until="domcontentloaded")
            page.wait_for_timeout(capture_ms if capture_ms is not None else 150)
            metrics = page.evaluate(
                """() => {
                    const screen = document.querySelector(".screen");
                    const intro = document.querySelector("#introSplash");
                    const introVideo = document.querySelector(".intro-splash-video");
                    if (!intro) {
                        return { error: "Missing #introSplash element" };
                    }
                    const introStyle = getComputedStyle(intro);
                    const screenStyle = screen ? getComputedStyle(screen) : null;
                    return {
                        viewport_width: window.innerWidth,
                        viewport_height: window.innerHeight,
                        has_intro: !!intro,
                        intro_visible: !!intro && !intro.classList.contains("is-hidden") && introStyle.visibility !== "hidden" && introStyle.opacity !== "0",
                        intro_video_present: !!introVideo,
                        intro_video_ready_state: introVideo ? introVideo.readyState : -1,
                        body_booting: document.body.classList.contains("is-booting"),
                        screen_hidden: !!screen && (screen.hidden || screenStyle.display === "none" || screenStyle.visibility === "hidden"),
                    };
                }"""
            )
        elif phase == "home":
            page.goto(base_url + "/touch.html", wait_until="domcontentloaded")
            page.wait_for_timeout(capture_ms if capture_ms is not None else 3900)
            metrics = page.evaluate(
                """() => {
                    const screen = document.querySelector(".screen");
                    const leftNav = document.querySelector(".left-nav");
                    const topbar = document.querySelector(".topbar");
                    const tablePanel = document.querySelector(".table-panel");
                    const homeStage = document.querySelector("#homeStage");
                    const batteryPanel = document.querySelector(".home-battery-panel");
                    const captionValue = document.querySelector("#homeBatteryCaptionValue");
                    const timeValue = document.querySelector("#homeBatteryTimeValue");
                    const dateValue = document.querySelector("#homeBatteryDateValue");
                    const percentValue = document.querySelector("#homeBatteryPercentValue");
                    const rangeValue = document.querySelector("#homeBatteryRangeValue");
                    const rangeUnit = document.querySelector("#homeBatteryRangeUnit");
                    const heroImage = document.querySelector("#homeHeroImage");
                    const activeNav = document.querySelector(".l1-icon.active");
                    const intro = document.querySelector("#introSplash");

                    if (!screen) {
                        return { error: "Missing .screen root element" };
                    }

                    const screenRect = screen.getBoundingClientRect();
                    const topbarStyle = topbar ? getComputedStyle(topbar) : null;
                    const tableStyle = tablePanel ? getComputedStyle(tablePanel) : null;
                    const imageRect = heroImage ? heroImage.getBoundingClientRect() : null;
                    const batteryRect = batteryPanel ? batteryPanel.getBoundingClientRect() : null;
                    return {
                        screen_width: Math.round(screenRect.width),
                        screen_height: Math.round(screenRect.height),
                        is_home: screen.classList.contains("is-home"),
                        has_left_nav: !!leftNav,
                        topbar_hidden: !topbar || topbar.hidden || topbarStyle.display === "none" || topbarStyle.visibility === "hidden",
                        table_hidden: !tablePanel || tableStyle.display === "none" || tableStyle.visibility === "hidden",
                        home_stage_visible: !!homeStage && !homeStage.hidden,
                        has_battery_panel: !!batteryPanel,
                        active_nav_section: activeNav ? activeNav.dataset.section : "",
                        image_src: heroImage ? heroImage.getAttribute("src") || "" : "",
                        image_width: imageRect ? Math.round(imageRect.width) : 0,
                        image_height: imageRect ? Math.round(imageRect.height) : 0,
                        image_right_gap: imageRect ? Math.round(screenRect.right - imageRect.right) : -1,
                        battery_width: batteryRect ? Math.round(batteryRect.width) : 0,
                        battery_to_image_gap: batteryRect && imageRect ? Math.round(imageRect.left - batteryRect.right) : -1,
                        battery_caption_text: captionValue ? captionValue.textContent || "" : "",
                        battery_time_text: timeValue ? timeValue.textContent || "" : "",
                        battery_date_text: dateValue ? dateValue.textContent || "" : "",
                        battery_percent_text: percentValue ? percentValue.textContent || "" : "",
                        battery_range_text: rangeValue ? rangeValue.textContent || "" : "",
                        battery_range_unit: rangeUnit ? rangeUnit.textContent || "" : "",
                        intro_removed_or_hidden: !intro || intro.classList.contains("is-hidden"),
                    };
                }"""
            )
        elif phase == "coolant":
            page.goto(base_url + "/touch.html", wait_until="domcontentloaded")
            page.wait_for_timeout(3900)
            page.tap('.l1-icon[data-section="coolant"]')
            page.wait_for_timeout(capture_ms if capture_ms is not None else 700)
            metrics = page.evaluate(
                """() => {
                    const screen = document.querySelector(".screen");
                    const topbar = document.querySelector(".topbar");
                    const tablePanel = document.querySelector(".table-panel");
                    const imagePanel = document.querySelector(".image-panel");
                    const coolantStage = document.querySelector("#coolantStage");
                    const activeNav = document.querySelector(".l1-icon.active");
                    const intro = document.querySelector("#introSplash");
                    const topbarStyle = topbar ? getComputedStyle(topbar) : null;
                    const tableStyle = tablePanel ? getComputedStyle(tablePanel) : null;
                    const imageStyle = imagePanel ? getComputedStyle(imagePanel) : null;

                    if (!screen) {
                        return { error: "Missing .screen root element" };
                    }

                    const rect = screen.getBoundingClientRect();
                    return {
                        screen_width: Math.round(rect.width),
                        screen_height: Math.round(rect.height),
                        active_nav_section: activeNav ? activeNav.dataset.section : "",
                        topbar_hidden: !topbar || topbar.hidden || topbarStyle.display === "none" || topbarStyle.visibility === "hidden",
                        table_hidden: !tablePanel || tableStyle.display === "none" || tableStyle.visibility === "hidden",
                        image_hidden: !imagePanel || imageStyle.display === "none" || imageStyle.visibility === "hidden",
                        coolant_stage_visible: !!coolantStage && !coolantStage.hidden,
                        coolant_pipe_count: document.querySelectorAll(".coolant-pipe").length,
                        blue_pipe_count: document.querySelectorAll(".coolant-pipe.blue").length,
                        red_pipe_count: document.querySelectorAll(".coolant-pipe.red").length,
                        gray_pipe_count: document.querySelectorAll(".coolant-pipe.gray").length,
                        coolant_title: (document.querySelector(".coolant-system-copy h2") || {}).textContent || "",
                        intro_removed_or_hidden: !intro || intro.classList.contains("is-hidden"),
                    };
                }"""
            )
        elif phase == "vehicle-battery":
            page.goto(base_url + "/touch.html", wait_until="domcontentloaded")
            page.wait_for_timeout(3900)
            page.tap(".home-battery-panel")
            page.wait_for_selector("#valueRows tr")
            page.wait_for_timeout(capture_ms if capture_ms is not None else 900)
            metrics = page.evaluate(
                """() => {
                    const screen = document.querySelector(".screen");
                    const activeNav = document.querySelector(".l1-icon.active");
                    const activeTab = document.querySelector(".tab.active");
                    const pageHeading = document.querySelector("#pageHeading");
                    const heroTitle = document.querySelector("#heroTitle");
                    const rowLabels = Array.from(document.querySelectorAll("#valueRows tr td:first-child")).map((cell) => cell.textContent || "");
                    const intro = document.querySelector("#introSplash");

                    if (!screen) {
                        return { error: "Missing .screen root element" };
                    }

                    const rect = screen.getBoundingClientRect();
                    return {
                        screen_width: Math.round(rect.width),
                        screen_height: Math.round(rect.height),
                        active_nav_section: activeNav ? activeNav.dataset.section : "",
                        active_tab_text: activeTab ? activeTab.textContent || "" : "",
                        heading_text: pageHeading ? pageHeading.textContent || "" : "",
                        hero_title_text: heroTitle ? heroTitle.textContent || "" : "",
                        row_labels: rowLabels,
                        intro_removed_or_hidden: !intro || intro.classList.contains("is-hidden"),
                    };
                }"""
            )
        elif phase == "battery-modal":
            page.goto(base_url + "/touch.html", wait_until="domcontentloaded")
            page.wait_for_timeout(3900)
            page.tap(".home-battery-panel")
            page.wait_for_selector("#valueRows tr")
            page.tap('tr[data-control-key="target_charge_pct"]')
            page.wait_for_selector("#touchBatterySettingModal:not([hidden])")
            page.locator("#touchBatterySettingSlider").evaluate(
                "(element, value) => { element.value = value; element.dispatchEvent(new Event('input', { bubbles: true })); }",
                "90"
            )
            page.wait_for_timeout(capture_ms if capture_ms is not None else 400)
            metrics = page.evaluate(
                """() => {
                    const modal = document.querySelector("#touchBatterySettingModal");
                    const title = document.querySelector("#touchBatterySettingTitle");
                    const slider = document.querySelector("#touchBatterySettingSlider");
                    const sliderValue = document.querySelector("#touchBatterySettingValue");
                    const rowMap = {};
                    document.querySelectorAll("#valueRows tr").forEach((row) => {
                        const cells = row.querySelectorAll("td");
                        if (cells.length >= 2) {
                            rowMap[cells[0].textContent || ""] = cells[1].textContent || "";
                        }
                    });
                    return {
                        modal_visible: !!modal && !modal.hidden,
                        modal_title: title ? title.textContent || "" : "",
                        slider_present: !!slider,
                        slider_value_text: sliderValue ? sliderValue.textContent || "" : "",
                        target_charge_row_value: rowMap["Target Charge"] || "",
                    };
                }"""
            )
        elif phase == "battery-modal-close":
            page.goto(base_url + "/touch.html", wait_until="domcontentloaded")
            page.wait_for_timeout(3900)
            page.tap(".home-battery-panel")
            page.wait_for_selector("#valueRows tr")
            page.tap('tr[data-control-key="target_charge_pct"]')
            page.wait_for_selector("#touchBatterySettingModal:not([hidden])")
            page.tap("#touchBatterySettingClose")
            page.wait_for_timeout(capture_ms if capture_ms is not None else 250)
            metrics = page.evaluate(
                """() => {
                    const modal = document.querySelector("#touchBatterySettingModal");
                    const rowMap = {};
                    document.querySelectorAll("#valueRows tr").forEach((row) => {
                        const cells = row.querySelectorAll("td");
                        if (cells.length >= 2) {
                            rowMap[cells[0].textContent || ""] = cells[1].textContent || "";
                        }
                    });
                    return {
                        modal_hidden: !modal || modal.hidden,
                        target_charge_row_value: rowMap["Target Charge"] || "",
                    };
                }"""
            )
        else:
            page.goto(base_url + "/touch.html", wait_until="domcontentloaded")
            page.wait_for_timeout(3900)
            page.click('.l1-icon[data-section="vehicle"]')
            page.wait_for_function(
                """() => (
                    document.querySelectorAll("#valueRows tr").length > 0 ||
                    document.querySelectorAll("#metricBoard .metric-tile").length > 0
                )"""
            )
            page.wait_for_timeout(capture_ms if capture_ms is not None else 1400)
            page.click('#metricBoard .metric-tile[data-chart-key]')
            page.wait_for_selector("#metricChartCanvas")
            page.wait_for_timeout(1200)
            chart_metrics = page.evaluate(
                """() => {
                    const canvas = document.querySelector("#metricChartCanvas");
                    const chartPanel = document.querySelector(".metric-chart-panel");
                    const tableScroll = document.querySelector(".table-scroll");
                    return {
                        chart_visible: !!canvas && !!chartPanel,
                        chart_canvas_width: canvas ? Math.round(canvas.getBoundingClientRect().width) : 0,
                        chart_canvas_height: canvas ? Math.round(canvas.getBoundingClientRect().height) : 0,
                        chart_scroll_locked: !!tableScroll && tableScroll.classList.contains("is-chart-view"),
                    };
                }"""
            )
            page.click(".tab.active")
            page.wait_for_selector('#metricBoard .metric-tile[data-chart-key]')
            page.wait_for_timeout(300)
            metrics = page.evaluate(
                """() => {
                    const screen = document.querySelector(".screen");
                    const leftNav = document.querySelector(".left-nav");
                    const topbar = document.querySelector(".topbar");
                    const tableScroll = document.querySelector(".table-scroll");
                    const tableHead = document.querySelector(".table-head");
                    const imagePanel = document.querySelector(".image-panel");
                    const heroMetrics = document.querySelectorAll(".hero-card").length;
                    const rowCount = document.querySelectorAll("#valueRows tr").length;
                    const tileCount = document.querySelectorAll("#metricBoard .metric-tile").length;
                    const chartableTileCount = document.querySelectorAll("#metricBoard .metric-tile[data-chart-key]").length;
                    const activeNav = document.querySelector(".l1-icon.active");
                    const activeTab = document.querySelector(".tab.active");
                    const intro = document.querySelector("#introSplash");
                    const imageStyle = imagePanel ? getComputedStyle(imagePanel) : null;
                    const tableHeadStyle = tableHead ? getComputedStyle(tableHead) : null;
                    const tileRects = Array.from(document.querySelectorAll("#metricBoard .metric-tile"))
                        .slice(0, 8)
                        .map((tile) => {
                            const tileRect = tile.getBoundingClientRect();
                            return {
                                left: Math.round(tileRect.left),
                                top: Math.round(tileRect.top),
                                height: Math.round(tileRect.height),
                            };
                        });
                    const firstRowTop = tileRects.length ? tileRects[0].top : 0;
                    const firstRowColumns = new Set(tileRects.filter((tile) => tile.top === firstRowTop).map((tile) => tile.left)).size;
                    const fixedTileHeights = tileRects.length > 0 && tileRects.every((tile) => tile.height === tileRects[0].height);
                    const originalScrollTop = tableScroll ? tableScroll.scrollTop : 0;
                    if (tableScroll) {
                        tableScroll.scrollTop = 400;
                    }
                    const scrollTopAfterSet = tableScroll ? tableScroll.scrollTop : 0;
                    if (tableScroll) {
                        tableScroll.scrollTop = originalScrollTop;
                    }

                    if (!screen) {
                        return { error: "Missing .screen root element" };
                    }

                    const rect = screen.getBoundingClientRect();
                    return {
                        screen_width: Math.round(rect.width),
                        screen_height: Math.round(rect.height),
                        has_left_nav: !!leftNav,
                        has_topbar: !!topbar,
                        has_table_scroll: !!tableScroll,
                        has_image_panel: !!imagePanel,
                        image_hidden: !imagePanel || imagePanel.hidden || imageStyle.display === "none" || imageStyle.visibility === "hidden",
                        table_head_hidden: !tableHead || tableHead.hidden || tableHeadStyle.display === "none" || tableHeadStyle.visibility === "hidden",
                        has_active_nav: !!activeNav,
                        has_active_tab: !!activeTab,
                        hero_metric_count: heroMetrics,
                        row_count: rowCount,
                        tile_count: tileCount,
                        chartable_tile_count: chartableTileCount,
                        chart_closed_back_to_tiles: tileCount > 0 && !document.querySelector("#metricChartCanvas"),
                        table_scroll_client_height: tableScroll ? Math.round(tableScroll.clientHeight) : 0,
                        table_scroll_height: tableScroll ? Math.round(tableScroll.scrollHeight) : 0,
                        table_scroll_can_move: scrollTopAfterSet > 0,
                        first_row_tile_columns: firstRowColumns,
                        fixed_tile_heights: fixedTileHeights,
                        first_tile_height: tileRects.length ? tileRects[0].height : 0,
                        l1_count: Array.from(document.querySelectorAll(".l1-icon")).filter((button) => !button.hidden).length,
                        l2_count: document.querySelectorAll(".tab").length,
                        intro_removed_or_hidden: !intro || intro.classList.contains("is-hidden"),
                    };
                }"""
            )
            metrics.update(chart_metrics)

        page.screenshot(path=str(screenshot_path), full_page=False)
        context.close()
        browser.close()

    if "error" in metrics:
        return {"ok": False, "error": metrics["error"], "metrics": metrics}

    if phase == "intro":
        width_ok = metrics.get("viewport_width") == 1024
        height_ok = metrics.get("viewport_height") == 600
        phase_ok = (
            metrics.get("has_intro")
            and metrics.get("intro_visible")
            and metrics.get("intro_video_present")
            and metrics.get("body_booting")
            and metrics.get("screen_hidden")
        )
        checks = {
            "screen_width_1024": width_ok,
            "screen_height_600": height_ok,
            "intro_visible": bool(phase_ok),
        }
    elif phase == "home":
        width_ok = metrics.get("screen_width") == 1024
        height_ok = metrics.get("screen_height") == 600
        phase_ok = (
            metrics.get("is_home")
            and metrics.get("has_left_nav")
            and metrics.get("topbar_hidden")
            and metrics.get("table_hidden")
            and metrics.get("home_stage_visible")
            and metrics.get("has_battery_panel")
            and metrics.get("active_nav_section") == "home"
            and "half_front.png" in metrics.get("image_src", "")
            and metrics.get("image_height", 0) >= 598
            and metrics.get("image_width", 999) <= 400
            and metrics.get("image_right_gap", 999) <= 1
            and metrics.get("battery_width", 0) >= 280
            and metrics.get("battery_to_image_gap", 999) >= 90
            and metrics.get("battery_to_image_gap", 999) <= 140
            and len(metrics.get("battery_caption_text", "").strip()) >= 8
            and len(metrics.get("battery_time_text", "").strip()) >= 4
            and len(metrics.get("battery_date_text", "").strip()) >= 6
            and metrics.get("battery_percent_text") == "28"
            and metrics.get("battery_range_text") == "85"
            and metrics.get("battery_range_unit") == "km"
            and metrics.get("intro_removed_or_hidden")
        )
        checks = {
            "screen_width_1024": width_ok,
            "screen_height_600": height_ok,
            "home_layout": bool(phase_ok),
        }
    elif phase == "coolant":
        width_ok = metrics.get("screen_width") == 1024
        height_ok = metrics.get("screen_height") == 600
        phase_ok = (
            metrics.get("active_nav_section") == "coolant"
            and metrics.get("topbar_hidden")
            and metrics.get("table_hidden")
            and metrics.get("image_hidden")
            and metrics.get("coolant_stage_visible")
            and metrics.get("coolant_pipe_count") == 7
            and metrics.get("blue_pipe_count") == 3
            and metrics.get("red_pipe_count") == 3
            and metrics.get("gray_pipe_count") == 1
            and metrics.get("coolant_title", "").strip() == "Coolant System"
            and metrics.get("intro_removed_or_hidden")
        )
        checks = {
            "screen_width_1024": width_ok,
            "screen_height_600": height_ok,
            "coolant_layout": bool(phase_ok),
        }
    elif phase == "vehicle-battery":
        width_ok = metrics.get("screen_width") == 1024
        height_ok = metrics.get("screen_height") == 600
        row_labels = metrics.get("row_labels", [])
        phase_ok = (
            metrics.get("active_nav_section") == "vehicle"
            and metrics.get("active_tab_text", "").strip() == "Battery"
            and metrics.get("heading_text", "").strip() == "Vehicle Battery"
            and metrics.get("hero_title_text", "").strip() == "Vehicle Battery"
            and "Battery Charge" in row_labels
            and "Estimated Range" in row_labels
            and "DC Bus Voltage" in row_labels
            and metrics.get("intro_removed_or_hidden")
        )
        checks = {
            "screen_width_1024": width_ok,
            "screen_height_600": height_ok,
            "vehicle_battery_navigation": bool(phase_ok),
        }
    elif phase == "battery-modal":
        width_ok = True
        height_ok = True
        phase_ok = (
            metrics.get("modal_visible")
            and metrics.get("modal_title", "").strip() == "Target Charge"
            and metrics.get("slider_present")
            and metrics.get("slider_value_text", "").strip() == "90 %"
            and metrics.get("target_charge_row_value", "").strip() == "90 %"
        )
        checks = {
            "modal_visible": bool(metrics.get("modal_visible")),
            "slider_present": bool(metrics.get("slider_present")),
            "target_charge_updated": bool(metrics.get("target_charge_row_value", "").strip() == "90 %"),
        }
    elif phase == "battery-modal-close":
        width_ok = True
        height_ok = True
        phase_ok = (
            metrics.get("modal_hidden")
            and metrics.get("target_charge_row_value", "").strip() == "80 %"
        )
        checks = {
            "modal_hidden": bool(metrics.get("modal_hidden")),
            "row_value_preserved": bool(metrics.get("target_charge_row_value", "").strip() == "80 %"),
        }
    else:
        width_ok = metrics.get("screen_width") == 1024
        height_ok = metrics.get("screen_height") == 600
        phase_ok = (
            metrics.get("has_left_nav")
            and metrics.get("has_topbar")
            and metrics.get("has_table_scroll")
            and metrics.get("has_image_panel")
            and metrics.get("image_hidden")
            and metrics.get("table_head_hidden")
            and metrics.get("has_active_nav")
            and metrics.get("has_active_tab")
            and metrics.get("hero_metric_count") == 3
            and (metrics.get("row_count", 0) >= 6 or metrics.get("tile_count", 0) >= 8)
            and metrics.get("first_row_tile_columns") == 3
            and metrics.get("fixed_tile_heights")
            and metrics.get("first_tile_height") == 110
            and metrics.get("chartable_tile_count", 0) == 26
            and metrics.get("chart_visible")
            and metrics.get("chart_canvas_width", 0) >= 800
            and metrics.get("chart_canvas_height", 0) >= 400
            and metrics.get("chart_scroll_locked")
            and metrics.get("chart_closed_back_to_tiles")
            and metrics.get("table_scroll_height", 0) > metrics.get("table_scroll_client_height", 0)
            and metrics.get("table_scroll_can_move")
            and metrics.get("l1_count") == 6
            and metrics.get("l2_count") == 4
            and metrics.get("intro_removed_or_hidden")
        )
        checks = {
            "screen_width_1024": width_ok,
            "screen_height_600": height_ok,
            "core_structure": bool(phase_ok),
        }

    return {
        "ok": bool(width_ok and height_ok and phase_ok),
        "phase": phase,
        "metrics": metrics,
        "checks": checks,
        "screenshot": str(screenshot_path),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description="Capture and validate local touch UI screenshot at 1024x600")
    parser.add_argument("--host", default="127.0.0.1", help="Bind host (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=0, help="Bind port (default: auto)")
    parser.add_argument("--phase", choices=("intro", "home", "coolant", "vehicle-battery", "battery-modal", "battery-modal-close", "ready"), default="ready", help="Capture intro splash, home landing page, coolant system view, vehicle battery view, battery settings modal, modal close behavior, or ready UI")
    parser.add_argument("--capture-ms", type=int, default=None, help="Override capture timing in milliseconds")
    parser.add_argument(
        "--screenshot",
        default=str(DEFAULT_SCREENSHOT),
        help="Output screenshot path",
    )
    args = parser.parse_args()

    if not WEB_ROOT.exists():
        print(json.dumps({"ok": False, "error": f"Missing web root: {WEB_ROOT}"}, indent=2))
        return 2

    port = args.port or find_free_port(args.host)
    base_url = f"http://{args.host}:{port}"
    screenshot_path = Path(args.screenshot)

    server, thread = start_server(args.host, port)
    try:
        report = {"base_url": base_url}
        capture = run_layout_capture(base_url, screenshot_path, args.phase, args.capture_ms)
        report.update(capture)
        print(json.dumps(report, indent=2))

        if not report.get("ok") and "Playwright is not installed" in str(report.get("error", "")):
            return 3
        return 0 if report.get("ok") else 2
    finally:
        server.shutdown()
        server.server_close()
        thread.join(timeout=1.0)


if __name__ == "__main__":
    raise SystemExit(main())
