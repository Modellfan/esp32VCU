// Engineering page "Signals" (VCU-REQ-022): every /api/live value by category.
// The catalog comes from the firmware: /api/signals (name, group, type) plus every other
// /api/live field. New firmware signals appear without a UI change.
(function () {
  const TABS = [
    { label: "Vehicle", title: "Vehicle signals", subtitle: "Vehicle bus and engine emulation." },
    { label: "Drive", title: "Drive signals", subtitle: "Inverter values." },
    { label: "Battery", title: "Battery signals", subtitle: "Battery, heater and DC/DC." },
    { label: "VCU", title: "VCU signals", subtitle: "Internal functions, CAN and system." }
  ];

  // Firmware signal group -> tab index and row subtitle (max. 3 words, no CAN IDs).
  const GROUPS = {
    vehicle: [0, "Vehicle bus"],
    ecu: [0, "Engine emulation"],
    ecu_override: [0, "Engine override"],
    tesla_sdu: [1, "Inverter"],
    battery: [2, "Battery BMS"],
    heater: [2, "Heater request"],
    cluster: [3, "Cluster calculation"],
    fan: [3, "Coolant fan"],
    digipot: [3, "Fuel gauge"],
    charging: [3, "Charge settings"],
    can: [3, "CAN diagnostics"]
  };
  const BATTERY_PREFIXES = [["heater_", "Heater status"], ["dcdc_", "DC/DC converter"]];
  const OTHER = [3, "System status"];

  const EXACT_UNITS = {
    rpm: "rpm", sdu_udc: "V", sdu_idc: "A", sdu_il1: "A", sdu_il2: "A", sdu_ilmax: "A",
    sdu_il1rms: "A", sdu_il2rms: "A", sdu_uac: "V", sdu_uaux: "V", sdu_fstat: "Hz",
    sdu_fweakcalc: "Hz", sdu_speed: "rpm", sdu_cruisespeed: "rpm", sdu_tmphs: "°C",
    sdu_tmpm: "°C", sdu_temp_heatsink: "°C", sdu_temp_heatsink74: "°C",
    sdu_potnom: "%", sdu_regenpreset: "%", sdu_angle: "deg", sdu_cpuload: "%",
    can_bitrate: "bit/s", lenkwinkel_rb: "deg", lat_g: "g", acceleration: "g",
    battery_cell_v_min: "V", battery_cell_v_max: "V"
  };
  // Longest suffix first.
  const SUFFIX_UNITS = [
    ["_kmh", "km/h"], ["_rpm", "rpm"], ["_pct", "%"], ["_percent", "%"], ["_kwh", "kWh"],
    ["_mbar", "mbar"], ["_bar", "bar"], ["_mv", "mV"], ["_kw", "kW"], ["_hz", "Hz"],
    ["_ms", "ms"], ["_us", "µs"], ["_km", "km"], ["_c", "°C"], ["_v", "V"],
    ["_a", "A"], ["_w", "W"], ["_l", "L"], ["_s", "s"]
  ];
  const SKIP = { stale: true };

  let catalog = null;  // [{ name, tab, subtitle, type, unit }]
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

  function categoryFor(name, group) {
    if (group === "battery") {
      for (let i = 0; i < BATTERY_PREFIXES.length; i += 1) {
        if (name.indexOf(BATTERY_PREFIXES[i][0]) === 0) {
          return [2, BATTERY_PREFIXES[i][1]];
        }
      }
    }
    if (group && Object.prototype.hasOwnProperty.call(GROUPS, group)) {
      return GROUPS[group];
    }
    // Live fields outside the signal table: override flags belong to the engine emulation.
    if (/_override$/.test(name) || /^set_.*_active$/.test(name)) {
      return GROUPS.ecu_override;
    }
    return OTHER;
  }

  function buildCatalog(signalList, liveSample) {
    const seen = {};
    const entries = [];
    function add(name, group, type) {
      if (!name || seen[name] || SKIP[name]) {
        return;
      }
      seen[name] = true;
      const category = categoryFor(name, group);
      entries.push({ name: name, tab: category[0], subtitle: category[1], type: type || "", unit: unitFor(name) });
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
    // Stable order: tab, category order of first appearance, then firmware order.
    const categoryRank = {};
    entries.forEach(function (entry) {
      const key = entry.tab + "|" + entry.subtitle;
      if (!Object.prototype.hasOwnProperty.call(categoryRank, key)) {
        categoryRank[key] = Object.keys(categoryRank).length;
      }
    });
    return entries.map(function (entry, index) {
      return { entry: entry, index: index };
    }).sort(function (a, b) {
      return (a.entry.tab - b.entry.tab) ||
        (categoryRank[a.entry.tab + "|" + a.entry.subtitle] - categoryRank[b.entry.tab + "|" + b.entry.subtitle]) ||
        (a.index - b.index);
    }).map(function (item) {
      return item.entry;
    });
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

  function entriesFor(tabIndex) {
    return (catalog || []).filter(function (entry) {
      return entry.tab === tabIndex;
    });
  }

  function keysFor(tabIndex) {
    return entriesFor(tabIndex).map(function (entry) {
      return entry.name;
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
    TABS: TABS,
    load: load,
    loaded: function () { return !!catalog; },
    entriesFor: entriesFor,
    keysFor: keysFor,
    formatValue: formatValue,
    isStale: isStale,
    _buildCatalog: buildCatalog,
    _unitFor: unitFor
  };
})();
