#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <SPI.h>
#include <ACAN2515.h>

extern ACAN2515 can;   // CAN 1: vehicle (Porsche 986)
extern ACAN2515 can2;  // CAN 2: drive (Tesla SDU / OpenInverter)

void canManager_setup();
// 0 = initialised; anything else is the ACAN2515 error code (0xFFFF = not started).
uint16_t canManager_error(uint8_t bus);

#endif // CANMANAGER_H
