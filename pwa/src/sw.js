/*
 * Service worker for the VCU touch PWA.
 * Only active in a secure context (HTTPS or localhost); plain HTTP from the VCU skips it.
 * - App shell: precached (versioned URLs, one file at a time), served cache-first.
 * - Navigation: network first (2.5 s), cached shell as fallback.
 * - /api/*: never cached; offline yields a 503 JSON so the UI shows "connection lost".
 * The build (pwa/tools/build.py) replaces VERSION and PRECACHE.
 */
const VERSION = "dev"; /* @build:version */
const PRECACHE = ["./", "index.html", "manifest.json"]; /* @build:precache */
const CACHE = "vcu-pwa-" + VERSION;

/* The ESP32 web server handles one connection at a time: fetch the app shell one file
   after the other, with retries. A file that still fails aborts the install, so the
   browser retries on the next visit instead of keeping an incomplete cache. */
async function fetchWithRetry(path, attempts) {
  let lastError = null;
  for (let i = 0; i < attempts; i++) {
    try {
      const response = await fetch(new Request(path, { cache: "reload" }));
      if (response.ok) {
        return response;
      }
      lastError = new Error("HTTP " + response.status + " for " + path);
    } catch (error) {
      lastError = error;
    }
    await new Promise(function (resolve) { setTimeout(resolve, 300 * (i + 1)); });
  }
  throw lastError || new Error("failed " + path);
}

self.addEventListener("install", function (event) {
  event.waitUntil((async function () {
    const cache = await caches.open(CACHE);
    for (const path of PRECACHE) {
      if (await cache.match(path)) {
        continue;
      }
      const response = await fetchWithRetry(path, 3);
      await cache.put(path, response);
    }
  })());
});

async function cacheStatus() {
  const cache = await caches.open(CACHE);
  let cached = 0;
  for (const path of PRECACHE) {
    if (await cache.match(path)) {
      cached++;
    }
  }
  return { version: VERSION, cached: cached, total: PRECACHE.length };
}

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
  if (!event.data) {
    return;
  }
  if (event.data.type === "SKIP_WAITING") {
    self.skipWaiting();
  } else if (event.data.type === "STATUS" && event.ports && event.ports[0]) {
    cacheStatus().then(function (status) {
      event.ports[0].postMessage(status);
    }, function (error) {
      event.ports[0].postMessage({ version: VERSION, cached: 0, total: PRECACHE.length, error: String(error) });
    });
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
  /* Assets are referenced with a content hash (?v=...), so a cached copy never goes stale. */
  event.respondWith(
    caches.open(CACHE).then(function (cache) {
      return cache.match(request).then(function (cached) {
        if (cached) {
          return cached;
        }
        return cache.match(request, { ignoreSearch: true }).then(function (similar) {
          return fetch(request).then(function (response) {
            if (response && response.ok) {
              cache.put(request, response.clone());
            }
            return response;
          }).catch(function (error) {
            if (similar) {
              return similar;  /* offline: an older version is better than a broken image */
            }
            throw error;
          });
        });
      });
    })
  );
});
