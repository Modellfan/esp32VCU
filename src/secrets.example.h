// Template for src/secrets.h (VCU-REQ-015). Copy to secrets.h and fill in.
// secrets.h is ignored by git; never put real credentials into this template.
#ifndef VCU_SECRETS_H
#define VCU_SECRETS_H

// Home WiFi the VCU joins when in range (optional; empty string disables it).
#define VCU_WIFI_HOME_SSID ""
#define VCU_WIFI_HOME_PASSWORD ""

// Access point of the VCU in the car (always on). WPA2 needs at least 8 characters.
#define VCU_WIFI_AP_SSID "eboxster"
#define VCU_WIFI_AP_PASSWORD "change-me-please"

#endif
