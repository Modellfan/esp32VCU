#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <Arduino.h>

void webinterfaceBegin();
void webinterfaceHandle();
const char *webinterfaceApSsid();
IPAddress webinterfaceApIp();

#endif
