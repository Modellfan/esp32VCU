/*
 * Network layer for the VCU touch PWA.
 * - Every request has a timeout (a hung VCU never blocks the UI).
 * - Responses must be a JSON object; anything else is rejected.
 * - Set requests run one at a time; a newer value for the same key replaces a queued one.
 * - Polling pauses while the page is hidden and backs off after failures.
 */
(function () {
  "use strict";

  const config = window.VCU_CONFIG || {};
  const API_BASE = typeof config.apiBase === "string" ? config.apiBase.replace(/\/+$/, "") : "";
  const DEFAULT_TIMEOUT_MS = 2500;

  function NetError(kind, message, status) {
    const error = new Error(message);
    error.kind = kind;
    error.status = status || 0;
    return error;
  }

  async function getJson(path, options) {
    const opts = options || {};
    const timeoutMs = opts.timeoutMs || DEFAULT_TIMEOUT_MS;
    const controller = typeof AbortController === "function" ? new AbortController() : null;
    const timer = controller ? window.setTimeout(function () { controller.abort(); }, timeoutMs) : null;
    let response;
    try {
      response = await fetch(API_BASE + path, {
        method: opts.method || "GET",
        cache: "no-store",
        headers: opts.headers,
        body: opts.body,
        signal: controller ? controller.signal : undefined
      });
    } catch (error) {
      if (error && error.name === "AbortError") {
        throw NetError("timeout", "timeout after " + timeoutMs + " ms");
      }
      throw NetError("network", "VCU not reachable");
    } finally {
      if (timer !== null) {
        window.clearTimeout(timer);
      }
    }
    if (!response.ok) {
      throw NetError("http", "HTTP " + response.status, response.status);
    }
    let text;
    try {
      text = await response.text();
    } catch (_) {
      throw NetError("network", "response interrupted");
    }
    if (text.length > 512 * 1024) {
      throw NetError("format", "response too large");
    }
    let data;
    try {
      data = JSON.parse(text);
    } catch (_) {
      throw NetError("format", "invalid JSON");
    }
    if (data === null || typeof data !== "object" || Array.isArray(data)) {
      throw NetError("format", "unexpected response");
    }
    return data;
  }

  /* Serialised set queue: one request in flight, latest value per key wins. */
  const pending = new Map();
  let draining = false;
  const listeners = [];

  function onSetResult(listener) {
    listeners.push(listener);
  }

  function emit(result) {
    listeners.forEach(function (listener) {
      try {
        listener(result);
      } catch (error) {
        console.error("set listener failed", error);
      }
    });
  }

  async function drain() {
    if (draining) {
      return;
    }
    draining = true;
    try {
      while (pending.size) {
        const entry = pending.entries().next().value;
        const key = entry[0];
        const job = entry[1];
        pending.delete(key);
        const query = new URLSearchParams();
        query.set(job.param, String(job.value));
        try {
          const data = await getJson(job.endpoint + "?" + query.toString(), { timeoutMs: 3000 });
          emit({ ok: true, key: job.param, value: job.value, data: data });
        } catch (error) {
          emit({ ok: false, key: job.param, value: job.value, error: error });
        }
      }
    } finally {
      draining = false;
    }
  }

  function set(param, value, endpoint) {
    const target = endpoint || "/api/set";
    pending.set(target + "|" + param, { endpoint: target, param: param, value: value });
    drain();
  }

  function isSetPending(param) {
    for (const job of pending.values()) {
      if (job.param === param) {
        return true;
      }
    }
    return false;
  }

  /* Poller with backoff and visibility handling. */
  function createPoller(task, options) {
    const opts = options || {};
    const baseMs = opts.intervalMs || 1000;
    const maxMs = opts.maxIntervalMs || 5000;
    let failures = 0;
    let timer = null;
    let running = false;
    let stopped = true;

    function nextDelay() {
      if (!failures) {
        return baseMs;
      }
      return Math.min(maxMs, baseMs * Math.pow(2, failures - 1));
    }

    function schedule(delay) {
      if (timer !== null) {
        window.clearTimeout(timer);
      }
      timer = window.setTimeout(tick, delay);
    }

    async function tick() {
      timer = null;
      if (stopped || running) {
        return;
      }
      if (document.hidden) {
        return; /* resumed by visibilitychange */
      }
      running = true;
      try {
        const ok = await task();
        failures = ok ? 0 : failures + 1;
      } catch (error) {
        failures += 1;
        console.error("poll task failed", error);
      } finally {
        running = false;
      }
      if (!stopped) {
        schedule(nextDelay());
      }
    }

    function kick() {
      if (!stopped && !running) {
        schedule(0);
      }
    }

    document.addEventListener("visibilitychange", function () {
      if (!document.hidden) {
        kick();
      }
    });
    window.addEventListener("online", kick);
    window.addEventListener("focus", kick);

    return {
      start: function () {
        stopped = false;
        kick();
      },
      stop: function () {
        stopped = true;
        if (timer !== null) {
          window.clearTimeout(timer);
          timer = null;
        }
      },
      kick: kick,
      failures: function () {
        return failures;
      }
    };
  }

  window.VcuNet = {
    apiBase: API_BASE,
    getJson: getJson,
    set: set,
    isSetPending: isSetPending,
    onSetResult: onSetResult,
    createPoller: createPoller
  };
})();
