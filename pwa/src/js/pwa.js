/*
 * PWA bootstrap: registers the service worker where the browser allows it
 * (HTTPS or localhost) and applies updates without interrupting the driver.
 * On plain HTTP (VCU access point) the browser refuses service workers; on the Android radio
 * Chrome is set to treat http://192.168.4.1 as secure (chrome://flags, see pwa/README.md).
 */
(function () {
  "use strict";

  /* cache: { version, cached, total } as reported by the service worker. */
  const state = { registered: false, reason: "", cache: null, refreshCache: function () {} };
  window.VcuPwa = state;

  function notify() {
    window.dispatchEvent(new CustomEvent("vcu-pwa-status"));
  }

  if (!window.isSecureContext) {
    state.reason = "insecure-context";
    return;
  }
  if (!("serviceWorker" in navigator)) {
    state.reason = "unsupported";
    return;
  }

  function queryCacheStatus() {
    const controller = navigator.serviceWorker.controller;
    if (!controller || typeof MessageChannel !== "function") {
      return;
    }
    const channel = new MessageChannel();
    channel.port1.onmessage = function (event) {
      state.cache = event.data || null;
      notify();
    };
    controller.postMessage({ type: "STATUS" }, [channel.port2]);
  }
  state.refreshCache = queryCacheStatus;
  navigator.serviceWorker.addEventListener("controllerchange", queryCacheStatus);
  /* The first install fills the cache while the page runs: poll until it is complete. */
  const cachePoll = window.setInterval(function () {
    queryCacheStatus();
    if (state.cache && state.cache.cached === state.cache.total) {
      window.clearInterval(cachePoll);
    }
  }, 2000);

  let reloading = false;
  navigator.serviceWorker.addEventListener("controllerchange", function () {
    if (reloading) {
      return;
    }
    reloading = true;
    /* Reload only while the page is not in front of the driver. */
    if (document.hidden) {
      window.location.reload();
    } else {
      document.addEventListener("visibilitychange", function onHidden() {
        if (document.hidden) {
          document.removeEventListener("visibilitychange", onHidden);
          window.location.reload();
        }
      });
    }
  });

  window.addEventListener("load", function () {
    navigator.serviceWorker.register("sw.js", { scope: "./" }).then(function (registration) {
      if (!registration) {
        state.reason = "register-failed";
        return;
      }
      state.registered = true;
      notify();
      queryCacheStatus();
      function activateWaiting(worker) {
        if (worker && navigator.serviceWorker.controller) {
          worker.postMessage({ type: "SKIP_WAITING" });
        }
      }
      activateWaiting(registration.waiting);
      registration.addEventListener("updatefound", function () {
        const worker = registration.installing;
        if (!worker) {
          return;
        }
        worker.addEventListener("statechange", function () {
          if (worker.state === "installed") {
            activateWaiting(worker);
          }
        });
      });
    }).catch(function (error) {
      state.reason = "register-failed";
      console.warn("service worker registration failed", error);
    });
  });
})();
