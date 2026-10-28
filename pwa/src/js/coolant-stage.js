(function () {
  const BASE_WIDTH = 948;
  const BASE_HEIGHT = 600;
  const PIPE_LAYOUT = [
    { color: "blue", speed: 1, thickness: 7, x1: 90, y1: 170, x2: 420, y2: 170 },
    { color: "blue", speed: 1, thickness: 7, x1: 420, y1: 170, x2: 90, y2: 350 },
    { color: "blue", speed: 1, thickness: 7, x1: 90, y1: 350, x2: 420, y2: 350 },
    { color: "red", speed: 1, thickness: 7, x1: 528, y1: 170, x2: 858, y2: 170 },
    { color: "red", speed: 1, thickness: 7, x1: 858, y1: 170, x2: 528, y2: 350 },
    { color: "red", speed: 1, thickness: 7, x1: 528, y1: 350, x2: 858, y2: 350 },
    { color: "gray", speed: 1, thickness: 7, x1: 210, y1: 470, x2: 740, y2: 470 }
  ];

  function toNumber(value, fallback) {
    const parsed = Number(value);
    return Number.isFinite(parsed) ? parsed : fallback;
  }

  function formatTemp(value) {
    const numeric = toNumber(value, null);
    if (numeric === null) {
      return "--";
    }
    return numeric.toFixed(1) + " C";
  }

  function createMetric(label, id) {
    return [
      '<div class="coolant-system-metric">',
      '  <span class="coolant-system-metric-label">' + label + "</span>",
      '  <strong class="coolant-system-metric-value" id="' + id + '">--</strong>',
      "</div>"
    ].join("");
  }

  function createFanSwitch() {
    return [
      '<div class="coolant-fan-control" role="group" aria-label="Coolant fan">',
      '  <span class="coolant-fan-control-label">Fan</span>',
      '  <button class="coolant-fan-button" type="button" data-fan-level="0">Off</button>',
      '  <button class="coolant-fan-button" type="button" data-fan-level="1">Low</button>',
      '  <button class="coolant-fan-button" type="button" data-fan-level="2">High</button>',
      "</div>"
    ].join("");
  }

  function createPipe(parent, spec, scaleX, scaleY) {
    const pipe = document.createElement("div");
    pipe.className = "coolant-pipe " + spec.color;

    const flowA = document.createElement("div");
    flowA.className = "flow a";

    const flowB = document.createElement("div");
    flowB.className = "flow b";

    const shine = document.createElement("div");
    shine.className = "shine";

    const x1 = spec.x1 * scaleX;
    const y1 = spec.y1 * scaleY;
    const x2 = spec.x2 * scaleX;
    const y2 = spec.y2 * scaleY;
    const dx = x2 - x1;
    const dy = y2 - y1;
    const length = Math.hypot(dx, dy);
    const angle = Math.atan2(dy, dx) * 180 / Math.PI;
    const thickness = spec.thickness * ((scaleX + scaleY) / 2);

    pipe.style.width = length + "px";
    pipe.style.height = thickness + "px";
    pipe.style.left = x1 + "px";
    pipe.style.top = (y1 - thickness / 2) + "px";
    pipe.style.transform = "rotate(" + angle + "deg)";
    pipe.style.transformOrigin = "0 50%";
    pipe.style.setProperty("--speed", (spec.speed * 6) + "s");

    pipe.appendChild(flowA);
    pipe.appendChild(flowB);
    pipe.appendChild(shine);
    parent.appendChild(pipe);
  }

  function mount(host, options) {
    const settings = options || {};
    if (!host) {
      return null;
    }

    host.innerHTML = [
      '<section class="coolant-system-panel" aria-label="Coolant system animated overview">',
      '  <div class="coolant-system-copy">',
      '    <h2>Coolant System</h2>',
      '    <p>Animated loop overview without the tab strip.</p>',
      '    <div class="coolant-system-metrics">',
           createMetric("Coolant", "coolantMetricCoolant"),
           createMetric("Heatsink", "coolantMetricHeatsink"),
           createMetric("Ambient", "coolantMetricAmbient"),
      "    </div>",
           createFanSwitch(),
      "  </div>",
      '  <div class="coolant-system-scene" id="coolantSystemScene" aria-hidden="true"></div>',
      "</section>"
    ].join("");

    const panel = host.querySelector(".coolant-system-panel");
    const scene = host.querySelector("#coolantSystemScene");
    const coolantMetric = host.querySelector("#coolantMetricCoolant");
    const heatsinkMetric = host.querySelector("#coolantMetricHeatsink");
    const ambientMetric = host.querySelector("#coolantMetricAmbient");
    const fanButtons = Array.from(host.querySelectorAll(".coolant-fan-button"));

    fanButtons.forEach(function (button) {
      button.addEventListener("click", function () {
        const nextLevel = Number(button.dataset.fanLevel || "0");
        if (typeof settings.onFanLevelChange === "function") {
          settings.onFanLevelChange(nextLevel);
        }
      });
    });

    function redraw() {
      if (!panel || !scene) {
        return;
      }
      const rect = panel.getBoundingClientRect();
      if (!rect.width || !rect.height) {
        return;
      }
      const scaleX = rect.width / BASE_WIDTH;
      const scaleY = rect.height / BASE_HEIGHT;
      scene.innerHTML = "";
      PIPE_LAYOUT.forEach(function (spec) {
        createPipe(scene, spec, scaleX, scaleY);
      });
    }

    const resizeObserver = typeof window.ResizeObserver === "function"
      ? new window.ResizeObserver(redraw)
      : null;

    if (resizeObserver) {
      resizeObserver.observe(panel);
    } else {
      window.addEventListener("resize", redraw);
    }

    redraw();

    return {
      update: function (live) {
        const data = live || {};
        if (coolantMetric) {
          coolantMetric.textContent = formatTemp(data.motor2_coolant_temperature);
        }
        if (heatsinkMetric) {
          heatsinkMetric.textContent = formatTemp(data.sdu_tmphs);
        }
        if (ambientMetric) {
          ambientMetric.textContent = formatTemp(data.ambient_c);
        }
        const fanLevel = Math.max(0, Math.min(2, Math.round(Number(data.coolant_fan_level) || 0)));
        fanButtons.forEach(function (button) {
          const active = Number(button.dataset.fanLevel || "0") === fanLevel;
          button.classList.toggle("active", active);
          button.setAttribute("aria-pressed", active ? "true" : "false");
        });
      },
      destroy: function () {
        if (resizeObserver) {
          resizeObserver.disconnect();
        } else {
          window.removeEventListener("resize", redraw);
        }
      }
    };
  }

  window.TouchCoolantStage = {
    mount: mount
  };
})();
