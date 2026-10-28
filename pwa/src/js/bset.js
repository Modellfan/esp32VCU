(function () {
  const STYLE_ID = "touch-battery-settings-modal-style";
  const ROOT_ID = "touchBatterySettingModalRoot";

  const template = [
    '<div class="touch-battery-modal-backdrop" id="touchBatterySettingModal" hidden aria-hidden="true">',
    '  <div class="touch-battery-modal-card" role="dialog" aria-modal="true" aria-labelledby="touchBatterySettingTitle">',
    '    <button class="touch-battery-modal-close" id="touchBatterySettingClose" type="button" aria-label="Close battery setting" data-touch-battery-close="1" onclick="return window.TouchBatterySettingsModalClose(event)" ontouchend="return window.TouchBatterySettingsModalClose(event)">X</button>',
    '    <div class="touch-battery-modal-copy">',
    '      <div class="touch-battery-modal-eyebrow">Vehicle control</div>',
    '      <h2 class="touch-battery-modal-title" id="touchBatterySettingTitle">Setting</h2>',
    '      <p class="touch-battery-modal-description" id="touchBatterySettingDescription"></p>',
    '    </div>',
    '    <div class="touch-battery-slider-tile">',
    '      <div class="touch-battery-slider-head">',
    '        <span class="touch-battery-slider-label" id="touchBatterySettingLabel">Setting</span>',
    '        <span class="touch-battery-slider-value" id="touchBatterySettingValue">0</span>',
    '      </div>',
    '      <input class="touch-battery-slider" id="touchBatterySettingSlider" type="range" min="0" max="100" step="1" value="0">',
    '      <div class="touch-battery-slider-scale">',
    '        <span id="touchBatterySettingMin">0</span>',
    '        <span id="touchBatterySettingMax">100</span>',
    '      </div>',
    '    </div>',
    '  </div>',
    '</div>'
  ].join("");

  const styles = [
    ".touch-battery-modal-backdrop {",
    "  position: fixed;",
    "  inset: 0;",
    "  z-index: 30000;",
    "  display: grid;",
    "  place-items: center;",
    "  opacity: 0;",
    "  visibility: hidden;",
    "  pointer-events: none;",
    "  background: rgba(0, 0, 0, 0.72);",
    "  backdrop-filter: blur(10px);",
    "  transition: opacity 120ms ease, visibility 120ms ease;",
    "}",
    ".touch-battery-modal-backdrop[hidden] {",
    "  display: none !important;",
    "}",
    ".touch-battery-modal-backdrop.is-open {",
    "  opacity: 1;",
    "  visibility: visible;",
    "  pointer-events: auto;",
    "}",
    ".touch-battery-modal-card {",
    "  position: relative;",
    "  width: min(560px, calc(100vw - 56px));",
    "  border: 1px solid rgba(255, 255, 255, 0.10);",
    "  border-radius: 22px;",
    "  padding: 28px 28px 24px;",
    "  background:",
    "    radial-gradient(circle at top right, rgba(0, 183, 255, 0.12), transparent 36%),",
    "    linear-gradient(180deg, rgba(10, 14, 20, 0.96), rgba(4, 6, 10, 0.98));",
    "  box-shadow: 0 30px 80px rgba(0, 0, 0, 0.48);",
    "}",
    ".touch-battery-modal-close {",
      "  position: absolute;",
      "  top: 18px;",
      "  right: 18px;",
      "  width: 58px;",
      "  height: 58px;",
      "  display: grid;",
      "  place-items: center;",
      "  border: 1px solid rgba(255, 255, 255, 0.12);",
      "  border-radius: 999px;",
      "  background: rgba(255, 255, 255, 0.08);",
      "  color: #ffffff;",
      "  font-size: 22px;",
      "  line-height: 1;",
      "  font-weight: 700;",
      "  cursor: pointer;",
      "  pointer-events: auto;",
      "  touch-action: manipulation;",
      "  user-select: none;",
      "  -webkit-user-select: none;",
      "  z-index: 6;",
    "}",
    ".touch-battery-modal-close:active {",
    "  transform: scale(0.96);",
    "}",
    ".touch-battery-modal-copy {",
    "  margin-top: 8px;",
    "  padding-right: 56px;",
    "}",
    ".touch-battery-modal-eyebrow {",
    "  color: rgba(255, 255, 255, 0.56);",
    "  font-size: 11px;",
    "  font-weight: 700;",
    "  letter-spacing: 0.16em;",
    "  text-transform: uppercase;",
    "}",
    ".touch-battery-modal-title {",
    "  margin: 10px 0 0;",
    "  color: #ffffff;",
    "  font-size: 30px;",
    "  font-weight: 700;",
    "  letter-spacing: 0.01em;",
    "}",
    ".touch-battery-modal-description {",
    "  margin: 10px 0 0;",
    "  color: rgba(255, 255, 255, 0.70);",
    "  font-size: 14px;",
    "  line-height: 1.5;",
    "}",
    ".touch-battery-slider-tile {",
    "  margin-top: 24px;",
    "  background: rgba(255, 255, 255, 0.05);",
    "  border: 1px solid rgba(255, 255, 255, 0.08);",
    "  border-radius: 18px;",
    "  padding: 18px;",
    "}",
    ".touch-battery-slider-head {",
    "  display: flex;",
    "  justify-content: space-between;",
    "  align-items: center;",
    "  margin-bottom: 12px;",
    "}",
    ".touch-battery-slider-label {",
    "  color: rgba(255, 255, 255, 0.72);",
    "  font-size: 14px;",
    "  font-weight: 700;",
    "}",
    ".touch-battery-slider-value {",
    "  color: #ffffff;",
    "  font-size: 18px;",
    "  font-weight: 800;",
    "  font-variant-numeric: tabular-nums;",
    "}",
    ".touch-battery-slider {",
    "  width: 100%;",
    "  height: 6px;",
    "  appearance: none;",
    "  background: linear-gradient(90deg, #1a3f27 0%, #3ddc6b 100%);",
    "  border-radius: 999px;",
    "  outline: none;",
    "}",
    ".touch-battery-slider::-webkit-slider-thumb {",
    "  appearance: none;",
    "  width: 22px;",
    "  height: 22px;",
    "  border-radius: 50%;",
    "  background: #ffffff;",
    "  border: 2px solid #d5d5d5;",
    "}",
    ".touch-battery-slider::-moz-range-thumb {",
    "  width: 22px;",
    "  height: 22px;",
    "  border-radius: 50%;",
    "  background: #ffffff;",
    "  border: 2px solid #d5d5d5;",
    "}",
    ".touch-battery-slider-scale {",
    "  margin-top: 10px;",
    "  display: flex;",
    "  justify-content: space-between;",
    "  color: rgba(255, 255, 255, 0.52);",
    "  font-size: 12px;",
    "  font-weight: 700;",
    "}",
    "@media (max-width: 680px) {",
    "  .touch-battery-modal-card { width: calc(100vw - 28px); padding: 22px 20px 20px; }",
    "  .touch-battery-modal-title { font-size: 26px; }",
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

  function mount() {
    ensureStyles();

    let root = document.getElementById(ROOT_ID);
    if (!root) {
      root = document.createElement("div");
      root.id = ROOT_ID;
      root.innerHTML = template;
      document.body.appendChild(root);
    }

    const backdrop = document.getElementById("touchBatterySettingModal");
    const closeButton = document.getElementById("touchBatterySettingClose");
    const title = document.getElementById("touchBatterySettingTitle");
    const description = document.getElementById("touchBatterySettingDescription");
    const label = document.getElementById("touchBatterySettingLabel");
    const value = document.getElementById("touchBatterySettingValue");
    const slider = document.getElementById("touchBatterySettingSlider");
    const minLabel = document.getElementById("touchBatterySettingMin");
    const maxLabel = document.getElementById("touchBatterySettingMax");

    let currentConfig = null;
    let commitTimer = 0;

    function cancelPendingCommit() {
      if (commitTimer) {
        window.clearTimeout(commitTimer);
        commitTimer = 0;
      }
    }

    function formatValue(number) {
      if (!currentConfig) {
        return String(number);
      }
      if (typeof currentConfig.formatValue === "function") {
        return currentConfig.formatValue(number);
      }
      if (currentConfig.unit) {
        return String(Math.round(number)) + " " + currentConfig.unit;
      }
      return String(Math.round(number));
    }

    function scheduleCommit(number) {
      if (!currentConfig || typeof currentConfig.onCommit !== "function") {
        return;
      }
      cancelPendingCommit();
      commitTimer = window.setTimeout(function () {
        currentConfig.onCommit(number);
      }, 120);
    }

    function setSliderValue(number, shouldCommit) {
      if (!slider || !value || !currentConfig) {
        return;
      }
      const clamped = Math.max(currentConfig.min, Math.min(currentConfig.max, Number(number) || currentConfig.min));
      slider.value = String(clamped);
      value.textContent = formatValue(clamped);
      if (shouldCommit) {
        scheduleCommit(clamped);
      }
    }

    function close() {
      cancelPendingCommit();
      currentConfig = null;
      if (backdrop) {
        backdrop.classList.remove("is-open");
        backdrop.setAttribute("aria-hidden", "true");
        backdrop.style.display = "none";
        backdrop.hidden = true;
      }
      document.body.classList.remove("touch-battery-modal-open");
    }

    function handleCloseAction(event) {
      if (event) {
        event.preventDefault();
        event.stopPropagation();
      }
      close();
    }

    function isCloseTarget(target) {
      if (!target || !closeButton) {
        return false;
      }
      if (target === closeButton) {
        return true;
      }
      if (typeof target.closest === "function") {
        return target.closest("[data-touch-battery-close='1']") === closeButton;
      }
      return false;
    }

    function open(config) {
      currentConfig = {
        description: config.description || "",
        formatValue: config.formatValue,
        label: config.label || config.title || "Setting",
        max: Number(config.max),
        min: Number(config.min),
        onCommit: config.onCommit,
        step: Number(config.step) || 1,
        title: config.title || "Setting",
        unit: config.unit || "",
        value: Number(config.value)
      };

      if (title) {
        title.textContent = currentConfig.title;
      }
      if (description) {
        description.textContent = currentConfig.description;
      }
      if (label) {
        label.textContent = currentConfig.label;
      }
      if (slider) {
        slider.min = String(currentConfig.min);
        slider.max = String(currentConfig.max);
        slider.step = String(currentConfig.step);
      }
      if (minLabel) {
        minLabel.textContent = String(currentConfig.min);
      }
      if (maxLabel) {
        maxLabel.textContent = String(currentConfig.max);
      }

      setSliderValue(currentConfig.value, false);
      if (backdrop) {
        backdrop.hidden = false;
        backdrop.style.display = "grid";
        backdrop.setAttribute("aria-hidden", "false");
        window.requestAnimationFrame(function () {
          backdrop.classList.add("is-open");
        });
      }
      document.body.classList.add("touch-battery-modal-open");
      if (slider) {
        window.setTimeout(function () {
          slider.focus();
        }, 20);
      }
    }

    window.TouchBatterySettingsModalClose = function (event) {
      handleCloseAction(event);
      return false;
    };

    if (slider && !slider.dataset.modalBound) {
      slider.addEventListener("input", function () {
        setSliderValue(slider.value, true);
      });
      slider.dataset.modalBound = "1";
    }

    if (closeButton && !closeButton.dataset.modalBound) {
      closeButton.addEventListener("touchend", handleCloseAction, { passive: false });
      closeButton.addEventListener("pointerup", handleCloseAction);
      closeButton.addEventListener("mouseup", handleCloseAction);
      closeButton.addEventListener("click", handleCloseAction);
      closeButton.dataset.modalBound = "1";
    }

    if (backdrop && !backdrop.dataset.modalBound) {
      ["touchend", "pointerup", "mouseup", "click"].forEach(function (eventName) {
        backdrop.addEventListener(eventName, function (event) {
          if (isCloseTarget(event.target)) {
            handleCloseAction(event);
            return;
          }
          if (event.target === backdrop) {
            close();
          }
        }, eventName === "touchend" ? { passive: false } : false);
      });
      backdrop.dataset.modalBound = "1";
    }

    if (!document.body.dataset.touchBatteryModalBound) {
      document.addEventListener("keydown", function (event) {
        if (event.key === "Escape" && backdrop && !backdrop.hidden) {
          close();
        }
      });
      document.body.dataset.touchBatteryModalBound = "1";
    }

    return {
      close: close,
      open: open
    };
  }

  window.TouchBatterySettingsModal = {
    mount: mount
  };
})();
