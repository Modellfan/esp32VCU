// Engineering page "Flow" (2026-11-17): Simulink-like block diagrams of the firmware
// calculation in src/app/cluster_calc.cpp, with the live values in the blocks.
// Needle: inverter DC power -> power share -> needle map -> override -> 0x280 engine speed.
// Fuel:   fixed step every 30 ms -> total fuel -> 15-bit counter + overflow flag -> 0x4E0.
//
// Layout rules (keep when changing): 5 columns (COL) and 4 rows (ROW) on a fixed grid; blocks
// 150 x 64 (tables taller); inputs/outputs are pills, calculations rectangles; wires only
// horizontal/vertical; per block one title, one value (green), at most one grey line;
// blue = active signal path only, amber = warning only.
// The constants below mirror the firmware; change both together.
(function () {
  const FW = {
    refVoltage: 348, driveCurrent: 300, regenCurrent: 100, deadband: 0.05,
    fuelStepUl: 500, fuelSliceMs: 30, fuelMaxL: 20, counterWrap: 32768, overflowCycles: 5
  };
  const KEYS = {
    Needle: ["sdu_udc", "sdu_idc", "sdu_seldir", "cluster_sdu_data_ok", "cluster_power_percent_max",
      "cluster_needle_position", "cluster_needle_reverse", "cluster_needle_neutral", "cluster_needle_offset",
      "cluster_needle_drive_max", "cluster_needle_regen_min", "cluster_needle_drive_offset", "rpm_override",
      "engine_speed_rpm", "stale"],
    Fuel: ["cluster_total_fuel_l", "mo5_verbrauch_ul", "mo5_ueberlverb", "fuel_override", "stale"]
  };
  const W = 150;        // block width
  const H = 64;         // block height
  const COL = [20, 205, 390, 575, 760];  // left edge of the columns
  const ROW = [88, 200, 312, 424];       // vertical centre of the rows

  function esc(text) {
    return String(text).replace(/[&<>"']/g, function (c) {
      return { "&": "&amp;", "<": "&lt;", ">": "&gt;", "\"": "&quot;", "'": "&#39;" }[c];
    });
  }

  function num(live, key) {
    if (live && Array.isArray(live.stale) && live.stale.indexOf(key) !== -1) {
      return null;  // not received / too old: "--"
    }
    const value = Number((live || {})[key]);
    return Number.isFinite(value) ? value : null;
  }

  function fmt(value, digits, unit) {
    return value === null ? "--" : value.toFixed(digits) + (unit ? " " + unit : "");
  }

  // ---------------------------------------------------------------- SVG helpers
  // Block on the grid: column c, vertical centre cy; one title, one value, one grey line below.
  function block(c, cy, title, value, detail, opts) {
    const o = opts || {};
    const h = o.h || H;
    const w = o.w || W;
    const x = COL[c];
    const y = cy - h / 2;
    return "<g class=\"fl-block" + (o.active ? " is-active" : "") + (o.port ? " is-port" : "") + "\">" +
      "<rect x=\"" + x + "\" y=\"" + y + "\" width=\"" + w + "\" height=\"" + h + "\" rx=\"" + (o.port ? h / 2 : 4) + "\"/>" +
      "<text class=\"fl-title\" x=\"" + (x + w / 2) + "\" y=\"" + (cy - 8) + "\">" + esc(title) + "</text>" +
      "<text class=\"fl-big" + (o.warn ? " is-warn" : "") + "\" x=\"" + (x + w / 2) + "\" y=\"" + (cy + 14) + "\">" + esc(value) + "</text>" +
      (detail ? "<text class=\"fl-detail\" x=\"" + (x + w / 2) + "\" y=\"" + (y + h + 16) + "\">" + esc(detail) + "</text>" : "") +
    "</g>";
  }

  // Table block (needle map): title, then label left / value right; `on` marks the active lines.
  function table(c, top, h, w, title, lines, on) {
    const x = COL[c];
    let out = "<g class=\"fl-block is-active\"><rect x=\"" + x + "\" y=\"" + top + "\" width=\"" + w + "\" height=\"" + h + "\" rx=\"4\"/>" +
      "<text class=\"fl-title\" x=\"" + (x + w / 2) + "\" y=\"" + (top + 24) + "\">" + esc(title) + "</text>";
    lines.forEach(function (line, i) {
      const y = top + 52 + i * 25;
      const cls = line[0] === on ? " is-on" : "";
      out += "<text class=\"fl-cell" + cls + "\" x=\"" + (x + 12) + "\" y=\"" + y + "\">" + esc(line[1]) + "</text>" +
        "<text class=\"fl-cell is-value" + cls + "\" x=\"" + (x + w - 12) + "\" y=\"" + y + "\">" + esc(line[2]) + "</text>";
    });
    return out + "</g>";
  }

  // Orthogonal wire through the points (horizontal/vertical segments only), arrow at the end.
  function wire(points, active) {
    const d = points.map(function (p, i) { return (i ? "L" : "M") + p[0] + " " + p[1]; }).join(" ");
    return "<path class=\"fl-wire" + (active ? " is-active" : "") + "\" d=\"" + d + "\" marker-end=\"url(#flArrow" + (active ? "On" : "") + ")\"/>";
  }

  function notes(x, y, lines) {
    return lines.map(function (line, i) {
      return "<text class=\"fl-note\" x=\"" + x + "\" y=\"" + (y + i * 20) + "\">" + esc(line) + "</text>";
    }).join("");
  }

  function svg(content) {
    return "<svg class=\"fl-svg\" viewBox=\"0 44 940 432\" preserveAspectRatio=\"xMidYMid meet\" role=\"img\">" +
      "<defs>" +
        "<marker id=\"flArrow\" viewBox=\"0 0 10 10\" refX=\"9\" refY=\"5\" markerWidth=\"6\" markerHeight=\"6\" orient=\"auto\"><path d=\"M0 0 L10 5 L0 10 Z\" fill=\"#6d757a\"/></marker>" +
        "<marker id=\"flArrowOn\" viewBox=\"0 0 10 10\" refX=\"9\" refY=\"5\" markerWidth=\"6\" markerHeight=\"6\" orient=\"auto\"><path d=\"M0 0 L10 5 L0 10 Z\" fill=\"#18caff\"/></marker>" +
      "</defs>" + content + "</svg>";
  }

  function right(c, w) { return COL[c] + (w || W); }
  function mid(c, w) { return COL[c] + (w || W) / 2; }

  // ---------------------------------------------------------------- needle
  function needleMarkup(live) {
    const ok = live.cluster_sdu_data_ok === true;
    const udc = num(live, "sdu_udc");
    const idc = num(live, "sdu_idc");
    const dirRaw = num(live, "sdu_seldir");
    const u = ok ? udc : 0;
    const i = ok ? idc : 0;
    const dir = ok ? dirRaw : 0;
    const regen = i !== null && i < 0;
    const pKw = u !== null && i !== null ? u * i / 1000 : null;
    const pRefKw = FW.refVoltage * (regen ? FW.regenCurrent : FW.driveCurrent) / 1000;
    const share = num(live, "cluster_power_percent_max");
    const cal = {
      r: num(live, "cluster_needle_reverse"), n: num(live, "cluster_needle_neutral"), o: num(live, "cluster_needle_offset"),
      max: num(live, "cluster_needle_drive_max"), regen: num(live, "cluster_needle_regen_min"),
      lag: num(live, "cluster_needle_drive_offset")
    };
    const needle = num(live, "cluster_needle_position");
    const override = live.rpm_override === true;
    const out = num(live, "engine_speed_rpm");
    const branch = dir === null ? "" : dir > 0 ? "R" : dir === 0 ? "N"
      : share === null ? "" : Math.abs(share) < FW.deadband ? "0" : share > 0 ? "D" : "G";
    const shareText = share === null ? "--" : (share * 100).toFixed(0) + " %";
    const top = ROW[0] - H / 2;
    const shareY = (ROW[0] + ROW[1]) / 2;
    const mapW = 170;
    const mapH = 200;
    const busX = right(2) + 17;          // vertical bus between columns 2 and 3
    const dirX = COL[4] - 18;            // vertical lane of the direction wire
    const outW = mapW;
    return svg(
      // column 0: inputs
      block(0, ROW[0], "DC voltage", fmt(udc, 1, "V"), "", { port: true, active: true }) +
      block(0, ROW[1], "DC current", fmt(idc, 1, "A"), "", { port: true, active: true }) +
      block(0, ROW[2], "Direction", dirRaw === null ? "--" : String(dirRaw), "", { port: true, active: true }) +
      block(0, ROW[3], "Inverter data", ok ? "fresh" : "missing", "", { port: true, active: true, warn: !ok }) +
      // column 1: validity gate over all rows
      "<g class=\"fl-block" + (ok ? "" : " is-active") + "\"><rect x=\"" + COL[1] + "\" y=\"" + top + "\" width=\"" + W + "\" height=\"" + (ROW[3] + H / 2 - top) + "\" rx=\"4\"/>" +
        "<text class=\"fl-title\" x=\"" + mid(1) + "\" y=\"" + (ROW[0] - 8) + "\">Data valid?</text>" +
        "<text class=\"fl-big" + (ok ? "" : " is-warn") + "\" x=\"" + mid(1) + "\" y=\"" + (ROW[0] + 14) + "\">" + (ok ? "yes" : "no") + "</text>" +
        "<text class=\"fl-detail\" x=\"" + mid(1) + "\" y=\"" + (ROW[3] - 8) + "\">0x500 / 0x504 younger</text>" +
        "<text class=\"fl-detail\" x=\"" + mid(1) + "\" y=\"" + (ROW[3] + 10) + "\">than 500 ms, else</text>" +
        "<text class=\"fl-detail\" x=\"" + mid(1) + "\" y=\"" + (ROW[3] + 28) + "\">0 V, 0 A, N</text>" +
      "</g>" +
      [0, 1, 2, 3].map(function (r) { return wire([[right(0), ROW[r]], [COL[1], ROW[r]]], true); }).join("") +
      // column 2: power and reference
      block(2, ROW[0], "U × I", fmt(pKw, 1, "kW"), "", { active: true }) +
      block(2, ROW[1], "Reference", pRefKw.toFixed(1) + " kW", FW.refVoltage + " V × " + (regen ? FW.regenCurrent + " A regen" : FW.driveCurrent + " A drive")) +
      wire([[right(1), ROW[0]], [COL[2], ROW[0]]], true) +
      wire([[right(1), ROW[1]], [COL[2], ROW[1]]], true) +
      // column 3: share
      block(3, shareY, "P ÷ Pref", shareText, "limited to ±100 %", { active: true }) +
      wire([[right(2), ROW[0]], [busX, ROW[0]], [busX, shareY - 12], [COL[3], shareY - 12]], true) +
      wire([[right(2), ROW[1]], [busX, ROW[1]], [busX, shareY + 12], [COL[3], shareY + 12]], true) +
      // column 4: needle map, override, output
      table(4, top, mapH, mapW, "Needle map", [
        ["R", "R  (dir > 0)", fmt(cal.r, 0)],
        ["N", "N  (dir = 0)", fmt(cal.n, 0)],
        ["0", "|p| < 5 %", fmt(cal.o, 0)],
        ["D", "p > 0", fmt(cal.o, 0) + " → " + fmt(cal.max, 0)],
        ["D", "+ drive offset", fmt(cal.lag, 0)],
        ["G", "p < 0", fmt(cal.o, 0) + " → " + fmt(cal.regen, 0)]
      ], branch) +
      wire([[right(3), shareY], [COL[4], shareY]], true) +
      wire([[right(1), ROW[2]], [dirX, ROW[2]], [dirX, top + mapH - 24], [COL[4], top + mapH - 24]], true) +
      block(4, ROW[2] + 34, "Override", override ? "fixed" : "auto", "", { w: outW, active: override, warn: override }) +
      wire([[mid(4, outW), top + mapH], [mid(4, outW), ROW[2] + 34 - H / 2]], !override) +
      block(4, ROW[3] + 22, "0x280 speed", fmt(out, 0, "rpm"), "", { w: outW, port: true, active: true }) +
      wire([[mid(4, outW), ROW[2] + 34 + H / 2], [mid(4, outW), ROW[3] + 22 - H / 2]], true) +
      notes(COL[2], ROW[2] + 44, [
        "Map value: " + fmt(needle, 0),
        "Needle = engine speed in 0x280 (0.25 rpm/bit).",
        "Calibration: ECU > Needle (stored).",
        "Override: ECU > Emulation.",
        "Blue = active signal path."
      ])
    );
  }

  // ---------------------------------------------------------------- fuel
  function fuelMarkup(live) {
    const total = num(live, "cluster_total_fuel_l");
    const counter = num(live, "mo5_verbrauch_ul");
    const flag = live.mo5_ueberlverb === true;
    const override = live.fuel_override === true;
    const full = total !== null && total >= FW.fuelMaxL;
    const totalUl = total === null ? null : Math.round(total * 1e6);
    const wraps = totalUl === null ? null : Math.floor(totalUl / FW.counterWrap);
    const raw = totalUl === null ? null : totalUl % FW.counterWrap;
    const perMinute = FW.fuelStepUl / FW.fuelSliceMs * 60000 / 1e6;
    const y = ROW[0];
    const y2 = ROW[1] + 20;
    const y3 = ROW[3];
    return svg(
      // row 0: the chain
      block(0, y, "Clock", "every " + FW.fuelSliceMs + " ms", "time, not power", { port: true, active: !full }) +
      block(1, y, "Step", "+ " + FW.fuelStepUl + " µl", perMinute.toFixed(1) + " l/min", { active: !full }) +
      block(2, y, "Total fuel", fmt(total, 3, "l"), "stops at " + FW.fuelMaxL + " l", { active: !full, warn: full }) +
      block(3, y, "litre → µl", totalUl === null ? "--" : String(totalUl), "", { active: true }) +
      block(4, y, "mod 32768", raw === null ? "--" : String(raw), "", { active: true }) +
      [0, 1, 2, 3].map(function (c) { return wire([[right(c), y], [COL[c + 1], y]], c >= 2 || !full); }).join("") +
      // column 3: overflow
      block(3, y2, "÷ 32768", "wraps " + (wraps === null ? "--" : wraps), "flag " + FW.overflowCycles + " cycles on change", { active: flag }) +
      wire([[mid(3), y + H / 2], [mid(3), y2 - H / 2]], true) +
      block(3, y3, "0x4E0 overflow", flag ? "1" : "0", "", { port: true, active: flag, warn: flag }) +
      wire([[mid(3), y2 + H / 2], [mid(3), y3 - H / 2]], flag) +
      // column 4: override and counter
      block(4, y2, "Override", override ? "fixed" : "auto", "", { active: override, warn: override }) +
      wire([[mid(4), y + H / 2], [mid(4), y2 - H / 2]], !override) +
      block(4, y3, "0x4E0 counter", fmt(counter, 0), "", { port: true, active: true }) +
      wire([[mid(4), y2 + H / 2], [mid(4), y3 - H / 2]], true) +
      notes(COL[0], ROW[1] + 4, [
        "The counter does not depend on power:",
        "the VCU adds " + FW.fuelStepUl + " µl every " + FW.fuelSliceMs + " ms",
        "from start-up and stops at " + FW.fuelMaxL + " l",
        "(after about " + Math.round(FW.fuelMaxL / perMinute) + " min).",
        "The cluster derives consumption",
        "from the counter steps.",
        "",
        "Override: ECU > Emulation.",
        "Blue = active signal path."
      ])
    );
  }

  function markup(tab, live) {
    return "<div class=\"fl-page\">" + (tab === "Fuel" ? fuelMarkup(live || {}) : needleMarkup(live || {})) + "</div>";
  }

  window.VcuFlow = {
    markup: markup,
    keys: function (tab) { return (KEYS[tab] || KEYS.Needle).slice(); }
  };
})();
