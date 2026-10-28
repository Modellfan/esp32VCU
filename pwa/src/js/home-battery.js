(function () {
  const STYLE_ID = "touch-home-battery-style";
  const DEFAULT_LEVEL = 28;
  const DEFAULT_RANGE_UNIT = "km";
  const DEFAULT_POWER_UNIT = "kW";

  const template = [
    '<button class="home-battery-panel" id="homeBatteryPanel" type="button" aria-label="Open vehicle battery overview">',
    '  <div class="home-battery-topbar">',
    '    <div class="home-battery-caption" id="homeBatteryCaptionValue">--.--.----</div>',
    '    <div class="home-battery-time" id="homeBatteryTimeValue">--:--</div>',
    '    <div class="home-battery-date" id="homeBatteryDateValue">--</div>',
    '  </div>',
    '  <div class="home-battery-center">',
    '    <div class="home-battery-metric home-battery-metric-left">',
    '      <span class="home-battery-value" id="homeBatteryPercentValue">28</span>',
    '      <span class="home-battery-unit">%</span>',
    '    </div>',
    '    <div class="home-battery-wrap">',
    '      <div class="home-battery-shell">',
    '        <div class="home-battery-fill" id="homeBatteryFill">',
    '          <div class="home-battery-flow"></div>',
    '        </div>',
    '        <div class="home-battery-grid"></div>',
    '      </div>',
    '      <div class="home-battery-power" id="homeBatteryPowerValue">193 kW</div>',
    '    </div>',
    '    <div class="home-battery-metric home-battery-metric-right">',
    '      <span class="home-battery-value" id="homeBatteryRangeValue">85</span>',
    '      <span class="home-battery-unit" id="homeBatteryRangeUnit">km</span>',
    '    </div>',
    '  </div>',
    '</button>'
  ].join("");

  const styles = [
    ":root {",
    "  --home-battery-level: 28;",
    "  --home-battery-flow-speed: 6.8s;",
    "  --home-battery-grid: rgba(255, 255, 255, 0.14);",
    "  --home-battery-value: #f3f5f8;",
    "  --home-battery-muted: rgba(255, 255, 255, 0.60);",
    "  --home-battery-green-1: rgba(95, 255, 145, 0.62);",
    "  --home-battery-green-2: rgba(45, 230, 95, 0.62);",
    "  --home-battery-green-3: rgba(12, 175, 55, 0.62);",
    "  --home-battery-green-4: rgba(190, 255, 210, 0.28);",
    "}",
    ".home-battery-panel {",
    "  display: flex;",
    "  flex-direction: column;",
    "  align-items: center;",
    "  gap: 22px;",
    "  padding: 0;",
    "  margin: 0;",
    "  border: 0;",
    "  background: transparent;",
    "  text-align: center;",
    "  cursor: pointer;",
    "  font: inherit;",
    "}",
    ".home-battery-panel:focus-visible {",
    "  outline: 1px solid rgba(140, 255, 143, 0.65);",
    "  outline-offset: 10px;",
    "}",
    ".home-battery-panel:hover .home-battery-shell,",
    ".home-battery-panel:focus-visible .home-battery-shell {",
    "  box-shadow:",
    "    inset 0 1px 0 rgba(255, 255, 255, 0.28),",
    "    inset 0 0 18px rgba(255, 255, 255, 0.05),",
    "    0 0 18px rgba(140, 255, 143, 0.16);",
    "}",
    ".home-battery-topbar {",
    "  display: flex;",
    "  flex-direction: column;",
    "  align-items: center;",
    "  gap: 4px;",
    "}",
    ".home-battery-caption {",
    "  color: var(--home-battery-muted);",
    "  font-size: 15px;",
    "  letter-spacing: 0.03em;",
    "  text-transform: none;",
    "}",
    ".home-battery-time {",
    "  color: var(--home-battery-value);",
    "  font-size: 38px;",
    "  line-height: 0.94;",
    "  font-weight: 300;",
    "  letter-spacing: -0.04em;",
    "  font-variant-numeric: tabular-nums;",
    "}",
    ".home-battery-date {",
    "  color: var(--home-battery-muted);",
    "  font-size: 15px;",
    "  letter-spacing: 0.03em;",
    "}",
    ".home-battery-center {",
    "  display: flex;",
    "  align-items: center;",
    "  gap: 18px;",
    "}",
    ".home-battery-metric {",
    "  display: flex;",
    "  flex-direction: column;",
    "  align-items: center;",
    "  justify-content: center;",
    "  min-width: 88px;",
    "}",
    ".home-battery-value {",
    "  color: var(--home-battery-value);",
    "  font-size: 66px;",
    "  line-height: 0.92;",
    "  font-weight: 300;",
    "  letter-spacing: -0.06em;",
    "  font-variant-numeric: tabular-nums;",
    "}",
    ".home-battery-unit {",
    "  margin-top: 9px;",
    "  color: var(--home-battery-muted);",
    "  font-size: 15px;",
    "  letter-spacing: 0.20em;",
    "  text-transform: uppercase;",
    "}",
    ".home-battery-wrap {",
    "  display: flex;",
    "  flex-direction: column;",
    "  align-items: center;",
    "  gap: 18px;",
    "}",
    ".home-battery-shell {",
    "  position: relative;",
    "  width: 118px;",
    "  height: 238px;",
    "  overflow: hidden;",
    "  border: 1.5px solid rgba(255, 255, 255, 0.92);",
    "  border-radius: 4px;",
    "  background:",
    "    linear-gradient(180deg, rgba(255, 255, 255, 0.08), rgba(255, 255, 255, 0.01)),",
    "    linear-gradient(90deg, rgba(255, 255, 255, 0.05), rgba(255, 255, 255, 0.01) 22%, rgba(255, 255, 255, 0.00) 45%, rgba(255, 255, 255, 0.05) 100%);",
    "  box-shadow:",
    "    inset 0 1px 0 rgba(255, 255, 255, 0.28),",
    "    inset 0 0 18px rgba(255, 255, 255, 0.05),",
    "    0 0 14px rgba(255, 255, 255, 0.04);",
    "  backdrop-filter: blur(8px) saturate(135%);",
    "  -webkit-backdrop-filter: blur(8px) saturate(135%);",
    "}",
    ".home-battery-shell::before {",
    "  content: \"\";",
    "  position: absolute;",
    "  top: -10px;",
    "  left: 50%;",
    "  transform: translateX(-50%);",
    "  width: 24px;",
    "  height: 8px;",
    "  border-radius: 2px 2px 0 0;",
    "  background: rgba(255, 255, 255, 0.95);",
    "  box-shadow: 0 0 8px rgba(255, 255, 255, 0.08);",
    "}",
    ".home-battery-shell::after {",
    "  content: \"\";",
    "  position: absolute;",
    "  inset: 0;",
    "  z-index: 4;",
    "  pointer-events: none;",
    "  background:",
    "    linear-gradient(100deg,",
    "      rgba(255, 255, 255, 0.00) 0%,",
    "      rgba(255, 255, 255, 0.18) 14%,",
    "      rgba(255, 255, 255, 0.04) 28%,",
    "      rgba(255, 255, 255, 0.00) 42%,",
    "      rgba(255, 255, 255, 0.10) 72%,",
    "      rgba(255, 255, 255, 0.00) 100%);",
    "  mix-blend-mode: screen;",
    "}",
    ".home-battery-grid {",
    "  position: absolute;",
    "  inset: 10px 8px;",
    "  z-index: 3;",
    "  opacity: 0.75;",
    "  pointer-events: none;",
    "  background-image: linear-gradient(to bottom, var(--home-battery-grid) 1px, transparent 1px);",
    "  background-size: 100% 20px;",
    "}",
    ".home-battery-fill {",
    "  position: absolute;",
    "  left: 4px;",
    "  right: 4px;",
    "  bottom: 4px;",
    "  min-height: 0;",
    "  overflow: hidden;",
    "  height: calc(var(--home-battery-level) * 1%);",
    "  background:",
    "    linear-gradient(180deg, rgba(120, 255, 165, 0.12), rgba(15, 200, 50, 0.08)),",
    "    rgba(80, 255, 130, 0.05);",
    "  box-shadow:",
    "    inset 0 0 18px rgba(255, 255, 255, 0.10),",
    "    inset 0 -8px 14px rgba(0, 0, 0, 0.08);",
    "  transition: height 700ms cubic-bezier(.2,.8,.2,1);",
    "  backdrop-filter: blur(6px) saturate(140%);",
    "  -webkit-backdrop-filter: blur(6px) saturate(140%);",
    "}",
    ".home-battery-fill::after {",
    "  content: \"\";",
    "  position: absolute;",
    "  left: 0;",
    "  right: 0;",
    "  top: 0;",
    "  height: 10px;",
    "  opacity: 0.95;",
    "  z-index: 2;",
    "  background: linear-gradient(180deg, rgba(255, 255, 255, 0.34), rgba(255, 255, 255, 0.10), rgba(255, 255, 255, 0.00));",
    "}",
    ".home-battery-flow {",
    "  position: absolute;",
    "  top: 0;",
    "  left: 0;",
    "  width: 200%;",
    "  height: 100%;",
    "  will-change: transform;",
    "  background:",
    "    repeating-linear-gradient(45deg,",
    "      var(--home-battery-green-1) 0px 18px,",
    "      var(--home-battery-green-2) 18px 36px,",
    "      var(--home-battery-green-3) 36px 54px,",
    "      var(--home-battery-green-2) 54px 72px,",
    "      var(--home-battery-green-1) 72px 90px,",
    "      var(--home-battery-green-1) 90px 108px),",
    "    repeating-linear-gradient(135deg,",
    "      var(--home-battery-green-4) 0px 14px,",
    "      rgba(120, 255, 160, 0.24) 14px 28px,",
    "      rgba(40, 220, 85, 0.20) 28px 42px,",
    "      rgba(18, 170, 55, 0.18) 42px 56px,",
    "      rgba(120, 255, 160, 0.22) 56px 70px,",
    "      var(--home-battery-green-4) 70px 84px),",
    "    linear-gradient(180deg, rgba(255, 255, 255, 0.20), rgba(255, 255, 255, 0.05) 28%, rgba(0, 0, 0, 0.10) 100%);",
    "  box-shadow: inset 0 0 16px rgba(255, 255, 255, 0.10);",
    "  animation: home-battery-flow-move var(--home-battery-flow-speed) linear infinite;",
    "}",
    ".home-battery-power {",
    "  color: #8cff8f;",
    "  font-size: 20px;",
    "  text-shadow: 0 0 10px rgba(70, 255, 110, 0.22);",
    "}",
    ".home-battery-power::before {",
    "  content: \"\\2197 \";",
    "}",
    "@keyframes home-battery-flow-move {",
    "  from { transform: translateX(0); }",
    "  to { transform: translateX(-50%); }",
    "}",
    "@media (max-width: 900px), (max-height: 540px) {",
    "  .home-battery-panel { gap: 18px; }",
    "  .home-battery-time { font-size: 32px; }",
    "  .home-battery-caption { font-size: 13px; }",
    "  .home-battery-date { font-size: 13px; }",
    "  .home-battery-center { gap: 12px; }",
    "  .home-battery-metric { min-width: 70px; }",
    "  .home-battery-shell { width: 96px; height: 194px; }",
    "  .home-battery-value { font-size: 54px; }",
    "  .home-battery-unit { font-size: 13px; }",
    "  .home-battery-power { font-size: 17px; }",
    "}",
    "@media (prefers-reduced-motion: reduce) {",
    "  .home-battery-flow { animation-duration: 0s; animation-iteration-count: 1; }",
    "}",
  ].join("\n");

  function ensureStyles() {
    if (document.getElementById(STYLE_ID)) {
      return;
    }
    const style = document.createElement("style");
    style.id = STYLE_ID;
    style.textContent = styles;
    document.head.appendChild(style);
  }

  function clamp(value, min, max) {
    const number = Number(value);
    if (!Number.isFinite(number)) {
      return min;
    }
    return Math.max(min, Math.min(max, number));
  }

  function estimateRange(level) {
    return Math.round(level * 3.05);
  }

  function estimatePower(level) {
    return Math.round(120 + (level / 100) * 260);
  }

  function formatClientTime(date) {
    return new Intl.DateTimeFormat("de-DE", {
      hour: "2-digit",
      minute: "2-digit",
      hour12: false
    }).format(date);
  }

  function formatClientDate(date) {
    const day = String(date.getDate()).padStart(2, "0");
    const month = String(date.getMonth() + 1).padStart(2, "0");
    return day + "." + month + "." + String(date.getFullYear());
  }

  function formatClientWeekday(date) {
    return new Intl.DateTimeFormat("de-DE", {
      weekday: "long"
    }).format(date);
  }

  function mount(container) {
    if (!container) {
      return null;
    }

    ensureStyles();
    container.innerHTML = template;

    const panel = container.querySelector("#homeBatteryPanel");
    const captionValue = container.querySelector("#homeBatteryCaptionValue");
    const timeValue = container.querySelector("#homeBatteryTimeValue");
    const dateValue = container.querySelector("#homeBatteryDateValue");
    const percentValue = container.querySelector("#homeBatteryPercentValue");
    const rangeValue = container.querySelector("#homeBatteryRangeValue");
    const rangeUnit = container.querySelector("#homeBatteryRangeUnit");
    const powerValue = container.querySelector("#homeBatteryPowerValue");

    function updateClock() {
      const now = new Date();
      if (captionValue) {
        captionValue.textContent = formatClientDate(now);
      }
      if (timeValue) {
        timeValue.textContent = formatClientTime(now);
      }
      if (dateValue) {
        dateValue.textContent = formatClientWeekday(now);
      }
    }

    function update(config) {
      const options = config || {};
      const level = clamp(options.level == null ? DEFAULT_LEVEL : options.level, 0, 100);
      const resolvedRange = options.rangeValue == null ? estimateRange(level) : Math.round(Number(options.rangeValue) || 0);
      const resolvedRangeUnit = options.rangeUnit || DEFAULT_RANGE_UNIT;
      const resolvedPower = options.powerValue == null ? estimatePower(level) : Math.round(Number(options.powerValue) || 0);
      const resolvedPowerUnit = options.powerUnit || DEFAULT_POWER_UNIT;

      document.documentElement.style.setProperty("--home-battery-level", String(level));
      document.documentElement.style.setProperty(
        "--home-battery-flow-speed",
        level > 80 ? "5.2s" : level > 40 ? "6.5s" : "8s"
      );

      if (percentValue) {
        percentValue.textContent = String(Math.round(level));
      }
      if (rangeValue) {
        rangeValue.textContent = String(resolvedRange);
      }
      if (rangeUnit) {
        rangeUnit.textContent = resolvedRangeUnit;
      }
      if (powerValue) {
        powerValue.textContent = String(resolvedPower) + " " + resolvedPowerUnit;
      }
      updateClock();
    }

    const clockHandle = window.setInterval(updateClock, 1000);
    updateClock();
    update({ level: DEFAULT_LEVEL });

    return {
      destroy: function () {
        window.clearInterval(clockHandle);
      },
      element: panel,
      update: update
    };
  }

  window.TouchHomeBattery = {
    clamp: clamp,
    estimatePower: estimatePower,
    estimateRange: estimateRange,
    mount: mount
  };
})();
