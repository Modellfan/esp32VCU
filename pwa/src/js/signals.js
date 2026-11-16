// Signal catalog for the engineering rows (2026-11-16): every /api/signals name and every other
// /api/live field belongs to exactly one page. In engineering mode each page lists its remaining
// signals below the normal rows. New firmware signals appear without a UI change.
(function () {
  // Battery-group signals that come from the Zombie slave (0x437..0x439) or the battery plug.
  const ZOMBIE = {
    battery_vehicle_state: true, battery_force_shutdown: true, battery_hv_request: true,
    battery_charging_active: true, battery_charge_power_w: true, battery_charge_power_max_w: true,
    battery_plug_connected: true
  };
  const GROUP_PAGES = {
    tesla_sdu: "engine",
    vehicle: "vehicle",
    battery: "battery",
    charging: "charger",
    heater: "zombie",
    ecu: "emulation",
    ecu_override: "emulation",
    fan: "emulation",
    digipot: "emulation",
    cluster: "needle",
    can: "can"
  };

  const EXACT_UNITS = {
    rpm: "rpm", sdu_udc: "V", sdu_idc: "A", sdu_il1: "A", sdu_il2: "A", sdu_ilmax: "A",
    sdu_il1rms: "A", sdu_il2rms: "A", sdu_uac: "V", sdu_uaux: "V", sdu_fstat: "Hz",
    sdu_fweakcalc: "Hz", sdu_speed: "rpm", sdu_cruisespeed: "rpm", sdu_tmphs: "°C",
    sdu_tmpm: "°C", sdu_temp_heatsink: "°C", sdu_temp_heatsink74: "°C",
    sdu_potnom: "%", sdu_regenpreset: "%", sdu_angle: "deg", sdu_cpuload: "%",
    can_bitrate: "bit/s", lenkwinkel_rb: "deg", lat_g: "g", acceleration: "g",
    battery_cell_v_min: "V", battery_cell_v_max: "V", free_heap: "B"
  };
  // Longest suffix first.
  const SUFFIX_UNITS = [
    ["_kmh", "km/h"], ["_rpm", "rpm"], ["_pct", "%"], ["_percent", "%"], ["_kwh", "kWh"],
    ["_mbar", "mbar"], ["_bar", "bar"], ["_mv", "mV"], ["_kw", "kW"], ["_hz", "Hz"],
    ["_ms", "ms"], ["_us", "µs"], ["_km", "km"], ["_c", "°C"], ["_v", "V"],
    ["_a", "A"], ["_w", "W"], ["_l", "L"], ["_s", "s"]
  ];
  const SKIP = { stale: true };

  // Readable names for the CAN diagnostics (all other rows show the signal name).
  const CAN_BUS_FIELDS = {
    up: "bus up", error: "error", rx_frames: "frames received", tec: "transmit error counter",
    rec: "receive error counter", eflg: "error flags", bus_off: "bus off", rx_peak: "receive queue peak",
    rx_overflow: "receive overflows"
  };
  const LABELS = {
    can_bitrate: "CAN bitrate", can_spi_clock_hz: "SPI clock", can_mcp2515_quartz_hz: "MCP2515 crystal",
    can_mcp2515_rx_buffer: "MCP2515 receive buffer", can_mcp2515_tx_buffer: "MCP2515 transmit buffer",
    can_can0_rx_buffer: "CAN0 receive buffer", can_can0_tx_buffer: "CAN0 transmit buffer",
    can_rx_budget_per_bus: "Receive budget per bus", can_ecu_period_ms: "ECU task period",
    can_error_refresh_ms: "Error status refresh", can_ecu_task_priority: "ECU task priority",
    can_web_task_priority: "Web task priority", ecu_cycles: "ECU cycles", ecu_overruns: "ECU cycle overruns",
    ecu_max_cycle_us: "ECU longest cycle", ecu_tx_failed: "ECU send failures", ecu_started_ms: "ECU task start",
    gvret_dropped: "GVRET dropped frames"
  };

  let catalog = null;  // [{ name, label, page, type, unit }]
  let loading = null;

  function unitFor(name) {
    if (Object.prototype.hasOwnProperty.call(EXACT_UNITS, name)) {
      return EXACT_UNITS[name];
    }
    for (let i = 0; i < SUFFIX_UNITS.length; i += 1) {
      const suffix = SUFFIX_UNITS[i][0];
      if (name.length > suffix.length && name.slice(-suffix.length) === suffix) {
        return SUFFIX_UNITS[i][1];
      }
    }
    return "";
  }

  function labelFor(name) {
    if (Object.prototype.hasOwnProperty.call(LABELS, name)) {
      return LABELS[name];
    }
    const bus = /^can(\d)_(.+)$/.exec(name);
    if (bus && CAN_BUS_FIELDS[bus[2]]) {
      return "CAN" + bus[1] + " " + CAN_BUS_FIELDS[bus[2]];
    }
    return name;
  }

  function pageFor(name, group) {
    if (/^dcdc_/.test(name)) {
      return "dcdc";
    }
    if (/^heater_/.test(name) || ZOMBIE[name]) {
      return "zombie";
    }
    if (group && Object.prototype.hasOwnProperty.call(GROUP_PAGES, group)) {
      return GROUP_PAGES[group];
    }
    // Live fields outside the signal table.
    if (/_override$/.test(name) || /^set_.*_active$/.test(name)) {
      return "emulation";
    }
    if (name === "gvret_dropped") {
      return "can";
    }
    return "wifi";  // network and system status
  }

  function buildCatalog(signalList, liveSample) {
    const seen = {};
    const entries = [];
    function add(name, group, type) {
      if (!name || seen[name] || SKIP[name]) {
        return;
      }
      seen[name] = true;
      entries.push({ name: name, label: labelFor(name), page: pageFor(name, group), type: type || "", unit: unitFor(name) });
    }
    (signalList || []).forEach(function (signal) {
      if (signal && typeof signal.name === "string") {
        add(signal.name, signal.group, signal.type);
      }
    });
    Object.keys(liveSample || {}).forEach(function (name) {
      const value = liveSample[name];
      add(name, null, typeof value === "boolean" ? "bool" : typeof value === "string" ? "text" : "");
    });
    return entries;
  }

  // getJson(url) -> Promise of parsed JSON (net.getJson of the app).
  function load(getJson) {
    if (catalog) {
      return Promise.resolve(catalog);
    }
    if (!loading) {
      loading = Promise.all([getJson("/api/signals"), getJson("/api/live")]).then(function (results) {
        const signals = results[0] && Array.isArray(results[0].signals) ? results[0].signals : [];
        const live = results[1] && typeof results[1] === "object" && !Array.isArray(results[1]) ? results[1] : {};
        catalog = buildCatalog(signals, live);
        return catalog;
      }).finally(function () {
        loading = null;
      });
    }
    return loading;
  }

  // Entries of a page, without the names in `exclude` (already shown in the normal rows).
  function entriesFor(page, exclude) {
    const skip = {};
    (exclude || []).forEach(function (name) { skip[name] = true; });
    return (catalog || []).filter(function (entry) {
      return entry.page === page && !skip[entry.name];
    });
  }

  function formatValue(entry, live) {
    const value = live ? live[entry.name] : undefined;
    if (value === undefined || value === null || value === "") {
      return { text: "--", unit: "" };
    }
    if (typeof value === "boolean") {
      return { text: value ? "On" : "Off", unit: "" };
    }
    if (typeof value === "number") {
      return Number.isFinite(value) ? { text: String(value), unit: entry.unit } : { text: "--", unit: "" };
    }
    if (typeof value === "object") {
      return { text: JSON.stringify(value), unit: "" };
    }
    return { text: String(value), unit: entry.type === "text" ? "" : entry.unit };
  }

  function isStale(entry, live) {
    return !!(live && Array.isArray(live.stale) && live.stale.indexOf(entry.name) !== -1);
  }

  window.VcuSignals = {
    load: load,
    loaded: function () { return !!catalog; },
    entriesFor: entriesFor,
    formatValue: formatValue,
    isStale: isStale,
    labelFor: labelFor,
    _buildCatalog: buildCatalog,
    _unitFor: unitFor
  };
})();
