#include <Arduino.h>

// =====================================================
//               #define Konfiguration
// =====================================================

// Referenzmodus
#define REF_MODE_MAX_POWER      1   // Referenzspannung fix (z.B. Ladeschlussspannung)
#define REF_MODE_CURRENT_VOLT   2   // Referenzspannung = aktuelle Batteriespannung

// --- Fixe Referenzspannung (nur bei MAX_POWER)
#define REF_VOLTAGE_V           403.0f

// --- "Absolute" Maximal erlaubte Ströme (positiv angeben!)
#define MAX_DRIVE_CURRENT_A     220.0f
#define MAX_REGEN_CURRENT_A     90.0f

// --- Dynamische Limits (positiv angeben!)
// Für MAX_POWER-Referenzmodus
#define DYN_DRIVE_CURRENT_A_MP  180.0f
#define DYN_REGEN_CURRENT_A_MP  70.0f

// Für CURRENT_VOLT-Referenzmodus: eigene dyn-Limits
#define DYN_DRIVE_CURRENT_A_CV  160.0f
#define DYN_REGEN_CURRENT_A_CV  60.0f

// --- Nadelwerte (Skala frei: PWM 0..255, DAC, ...)
#define NEEDLE_REVERSE_VALUE    20
#define NEEDLE_NEUTRAL_VALUE    40
#define NEEDLE_OFFSET_VALUE     80
#define NEEDLE_DRIVE_MAX_VALUE  230
#define NEEDLE_REGEN_MIN_VALUE  30

#define NEEDLE_DO_CLAMP         1
#define NEEDLE_MIN_LIMIT        0
#define NEEDLE_MAX_LIMIT        255

// =====================================================
//                    Hilfsfunktionen
// =====================================================

static inline float clampf(float x, float lo, float hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}
static inline int clampi(int x, int lo, int hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}
static inline float lerpf(float a, float b, float t) {
  return a + (b - a) * t;
}

// =====================================================
// 1) Prozent bezogen auf MAX-Referenz
//    Rückgabe: signed percent in [-1..+1]
// =====================================================
float powerPercentMax(float battVoltage_V,
                      float motorCurrent_A,
                      uint8_t refMode) {
  const bool isRegen = (motorCurrent_A < 0.0f);

  const float vRef = (refMode == REF_MODE_MAX_POWER) ? (float)REF_VOLTAGE_V : battVoltage_V;
  const float iMax = isRegen ? (float)MAX_REGEN_CURRENT_A : (float)MAX_DRIVE_CURRENT_A;

  const float pRef_W = vRef * iMax;
  if (pRef_W <= 1e-6f) return 0.0f;

  const float pAbs_W = fabs(battVoltage_V * motorCurrent_A);
  float percent = pAbs_W / pRef_W;
  percent = clampf(percent, 0.0f, 1.0f);

  return isRegen ? -percent : percent;
}

// =====================================================
// 2) Prozent bezogen auf DYN-Referenz
//    (bei CURRENT_VOLT gibt es separate dyn-drive/dyn-regen limits)
//    Rückgabe: signed percent in [-1..+1]
// =====================================================
float powerPercentDyn(float battVoltage_V,
                      float motorCurrent_A,
                      uint8_t refMode) {
  const bool isRegen = (motorCurrent_A < 0.0f);

  const float vRef = (refMode == REF_MODE_MAX_POWER) ? (float)REF_VOLTAGE_V : battVoltage_V;

  float iDyn;
  if (refMode == REF_MODE_MAX_POWER) {
    iDyn = isRegen ? (float)DYN_REGEN_CURRENT_A_MP : (float)DYN_DRIVE_CURRENT_A_MP;
  } else { // REF_MODE_CURRENT_VOLT
    iDyn = isRegen ? (float)DYN_REGEN_CURRENT_A_CV : (float)DYN_DRIVE_CURRENT_A_CV;
  }

  const float pRef_W = vRef * iDyn;
  if (pRef_W <= 1e-6f) return 0.0f;

  const float pAbs_W = fabs(battVoltage_V * motorCurrent_A);
  float percent = pAbs_W / pRef_W;
  percent = clampf(percent, 0.0f, 1.0f);

  return isRegen ? -percent : percent;
}

// =====================================================
// 3) Nadelposition aus signed Prozent berechnen
//    dir: -1 rückwärts, 0 neutral, +1 vorwärts
// =====================================================
int needleFromPercent(int8_t dir, float percentSigned) {
  if (dir < 0) return NEEDLE_REVERSE_VALUE;
  if (dir == 0) return NEEDLE_NEUTRAL_VALUE;

  const float p = clampf(percentSigned, -1.0f, 1.0f);

  if (fabs(p) < 1e-6f) {
    int v = NEEDLE_OFFSET_VALUE;
#if (NEEDLE_DO_CLAMP == 1)
    v = clampi(v, NEEDLE_MIN_LIMIT, NEEDLE_MAX_LIMIT);
#endif
    return v;
  }

  int needle;
  if (p > 0.0f) {
    float n = lerpf((float)NEEDLE_OFFSET_VALUE, (float)NEEDLE_DRIVE_MAX_VALUE, p);
    needle = (int)(n + 0.5f);
  } else {
    float t = -p; // 0..1
    float n = lerpf((float)NEEDLE_OFFSET_VALUE, (float)NEEDLE_REGEN_MIN_VALUE, t);
    needle = (int)(n + 0.5f);
  }

#if (NEEDLE_DO_CLAMP == 1)
  needle = clampi(needle, NEEDLE_MIN_LIMIT, NEEDLE_MAX_LIMIT);
#endif
  return needle;
}
