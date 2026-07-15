(function () {
  const INTRO_DURATION_MS = 3500;
  const pageHeading = document.getElementById("pageHeading");
  const statusText = document.getElementById("statusText");
  const heroImage = document.getElementById("heroImage");
  const heroTitle = document.getElementById("heroTitle");
  const heroSubtitle = document.getElementById("heroSubtitle");
  const heroMetrics = document.getElementById("heroMetrics");
  const homeStage = document.getElementById("homeStage");
  const homeBatteryMount = document.getElementById("homeBatteryMount");
  const homeHeroImage = document.getElementById("homeHeroImage");
  const coolantStage = document.getElementById("coolantStage");
  const coolantStageMount = document.getElementById("coolantStageMount");
  const styleTestStage = document.getElementById("styleTestStage");
  const tableHead = document.querySelector(".table-head");
  const tableScroll = document.getElementById("tableScroll");
  const valueTable = document.querySelector(".value-table");
  const valueRows = document.getElementById("valueRows");
  const metricBoard = document.getElementById("metricBoard");
  const introSplash = document.getElementById("introSplash");
  const introSplashVideo = document.getElementById("introSplashVideo");
  const screenRoot = document.getElementById("screenRoot");
  const topbarTime = document.getElementById("topbarTime");
  const l1Icons = Array.from(document.querySelectorAll(".l1-icon"));
  const tabs = Array.from(document.querySelectorAll(".tab"));
  let homeBattery = null;
  let coolantStageView = null;
  let batterySettingsModal = null;
  const ENGINEERING_STORAGE_KEY = "touchEngineeringMode";
  const ENGINEERING_SECTIONS = ["cluster", "adjust"];
  const bootState = {
    introElapsed: !introSplashVideo,
    windowLoaded: document.readyState === "complete",
    hidden: false
  };

  function tryHideIntroSplash() {
    if (bootState.hidden) {
      return;
    }
    if (!bootState.introElapsed || !bootState.windowLoaded) {
      return;
    }
    bootState.hidden = true;
    if (screenRoot) {
      screenRoot.hidden = false;
    }
    if (!introSplash) {
      document.body.classList.remove("is-booting");
      return;
    }
    document.body.classList.remove("is-booting");
    introSplash.classList.add("is-hidden");
    window.setTimeout(function () {
      if (introSplash && introSplash.parentNode) {
        introSplash.parentNode.removeChild(introSplash);
      }
    }, 220);
  }

  if (introSplashVideo) {
    const markIntroVideoReady = function () {
      introSplashVideo.classList.add("is-ready");
    };
    introSplashVideo.addEventListener("loadeddata", markIntroVideoReady, { once: true });
    introSplashVideo.addEventListener("playing", markIntroVideoReady, { once: true });
    introSplashVideo.addEventListener("error", function () {
      bootState.introElapsed = true;
      tryHideIntroSplash();
    }, { once: true });
  }

  window.addEventListener("load", function () {
    bootState.windowLoaded = true;
    if (introSplashVideo && introSplashVideo.paused && !introSplashVideo.ended) {
      const playResult = introSplashVideo.play();
      if (playResult && typeof playResult.catch === "function") {
        playResult.catch(function () {
          bootState.introElapsed = true;
          tryHideIntroSplash();
        });
      }
    }
    tryHideIntroSplash();
  });

  function toNumber(value) {
    const parsed = Number(value);
    return Number.isFinite(parsed) ? parsed : null;
  }

  function clampNumber(value, min, max, fallback) {
    const numeric = toNumber(value);
    if (numeric === null) {
      return fallback;
    }
    return Math.max(min, Math.min(max, numeric));
  }

  function readEngineeringMode() {
    try {
      return window.localStorage.getItem(ENGINEERING_STORAGE_KEY) === "1";
    } catch (_) {
      return false;
    }
  }

  function persistEngineeringMode(enabled) {
    try {
      window.localStorage.setItem(ENGINEERING_STORAGE_KEY, enabled ? "1" : "0");
    } catch (_) {
      // Keep the in-memory state even when storage is unavailable.
    }
  }

  function firstNumericField(live, keys) {
    if (!live) {
      return null;
    }
    for (let index = 0; index < keys.length; index += 1) {
      const key = keys[index];
      if (!Object.prototype.hasOwnProperty.call(live, key)) {
        continue;
      }
      const numeric = toNumber(live[key]);
      if (numeric !== null) {
        return numeric;
      }
    }
    return null;
  }

  function resolveBatteryLevel(live) {
    return firstNumericField(live, [
      "battery_level",
      "battery_percent",
      "battery_soc",
      "soc",
      "hv_soc",
      "state_of_charge"
    ]);
  }

  function resolveBatteryRangeKm(live, batteryLevel) {
    const rangeKm = firstNumericField(live, [
      "range_km",
      "estimated_range_km",
      "battery_range_km"
    ]);
    if (rangeKm !== null) {
      return rangeKm;
    }
    const rangeMi = firstNumericField(live, [
      "range_mi",
      "estimated_range_mi",
      "battery_range_mi"
    ]);
    if (rangeMi !== null) {
      return rangeMi * 1.60934;
    }
    const resolvedLevel = batteryLevel === null || batteryLevel === undefined ? resolveBatteryLevel(live) : batteryLevel;
    return resolvedLevel === null ? 85 : resolvedLevel * 3.05;
  }

  function resolveBatteryPowerKw(live, batteryLevel) {
    const powerKw = firstNumericField(live, [
      "charge_power_kw",
      "battery_power_kw",
      "hv_power_kw"
    ]);
    if (powerKw !== null) {
      return powerKw;
    }
    const voltage = firstNumericField(live, ["sdu_udc"]);
    const current = firstNumericField(live, ["sdu_idc"]);
    if (voltage !== null && current !== null) {
      return (voltage * current) / 1000;
    }
    const resolvedLevel = batteryLevel === null || batteryLevel === undefined ? resolveBatteryLevel(live) : batteryLevel;
    return resolvedLevel === null ? 193 : 120 + (resolvedLevel / 100) * 260;
  }

  function firstBooleanField(live, keys) {
    if (!live) {
      return null;
    }
    for (let index = 0; index < keys.length; index += 1) {
      const key = keys[index];
      if (!Object.prototype.hasOwnProperty.call(live, key)) {
        continue;
      }
      return !!live[key];
    }
    return null;
  }

  function resolveChargePowerKw(live) {
    const explicitChargePower = firstNumericField(live, [
      "charge_power_kw",
      "ac_charge_power_kw",
      "charger_power_kw"
    ]);
    if (explicitChargePower !== null && explicitChargePower > 0) {
      return explicitChargePower;
    }

    const vehicleSpeed = firstNumericField(live, ["vehicle_speed_510"]);
    const dcCurrent = firstNumericField(live, ["sdu_idc"]);
    const dcVoltage = firstNumericField(live, ["sdu_udc"]);
    if (
      vehicleSpeed !== null &&
      vehicleSpeed < 1 &&
      dcCurrent !== null &&
      dcVoltage !== null &&
      dcCurrent < -1
    ) {
      return Math.abs((dcVoltage * dcCurrent) / 1000);
    }

    return 0;
  }

  function resolveChargeActive(live) {
    const explicitState = firstBooleanField(live, [
      "charging",
      "is_charging",
      "charge_active",
      "ac_charging_active"
    ]);
    if (explicitState !== null) {
      return explicitState;
    }
    return resolveChargePowerKw(live) > 0.1;
  }

  function resolveChargeCableConnected(live) {
    const explicitState = firstBooleanField(live, [
      "charge_cable_connected",
      "charging_cable_connected",
      "plug_connected",
      "charge_plug_connected",
      "charger_connected",
      "evse_connected",
      "ac_plug_connected"
    ]);
    if (explicitState !== null) {
      return explicitState;
    }
    return resolveChargeActive(live);
  }

  function estimateChargeFinishTimestamp(live, batteryLevel) {
    if (!resolveChargeActive(live)) {
      return null;
    }

    const currentLevel = batteryLevel === null || batteryLevel === undefined ? resolveBatteryLevel(live) : batteryLevel;
    const resolvedLevel = currentLevel === null ? 28 : currentLevel;
    const targetCharge = state.batteryControls.targetChargePct;
    if (targetCharge <= resolvedLevel) {
      return Date.now();
    }

    let chargePowerKw = resolveChargePowerKw(live);
    if (chargePowerKw <= 0.1) {
      chargePowerKw = state.batteryControls.acCurrentLimitA * 230 * 0.92 / 1000;
    }
    if (chargePowerKw <= 0.1) {
      return null;
    }

    const batteryCapacityKwh = firstNumericField(live, [
      "battery_capacity_kwh",
      "hv_battery_capacity_kwh"
    ]) || 82;
    const remainingEnergyKwh = batteryCapacityKwh * ((targetCharge - resolvedLevel) / 100);
    const remainingMs = Math.max(0, (remainingEnergyKwh / chargePowerKw) * 3600000);
    return Date.now() + remainingMs;
  }

  function batteryLevelField() {
    const render = customField(function (live) {
      const value = resolveBatteryLevel(live);
      return formatInteger(value === null ? 28 : value, "%");
    });
    render.chartKey = "battery_level_pct";
    render.chartUnit = "%";
    render.chartValue = function (live) {
      const value = resolveBatteryLevel(live || {});
      return value === null ? 28 : Number(value);
    };
    return render;
  }

  function batteryRangeKmField() {
    const render = customField(function (live) {
      const level = resolveBatteryLevel(live);
      return formatInteger(resolveBatteryRangeKm(live, level), "km");
    });
    render.chartKey = "battery_range_km";
    render.chartUnit = "km";
    render.chartValue = function (live) {
      const level = resolveBatteryLevel(live || {});
      return Number(resolveBatteryRangeKm(live || {}, level));
    };
    return render;
  }

  function batteryPowerKwField() {
    const render = customField(function (live) {
      const level = resolveBatteryLevel(live);
      return formatFixed(resolveBatteryPowerKw(live, level), 1, "kW");
    });
    render.chartKey = "battery_power_kw";
    render.chartUnit = "kW";
    render.chartValue = function (live) {
      const level = resolveBatteryLevel(live || {});
      return Number(resolveBatteryPowerKw(live || {}, level));
    };
    return render;
  }

  function batteryTargetChargeField() {
    const render = customField(function () {
      return formatInteger(state.batteryControls.targetChargePct, "%");
    });
    render.chartKey = "target_charge_pct";
    render.chartUnit = "%";
    render.chartValue = function () {
      return Number(state.batteryControls.targetChargePct);
    };
    return render;
  }

  function batteryAcCurrentLimitField() {
    const render = customField(function () {
      return formatInteger(state.batteryControls.acCurrentLimitA, "A");
    });
    render.chartKey = "ac_current_limit_a";
    render.chartUnit = "A";
    render.chartValue = function () {
      return Number(state.batteryControls.acCurrentLimitA);
    };
    return render;
  }

  function digitalPotField() {
    const render = customField(function () {
      return formatInteger(state.batteryControls.digitalPotPct, "%");
    });
    render.chartKey = "digital_pot_pct";
    render.chartUnit = "%";
    render.chartValue = function () {
      return Number(state.batteryControls.digitalPotPct);
    };
    return render;
  }

  function formatFixed(value, digits, unit) {
    const numeric = toNumber(value);
    if (numeric === null) {
      return "--";
    }
    const body = numeric.toFixed(digits);
    return unit ? body + " " + unit : body;
  }

  function formatInteger(value, unit) {
    const numeric = toNumber(value);
    if (numeric === null) {
      return "--";
    }
    const body = String(Math.round(numeric));
    return unit ? body + " " + unit : body;
  }

  function formatBoolValue(live, key, onLabel, offLabel) {
    if (!live || !Object.prototype.hasOwnProperty.call(live, key)) {
      return "--";
    }
    return live[key] ? onLabel : offLabel;
  }

  function formatPrndValue(value) {
    const numeric = Math.round(Number(value));
    if (!Number.isFinite(numeric)) {
      return "--";
    }
    const labels = { 0: "P", 1: "R", 2: "N", 3: "D" };
    return labels[numeric] || String(numeric);
  }

  function escapeHtml(value) {
    return String(value)
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;")
      .replace(/"/g, "&quot;")
      .replace(/'/g, "&#39;");
  }

  function textField(key, fallback) {
    return function (live) {
      if (!live || live[key] == null || live[key] === "") {
        return fallback || "--";
      }
      return String(live[key]);
    };
  }

  function fixedField(key, digits, unit) {
    const render = function (live) {
      return formatFixed(live ? live[key] : null, digits, unit || "");
    };
    render.chartKey = key;
    render.chartUnit = unit || "";
    render.chartValue = function (live) {
      return live ? Number(live[key]) : NaN;
    };
    return render;
  }

  function accelerationTimeField(key, validKey) {
    return function (live) {
      if (!live || !live[validKey]) {
        return "--";
      }
      return formatFixed(live[key], 2, "s");
    };
  }

  function intField(key, unit) {
    const render = function (live) {
      return formatInteger(live ? live[key] : null, unit || "");
    };
    render.chartKey = key;
    render.chartUnit = unit || "";
    render.chartValue = function (live) {
      return live ? Number(live[key]) : NaN;
    };
    return render;
  }

  function boolField(key, onLabel, offLabel) {
    return function (live) {
      return formatBoolValue(live, key, onLabel || "ON", offLabel || "OFF");
    };
  }

  function coolantFanField() {
    return customField(function (live) {
      const level = Math.max(0, Math.min(2, Math.round(Number(live.coolant_fan_level) || 0)));
      return ["Off", "Low", "High"][level];
    });
  }

  function customField(render) {
    return function (live) {
      return render(live || {});
    };
  }

  function field(label, render, options) {
    const row = { label: label, render: render };
    if (options && options.controlKey) {
      row.controlKey = options.controlKey;
    }
    if (options && options.chartKey) {
      row.chartKey = options.chartKey;
      row.chartUnit = options.chartUnit || "";
    }
    if (!row.chartKey && render && render.chartKey) {
      row.chartKey = render.chartKey;
      row.chartUnit = render.chartUnit || "";
    }
    if (render && render.chartValue) {
      row.chartValue = render.chartValue;
    }
    return row;
  }

  function numericField(label, key, render, unit) {
    return field(label, render, {
      chartKey: key,
      chartUnit: unit || ""
    });
  }

  const SDU_ENUMS = {
    OPMODES: {
      "0": "Off",
      "1": "Run",
      "2": "ManualRun",
      "3": "Boost",
      "4": "Buck",
      "5": "Sine",
      "6": "AcHeat"
    },
    ERRORS: {
      "0": "OVERCURRENT",
      "1": "THROTTLE1",
      "2": "THROTTLE2",
      "3": "CANTIMEOUT",
      "4": "EMCYSTOP",
      "5": "MPROT",
      "6": "DESAT",
      "7": "OVERVOLTAGE",
      "8": "ENCODER",
      "9": "PRECHARGE",
      "10": "TMPHSMAX",
      "11": "CURRENTLIMIT",
      "12": "PWMSTUCK",
      "13": "HICUROFS1",
      "14": "HICUROFS2",
      "15": "HIRESOFS",
      "16": "LORESAMP",
      "17": "TMPMMAX",
      "18": "CANCRC",
      "19": "CANCOUNTER"
    },
    DIRS: {
      "-1": "Reverse",
      "0": "Neutral",
      "1": "Forward"
    },
    ONOFF: {
      "0": "Off",
      "1": "On",
      "2": "na"
    },
    OKERR: {
      "0": "Error",
      "1": "Ok",
      "2": "na"
    },
    adjust: {
      image: "/touch/assets/adjust.svg",
      tabs: [
        {
          label: "Setup",
          title: "Adjust Setup",
          subtitle: "Adjustment shortcuts and display calibration values.",
          rows: [
            field("Light Dimmer", intField("light_dimmer", "")),
            field("Target Charge", batteryTargetChargeField(), { controlKey: "target_charge_pct" }),
            field("AC Current Limit", batteryAcCurrentLimitField(), { controlKey: "ac_current_limit_a" }),
            field("Digital Potentiometer", digitalPotField(), { controlKey: "digital_pot_pct" }),
            field("Access Point IP", textField("ap_ip", "--"))
          ],
          hero: [
            field("Dimmer", intField("light_dimmer", "")),
            field("Target", batteryTargetChargeField()),
            field("AC Limit", batteryAcCurrentLimitField())
          ]
        },
        {
          label: "Drive",
          title: "Adjust Drive",
          subtitle: "Drive-related controls and live reference values.",
          rows: [
            field("Vehicle Speed", fixedField("vehicle_speed_510", 1, "km/h")),
            field("Pedal Position 1", fixedField("pedal_position_1", 1, "%")),
            field("Pedal Position 2", fixedField("pedal_position_2", 1, "%")),
            field("Throttle Position", fixedField("throttle_position_280", 1, "%")),
            field("Brake Offset", fixedField("brake_normal_or_offset", 2, "%"))
          ],
          hero: [
            field("Speed", fixedField("vehicle_speed_510", 1, "km/h")),
            field("Pedal", fixedField("pedal_position_1", 1, "%")),
            field("Brake", fixedField("brake_normal_or_offset", 2, "%"))
          ]
        },
        {
          label: "Power",
          title: "Adjust Power",
          subtitle: "Power calibration values for battery and SDU context.",
          rows: [
            field("Battery Charge", batteryLevelField()),
            field("Estimated Range", batteryRangeKmField()),
            field("Battery Power", batteryPowerKwField()),
            field("DC Bus Voltage", fixedField("sdu_udc", 2, "V")),
            field("DC Bus Current", fixedField("sdu_idc", 2, "A"))
          ],
          hero: [
            field("Charge", batteryLevelField()),
            field("Power", batteryPowerKwField()),
            field("UDC", fixedField("sdu_udc", 2, "V"))
          ]
        },
        {
          label: "Info",
          title: "Adjust Info",
          subtitle: "Reference state used by the adjustment page.",
          rows: [
            field("AP SSID", textField("ap_ssid", "eboxster")),
            field("Cluster Counter", intField("cluster_counter", "")),
            field("ACC Receipt", boolField("receipt_for_acc_message", "YES", "NO")),
            field("Tempomat Button", boolField("tempomat_button", "PRESSED", "IDLE")),
            field("Driver Braking", boolField("driver_braking", "YES", "NO"))
          ],
          hero: [
            field("SSID", textField("ap_ssid", "eboxster")),
            field("Counter", intField("cluster_counter", "")),
            field("ACC", boolField("receipt_for_acc_message", "YES", "NO"))
          ]
        }
      ]
    },
    ventilation: {
      image: "/touch/assets/active-cabin-ventilation.svg",
      tabs: [
        {
          label: "Active",
          title: "Active Ventilation",
          subtitle: "Cabin ventilation and thermal context.",
          rows: [
            field("Ambient Temperature", fixedField("ambient_c", 1, "C")),
            field("Inlet Air", fixedField("inlet_air_temperature", 1, "C")),
            field("ECU Coolant", fixedField("motor2_coolant_temperature", 1, "C")),
            field("AC Compressor", boolField("motor2_ac_compressor_active", "ON", "OFF")),
            field("Fan Level", coolantFanField())
          ],
          hero: [
            field("Ambient", fixedField("ambient_c", 1, "C")),
            field("Inlet", fixedField("inlet_air_temperature", 1, "C")),
            field("Fan", coolantFanField())
          ]
        },
        {
          label: "Thermal",
          title: "Ventilation Thermal",
          subtitle: "Temperature channels that influence cabin ventilation decisions.",
          rows: [
            field("SDU Heatsink", fixedField("sdu_tmphs", 2, "C")),
            field("SDU Motor", fixedField("sdu_tmpm", 2, "C")),
            field("Motor Critical", boolField("cluster_motor_temp_critical", "YES", "NO")),
            field("Heatsink Critical", boolField("cluster_heatsink_temp_critical", "YES", "NO")),
            field("Oil Pressure", fixedField("oil_pressure_bar", 2, "bar"))
          ],
          hero: [
            field("Heatsink", fixedField("sdu_tmphs", 2, "C")),
            field("Motor", fixedField("sdu_tmpm", 2, "C")),
            field("Oil", fixedField("oil_pressure_bar", 2, "bar"))
          ]
        },
        {
          label: "Outputs",
          title: "Ventilation Outputs",
          subtitle: "Cooling and auxiliary output states.",
          rows: [
            field("Coolant Fan Low", boolField("coolant_fan_low_output", "ON", "OFF")),
            field("Coolant Fan High", boolField("coolant_fan_high_output", "ON", "OFF")),
            field("Fan Error", boolField("fan_error", "YES", "NO")),
            field("Auxiliary Voltage", fixedField("sdu_uaux", 2, "V")),
            field("Light Dimmer", intField("light_dimmer", ""))
          ],
          hero: [
            field("Fan Low", boolField("coolant_fan_low_output", "ON", "OFF")),
            field("Fan High", boolField("coolant_fan_high_output", "ON", "OFF")),
            field("UAux", fixedField("sdu_uaux", 2, "V"))
          ]
        },
        {
          label: "Cabin",
          title: "Cabin State",
          subtitle: "Driver input and cabin-related state overview.",
          rows: [
            field("Indicator Left", boolField("indicator_left", "ON", "OFF")),
            field("Indicator Right", boolField("indicator_right", "ON", "OFF")),
            field("Tempomat Button", boolField("tempomat_button", "PRESSED", "IDLE")),
            field("Wheel Up Button", boolField("steeringwheel_button_up", "PRESSED", "IDLE")),
            field("Wheel Down Button", boolField("steeringwheel_button_down", "PRESSED", "IDLE"))
          ],
          hero: [
            field("Left", boolField("indicator_left", "ON", "OFF")),
            field("Right", boolField("indicator_right", "ON", "OFF")),
            field("Tempomat", boolField("tempomat_button", "PRESSED", "IDLE"))
          ]
        }
      ]
    },
    stopwatch: {
      image: "/touch/assets/stopwatch.svg",
      tabs: [
        {
          label: "Timer",
          title: "Stopwatch Timer",
          subtitle: "Timing context derived from live counters and speed.",
          rows: [
            field("SDU Uptime", intField("sdu_uptime", "10ms")),
            field("Cluster Counter", intField("cluster_counter", "")),
            field("Vehicle Speed", fixedField("vehicle_speed_510", 1, "km/h")),
            field("Reference Speed", fixedField("vref_kmh", 1, "km/h")),
            field("Acceleration", fixedField("acceleration", 3, "g"))
          ],
          hero: [
            field("Uptime", intField("sdu_uptime", "10ms")),
            field("Speed", fixedField("vehicle_speed_510", 1, "km/h")),
            field("Accel", fixedField("acceleration", 3, "g"))
          ]
        },
        {
          label: "Lap",
          title: "Stopwatch Lap",
          subtitle: "Lap-style signal snapshot for drivetrain timing.",
          rows: [
            field("Motor Speed", intField("sdu_speed", "rpm")),
            field("Cruise Speed", intField("sdu_cruisespeed", "rpm")),
            field("Turns", intField("sdu_turns", "")),
            field("Angle", fixedField("sdu_angle", 1, "deg")),
            field("Frequency", fixedField("sdu_fstat", 2, "Hz"))
          ],
          hero: [
            field("Speed", intField("sdu_speed", "rpm")),
            field("Turns", intField("sdu_turns", "")),
            field("Freq", fixedField("sdu_fstat", 2, "Hz"))
          ]
        },
        {
          label: "Trip",
          title: "Stopwatch Trip",
          subtitle: "Trip timing reference values.",
          rows: [
            field("Battery Charge", batteryLevelField()),
            field("Estimated Range", batteryRangeKmField()),
            field("Battery Power", batteryPowerKwField()),
            field("Consumption", intField("mo5_verbrauch_ul", "uL")),
            field("Cluster Power Dyn", fixedField("cluster_power_percent_dyn", 3, "%"))
          ],
          hero: [
            field("Charge", batteryLevelField()),
            field("Range", batteryRangeKmField()),
            field("Dyn", fixedField("cluster_power_percent_dyn", 3, "%"))
          ]
        },
        {
          label: "Status",
          title: "Stopwatch Status",
          subtitle: "Status and warning context for timed runs.",
          rows: [
            field("TCS", boolField("tcs", "ACTIVE", "OFF")),
            field("ABS", boolField("abs", "ACTIVE", "OFF")),
            field("FDR", boolField("fdr", "ACTIVE", "OFF")),
            field("FDR Lamp", boolField("fdr_lamp", "ON", "OFF")),
            field("PSM Lamp", boolField("psm_button_lamp", "ON", "OFF"))
          ],
          hero: [
            field("TCS", boolField("tcs", "ACTIVE", "OFF")),
            field("ABS", boolField("abs", "ACTIVE", "OFF")),
            field("FDR", boolField("fdr", "ACTIVE", "OFF"))
          ]
        }
      ]
    }
  };

  const SDU_BITFIELDS = {
    CANIOS: {
      "1": "Cruise",
      "2": "Start",
      "4": "Brake",
      "8": "Fwd",
      "16": "Rev",
      "32": "Bms"
    },
    STATUS: {
      "1": "UdcLow",
      "2": "UdcHigh",
      "4": "UdcBelowUdcSw",
      "8": "UdcLim",
      "16": "EmcyStop",
      "32": "MProt",
      "64": "PotPressed",
      "128": "TmpHs",
      "256": "WaitStart",
      "512": "BrakeCheck"
    }
  };

  function integerValue(value) {
    const numeric = toNumber(value);
    return numeric === null ? null : Math.round(numeric);
  }

  function enumText(enumDef, value) {
    const numeric = integerValue(value);
    if (numeric === null) {
      return "--";
    }
    const key = String(numeric);
    return Object.prototype.hasOwnProperty.call(enumDef, key) ? enumDef[key] : key;
  }

  function bitfieldText(bitfieldDef, value) {
    const numeric = integerValue(value);
    if (numeric === null) {
      return "--";
    }
    const labels = [];
    let knownMask = 0;
    Object.keys(bitfieldDef).map(Number).sort(function (a, b) { return a - b; }).forEach(function (flag) {
      knownMask |= flag;
      if ((numeric & flag) === flag) {
        labels.push(bitfieldDef[String(flag)]);
      }
    });
    const unknownMask = numeric & ~knownMask;
    if (unknownMask) {
      labels.push(String(unknownMask));
    }
    return labels.length ? labels.join(" | ") : "None";
  }

  function enumField(key, enumDef) {
    return function (live) {
      return enumText(enumDef, live ? live[key] : null);
    };
  }

  function bitfieldField(key, bitfieldDef) {
    return function (live) {
      return bitfieldText(bitfieldDef, live ? live[key] : null);
    };
  }

  const sduDriveTileFields = [
    field("opmode", enumField("sdu_opmode", SDU_ENUMS.OPMODES)),
    field("lasterr", enumField("sdu_lasterr", SDU_ENUMS.ERRORS)),
    field("status", bitfieldField("sdu_status", SDU_BITFIELDS.STATUS)),
    numericField("udc", "sdu_udc", fixedField("sdu_udc", 3, "V"), "V"),
    numericField("idc", "sdu_idc", fixedField("sdu_idc", 3, "A"), "A"),
    numericField("il1", "sdu_il1", fixedField("sdu_il1", 2, "A"), "A"),
    numericField("il2", "sdu_il2", fixedField("sdu_il2", 2, "A"), "A"),
    numericField("fstat", "sdu_fstat", fixedField("sdu_fstat", 2, "Hz"), "Hz"),
    numericField("speed", "sdu_speed", intField("sdu_speed", "rpm"), "rpm"),
    numericField("cruisespeed", "sdu_cruisespeed", intField("sdu_cruisespeed", "rpm"), "rpm"),
    numericField("turns", "sdu_turns", intField("sdu_turns", ""), ""),
    numericField("amp", "sdu_amp", fixedField("sdu_amp", 2, ""), ""),
    numericField("angle", "sdu_angle", fixedField("sdu_angle", 1, "deg"), "deg"),
    numericField("pot", "sdu_pot", intField("sdu_pot", ""), ""),
    numericField("pot2", "sdu_pot2", intField("sdu_pot2", ""), ""),
    numericField("potnom", "sdu_potnom", fixedField("sdu_potnom", 2, "%"), "%"),
    numericField("regenpreset", "sdu_regenpreset", fixedField("sdu_regenpreset", 2, "%"), "%"),
    field("seldir", enumField("sdu_seldir", SDU_ENUMS.DIRS)),
    field("rotordir", enumField("sdu_rotordir", SDU_ENUMS.DIRS)),
    numericField("tmphs", "sdu_tmphs", fixedField("sdu_tmphs", 2, "\u00B0C"), "\u00B0C"),
    numericField("tmpm", "sdu_tmpm", fixedField("sdu_tmpm", 2, "\u00B0C"), "\u00B0C"),
    numericField("uaux", "sdu_uaux", fixedField("sdu_uaux", 2, "V"), "V"),
    numericField("pwmio", "sdu_pwmio", intField("sdu_pwmio", ""), ""),
    field("canio", bitfieldField("sdu_canio", SDU_BITFIELDS.CANIOS)),
    field("din_cruise", enumField("sdu_din_cruise", SDU_ENUMS.ONOFF)),
    field("din_start", enumField("sdu_din_start", SDU_ENUMS.ONOFF)),
    field("din_brake", enumField("sdu_din_brake", SDU_ENUMS.ONOFF)),
    field("din_mprot", enumField("sdu_din_mprot", SDU_ENUMS.OKERR)),
    field("din_forward", enumField("sdu_din_forward", SDU_ENUMS.ONOFF)),
    field("din_reverse", enumField("sdu_din_reverse", SDU_ENUMS.ONOFF)),
    field("din_emcystop", enumField("sdu_din_emcystop", SDU_ENUMS.OKERR)),
    field("din_ocur", enumField("sdu_din_ocur", SDU_ENUMS.OKERR)),
    field("din_desat", enumField("sdu_din_desat", SDU_ENUMS.OKERR)),
    field("din_bms", enumField("sdu_din_bms", SDU_ENUMS.ONOFF)),
    numericField("uptime", "sdu_uptime", intField("sdu_uptime", "10ms"), "10ms"),
    numericField("cpuload", "sdu_cpuload", fixedField("sdu_cpuload", 1, "%"), "%"),
    numericField("ilmax", "sdu_ilmax", fixedField("sdu_ilmax", 2, "A"), "A"),
    numericField("uac", "sdu_uac", fixedField("sdu_uac", 2, "V"), "V"),
    numericField("il1rms", "sdu_il1rms", fixedField("sdu_il1rms", 2, "A"), "A"),
    numericField("il2rms", "sdu_il2rms", fixedField("sdu_il2rms", 2, "A"), "A"),
    numericField("boostcalc", "sdu_boostcalc", fixedField("sdu_boostcalc", 2, ""), ""),
    numericField("fweakcalc", "sdu_fweakcalc", fixedField("sdu_fweakcalc", 2, "Hz"), "Hz")
  ];

  const sections = {
    home: {
      image: "/touch/img/half_front.png",
      tabs: [
        {
          label: "",
          title: "",
          subtitle: "",
          rows: [],
          hero: []
        }
      ]
    },
    vehicle: {
      image: "/touch/assets/car.svg",
      tabs: [
        {
          label: "Drive",
          title: "Vehicle Drive",
          subtitle: "Primary vehicle speed summary with the full SDU dashboard rendered as live tiles.",
          rows: [
            field("Vehicle Speed 510", fixedField("vehicle_speed_510", 1, "km/h")),
            field("Reference Speed", fixedField("vref_kmh", 1, "km/h")),
            field("Acceleration", fixedField("acceleration", 3, "g")),
            field("Gear Position", customField(function (live) { return formatPrndValue(live.prnd_pos); })),
            field("Pedal Position 1", fixedField("pedal_position_1", 1, "%")),
            field("Pedal Position 2", fixedField("pedal_position_2", 1, "%")),
            field("Throttle Position", fixedField("throttle_position_280", 1, "%")),
            field("Brake Offset", fixedField("brake_normal_or_offset", 2, "%"))
          ],
          tileBoard: {
            columns: 4,
            tiles: sduDriveTileFields
          },
          hero: [
            field("Vehicle Speed", fixedField("vehicle_speed_510", 1, "km/h")),
            field("Acceleration", fixedField("acceleration", 3, "g")),
            field("PRND", customField(function (live) { return formatPrndValue(live.prnd_pos); }))
          ]
        },
        {
          label: "Battery",
          title: "Vehicle Battery",
          subtitle: "High-voltage battery status, range, and drive-unit electrical values.",
          rows: [
            field("Battery Charge", batteryLevelField()),
            field("Estimated Range", batteryRangeKmField()),
            field("Target Charge", batteryTargetChargeField(), { controlKey: "target_charge_pct" }),
            field("AC Current Limit", batteryAcCurrentLimitField(), { controlKey: "ac_current_limit_a" }),
            field("Digital Potentiometer", digitalPotField(), { controlKey: "digital_pot_pct" }),
            field("Battery Power", batteryPowerKwField()),
            field("DC Bus Voltage", fixedField("sdu_udc", 2, "V")),
            field("DC Bus Current", fixedField("sdu_idc", 2, "A")),
            field("Auxiliary Voltage", fixedField("sdu_uaux", 2, "V")),
            field("Regen Available 1", fixedField("sdu_regenpresent", 2, "%")),
            field("Regen Available 2", fixedField("sdu_regenpresent32", 2, "%")),
            field("Cluster Power Max", fixedField("cluster_power_percent_max", 3, "%")),
            field("Cluster Power Dyn", fixedField("cluster_power_percent_dyn", 3, "%"))
          ],
          hero: [
            field("Charge", batteryLevelField()),
            field("Range", batteryRangeKmField()),
            field("Power", batteryPowerKwField())
          ]
        },
        {
          label: "Stability",
          title: "Vehicle Stability",
          subtitle: "ABS, TCS, FDR, and yaw-related intervention data.",
          rows: [
            field("TCS Intervention", boolField("tcs", "ACTIVE", "OFF")),
            field("MSR Request", boolField("msr", "ACTIVE", "OFF")),
            field("ABS Intervention", boolField("abs", "ACTIVE", "OFF")),
            field("ABD Intervention", boolField("abd", "ACTIVE", "OFF")),
            field("FDR Intervention", boolField("fdr", "ACTIVE", "OFF")),
            field("ASR Control Mode", intField("asr_mode", "")),
            field("TCS Slow", fixedField("tcs_slow_pct", 2, "%")),
            field("TCS Fast", fixedField("tcs_fast_pct", 2, "%")),
            field("MSR Torque Inverse", fixedField("msr_inv_pct", 2, "%")),
            field("Intervention Torque", fixedField("itq_pct", 2, "%")),
            field("Yaw Rate", fixedField("yaw_rate", 4, "deg/s"))
          ],
          hero: [
            field("TCS", boolField("tcs", "ACTIVE", "OFF")),
            field("FDR", boolField("fdr", "ACTIVE", "OFF")),
            field("Yaw Rate", fixedField("yaw_rate", 4, "deg/s"))
          ]
        },
        {
          label: "Steering",
          title: "Vehicle Steering",
          subtitle: "Steering wheel inputs, angle validity, and driver commands.",
          rows: [
            field("Steering Angle", fixedField("lenkwinkel_rb", 2, "deg")),
            field("Angle Valid", boolField("lws_ok_rb", "YES", "NO")),
            field("Fine Check", boolField("fine_check_rb", "PASS", "FAIL")),
            field("Coarse Check", boolField("coarse_check_rb", "PASS", "FAIL")),
            field("Indicator Left", boolField("indicator_left", "ON", "OFF")),
            field("Indicator Right", boolField("indicator_right", "ON", "OFF")),
            field("Tempomat Button", boolField("tempomat_button", "PRESSED", "IDLE")),
            field("Wheel Up Button", boolField("steeringwheel_button_up", "PRESSED", "IDLE")),
            field("Wheel Down Button", boolField("steeringwheel_button_down", "PRESSED", "IDLE")),
            field("Lateral Accel", fixedField("lat_g", 3, "g"))
          ],
          hero: [
            field("Steering Angle", fixedField("lenkwinkel_rb", 2, "deg")),
            field("Lateral Accel", fixedField("lat_g", 3, "g")),
            field("Tempomat", boolField("tempomat_button", "PRESSED", "IDLE"))
          ]
        }
      ]
    },
    coolant: {
      image: "/touch/assets/i_ecu.svg",
      tabs: [
        {
          label: "",
          title: "Coolant System",
          subtitle: "Animated coolant loop overview for the touch display.",
          rows: [
            field("Fan Level", coolantFanField(), { controlKey: "coolant_fan_level" }),
            field("Low Output", boolField("coolant_fan_low_output", "ON", "OFF")),
            field("High Output", boolField("coolant_fan_high_output", "ON", "OFF"))
          ],
          hero: []
        }
      ]
    },
    styleTest: {
      image: "/touch/assets/i_sdu.svg",
      tabs: [
        {
          label: "Drive",
          title: "Porsche Drive Reference",
          subtitle: "Reference recreation test page.",
          rows: [],
          hero: []
        },
        {
          label: "Assistance",
          title: "Porsche Assistance Reference",
          subtitle: "Reference recreation test page.",
          rows: [],
          hero: []
        },
        {
          label: "Trip",
          title: "Porsche Trip Reference",
          subtitle: "Reference recreation test page.",
          rows: [],
          hero: []
        },
        {
          label: "Comfort",
          title: "Porsche Comfort Reference",
          subtitle: "Reference recreation test page.",
          rows: [],
          hero: []
        }
      ]
    },
    sdu: {
      image: "/touch/assets/i_sdu.svg",
      tabs: [
        {
          label: "Power",
          title: "SDU Power",
          subtitle: "Electrical drive unit power signals for the wide touch layout.",
          rows: [
            field("DC Bus Voltage", fixedField("sdu_udc", 2, "V")),
            field("DC Bus Current", fixedField("sdu_idc", 2, "A")),
            field("Auxiliary Voltage", fixedField("sdu_uaux", 2, "V")),
            field("Regen Available 1", fixedField("sdu_regenpresent", 2, "%")),
            field("Regen Available 2", fixedField("sdu_regenpresent32", 2, "%")),
            field("Last Error", intField("sdu_lasterr", ""))
          ],
          hero: [
            field("UDC", fixedField("sdu_udc", 2, "V")),
            field("IDC", fixedField("sdu_idc", 2, "A")),
            field("UAux", fixedField("sdu_uaux", 2, "V"))
          ]
        },
        {
          label: "Drive",
          title: "SDU Drive",
          subtitle: "Complete SDU signal board with drive, input, power, and thermal telemetry.",
          rows: [
            field("Operating Mode", intField("sdu_opmode", "")),
            field("Status", intField("sdu_status", "")),
            field("Motor Speed", intField("sdu_speed", "rpm")),
            field("Cruise Speed", intField("sdu_cruisespeed", "rpm")),
            field("Selected Direction", intField("sdu_seldir", "")),
            field("Selected Direction Aux", intField("sdu_seldir43", ""))
          ],
          tileBoard: {
            columns: 4,
            tiles: sduDriveTileFields
          },
          hero: [
            field("Opmode", intField("sdu_opmode", "")),
            field("Speed", intField("sdu_speed", "rpm")),
            field("Direction", intField("sdu_seldir", ""))
          ]
        },
        {
          label: "Inputs",
          title: "SDU Inputs",
          subtitle: "Pedal and digital input states feeding the drive unit.",
          rows: [
            field("Pedal Input 1", intField("sdu_pot", "counts")),
            field("Pedal Input 2", intField("sdu_pot2", "counts")),
            field("Digital Input OCUR", intField("sdu_din_ocur", "")),
            field("Digital Input OCUR51", intField("sdu_din_ocur51", "")),
            field("Digital Input BMS", intField("sdu_din_bms", "")),
            field("Last Error", intField("sdu_lasterr", ""))
          ],
          hero: [
            field("POT", intField("sdu_pot", "")),
            field("POT2", intField("sdu_pot2", "")),
            field("BMS", intField("sdu_din_bms", ""))
          ]
        },
        {
          label: "Thermal",
          title: "SDU Thermal",
          subtitle: "Thermal channels from the drive unit with cluster critical overlays.",
          rows: [
            field("Heatsink Temperature", fixedField("sdu_temp_heatsink", 2, "C")),
            field("Heatsink Temperature 74", fixedField("sdu_temp_heatsink74", 2, "C")),
            field("tmphs", fixedField("sdu_tmphs", 2, "C")),
            field("tmpm", fixedField("sdu_tmpm", 2, "C")),
            field("Cluster Heatsink Critical", boolField("cluster_heatsink_temp_critical", "YES", "NO")),
            field("Cluster Motor Critical", boolField("cluster_motor_temp_critical", "YES", "NO"))
          ],
          hero: [
            field("Heatsink", fixedField("sdu_tmphs", 2, "C")),
            field("Motor", fixedField("sdu_tmpm", 2, "C")),
            field("Critical", customField(function (live) {
              const heatsink = formatBoolValue(live, "cluster_heatsink_temp_critical", "HS", "-");
              const motor = formatBoolValue(live, "cluster_motor_temp_critical", "M", "-");
              return heatsink + " / " + motor;
            }))
          ]
        }
      ]
    },
    cluster: {
      image: "/touch/assets/i_cluster.svg",
      tabs: [
        {
          label: "State",
          title: "Cluster State",
          subtitle: "Cluster activation and core output values mirrored from the firmware.",
          rows: [
            field("Cluster Activated", boolField("cluster_activated", "YES", "NO")),
            field("Needle Position", intField("cluster_needle_position", "")),
            field("Total Fuel", fixedField("cluster_total_fuel_l", 3, "L")),
            field("Heatsink Critical", boolField("cluster_heatsink_temp_critical", "YES", "NO")),
            field("Motor Critical", boolField("cluster_motor_temp_critical", "YES", "NO")),
            field("Access Point SSID", textField("ap_ssid", "eboxster")),
            field("Access Point IP", textField("ap_ip", "--"))
          ],
          hero: [
            field("Activated", boolField("cluster_activated", "YES", "NO")),
            field("Needle", intField("cluster_needle_position", "")),
            field("Fuel", fixedField("cluster_total_fuel_l", 3, "L"))
          ]
        },
        {
          label: "Power",
          title: "Cluster Power",
          subtitle: "Cluster power calculations correlated with vehicle and ECU context.",
          rows: [
            field("Power Percent Max", fixedField("cluster_power_percent_max", 3, "%")),
            field("Power Percent Dyn", fixedField("cluster_power_percent_dyn", 3, "%")),
            field("Oil Pressure", fixedField("oil_pressure_bar", 2, "bar")),
            field("Ambient Temperature", fixedField("ambient_c", 1, "C")),
            field("Consumption", intField("mo5_verbrauch_ul", "uL")),
            field("Vehicle Speed", fixedField("vehicle_speed_510", 1, "km/h"))
          ],
          hero: [
            field("Max Power", fixedField("cluster_power_percent_max", 3, "%")),
            field("Dyn Power", fixedField("cluster_power_percent_dyn", 3, "%")),
            field("Oil", fixedField("oil_pressure_bar", 2, "bar"))
          ]
        },
        {
          label: "Thermal",
          title: "Cluster Thermal",
          subtitle: "Temperature values that influence cluster state and protection logic.",
          rows: [
            field("ECU Coolant", fixedField("motor2_coolant_temperature", 1, "C")),
            field("Inlet Air", fixedField("inlet_air_temperature", 1, "C")),
            field("SDU Heatsink", fixedField("sdu_tmphs", 2, "C")),
            field("SDU Motor", fixedField("sdu_tmpm", 2, "C")),
            field("Heatsink Critical", boolField("cluster_heatsink_temp_critical", "YES", "NO")),
            field("Motor Critical", boolField("cluster_motor_temp_critical", "YES", "NO"))
          ],
          hero: [
            field("Coolant", fixedField("motor2_coolant_temperature", 1, "C")),
            field("Heatsink", fixedField("sdu_tmphs", 2, "C")),
            field("Motor", fixedField("sdu_tmpm", 2, "C"))
          ]
        },
        {
          label: "Acceleration",
          title: "Acceleration Timing",
          subtitle: "Automatic stopwatch and fastest acceleration runs since controller start.",
          rows: [
            field("Run Active", boolField("cluster_acceleration_run_active", "RUNNING", "READY")),
            field("Current Run", fixedField("cluster_acceleration_run_time_s", 2, "s")),
            field("Last 0-50", accelerationTimeField("cluster_acceleration_0_50_last_s", "cluster_acceleration_0_50_valid")),
            field("Best 0-50", accelerationTimeField("cluster_acceleration_0_50_best_s", "cluster_acceleration_0_50_valid")),
            field("Last 0-100", accelerationTimeField("cluster_acceleration_0_100_last_s", "cluster_acceleration_0_100_valid")),
            field("Best 0-100", accelerationTimeField("cluster_acceleration_0_100_best_s", "cluster_acceleration_0_100_valid")),
            field("Vehicle Speed", fixedField("vehicle_speed_510", 1, "km/h"))
          ],
          hero: [
            field("Stopwatch", fixedField("cluster_acceleration_run_time_s", 2, "s")),
            field("Best 0-50", accelerationTimeField("cluster_acceleration_0_50_best_s", "cluster_acceleration_0_50_valid")),
            field("Best 0-100", accelerationTimeField("cluster_acceleration_0_100_best_s", "cluster_acceleration_0_100_valid"))
          ]
        },
        {
          label: "Inputs",
          title: "Cluster Inputs",
          subtitle: "Switch and counter signals that influence cluster presentation.",
          rows: [
            field("Light Dimmer", intField("light_dimmer", "")),
            field("Cluster Counter", intField("cluster_counter", "")),
            field("FDR Lamp", boolField("fdr_lamp", "ON", "OFF")),
            field("PSM Lamp", boolField("psm_button_lamp", "ON", "OFF")),
            field("Rough Road Suppression", boolField("rough_road_suppression", "ON", "OFF")),
            field("ACC Receipt", boolField("receipt_for_acc_message", "YES", "NO")),
            field("Tempomat Button", boolField("tempomat_button", "PRESSED", "IDLE")),
            field("Driver Braking", boolField("driver_braking", "YES", "NO"))
          ],
          hero: [
            field("Dimmer", intField("light_dimmer", "")),
            field("Counter", intField("cluster_counter", "")),
            field("PSM Lamp", boolField("psm_button_lamp", "ON", "OFF"))
          ]
        }
      ]
    }
  };

  sections.adjust = SDU_ENUMS.adjust;
  sections.ventilation = SDU_ENUMS.ventilation;
  sections.stopwatch = SDU_ENUMS.stopwatch;

  const state = {
    section: "home",
    tabIndex: 0,
    engineeringMode: readEngineeringMode(),
    batteryControls: {
      acCurrentLimitA: 16,
      digitalPotPct: 0,
      targetChargePct: 80
    },
    styleTestControls: {
      brakeComfort: false,
      chassisLevel: "Medium",
      electricSportSound: true,
      powerSteeringAssist: 72,
      simulateHostLost: false,
      simulatePlugConnected: false,
      targetChargePct: 85
    },
    styleTestPopup: null,
    styleTestScroll: {},
    live: null,
    lastUpdated: "",
    error: "",
    chart: {
      tile: null,
      samples: [],
      hover: null
    }
  };

  const batteryControlDefs = {
    ac_current_limit_a: {
      description: "Set the maximum AC charging current for the on-board charger.",
      formatValue: function (value) { return String(Math.round(value)) + " A"; },
      label: "AC Current Limit",
      max: 32,
      min: 6,
      step: 1,
      title: "AC Current Limit",
      unit: "A"
    },
    target_charge_pct: {
      description: "Set the target state of charge where AC charging should stop.",
      formatValue: function (value) { return String(Math.round(value)) + " %"; },
      label: "Target Charge",
      max: 100,
      min: 50,
      step: 1,
      title: "Target Charge",
      unit: "%"
    },
    digital_pot_pct: {
      description: "Set the X9C103S digital potentiometer wiper position.",
      formatValue: function (value) { return String(Math.round(value)) + " %"; },
      label: "Digital Potentiometer",
      max: 100,
      min: 0,
      step: 1,
      title: "Digital Potentiometer",
      unit: "%"
    }
  };

  function currentSection() {
    return sections[state.section];
  }

  function isStageSection(sectionName) {
    return sectionName === "home" || sectionName === "coolant";
  }

  function currentTab() {
    return currentSection().tabs[state.tabIndex];
  }

  function syncBatteryControlsFromLive(live) {
    const targetCharge = firstNumericField(live, ["target_charge_pct"]);
    const acCurrentLimit = firstNumericField(live, ["ac_current_limit_a"]);
    const digitalPotPct = firstNumericField(live, ["digital_pot_pct"]);
    if (targetCharge !== null) {
      state.batteryControls.targetChargePct = clampNumber(targetCharge, 50, 100, state.batteryControls.targetChargePct);
    }
    if (acCurrentLimit !== null) {
      state.batteryControls.acCurrentLimitA = clampNumber(acCurrentLimit, 6, 32, state.batteryControls.acCurrentLimitA);
    }
    if (digitalPotPct !== null) {
      state.batteryControls.digitalPotPct = clampNumber(digitalPotPct, 0, 100, state.batteryControls.digitalPotPct);
    }
  }

  function updateNav() {
    l1Icons.forEach(function (button) {
      const engineeringOnly = button.dataset.engineeringOnly === "true";
      const visible = !engineeringOnly || state.engineeringMode;
      button.hidden = !visible;
      button.disabled = !visible;
      button.classList.toggle("active", visible && button.dataset.section === state.section);
    });
  }

  function updateTabs() {
    const activeSection = currentSection();
    tabs.forEach(function (button, index) {
      const tab = activeSection.tabs[index];
      const hasTab = !!tab;
      button.hidden = !hasTab;
      button.disabled = !hasTab;
      button.textContent = hasTab ? tab.label : "";
      button.classList.toggle("active", hasTab && index === state.tabIndex);
    });
  }

  function isEngineeringSection(sectionName) {
    return ENGINEERING_SECTIONS.indexOf(sectionName) !== -1;
  }

  function setEngineeringMode(enabled) {
    state.engineeringMode = !!enabled;
    persistEngineeringMode(state.engineeringMode);
    if (!state.engineeringMode && isEngineeringSection(state.section)) {
      state.section = "vehicle";
      state.tabIndex = 0;
      closeMetricChart();
    }
    if (screenRoot) {
      screenRoot.classList.toggle("is-engineering-mode", state.engineeringMode);
    }
    renderView();
  }

  function toggleEngineeringMode() {
    setEngineeringMode(!state.engineeringMode);
  }

  function isCurrentTabTileBoard() {
    const activeTab = currentTab();
    return !!(
      activeTab &&
      activeTab.tileBoard &&
      Array.isArray(activeTab.tileBoard.tiles) &&
      activeTab.tileBoard.tiles.length
    );
  }

  function findCurrentChartTile(chartKey) {
    const activeTab = currentTab();
    if (state.section === "styleTest" && chartKey === "battery_power_kw") {
      return porscheBatteryPowerTile();
    }
    const boardTiles = activeTab.tileBoard && Array.isArray(activeTab.tileBoard.tiles) ? activeTab.tileBoard.tiles : [];
    const tile = boardTiles.find(function (item) {
      return item.chartKey === chartKey;
    });
    if (tile) {
      return tile;
    }
    const rows = Array.isArray(activeTab.rows) ? activeTab.rows : [];
    return rows.find(function (item) {
      return item.chartKey === chartKey;
    }) || null;
  }

  function numericChartValue(tile) {
    if (!tile) {
      return null;
    }
    if (typeof tile.chartValue === "function") {
      const value = Number(tile.chartValue(state.live || {}));
      return Number.isFinite(value) ? value : null;
    }
    if (!tile.chartKey) {
      return null;
    }
    const value = state.live ? Number(state.live[tile.chartKey]) : NaN;
    return Number.isFinite(value) ? value : null;
  }

  function isCurrentChartStillAvailable() {
    if (!state.chart.tile || !state.chart.tile.chartKey) {
      return false;
    }
    return !!findCurrentChartTile(state.chart.tile.chartKey);
  }

  function hasCurrentTabChartableRows() {
    const activeTab = currentTab();
    const rows = activeTab && Array.isArray(activeTab.rows) ? activeTab.rows : [];
    return rows.some(function (row) {
      return !!row.chartKey;
    });
  }

  function isCurrentTabChartable() {
    return isCurrentTabTileBoard() || hasCurrentTabChartableRows() || state.section === "styleTest";
  }

  function rowCanOpenChart(row) {
    return !!(row && row.chartKey);
  }

  function rowCanOpenControl(row) {
    return !!(row && row.controlKey && !row.chartKey);
  }

  function numericLiveValue(key) {
    if (state.chart.tile && state.chart.tile.chartKey === key) {
      return numericChartValue(state.chart.tile);
    }
    const activeTab = currentTab();
    const boardTiles = activeTab.tileBoard && Array.isArray(activeTab.tileBoard.tiles) ? activeTab.tileBoard.tiles : [];
    const rows = Array.isArray(activeTab.rows) ? activeTab.rows : [];
    const tile = boardTiles.concat(rows).find(function (item) {
      return item.chartKey === key;
    });
    if (tile) {
      return numericChartValue(tile);
    }
    const value = state.live ? Number(state.live[key]) : NaN;
    return Number.isFinite(value) ? value : null;
  }

  function appendChartSample() {
    if (!state.chart.tile) {
      return;
    }
    const value = numericLiveValue(state.chart.tile.chartKey);
    if (value === null) {
      return;
    }
    state.chart.samples.push({
      time: Date.now(),
      value: value
    });
    if (state.chart.samples.length > 240) {
      state.chart.samples.splice(0, state.chart.samples.length - 240);
    }
  }

  function closeMetricChart() {
    state.chart.tile = null;
    state.chart.samples = [];
    state.chart.hover = null;
  }

  function openMetricChart(chartKey) {
    const tile = findCurrentChartTile(chartKey);
    if (!tile) {
      return;
    }
    state.chart.tile = tile;
    state.chart.samples = [];
    state.chart.hover = null;
    appendChartSample();
    renderView();
  }

  function setChartHoverFromPointer(event) {
    const canvas = event.currentTarget;
    const rect = canvas.getBoundingClientRect();
    state.chart.hover = {
      x: event.clientX - rect.left,
      y: event.clientY - rect.top
    };
    drawMetricChart();
  }

  function clearChartHover() {
    state.chart.hover = null;
    drawMetricChart();
  }

  function bindMetricChartPointer(canvas) {
    canvas.addEventListener("pointerdown", function (event) {
      canvas.setPointerCapture(event.pointerId);
      setChartHoverFromPointer(event);
    });
    canvas.addEventListener("pointermove", setChartHoverFromPointer);
    canvas.addEventListener("pointerup", function (event) {
      if (canvas.hasPointerCapture(event.pointerId)) {
        canvas.releasePointerCapture(event.pointerId);
      }
      setChartHoverFromPointer(event);
    });
    canvas.addEventListener("pointercancel", clearChartHover);
    canvas.addEventListener("pointerleave", clearChartHover);
  }

  function renderMetricChart() {
    if (!metricBoard || !state.chart.tile) {
      return;
    }
    const tile = state.chart.tile;
    const latest = tile.render(state.live);
    metricBoard.innerHTML =
      "<section class=\"metric-chart-panel\">" +
        "<div class=\"metric-chart-head\">" +
          "<div>" +
            "<div class=\"metric-chart-label\">" + escapeHtml(tile.label) + "</div>" +
          "</div>" +
          "<div class=\"metric-chart-latest\">" + escapeHtml(latest) + "</div>" +
        "</div>" +
        "<canvas class=\"metric-chart-canvas\" id=\"metricChartCanvas\"></canvas>" +
      "</section>";
    bindMetricChartPointer(document.getElementById("metricChartCanvas"));
    window.requestAnimationFrame(drawMetricChart);
  }

  function drawMetricChart() {
    const canvas = document.getElementById("metricChartCanvas");
    if (!canvas || !state.chart.tile) {
      return;
    }
    const rect = canvas.getBoundingClientRect();
    const dpr = window.devicePixelRatio || 1;
    const width = Math.max(1, Math.round(rect.width * dpr));
    const height = Math.max(1, Math.round(rect.height * dpr));
    if (canvas.width !== width || canvas.height !== height) {
      canvas.width = width;
      canvas.height = height;
    }

    const ctx = canvas.getContext("2d");
    const samples = state.chart.samples.slice();
    if (samples.length === 1) {
      samples.unshift({
        time: samples[0].time - 1000,
        value: samples[0].value
      });
    }

    ctx.save();
    ctx.scale(dpr, dpr);
    const w = rect.width;
    const h = rect.height;
    const padLeft = 58;
    const padRight = 22;
    const padTop = 24;
    const padBottom = 42;
    const plotW = Math.max(1, w - padLeft - padRight);
    const plotH = Math.max(1, h - padTop - padBottom);

    ctx.clearRect(0, 0, w, h);
    ctx.fillStyle = "#000000";
    ctx.fillRect(0, 0, w, h);

    const values = samples.map(function (sample) { return sample.value; });
    const actualMin = values.length ? Math.min.apply(null, values) : 0;
    const actualMax = values.length ? Math.max.apply(null, values) : 1;
    const actualAvg = values.length
      ? values.reduce(function (sum, value) { return sum + value; }, 0) / values.length
      : 0;
    let axisMin = actualMin;
    let axisMax = actualMax;
    if (axisMin === axisMax) {
      axisMin -= 1;
      axisMax += 1;
    }
    const range = axisMax - axisMin;
    axisMin -= range * 0.12;
    axisMax += range * 0.12;

    ctx.strokeStyle = "#1c1c1c";
    ctx.lineWidth = 1;
    ctx.fillStyle = "#6f6f6f";
    ctx.font = "15px Arial";
    ctx.textAlign = "right";
    ctx.textBaseline = "middle";
    for (let i = 0; i <= 4; i += 1) {
      const y = padTop + (plotH / 4) * i;
      const value = axisMax - ((axisMax - axisMin) / 4) * i;
      ctx.beginPath();
      ctx.moveTo(padLeft, y);
      ctx.lineTo(padLeft + plotW, y);
      ctx.stroke();
      ctx.fillText(value.toFixed(1), padLeft - 10, y);
    }

    ctx.strokeStyle = "#2a2a2a";
    ctx.beginPath();
    ctx.moveTo(padLeft, padTop);
    ctx.lineTo(padLeft, padTop + plotH);
    ctx.lineTo(padLeft + plotW, padTop + plotH);
    ctx.stroke();

    if (samples.length > 0) {
      const firstTime = samples[0].time;
      const lastTime = samples[samples.length - 1].time;
      const timeRange = Math.max(1, lastTime - firstTime);
      ctx.strokeStyle = "#ffffff";
      ctx.lineWidth = 2.5;
      ctx.beginPath();
      samples.forEach(function (sample, index) {
        const x = padLeft + ((sample.time - firstTime) / timeRange) * plotW;
        const y = padTop + ((axisMax - sample.value) / (axisMax - axisMin)) * plotH;
        if (index === 0) {
          ctx.moveTo(x, y);
        } else {
          ctx.lineTo(x, y);
        }
      });
      ctx.stroke();

      const latest = samples[samples.length - 1];
      const latestX = padLeft + ((latest.time - firstTime) / timeRange) * plotW;
      const latestY = padTop + ((axisMax - latest.value) / (axisMax - axisMin)) * plotH;
      ctx.fillStyle = "#ffffff";
      ctx.beginPath();
      ctx.arc(latestX, latestY, 4, 0, Math.PI * 2);
      ctx.fill();

      const hover = state.chart.hover;
      if (hover && hover.x >= padLeft && hover.x <= padLeft + plotW && hover.y >= padTop && hover.y <= padTop + plotH) {
        let nearestIndex = 0;
        let nearestDistance = Infinity;
        samples.forEach(function (sample, index) {
          const x = padLeft + ((sample.time - firstTime) / timeRange) * plotW;
          const distance = Math.abs(x - hover.x);
          if (distance < nearestDistance) {
            nearestDistance = distance;
            nearestIndex = index;
          }
        });
        const nearest = samples[nearestIndex];
        const hoverX = padLeft + ((nearest.time - firstTime) / timeRange) * plotW;
        const hoverY = padTop + ((axisMax - nearest.value) / (axisMax - axisMin)) * plotH;
        const valueText = nearest.value.toFixed(3).replace(/\.?0+$/, "") + (state.chart.tile.chartUnit ? " " + state.chart.tile.chartUnit : "");
        ctx.font = "17px Arial";
        const tooltipWidth = Math.max(96, ctx.measureText(valueText).width + 24);
        const tooltipHeight = 40;
        const tooltipX = Math.min(Math.max(hoverX - tooltipWidth / 2, padLeft), padLeft + plotW - tooltipWidth);
        const tooltipY = hoverY > padTop + tooltipHeight + 14 ? hoverY - tooltipHeight - 12 : hoverY + 12;

        ctx.fillStyle = "#00b7ff";
        ctx.beginPath();
        ctx.arc(hoverX, hoverY, 5, 0, Math.PI * 2);
        ctx.fill();

        ctx.fillStyle = "#111111";
        ctx.strokeStyle = "#1c1c1c";
        ctx.lineWidth = 1;
        ctx.fillRect(tooltipX, tooltipY, tooltipWidth, tooltipHeight);
        ctx.strokeRect(tooltipX, tooltipY, tooltipWidth, tooltipHeight);

        ctx.fillStyle = "#ffffff";
        ctx.font = "17px Arial";
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        ctx.fillText(valueText, tooltipX + tooltipWidth / 2, tooltipY + tooltipHeight / 2);
      }
    }

    ctx.fillStyle = "#7c7c7c";
    ctx.font = "15px Arial";
    ctx.textAlign = "left";
    ctx.textBaseline = "alphabetic";
    const unit = state.chart.tile.chartUnit ? " " + state.chart.tile.chartUnit : "";
    const summary = String(samples.length) + " samples" +
      "   min " + actualMin.toFixed(2).replace(/\.?0+$/, "") + unit +
      "   max " + actualMax.toFixed(2).replace(/\.?0+$/, "") + unit +
      "   avg " + actualAvg.toFixed(2).replace(/\.?0+$/, "") + unit;
    ctx.fillText(summary, padLeft, h - 14);
    ctx.restore();
  }

  function renderRows() {
    if (state.section === "styleTest") {
      if (valueTable) {
        valueTable.hidden = true;
      }
      if (metricBoard) {
        metricBoard.hidden = true;
        metricBoard.classList.remove("is-chart-view");
        metricBoard.innerHTML = "";
      }
      if (tableScroll) {
        tableScroll.classList.remove("is-metric-board");
        tableScroll.classList.remove("is-chart-view");
      }
      valueRows.innerHTML = "";
      return;
    }
    const activeTab = currentTab();
    const boardConfig = activeTab.tileBoard;
    const boardTiles = boardConfig && Array.isArray(boardConfig.tiles) ? boardConfig.tiles : null;
    const hasMetricBoard = !!(boardTiles && boardTiles.length);
    const hasChart = !!(state.chart.tile && isCurrentChartStillAvailable());

    if (valueTable) {
      valueTable.hidden = hasMetricBoard || hasChart;
    }
    if (metricBoard) {
      metricBoard.hidden = !hasMetricBoard && !hasChart;
      metricBoard.classList.toggle("is-chart-view", hasChart);
    }
    if (tableScroll) {
      tableScroll.classList.toggle("is-metric-board", hasMetricBoard);
      tableScroll.classList.toggle("is-chart-view", hasChart);
    }

    if (hasChart) {
      valueRows.innerHTML = "";
      renderMetricChart();
      return;
    }

    if (hasMetricBoard) {
      valueRows.innerHTML = "";
      metricBoard.innerHTML = boardTiles.map(function (tile) {
        const attrs = tile.chartKey
          ? " data-chart-key=\"" + escapeHtml(tile.chartKey) + "\" role=\"button\" tabindex=\"0\""
          : "";
        const classes = "metric-tile" + (tile.chartKey ? " is-chartable" : "");
        return "<article class=\"" + classes + "\"" + attrs + "><div class=\"metric-tile-label\">" +
          escapeHtml(tile.label) +
          "</div><div class=\"metric-tile-value\">" +
          escapeHtml(tile.render(state.live)) +
          "</div></article>";
      }).join("");
      return;
    }

    if (metricBoard) {
      metricBoard.innerHTML = "";
    }
    valueRows.innerHTML = activeTab.rows.map(function (row, index) {
      const value = escapeHtml(row.render(state.live));
      const classes = [];
      const attrs = [];
      if (index === 0) {
        classes.push("selected");
      }
      if (rowCanOpenChart(row)) {
        classes.push("is-actionable");
        attrs.push("data-chart-key=\"" + escapeHtml(row.chartKey) + "\"");
        attrs.push("role=\"button\"");
        attrs.push("tabindex=\"0\"");
      }
      if (rowCanOpenControl(row)) {
        classes.push("is-actionable");
        attrs.push("data-control-key=\"" + escapeHtml(row.controlKey) + "\"");
        attrs.push("role=\"button\"");
        attrs.push("tabindex=\"0\"");
      }
      if (classes.length > 0) {
        attrs.unshift("class=\"" + classes.join(" ") + "\"");
      }
      return "<tr " + attrs.join(" ") + "><td>" + escapeHtml(row.label) + "</td><td>" + value + "</td></tr>";
    }).join("");
  }

  function renderHeroMetrics() {
    const activeTab = currentTab();
    heroMetrics.innerHTML = activeTab.hero.map(function (item) {
      return "<div class=\"hero-card\"><span class=\"hero-card-label\">" +
        escapeHtml(item.label) +
        "</span><span class=\"hero-card-value\">" +
        escapeHtml(item.render(state.live)) +
        "</span></div>";
    }).join("");
  }

  function renderStatus() {
    const activeTab = currentTab();
    if (isStageSection(state.section)) {
      statusText.textContent = "";
      return;
    }
    const base = activeTab.title + " | " + activeTab.label;
    if (state.error) {
      statusText.textContent = base + " | offline mock or AP data unavailable (" + state.error + ")";
      return;
    }
    if (!state.lastUpdated) {
      statusText.textContent = base + " | waiting for /api/live";
      return;
    }
    statusText.textContent = base + " | updated " + state.lastUpdated;
  }

  function getHomeBatteryConfig() {
    const live = state.live || {};
    const batteryLevel = resolveBatteryLevel(live);
    const rangeKm = resolveBatteryRangeKm(live, batteryLevel);
    const isCharging = resolveChargeActive(live);
    const chargePowerKw = resolveChargePowerKw(live);
    const powerKw = isCharging && chargePowerKw > 0.1 ? chargePowerKw : resolveBatteryPowerKw(live, batteryLevel);
    return {
      level: batteryLevel === null ? 28 : batteryLevel,
      rangeValue: rangeKm,
      rangeUnit: "km",
      powerValue: powerKw
    };
  }

  function currentBatteryControlValue(controlKey) {
    if (controlKey === "target_charge_pct") {
      return state.batteryControls.targetChargePct;
    }
    if (controlKey === "ac_current_limit_a") {
      return state.batteryControls.acCurrentLimitA;
    }
    if (controlKey === "digital_pot_pct") {
      return state.batteryControls.digitalPotPct;
    }
    return 0;
  }

  async function updateCoolantFanLevel(nextLevel) {
    const level = clampNumber(Math.round(Number(nextLevel) || 0), 0, 2, 0);
    state.live = Object.assign({}, state.live || {}, { coolant_fan_level: level });
    renderView();

    const params = new URLSearchParams();
    params.set("coolant_fan_level", String(level));
    try {
      const response = await fetch("/api/set?" + params.toString(), { cache: "no-store" });
      if (!response.ok) {
        throw new Error("HTTP " + response.status);
      }
      state.live = await response.json();
      state.lastUpdated = new Date().toLocaleTimeString([], {
        hour: "2-digit",
        minute: "2-digit",
        second: "2-digit"
      });
      state.error = "";
      syncBatteryControlsFromLive(state.live);
      renderView();
    } catch (_) {
      renderView();
    }
  }

  async function updateBatteryControl(controlKey, nextValue) {
    const rounded = Math.round(Number(nextValue) || 0);
    if (controlKey === "target_charge_pct") {
      state.batteryControls.targetChargePct = clampNumber(rounded, 50, 100, state.batteryControls.targetChargePct);
    } else if (controlKey === "ac_current_limit_a") {
      state.batteryControls.acCurrentLimitA = clampNumber(rounded, 6, 32, state.batteryControls.acCurrentLimitA);
    } else if (controlKey === "digital_pot_pct") {
      state.batteryControls.digitalPotPct = clampNumber(rounded, 0, 100, state.batteryControls.digitalPotPct);
    }
    renderView();

    const params = new URLSearchParams();
    params.set(controlKey, String(currentBatteryControlValue(controlKey)));
    try {
      const response = await fetch("/api/set?" + params.toString(), { cache: "no-store" });
      if (!response.ok) {
        throw new Error("HTTP " + response.status);
      }
      state.live = await response.json();
      state.lastUpdated = new Date().toLocaleTimeString([], {
        hour: "2-digit",
        minute: "2-digit",
        second: "2-digit"
      });
      state.error = "";
      syncBatteryControlsFromLive(state.live);
      renderView();
    } catch (_) {
      renderView();
    }
  }

  function porscheSwitch(on) {
    return "<span class=\"pt-switch" + (on ? " is-on" : "") + "\"><span></span></span>";
  }

  function porscheRadio(on) {
    return "<span class=\"pt-radio" + (on ? " is-on" : "") + "\"></span>";
  }

  function porscheSeatIcon() {
    return "<span class=\"pt-seat-icon\"><span></span><span></span></span>";
  }

  function porscheValue(value, unit) {
    return "<span class=\"pt-live-value\"><strong>" + escapeHtml(value) + "</strong>" +
      (unit ? "<small>" + escapeHtml(unit) + "</small>" : "") +
      "</span>";
  }

  function porscheBatteryPowerTile() {
    return {
      chartKey: "battery_power_kw",
      chartUnit: "kW",
      label: "Battery power",
      source: "styleTest",
      chartValue: function (live) {
        const level = resolveBatteryLevel(live || {});
        return Number(resolveBatteryPowerKw(live || {}, level));
      },
      render: function (live) {
        const level = resolveBatteryLevel(live || {});
        return formatFixed(resolveBatteryPowerKw(live || {}, level), 1, "kW");
      }
    };
  }

  function porscheCheckbox(key, checked) {
    return "<button class=\"pt-checkbox" + (checked ? " is-on" : "") + "\" type=\"button\" data-pt-checkbox=\"" +
      escapeHtml(key) +
      "\" aria-pressed=\"" + (checked ? "true" : "false") + "\"></button>";
  }

  function porscheMenuValue(value) {
    return "<span class=\"pt-menu-value\"><span>" + escapeHtml(value) + "</span><span class=\"pt-chevron\">&gt;</span></span>";
  }

  function porscheRows(rows) {
    return rows.filter(function (row) {
      return !row.engineeringOnly || state.engineeringMode;
    }).map(function (row) {
      const detail = row.detail ? "<small>" + escapeHtml(row.detail) + "</small>" : "";
      const icon = row.icon ? "<span class=\"pt-row-icon\">" + row.icon + "</span>" : "";
      const value = row.control || row.value || "";
      const attrs = [];
      if (row.chartKey) {
        attrs.push("data-pt-chart-key=\"" + escapeHtml(row.chartKey) + "\"");
        attrs.push("role=\"button\"");
        attrs.push("tabindex=\"0\"");
      }
      if (row.menuKey) {
        attrs.push("data-pt-menu-key=\"" + escapeHtml(row.menuKey) + "\"");
        attrs.push("role=\"button\"");
        attrs.push("tabindex=\"0\"");
      }
      const classes = "pt-row" +
        (row.active ? " is-active" : "") +
        (row.chartKey ? " is-chartable" : "") +
        (row.menuKey ? " is-menu-action" : "") +
        (row.engineeringOnly ? " is-engineering-only" : "") +
        (!row.icon ? " is-no-icon" : "") +
        (!row.detail ? " is-no-detail" : "");
      return "<article class=\"" + classes + "\"" + (attrs.length ? " " + attrs.join(" ") : "") + ">" +
        icon +
        "<div class=\"pt-row-copy\"><strong>" + escapeHtml(row.label) + "</strong>" + detail + "</div>" +
        "<div class=\"pt-row-value\">" + value + "</div>" +
      "</article>";
    }).join("");
  }

  function porscheVehicleGraphic(mode) {
    const battery = mode === "trip" || mode === "comfort";
    const airflow = mode === "climate";
    const side = mode === "level";
    return "<div class=\"pt-car-scene pt-car-scene-" + escapeHtml(mode) + "\">" +
      "<div class=\"pt-dot-grid\"></div>" +
      "<svg class=\"pt-car\" viewBox=\"0 0 520 240\" role=\"img\" aria-label=\"Vehicle reference illustration\">" +
        "<defs>" +
          "<linearGradient id=\"ptCarPaint\" x1=\"0\" x2=\"1\"><stop offset=\"0\" stop-color=\"#15191c\"/><stop offset=\"0.46\" stop-color=\"#dce2e5\"/><stop offset=\"1\" stop-color=\"#6f777b\"/></linearGradient>" +
          "<linearGradient id=\"ptGreen\" x1=\"0\" x2=\"1\"><stop offset=\"0\" stop-color=\"#17d64d\"/><stop offset=\"1\" stop-color=\"#69ff77\"/></linearGradient>" +
        "</defs>" +
        "<g class=\"pt-car-shadow\"><ellipse cx=\"265\" cy=\"198\" rx=\"205\" ry=\"22\"/></g>" +
        "<path class=\"pt-car-body\" d=\"M60 160 C92 104 151 80 246 82 C332 84 411 111 466 148 C489 164 480 184 444 188 L118 189 C73 188 43 179 60 160 Z\"/>" +
        "<path class=\"pt-car-cabin\" d=\"M193 92 C241 76 318 82 370 126 L247 128 C217 126 194 115 193 92 Z\"/>" +
        "<path class=\"pt-car-line\" d=\"M88 160 C168 151 312 151 444 164\"/>" +
        "<circle class=\"pt-wheel\" cx=\"153\" cy=\"184\" r=\"34\"/><circle class=\"pt-wheel\" cx=\"394\" cy=\"184\" r=\"34\"/>" +
        "<circle class=\"pt-rim\" cx=\"153\" cy=\"184\" r=\"20\"/><circle class=\"pt-rim\" cx=\"394\" cy=\"184\" r=\"20\"/>" +
        (battery ? "<path class=\"pt-battery\" d=\"M239 137 h91 l-10 45 h-94 z\"/>" : "") +
        (side ? "<path class=\"pt-suspension\" d=\"M94 177 h340\"/><path class=\"pt-suspension\" d=\"M206 202 h150\"/><path class=\"pt-arrow\" d=\"M260 160 l18 18 h-36 z M260 221 l-18-18 h36 z\"/>" : "") +
        (airflow ? "<path class=\"pt-air\" d=\"M155 96 C211 40 319 45 390 96 M113 139 C193 91 323 93 428 135 M182 165 C238 135 314 136 369 163\"/>" : "") +
      "</svg>" +
    "</div>";
  }

  function porscheStyleChartMarkup(tile, modifier) {
    const latest = tile.render(state.live);
    return "<section class=\"pt-style-chart" + (modifier ? " " + escapeHtml(modifier) : "") + "\" aria-label=\"" + escapeHtml(tile.label) + " live graph\">" +
      "<div class=\"metric-chart-panel\">" +
        "<div class=\"metric-chart-head\">" +
          "<div class=\"metric-chart-label\">" + escapeHtml(tile.label) + "</div>" +
          "<div class=\"metric-chart-latest\">" + escapeHtml(latest) + "</div>" +
        "</div>" +
        "<canvas class=\"metric-chart-canvas\" id=\"metricChartCanvas\"></canvas>" +
      "</div>" +
    "</section>";
  }

  function porscheChassisPopupMarkup() {
    const options = ["Lift", "Medium", "Lowered", "Low"];
    return "<section class=\"pt-popup-layer\" aria-label=\"Chassis level options\">" +
      "<div class=\"pt-popup-panel\">" +
        "<div class=\"pt-popup-title\">Chassis level</div>" +
        options.map(function (option) {
          const selected = state.styleTestControls.chassisLevel === option;
          return "<button class=\"pt-popup-option\" type=\"button\" data-pt-option-key=\"chassisLevel\" data-pt-option-value=\"" +
            escapeHtml(option) +
            "\" aria-pressed=\"" + (selected ? "true" : "false") + "\">" +
              "<span>" + escapeHtml(option) + "</span>" +
              porscheRadio(selected) +
            "</button>";
        }).join("") +
      "</div>" +
    "</section>";
  }

  function porschePowerSteeringPopupMarkup() {
    const value = Math.round(state.styleTestControls.powerSteeringAssist);
    return "<section class=\"pt-popup-layer\" aria-label=\"Power steering assist\">" +
      "<div class=\"pt-popup-panel pt-popup-panel-slider\">" +
        "<div class=\"pt-popup-title\">Power steering</div>" +
        "<div class=\"pt-slider-content\">" +
          "<div class=\"pt-slider-readout\">" +
            "<span>Steering assist</span>" +
            "<strong id=\"ptPowerSteeringValue\">" + escapeHtml(String(value)) + "</strong>" +
            "<small>%</small>" +
          "</div>" +
          "<input class=\"pt-slider\" type=\"range\" min=\"0\" max=\"100\" step=\"1\" value=\"" +
            escapeHtml(String(value)) +
            "\" style=\"--pt-slider-fill: " + escapeHtml(String(value)) + "%\" data-pt-slider-key=\"powerSteeringAssist\" aria-label=\"Power steering assist\">" +
          "<div class=\"pt-slider-scale\"><span>Light</span><span>Firm</span></div>" +
        "</div>" +
      "</div>" +
    "</section>";
  }

  function porscheVerticalSliderVars(value, min, max) {
    const bounded = clampNumber(value, min, max, min);
    const range = Math.max(1, max - min);
    const y = ((max - bounded) / range) * 100;
    return {
      fill: 100 - y,
      y: y
    };
  }

  function porscheTargetChargePopupMarkup() {
    const min = 25;
    const max = 100;
    const value = Math.round(clampNumber(state.styleTestControls.targetChargePct, min, max, 85));
    const vars = porscheVerticalSliderVars(value, min, max);
    return "<section class=\"pt-popup-layer\" aria-label=\"Target charge setting\">" +
      "<div class=\"pt-popup-panel pt-popup-panel-vertical-slider\">" +
        "<div class=\"pt-popup-title\">General charging profile</div>" +
        "<div class=\"pt-vertical-slider-content\">" +
          "<div class=\"pt-vertical-slider-copy\">" +
            "<strong>Minimum charge</strong>" +
            "<p>Set charging target that the vehicle should reach as quickly as possible while charging.</p>" +
            "<span></span>" +
            "<em>Optimised charging</em>" +
            "<small>Charging operation can be automatically optimised using suitable equipment.</small>" +
          "</div>" +
          "<div class=\"pt-vertical-slider-wrap\">" +
            "<div class=\"pt-vertical-slider\" role=\"slider\" tabindex=\"0\" aria-label=\"Target charge\" aria-valuemin=\"" +
              String(min) +
              "\" aria-valuemax=\"" +
              String(max) +
              "\" aria-valuenow=\"" +
              escapeHtml(String(value)) +
              "\" data-pt-vertical-slider-key=\"targetChargePct\" data-min=\"" +
              String(min) +
              "\" data-max=\"" +
              String(max) +
              "\" style=\"--pt-vertical-slider-y: " +
              escapeHtml(vars.y.toFixed(2)) +
              "%; --pt-vertical-slider-fill: " +
              escapeHtml(vars.fill.toFixed(2)) +
              "%\">" +
                "<div class=\"pt-vertical-slider-track\"></div>" +
                "<div class=\"pt-vertical-slider-line\"></div>" +
                "<div class=\"pt-vertical-slider-badge\"><strong id=\"ptTargetChargeValue\">" +
                  escapeHtml(String(value)) +
                  "</strong><span>%</span></div>" +
            "</div>" +
          "</div>" +
        "</div>" +
      "</div>" +
    "</section>";
  }

  function porscheDriveMarkup() {
    const batteryTile = porscheBatteryPowerTile();
    const batteryValue = numericChartValue(batteryTile);
    const batteryText = batteryValue === null ? "--" : String(Math.round(batteryValue));
    const isBatteryChartActive = state.chart.tile && state.chart.tile.source === "styleTest" && state.chart.tile.chartKey === batteryTile.chartKey;
    const isChassisPopupActive = state.styleTestPopup === "chassisLevel";
    const isPowerSteeringPopupActive = state.styleTestPopup === "powerSteeringAssist";
    const isTargetChargePopupActive = state.styleTestPopup === "targetChargePct";
    return "<div class=\"pt-layout pt-layout-drive" + (state.styleTestPopup ? " has-popup" : "") + "\">" +
      "<section class=\"pt-list\">" +
        porscheRows([
          { active: true, label: "Drive mode", detail: "Driving setup", control: porscheMenuValue("Normal") },
          { active: isBatteryChartActive, label: "Battery power", detail: "Live value", control: porscheValue(batteryText, "kW"), chartKey: batteryTile.chartKey },
          { active: isTargetChargePopupActive, label: "Target charge", detail: "Charge target", control: porscheMenuValue(String(Math.round(state.styleTestControls.targetChargePct)) + "%"), menuKey: "targetChargePct" },
          { active: isChassisPopupActive, label: "Chassis level", detail: "Ride height", control: porscheMenuValue(state.styleTestControls.chassisLevel), menuKey: "chassisLevel" },
          { label: "Recuperation", detail: "Energy recovery", control: porscheMenuValue("On"), engineeringOnly: true },
          { label: "Electric Sport Sound", detail: "Sound profile", control: porscheCheckbox("electricSportSound", state.styleTestControls.electricSportSound) },
          { active: isPowerSteeringPopupActive, label: "Power steering", detail: "Steering assist", control: porscheMenuValue(String(Math.round(state.styleTestControls.powerSteeringAssist)) + "%"), menuKey: "powerSteeringAssist" },
          { label: "Traction control", detail: "Wheel slip", control: porscheMenuValue("Normal") },
          { label: "Brake comfort", detail: "Pedal response", control: porscheCheckbox("brakeComfort", state.styleTestControls.brakeComfort) },
          { label: "Sport response", control: porscheMenuValue("Off") },
          { label: "Simulate host lost", detail: "Warning test", control: porscheCheckbox("simulateHostLost", state.styleTestControls.simulateHostLost) },
          { label: "Simulate plug connected", detail: "Plug test", control: porscheCheckbox("simulatePlugConnected", state.styleTestControls.simulatePlugConnected) }
        ]) +
      "</section>" +
      "<section class=\"pt-front-stage\" aria-label=\"Front vehicle image\">" +
        "<img src=\"/touch/img/half_front.png\" alt=\"Front vehicle reference\">" +
      "</section>" +
      (isBatteryChartActive ? porscheStyleChartMarkup(batteryTile, "is-full") : "") +
      (isChassisPopupActive ? porscheChassisPopupMarkup() : "") +
      (isPowerSteeringPopupActive ? porschePowerSteeringPopupMarkup() : "") +
      (isTargetChargePopupActive ? porscheTargetChargePopupMarkup() : "") +
    "</div>";
  }

  function porscheAssistanceMarkup() {
    return "<div class=\"pt-layout pt-layout-assistance\">" +
      "<section class=\"pt-cardless-panel\">" +
        "<div class=\"pt-back-title\"><span>&lt;</span><strong>Porsche Active Ride</strong></div>" +
        porscheRows([
          { label: "Entry height", control: porscheSwitch(true) },
          { label: "Cornering comfort", detail: "Only available in chassis Normal", control: porscheSwitch(true) },
          { label: "Pitch comfort", detail: "Only available in chassis Normal", control: porscheSwitch(true) }
        ]) +
      "</section>" +
      porscheVehicleGraphic("comfort") +
    "</div>";
  }

  function porscheTripMarkup() {
    return "<div class=\"pt-layout pt-layout-trip\">" +
      "<section class=\"pt-trip-list\">" +
        "<div class=\"pt-trip-head\">Trip since 13:20</div>" +
        porscheRows([
          { label: "5:03 h", detail: "Driving time", value: "" },
          { label: "459.9 km", detail: "Distance", value: "" },
          { label: "24.2 kWh/100 km", detail: "Average consumption", value: "" },
          { label: "92 km/h", detail: "Average speed", value: "" }
        ]) +
        "<div class=\"pt-trip-foot\">Trip since charging</div>" +
      "</section>" +
      porscheVehicleGraphic("trip") +
    "</div>";
  }

  function porscheComfortMarkup() {
    return "<div class=\"pt-layout pt-layout-comfort\">" +
      "<section class=\"pt-climate-menu\"><strong>Diffus</strong><strong>Fokuseret</strong><strong class=\"active\">Individual</strong></section>" +
      porscheVehicleGraphic("climate") +
      "<section class=\"pt-climate-controls\">" +
        "<div class=\"pt-temp\"><button>^</button><strong>16.5</strong><button>v</button></div>" +
        "<div class=\"pt-auto\"><button>^</button><strong>AUTO</strong><span>*</span><button>v</button></div>" +
        "<div class=\"pt-auto\"><button>^</button><strong>AUTO</strong><span>*</span><button>v</button></div>" +
        "<div class=\"pt-temp\"><button>^</button><strong>24.0</strong><button>v</button></div>" +
      "</section>" +
      "<div class=\"pt-seat-left\">" + porscheSeatIcon() + porscheSeatIcon() + "</div>" +
      "<div class=\"pt-seat-right\">" + porscheSeatIcon() + porscheSeatIcon() + "</div>" +
    "</div>";
  }

  function styleTestScrollKey() {
    const tab = currentTab();
    return tab && tab.label ? tab.label : String(state.tabIndex);
  }

  function findStyleTestScroller() {
    if (!styleTestStage) {
      return null;
    }
    return styleTestStage.querySelector(".pt-list, .pt-option-panel, .pt-cardless-panel, .pt-trip-list");
  }

  function rememberStyleTestScroll() {
    const scroller = findStyleTestScroller();
    if (!scroller) {
      return;
    }
    state.styleTestScroll[styleTestScrollKey()] = scroller.scrollTop;
  }

  function restoreStyleTestScroll(key) {
    const scroller = findStyleTestScroller();
    if (!scroller) {
      return;
    }
    const top = state.styleTestScroll[key] || 0;
    scroller.scrollTop = top;
    window.requestAnimationFrame(function () {
      scroller.scrollTop = top;
    });
  }

  function renderStyleTestStage() {
    if (!styleTestStage) {
      return;
    }
    const tab = currentTab().label;
    const scrollKey = styleTestScrollKey();
    rememberStyleTestScroll();
    const content = tab === "Assistance"
      ? porscheAssistanceMarkup()
      : tab === "Trip"
        ? porscheTripMarkup()
        : tab === "Comfort"
          ? porscheComfortMarkup()
          : porscheDriveMarkup();
    styleTestStage.innerHTML =
      "<div class=\"pt-shell\">" +
        content +
      "</div>";
    const chartCanvas = document.getElementById("metricChartCanvas");
    if (chartCanvas && state.chart.tile && state.chart.tile.source === "styleTest") {
      bindMetricChartPointer(chartCanvas);
      window.requestAnimationFrame(drawMetricChart);
    }
    restoreStyleTestScroll(scrollKey);
  }

  function bindStyleTestDragScroll() {
    if (!styleTestStage) {
      return;
    }
    let dragState = null;
    let kineticFrame = null;
    let snapFrame = null;
    let wheelSnapTimer = null;
    let verticalSliderState = null;

    function cancelStyleTestScrollAnimation() {
      if (kineticFrame !== null) {
        window.cancelAnimationFrame(kineticFrame);
        kineticFrame = null;
      }
      if (snapFrame !== null) {
        window.cancelAnimationFrame(snapFrame);
        snapFrame = null;
      }
      if (wheelSnapTimer !== null) {
        window.clearTimeout(wheelSnapTimer);
        wheelSnapTimer = null;
      }
    }

    function maxScrollTop(scroller) {
      return Math.max(0, scroller.scrollHeight - scroller.clientHeight);
    }

    function rowSnapStep(scroller) {
      const row = scroller.querySelector(".pt-row");
      if (!row) {
        return 0;
      }
      return row.getBoundingClientRect().height || 0;
    }

    function setStyleTestScroll(scroller, top) {
      const boundedTop = Math.max(0, Math.min(maxScrollTop(scroller), top));
      scroller.scrollTop = boundedTop;
      state.styleTestScroll[styleTestScrollKey()] = boundedTop;
      return boundedTop;
    }

    function snapStyleTestScroll(scroller) {
      let targetScroller = scroller && scroller.isConnected ? scroller : findStyleTestScroller();
      if (!targetScroller) {
        return;
      }
      const step = rowSnapStep(targetScroller);
      if (step <= 0) {
        return;
      }
      const maxTop = maxScrollTop(targetScroller);
      const target = Math.max(0, Math.min(maxTop, Math.round(targetScroller.scrollTop / step) * step));
      const start = targetScroller.scrollTop;
      const distance = target - start;
      if (Math.abs(distance) < 0.5) {
        setStyleTestScroll(targetScroller, target);
        return;
      }
      const startedAt = performance.now();
      const duration = 260;

      function frame(now) {
        targetScroller = targetScroller && targetScroller.isConnected ? targetScroller : findStyleTestScroller();
        if (!targetScroller) {
          snapFrame = null;
          return;
        }
        const t = Math.min(1, (now - startedAt) / duration);
        const eased = 1 - Math.pow(1 - t, 3);
        setStyleTestScroll(targetScroller, start + distance * eased);
        if (t < 1) {
          snapFrame = window.requestAnimationFrame(frame);
        } else {
          snapFrame = null;
          setStyleTestScroll(targetScroller, target);
        }
      }

      snapFrame = window.requestAnimationFrame(frame);
    }

    function coastStyleTestScroll(scroller, velocity) {
      let targetScroller = scroller && scroller.isConnected ? scroller : findStyleTestScroller();
      if (!targetScroller) {
        return;
      }
      if (Math.abs(velocity) < 0.08) {
        snapStyleTestScroll(targetScroller);
        return;
      }
      let lastTime = performance.now();
      let currentVelocity = Math.max(-2.3, Math.min(2.3, velocity));

      function frame(now) {
        targetScroller = targetScroller && targetScroller.isConnected ? targetScroller : findStyleTestScroller();
        if (!targetScroller) {
          kineticFrame = null;
          return;
        }
        const dt = Math.min(32, now - lastTime);
        lastTime = now;
        const previousTop = targetScroller.scrollTop;
        const nextTop = setStyleTestScroll(targetScroller, previousTop + currentVelocity * dt);
        const hitEdge = nextTop <= 0 || nextTop >= maxScrollTop(targetScroller);
        currentVelocity *= Math.pow(0.94, dt / 16.67);
        if (hitEdge || Math.abs(currentVelocity) < 0.045) {
          kineticFrame = null;
          snapStyleTestScroll(targetScroller);
          return;
        }
        kineticFrame = window.requestAnimationFrame(frame);
      }

      kineticFrame = window.requestAnimationFrame(frame);
    }

    function setVerticalSliderValue(slider, clientY) {
      const key = slider.getAttribute("data-pt-vertical-slider-key");
      if (key !== "targetChargePct") {
        return;
      }
      const min = Number(slider.getAttribute("data-min")) || 0;
      const max = Number(slider.getAttribute("data-max")) || 100;
      const rect = slider.getBoundingClientRect();
      const ratio = clampNumber((clientY - rect.top) / Math.max(1, rect.height), 0, 1, 0);
      const value = Math.round(max - ratio * (max - min));
      const boundedValue = clampNumber(value, min, max, state.styleTestControls.targetChargePct);
      const vars = porscheVerticalSliderVars(boundedValue, min, max);
      state.styleTestControls.targetChargePct = boundedValue;
      slider.style.setProperty("--pt-vertical-slider-y", vars.y.toFixed(2) + "%");
      slider.style.setProperty("--pt-vertical-slider-fill", vars.fill.toFixed(2) + "%");
      slider.setAttribute("aria-valuenow", String(Math.round(boundedValue)));
      const readout = document.getElementById("ptTargetChargeValue");
      if (readout) {
        readout.textContent = String(Math.round(boundedValue));
      }
    }

    styleTestStage.addEventListener("pointerdown", function (event) {
      if (event.target.closest("button, [data-pt-checkbox], .pt-vertical-slider, .pt-row[data-pt-chart-key], .pt-row[data-pt-menu-key]")) {
        return;
      }
      if (state.styleTestPopup) {
        return;
      }
      const scrollTarget = event.target.closest(".pt-list, .pt-option-panel, .pt-cardless-panel, .pt-trip-list");
      if (!scrollTarget || scrollTarget.scrollHeight <= scrollTarget.clientHeight) {
        return;
      }
      cancelStyleTestScrollAnimation();
      const now = performance.now();
      dragState = {
        target: scrollTarget,
        pointerId: event.pointerId,
        startY: event.clientY,
        startScrollTop: scrollTarget.scrollTop,
        lastTime: now,
        lastScrollTop: scrollTarget.scrollTop,
        velocity: 0,
        moved: false
      };
      scrollTarget.classList.add("is-drag-scrolling");
      scrollTarget.setPointerCapture(event.pointerId);
    });

    styleTestStage.addEventListener("pointermove", function (event) {
      if (!dragState || event.pointerId !== dragState.pointerId) {
        return;
      }
      const deltaY = event.clientY - dragState.startY;
      if (Math.abs(deltaY) > 3) {
        dragState.moved = true;
      }
      const now = performance.now();
      const previousTop = dragState.target.scrollTop;
      const currentTop = setStyleTestScroll(dragState.target, dragState.startScrollTop - deltaY);
      const dt = Math.max(1, now - dragState.lastTime);
      dragState.velocity = (currentTop - previousTop) / dt;
      dragState.lastTime = now;
      dragState.lastScrollTop = currentTop;
      event.preventDefault();
    });

    function finishDragScroll(event) {
      if (!dragState || event.pointerId !== dragState.pointerId) {
        return;
      }
      const target = dragState.target;
      const velocity = dragState.velocity;
      const moved = dragState.moved;
      target.classList.remove("is-drag-scrolling");
      if (target.hasPointerCapture(event.pointerId)) {
        target.releasePointerCapture(event.pointerId);
      }
      dragState = null;
      if (moved) {
        coastStyleTestScroll(target, velocity);
      } else {
        snapStyleTestScroll(target);
      }
    }

    styleTestStage.addEventListener("pointerup", finishDragScroll);
    styleTestStage.addEventListener("pointercancel", finishDragScroll);
    styleTestStage.addEventListener("lostpointercapture", finishDragScroll);
    styleTestStage.addEventListener("pointerdown", function (event) {
      const slider = event.target.closest(".pt-vertical-slider[data-pt-vertical-slider-key]");
      if (!slider) {
        return;
      }
      verticalSliderState = {
        pointerId: event.pointerId,
        slider: slider
      };
      slider.setPointerCapture(event.pointerId);
      setVerticalSliderValue(slider, event.clientY);
      event.preventDefault();
    });
    styleTestStage.addEventListener("pointermove", function (event) {
      if (!verticalSliderState || event.pointerId !== verticalSliderState.pointerId) {
        return;
      }
      setVerticalSliderValue(verticalSliderState.slider, event.clientY);
      event.preventDefault();
    });
    function finishVerticalSlider(event) {
      if (!verticalSliderState || event.pointerId !== verticalSliderState.pointerId) {
        return;
      }
      const slider = verticalSliderState.slider;
      if (slider.hasPointerCapture(event.pointerId)) {
        slider.releasePointerCapture(event.pointerId);
      }
      verticalSliderState = null;
      renderStyleTestStage();
    }
    styleTestStage.addEventListener("pointerup", finishVerticalSlider);
    styleTestStage.addEventListener("pointercancel", finishVerticalSlider);
    styleTestStage.addEventListener("lostpointercapture", finishVerticalSlider);
    styleTestStage.addEventListener("scroll", function (event) {
      const scrollTarget = event.target.closest(".pt-list, .pt-option-panel, .pt-cardless-panel, .pt-trip-list");
      if (!scrollTarget) {
        return;
      }
      if (state.styleTestPopup) {
        scrollTarget.scrollTop = state.styleTestScroll[styleTestScrollKey()] || 0;
        return;
      }
      state.styleTestScroll[styleTestScrollKey()] = scrollTarget.scrollTop;
      if (!dragState && kineticFrame === null && snapFrame === null) {
        if (wheelSnapTimer !== null) {
          window.clearTimeout(wheelSnapTimer);
        }
        wheelSnapTimer = window.setTimeout(function () {
          wheelSnapTimer = null;
          snapStyleTestScroll(scrollTarget);
        }, 180);
      }
    }, true);
    styleTestStage.addEventListener("click", function (event) {
      const option = event.target.closest(".pt-popup-option[data-pt-option-key]");
      if (option) {
        const key = option.getAttribute("data-pt-option-key");
        const value = option.getAttribute("data-pt-option-value") || "";
        if (key === "chassisLevel") {
          state.styleTestControls.chassisLevel = value;
          state.styleTestPopup = null;
          renderStyleTestStage();
        }
        return;
      }
      const menuRow = event.target.closest(".pt-row[data-pt-menu-key]");
      if (menuRow) {
        if (state.styleTestPopup) {
          return;
        }
        state.styleTestPopup = menuRow.getAttribute("data-pt-menu-key") || null;
        closeMetricChart();
        renderStyleTestStage();
        return;
      }
      const chartRow = event.target.closest(".pt-row[data-pt-chart-key]");
      if (chartRow) {
        if (state.styleTestPopup) {
          return;
        }
        state.styleTestPopup = null;
        openMetricChart(chartRow.getAttribute("data-pt-chart-key") || "");
        return;
      }
      const checkbox = event.target.closest(".pt-checkbox[data-pt-checkbox]");
      if (!checkbox) {
        return;
      }
      const key = checkbox.getAttribute("data-pt-checkbox");
      if (!Object.prototype.hasOwnProperty.call(state.styleTestControls, key)) {
        return;
      }
      state.styleTestControls[key] = !state.styleTestControls[key];
      renderStyleTestStage();
      updateTopbarTime();
    });
    styleTestStage.addEventListener("input", function (event) {
      const slider = event.target.closest(".pt-slider[data-pt-slider-key]");
      if (!slider) {
        return;
      }
      const key = slider.getAttribute("data-pt-slider-key");
      const value = clampNumber(Number(slider.value), 0, 100, state.styleTestControls.powerSteeringAssist);
      if (key === "powerSteeringAssist") {
        state.styleTestControls.powerSteeringAssist = value;
        slider.style.setProperty("--pt-slider-fill", String(Math.round(value)) + "%");
        const readout = document.getElementById("ptPowerSteeringValue");
        if (readout) {
          readout.textContent = String(Math.round(value));
        }
      }
    });
    styleTestStage.addEventListener("change", function (event) {
      const slider = event.target.closest(".pt-slider[data-pt-slider-key]");
      if (!slider) {
        return;
      }
      const key = slider.getAttribute("data-pt-slider-key");
      if (key === "powerSteeringAssist") {
        state.styleTestControls.powerSteeringAssist = clampNumber(Number(slider.value), 0, 100, state.styleTestControls.powerSteeringAssist);
        renderStyleTestStage();
      }
    });
    styleTestStage.addEventListener("keydown", function (event) {
      const verticalSlider = event.target.closest(".pt-vertical-slider[data-pt-vertical-slider-key]");
      if (verticalSlider) {
        const key = verticalSlider.getAttribute("data-pt-vertical-slider-key");
        if (key !== "targetChargePct") {
          return;
        }
        const min = Number(verticalSlider.getAttribute("data-min")) || 0;
        const max = Number(verticalSlider.getAttribute("data-max")) || 100;
        let delta = 0;
        if (event.key === "ArrowUp" || event.key === "ArrowRight") {
          delta = 1;
        } else if (event.key === "ArrowDown" || event.key === "ArrowLeft") {
          delta = -1;
        } else if (event.key === "PageUp") {
          delta = 5;
        } else if (event.key === "PageDown") {
          delta = -5;
        } else {
          return;
        }
        event.preventDefault();
        state.styleTestControls.targetChargePct = clampNumber(state.styleTestControls.targetChargePct + delta, min, max, state.styleTestControls.targetChargePct);
        renderStyleTestStage();
        return;
      }
      if (event.key !== "Enter" && event.key !== " ") {
        return;
      }
      const chartRow = event.target.closest(".pt-row[data-pt-chart-key]");
      if (!chartRow) {
        const menuRow = event.target.closest(".pt-row[data-pt-menu-key]");
        if (!menuRow) {
          return;
        }
        if (state.styleTestPopup) {
          return;
        }
        event.preventDefault();
        state.styleTestPopup = menuRow.getAttribute("data-pt-menu-key") || null;
        closeMetricChart();
        renderStyleTestStage();
        return;
      }
      event.preventDefault();
      if (state.styleTestPopup) {
        return;
      }
      state.styleTestPopup = null;
      openMetricChart(chartRow.getAttribute("data-pt-chart-key") || "");
    });
  }

  function updateTopbarTime() {
    if (!topbarTime) {
      return;
    }
    const timeText = new Date().toLocaleTimeString([], {
      hour: "2-digit",
      hour12: false,
      minute: "2-digit"
    });
    const simulateHostLost = state.section === "styleTest" && state.styleTestControls.simulateHostLost;
    const simulatePlugConnected = state.section === "styleTest" && state.styleTestControls.simulatePlugConnected;
    if (state.error || simulateHostLost) {
      topbarTime.classList.add("is-api-offline");
      topbarTime.classList.remove("is-plugged");
      let timeValue = topbarTime.querySelector(".topbar-time-value");
      if (!timeValue || !topbarTime.querySelector(".topbar-warning")) {
        topbarTime.innerHTML =
          "<span class=\"topbar-time-value\"></span>" +
          "<span class=\"topbar-warning\" aria-label=\"Live API connection interrupted\">" +
            "<img class=\"topbar-warning-icon\" src=\"/touch/assets/warning.svg\" alt=\"\">" +
          "</span>";
        timeValue = topbarTime.querySelector(".topbar-time-value");
      }
      if (timeValue) {
        timeValue.textContent = timeText;
      }
      return;
    }
    if (simulatePlugConnected || resolveChargeCableConnected(state.live || {})) {
      topbarTime.classList.remove("is-api-offline");
      topbarTime.classList.add("is-plugged");
      let timeValue = topbarTime.querySelector(".topbar-time-value");
      if (!timeValue || !topbarTime.querySelector(".topbar-plug")) {
        topbarTime.innerHTML =
          "<span class=\"topbar-time-value\"></span>" +
          "<span class=\"topbar-plug\" aria-label=\"Charge cable connected\">" +
            "<img class=\"topbar-plug-icon\" src=\"/touch/assets/plug.svg\" alt=\"\">" +
          "</span>";
        timeValue = topbarTime.querySelector(".topbar-time-value");
      }
      if (timeValue) {
        timeValue.textContent = timeText;
      }
      return;
    }
    topbarTime.classList.remove("is-api-offline");
    topbarTime.classList.remove("is-plugged");
    topbarTime.textContent = timeText;
  }

  function openBatteryControlModal(controlKey) {
    const definition = batteryControlDefs[controlKey];
    if (!definition || !window.TouchBatterySettingsModal) {
      return;
    }
    if (!batterySettingsModal) {
      batterySettingsModal = window.TouchBatterySettingsModal.mount();
    }
    if (!batterySettingsModal) {
      return;
    }
    batterySettingsModal.open({
      description: definition.description,
      formatValue: definition.formatValue,
      label: definition.label,
      max: definition.max,
      min: definition.min,
      onCommit: function (value) {
        updateBatteryControl(controlKey, value);
      },
      step: definition.step,
      title: definition.title,
      unit: definition.unit,
      value: currentBatteryControlValue(controlKey)
    });
  }

  function renderHomeStage(activeSection) {
    if (homeStage) {
      homeStage.hidden = false;
    }
    if (homeHeroImage) {
      homeHeroImage.src = activeSection.image;
      homeHeroImage.alt = "Home hero image";
    }
    if (!homeBattery && window.TouchHomeBattery && homeBatteryMount) {
      homeBattery = window.TouchHomeBattery.mount(homeBatteryMount);
      if (homeBattery && homeBattery.element) {
        homeBattery.element.addEventListener("click", function () {
          setSection("vehicle", 1);
        });
      }
    }
    if (homeBattery) {
      homeBattery.update(getHomeBatteryConfig());
    }
  }

  function renderCoolantStage() {
    if (coolantStage) {
      coolantStage.hidden = false;
    }
    if (!coolantStageView && window.TouchCoolantStage && coolantStageMount) {
      coolantStageView = window.TouchCoolantStage.mount(coolantStageMount, {
        onFanLevelChange: updateCoolantFanLevel
      });
    }
    if (coolantStageView && typeof coolantStageView.update === "function") {
      coolantStageView.update(state.live);
    }
  }

  function renderView() {
    const isHome = state.section === "home";
    const isCoolant = state.section === "coolant";
    const isStyleTest = state.section === "styleTest";
    const activeSection = currentSection();
    const activeTab = currentTab();
    const isTileBoard = isCurrentTabTileBoard();
    updateTopbarTime();
    pageHeading.textContent = activeTab.title;
    heroTitle.textContent = activeTab.title;
    heroSubtitle.textContent = activeTab.subtitle;
    heroImage.src = activeSection.image;
    heroImage.alt = isHome ? "Home hero image" : activeTab.title;
    screenRoot.classList.toggle("is-home", isHome);
    screenRoot.classList.toggle("is-coolant", isCoolant);
    screenRoot.classList.toggle("is-style-test", isStyleTest);
    screenRoot.classList.toggle("is-tile-board", isTileBoard && !isHome && !isCoolant);
    screenRoot.classList.toggle("is-engineering-mode", state.engineeringMode);
    if (tableHead) {
      tableHead.hidden = isTileBoard;
    }
    if (homeStage) {
      homeStage.hidden = !isHome;
    }
    if (coolantStage) {
      coolantStage.hidden = !isCoolant;
    }
    if (styleTestStage) {
      styleTestStage.hidden = !isStyleTest;
      if (!isStyleTest) {
        styleTestStage.innerHTML = "";
      }
    }
    updateNav();
    updateTabs();
    renderRows();
    renderHeroMetrics();
    renderStatus();
    if (isHome) {
      renderHomeStage(activeSection);
    } else if (isCoolant) {
      renderCoolantStage();
    } else if (isStyleTest) {
      renderStyleTestStage();
    }
  }

  function setSection(sectionName, preferredTabIndex) {
    if (!sections[sectionName]) {
      return;
    }
    if (isEngineeringSection(sectionName) && !state.engineeringMode) {
      return;
    }
    state.styleTestPopup = null;
    closeMetricChart();
    state.section = sectionName;
    if (
      Number.isInteger(preferredTabIndex) &&
      preferredTabIndex >= 0 &&
      preferredTabIndex < sections[sectionName].tabs.length
    ) {
      state.tabIndex = preferredTabIndex;
    } else {
      state.tabIndex = 0;
    }
    renderView();
  }

  function setTab(index) {
    if (index < 0 || index >= currentSection().tabs.length) {
      return;
    }
    if (state.styleTestPopup) {
      state.styleTestPopup = null;
      closeMetricChart();
      if (index !== state.tabIndex) {
        state.tabIndex = index;
      }
      renderView();
      return;
    }
    if (state.chart.tile && index === state.tabIndex && isCurrentTabChartable()) {
      closeMetricChart();
      renderView();
      return;
    }
    closeMetricChart();
    state.tabIndex = index;
    renderView();
  }

  async function fetchLiveData() {
    try {
      const response = await fetch("/api/live", { cache: "no-store" });
      if (!response.ok) {
        throw new Error("HTTP " + response.status);
      }
      state.live = await response.json();
      syncBatteryControlsFromLive(state.live);
      state.lastUpdated = new Date().toLocaleTimeString([], {
        hour: "2-digit",
        minute: "2-digit",
        second: "2-digit"
      });
      state.error = "";
      appendChartSample();
    } catch (error) {
      state.error = error && error.message ? error.message : "request failed";
    }
    renderView();
  }

  function schedulePolling() {
    fetchLiveData().finally(function () {
      window.setTimeout(schedulePolling, 1000);
    });
  }

  function bindEngineeringModeGesture() {
    if (!topbarTime) {
      return;
    }
    let timer = null;
    let pointerId = null;

    function clearGesture() {
      if (timer !== null) {
        window.clearTimeout(timer);
        timer = null;
      }
      pointerId = null;
    }

    topbarTime.addEventListener("pointerdown", function (event) {
      clearGesture();
      pointerId = event.pointerId;
      timer = window.setTimeout(function () {
        timer = null;
        toggleEngineeringMode();
      }, 1800);
    });

    topbarTime.addEventListener("pointerup", function (event) {
      if (pointerId === event.pointerId) {
        clearGesture();
      }
    });
    topbarTime.addEventListener("pointercancel", clearGesture);
    topbarTime.addEventListener("pointerleave", clearGesture);
  }

  l1Icons.forEach(function (button) {
    button.addEventListener("click", function () {
      setSection(button.dataset.section || "vehicle");
    });
  });

  valueRows.addEventListener("click", function (event) {
    const chartRow = event.target.closest("tr[data-chart-key]");
    if (chartRow) {
      openMetricChart(chartRow.dataset.chartKey || "");
      return;
    }
    const row = event.target.closest("tr[data-control-key]");
    if (!row) {
      return;
    }
    openBatteryControlModal(row.dataset.controlKey || "");
  });

  valueRows.addEventListener("keydown", function (event) {
    if (event.key !== "Enter" && event.key !== " ") {
      return;
    }
    const chartRow = event.target.closest("tr[data-chart-key]");
    if (chartRow) {
      event.preventDefault();
      openMetricChart(chartRow.dataset.chartKey || "");
      return;
    }
    const row = event.target.closest("tr[data-control-key]");
    if (!row) {
      return;
    }
    event.preventDefault();
    openBatteryControlModal(row.dataset.controlKey || "");
  });

  if (metricBoard) {
    metricBoard.addEventListener("click", function (event) {
      const tile = event.target.closest(".metric-tile[data-chart-key]");
      if (!tile) {
        return;
      }
      openMetricChart(tile.dataset.chartKey || "");
    });

    metricBoard.addEventListener("keydown", function (event) {
      if (event.key !== "Enter" && event.key !== " ") {
        return;
      }
      const tile = event.target.closest(".metric-tile[data-chart-key]");
      if (!tile) {
        return;
      }
      event.preventDefault();
      openMetricChart(tile.dataset.chartKey || "");
    });
  }

  tabs.forEach(function (button) {
    button.addEventListener("click", function () {
      setTab(Number(button.dataset.tabIndex || "0"));
    });
  });

  bindStyleTestDragScroll();
  bindEngineeringModeGesture();
  renderView();
  window.setInterval(updateTopbarTime, 30000);
  window.setTimeout(function () {
    if (introSplashVideo) {
      introSplashVideo.pause();
    }
    bootState.introElapsed = true;
    tryHideIntroSplash();
  }, INTRO_DURATION_MS);
  schedulePolling();
})();
