#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <Arduino.h>

// Creates the service task on core 0 at the given priority. The task starts WiFi (access
// point + home network, non-blocking) and the HTTP server, then runs web, GVRET forwarding,
// the diagnostic log and the status LED.
void webinterfaceBegin(uint8_t priority);
// One text line with access point and home network state (serial command "wifi-status").
void webinterfaceLogWifiStatus();
// Serial command "wifi-scan": visible networks with RSSI, channel and auth mode (* = home).
void webinterfaceLogWifiScan();

#endif
