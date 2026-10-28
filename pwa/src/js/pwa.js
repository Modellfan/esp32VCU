/*
 * PWA bootstrap: registers the service worker where the browser allows it
 * (HTTPS or localhost) and applies updates without interrupting the driver.
 * On plain HTTP (VCU access point) this is a no-op; manifest and icons still work.
 */
(function () {
  "use strict";

  const state = { registered: false, reason: "" };
  window.VcuPwa = state;

  if (!window.isSecureContext) {
    state.reason = "insecure-context";
    return;
  }
  if (!("serviceWorker" in navigator)) {
    state.reason = "unsupported";
    return;
  }

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
