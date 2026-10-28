/*
 * Service worker for the VCU touch PWA.
 * Only active in a secure context (HTTPS or localhost); plain HTTP from the VCU skips it.
 * - App shell: precached, served cache-first, refreshed in the background.
 * - Navigation: network first (2.5 s), cached shell as fallback.
 * - /api/*: never cached; offline yields a 503 JSON so the UI shows "connection lost".
 * The build (pwa/tools/build.py) replaces VERSION and PRECACHE.
 */
const VERSION = "dev"; /* @build:version */
const PRECACHE = ["./", "index.html", "manifest.json"]; /* @build:precache */
const CACHE = "vcu-pwa-" + VERSION;

self.addEventListener("install", function (event) {
  event.waitUntil(
    caches.open(CACHE).then(function (cache) {
      return cache.addAll(PRECACHE.map(function (path) {
        return new Request(path, { cache: "reload" });
      }));
    })
  );
});

self.addEventListener("activate", function (event) {
  event.waitUntil(
    caches.keys().then(function (keys) {
      return Promise.all(keys.filter(function (key) {
        return key.indexOf("vcu-pwa-") === 0 && key !== CACHE;
      }).map(function (key) {
        return caches.delete(key);
      }));
    }).then(function () {
      return self.clients.claim();
    })
  );
});

self.addEventListener("message", function (event) {
  if (event.data && event.data.type === "SKIP_WAITING") {
    self.skipWaiting();
  }
});

function timeout(ms) {
  return new Promise(function (_, reject) {
    setTimeout(function () { reject(new Error("timeout")); }, ms);
  });
}

function offlineApi() {
  return new Response(JSON.stringify({ error: "offline" }), {
    status: 503,
    headers: { "Content-Type": "application/json", "Cache-Control": "no-store" }
  });
}

self.addEventListener("fetch", function (event) {
  const request = event.request;
  if (request.method !== "GET") {
    return;
  }
  const url = new URL(request.url);
  if (url.origin !== self.location.origin) {
    return;
  }
  if (url.pathname.indexOf("/api/") !== -1) {
    event.respondWith(fetch(request).catch(offlineApi));
    return;
  }
  if (request.mode === "navigate") {
    event.respondWith(
      Promise.race([fetch(request), timeout(2500)]).then(function (response) {
        if (response && response.ok) {
          const copy = response.clone();
          caches.open(CACHE).then(function (cache) { cache.put("index.html", copy); });
        }
        return response;
      }).catch(function () {
        return caches.match("index.html").then(function (cached) {
          return cached || caches.match("./") || offlineApi();
        });
      })
    );
    return;
  }
  event.respondWith(
    caches.open(CACHE).then(function (cache) {
      return cache.match(request, { ignoreSearch: true }).then(function (cached) {
        const network = fetch(request).then(function (response) {
          if (response && response.ok) {
            cache.put(request, response.clone());
          }
          return response;
        });
        if (cached) {
          network.catch(function () {});
          return cached;
        }
        return network;
      });
    })
  );
});
