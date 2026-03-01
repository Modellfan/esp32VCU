#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <SPI.h>
#include <ACAN2515.h>

extern ACAN2515 can;
extern ACAN2515 can2;

void canManager_setup();

#endif // CANMANAGER_H
