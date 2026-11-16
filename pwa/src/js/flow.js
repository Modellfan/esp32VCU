// Engineering page "Flow" (2026-11-17): Simulink-like block diagrams of the firmware
// calculation in src/app/cluster_calc.cpp, with the live values on every wire.
// Needle: inverter DC power -> power share -> needle map -> override -> 0x280 engine speed.
// Fuel:   fixed step every 30 ms -> total fuel -> 15-bit counter + overflow flag -> 0x4E0.
// The constants below mirror the firmware; change both together.
(function () {
  const FW = {
    refVoltage: 348, driveCurrent: 300, regenCurrent: 100, deadband: 0.05, lag: 30,
    fuelStepUl: 500, fuelSliceMs: 30, fuelMaxL: 20, counterWrap: 32768, overflowCycles: 5
  };
  const KEYS = {
    Needle: ["sdu_udc", "sdu_idc", "sdu_seldir", "cluster_sdu_data_ok", "cluster_power_percent_max",
      "cluster_needle_position", "cluster_needle_reverse", "cluster_needle_neutral", "cluster_needle_offset",
      "cluster_needle_drive_max", "cluster_needle_regen_min", "rpm_override", "engine_speed_rpm", "stale"],
    Fuel: ["cluster_total_fuel_l", "mo5_verbrauch_ul", "mo5_ueberlverb", "fuel_override", "stale"]
  };

  function esc(text) {
    return String(text).replace(/[&<>"']/g, function (c) {
      return { "&": "&amp;", "<": "&lt;", ">": "&gt;", "\"": "&quot;", "'": "&#39;" }[c];
    });
  }

  function num(live, key) {
    const value = Number((live || {})[key]);
    return Number.isFinite(value) ? value : null;
  }

  function fmt(value, digits, unit) {
    return value === null ? "--" : value.toFixed(digits) + (unit ? " " + unit : "");
  }

  // ---------------------------------------------------------------- SVG helpers
  function block(x, y, w, h, title, lines, opts) {
    const o = opts || {};
    let out = "<g class=\"fl-block" + (o.active ? " is-active" : "") + (o.dim ? " is-dim" : "") + (o.port ? " is-port" : "") + "\">" +
      "<rect x=\"" + x + "\" y=\"" + y + "\" width=\"" + w + "\" height=\"" + h + "\" rx=\"" + (o.port ? h / 2 : 6) + "\"/>" +
      "<text class=\"fl-title\" x=\"" + (x + w / 2) + "\" y=\"" + (y + 18) + "\">" + esc(title) + "</text>";
    (lines || []).forEach(function (line, i) {
      const cls = line.cls || "fl-line";
      out += "<text class=\"" + cls + "\" x=\"" + (x + w / 2) + "\" y=\"" + (y + 38 + i * 17) + "\">" + esc(line.text) + "</text>";
    });
    return out + "</g>";
  }

  // Wire through the points, arrow at the end, live value as label near `labelAt`.
  function wire(points, label, opts) {
    const o = opts || {};
    const d = points.map(function (p, i) { return (i ? "L" : "M") + p[0] + " " + p[1]; }).join(" ");
    let out = "<path class=\"fl-wire" + (o.active ? " is-active" : "") + (o.dim ? " is-dim" : "") + "\" d=\"" + d + "\" marker-end=\"url(#flArrow" + (o.active ? "On" : "") + ")\"/>";
    if (label) {
      const at = o.labelAt || points[0];
      out += "<text class=\"fl-value\" x=\"" + at[0] + "\" y=\"" + at[1] + "\">" + esc(label) + "</text>";
    }
    return out;
  }

  function svg(content) {
    return "<svg class=\"fl-svg\" viewBox=\"0 0 940 520\" preserveAspectRatio=\"xMidYMid meet\" role=\"img\">" +
      "<defs>" +
        "<marker id=\"flArrow\" viewBox=\"0 0 10 10\" refX=\"9\" refY=\"5\" markerWidth=\"7\" markerHeight=\"7\" orient=\"auto\"><path d=\"M0 0 L10 5 L0 10 Z\" fill=\"#8a9399\"/></marker>" +
        "<marker id=\"flArrowOn\" viewBox=\"0 0 10 10\" refX=\"9\" refY=\"5\" markerWidth=\"7\" markerHeight=\"7\" orient=\"auto\"><path d=\"M0 0 L10 5 L0 10 Z\" fill=\"#18caff\"/></marker>" +
      "</defs>" + content + "</svg>";
  }

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
      max: num(live, "cluster_needle_drive_max"), regen: num(live, "cluster_needle_regen_min")
    };
    const needle = num(live, "cluster_needle_position");
    const override = live.rpm_override === true;
    const out = num(live, "engine_speed_rpm");
    // active branch of the needle map (same order as needleFromPercent)
    const branch = dir === null ? "" : dir > 0 ? "R" : dir === 0 ? "N"
      : share === null ? "" : Math.abs(share) < FW.deadband ? "0" : share > 0 ? "D" : "G";
    const mapLines = [
      { text: "dir > 0 (R):  " + fmt(cal.r, 0), cls: branch === "R" ? "fl-line is-on" : "fl-line" },
      { text: "dir = 0 (N):  " + fmt(cal.n, 0), cls: branch === "N" ? "fl-line is-on" : "fl-line" },
      { text: "|p| < 5 %:  " + fmt(cal.o, 0), cls: branch === "0" ? "fl-line is-on" : "fl-line" },
      { text: "p > 0:  " + fmt(cal.o, 0) + " → " + fmt(cal.max, 0), cls: branch === "D" ? "fl-line is-on" : "fl-line" },
      { text: "      + " + FW.lag + ", max " + fmt(cal.max, 0), cls: branch === "D" ? "fl-line is-on" : "fl-line" },
      { text: "p < 0:  " + fmt(cal.o, 0) + " → " + fmt(cal.regen, 0), cls: branch === "G" ? "fl-line is-on" : "fl-line" }
    ];
    return svg(
      // inputs
      block(10, 30, 130, 52, "DC voltage", [{ text: fmt(udc, 1, "V"), cls: "fl-big" }], { port: true }) +
      block(10, 110, 130, 52, "DC current", [{ text: fmt(idc, 1, "A"), cls: "fl-big" }], { port: true }) +
      block(10, 330, 130, 52, "Direction", [{ text: dirRaw === null ? "--" : String(dirRaw), cls: "fl-big" }], { port: true }) +
      block(10, 420, 130, 52, "Inverter data", [{ text: ok ? "fresh" : "missing", cls: ok ? "fl-big" : "fl-big is-warn" }], { port: true }) +
      // data valid switch
      block(180, 40, 120, 360, "Data valid?", [
        { text: ok ? "yes: pass" : "no: 0 V, 0 A," }, { text: ok ? "U, I, dir" : "dir = 0 (N)" },
        { text: "" }, { text: "0x500 / 0x504" }, { text: "younger than" }, { text: "500 ms" }
      ], { active: !ok }) +
      wire([[140, 56], [180, 56]], "", { active: true }) +
      wire([[140, 136], [180, 136]], "", { active: true }) +
      wire([[140, 356], [180, 356]], "", { active: true }) +
      wire([[140, 446], [240, 446], [240, 400]], "", { active: true }) +
      // product and reference
      block(340, 30, 120, 70, "U × I", [{ text: fmt(pKw, 1, "kW"), cls: "fl-big" }]) +
      wire([[300, 56], [340, 56]], fmt(u, 0, "V"), { labelAt: [320, 48], active: true }) +
      wire([[300, 80], [340, 80]], fmt(i, 0, "A"), { labelAt: [320, 96], active: true }) +
      block(340, 140, 120, 86, "Reference", [
        { text: FW.refVoltage + " V ×" }, { text: (regen ? FW.regenCurrent + " A (regen)" : FW.driveCurrent + " A (drive)") },
        { text: "= " + pRefKw.toFixed(1) + " kW", cls: "fl-line is-on" }
      ]) +
      // divide + saturation
      block(500, 60, 120, 110, "P ÷ Pref", [{ text: "limit ±1" }, { text: "p = " + (share === null ? "--" : (share * 100).toFixed(0) + " %"), cls: "fl-big" }]) +
      wire([[460, 66], [500, 90]], fmt(pKw, 1, "kW"), { labelAt: [476, 62], active: true }) +
      wire([[460, 183], [480, 183], [480, 140], [500, 140]], fmt(pRefKw, 1, "kW"), { labelAt: [470, 200], active: true }) +
      // needle map
      block(650, 30, 160, 210, "Needle map", mapLines, { active: true }) +
      wire([[620, 115], [650, 115]], share === null ? "--" : (share * 100).toFixed(0) + " %", { labelAt: [634, 108], active: true }) +
      wire([[300, 356], [630, 356], [630, 200], [650, 200]], "dir " + (dir === null ? "--" : String(dir)), { labelAt: [470, 348], active: true }) +
      // override switch
      block(650, 300, 160, 90, "Override", [
        { text: override ? "fixed value" : "auto: map", cls: override ? "fl-line is-warn" : "fl-line" },
        { text: "Emulation > Engine speed" }
      ], { active: override }) +
      wire([[730, 240], [730, 300]], fmt(needle, 0), { labelAt: [770, 274], active: !override }) +
      // output
      block(840, 300, 90, 90, "0x280", [{ text: "engine speed" }, { text: fmt(out, 0, "rpm"), cls: "fl-big" }], { port: true }) +
      wire([[810, 345], [840, 345]], "", { active: true }) +
      "<text class=\"fl-note\" x=\"380\" y=\"440\">Needle = engine speed in 0x280 (0.25 rpm per bit).</text>" +
      "<text class=\"fl-note\" x=\"380\" y=\"462\">Calibration: ECU > Needle (stored).</text>" +
      "<text class=\"fl-note\" x=\"380\" y=\"484\">Blue = active path.</text>"
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
    return svg(
      block(10, 60, 130, 70, "Clock", [{ text: "every " + FW.fuelSliceMs + " ms" }, { text: "(time, not power)", cls: "fl-line is-warn" }], { port: true }) +
      block(180, 60, 120, 70, "Step", [{ text: "+ " + FW.fuelStepUl + " µl" }, { text: perMinute.toFixed(1) + " l/min" }]) +
      wire([[140, 95], [180, 95]], "", { active: !full }) +
      block(340, 40, 140, 110, "Total fuel", [{ text: "sum, stops at" }, { text: FW.fuelMaxL + " l", cls: full ? "fl-line is-warn" : "fl-line" }, { text: fmt(total, 3, "l"), cls: "fl-big" }], { active: !full }) +
      wire([[300, 95], [340, 95]], "", { active: !full }) +
      block(520, 40, 140, 110, "× 1 000 000", [{ text: "litre → µl" }, { text: totalUl === null ? "--" : String(totalUl), cls: "fl-big" }]) +
      wire([[480, 95], [520, 95]], fmt(total, 3, "l"), { labelAt: [500, 87], active: true }) +
      block(700, 30, 130, 80, "mod 32768", [{ text: "15-bit counter" }, { text: raw === null ? "--" : String(raw), cls: "fl-big" }]) +
      block(520, 200, 140, 100, "÷ 32768", [{ text: "wraps: " + (wraps === null ? "--" : wraps) }, { text: "on change:" }, { text: "flag for " + FW.overflowCycles + " cycles" }]) +
      wire([[660, 70], [700, 70]], "", { active: true }) +
      wire([[590, 150], [590, 200]], "", { active: true }) +
      block(700, 330, 130, 90, "Override", [{ text: override ? "fixed value" : "auto: counter", cls: override ? "fl-line is-warn" : "fl-line" }, { text: "Emulation > Fuel" }], { active: override }) +
      wire([[765, 110], [765, 330]], raw === null ? "--" : String(raw) + " µl", { labelAt: [800, 300], active: !override }) +
      block(860, 330, 70, 90, "0x4E0", [{ text: "counter" }, { text: fmt(counter, 0), cls: "fl-big" }], { port: true }) +
      wire([[830, 375], [860, 375]], "", { active: true }) +
      block(540, 360, 100, 80, "0x4E0", [{ text: "overflow" }, { text: flag ? "1" : "0", cls: flag ? "fl-big is-warn" : "fl-big" }], { port: true }) +
      wire([[590, 300], [590, 360]], "", { active: flag }) +
      "<text class=\"fl-note\" x=\"20\" y=\"200\">The fuel gauge counter does not depend on power:</text>" +
      "<text class=\"fl-note\" x=\"20\" y=\"222\">the VCU adds " + FW.fuelStepUl + " µl every " + FW.fuelSliceMs + " ms from start-up</text>" +
      "<text class=\"fl-note\" x=\"20\" y=\"244\">and stops at " + FW.fuelMaxL + " l (after about " + Math.round(FW.fuelMaxL / perMinute) + " min).</text>" +
      "<text class=\"fl-note\" x=\"20\" y=\"266\">The cluster derives consumption from the counter steps.</text>" +
      "<text class=\"fl-note\" x=\"20\" y=\"300\">Blue = active path.</text>"
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
