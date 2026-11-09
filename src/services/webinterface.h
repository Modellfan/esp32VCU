#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <Arduino.h>

// Starts WiFi (access point + home network, non-blocking), the HTTP server and the
// service task on core 0 (web, GVRET forwarding, status LED).
void webinterfaceBegin();
// One text line with access point and home network state (serial command "wifi-status").
void webinterfaceLogWifiStatus();

#endif
