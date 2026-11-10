// Service: keeps charging parameters, fan mode and acceleration best times over a restart
// (VCU-REQ-020). Storage: persistent_data_storage.h on the AT24C EEPROM.
#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdint.h>

// setup(), after eepromBegin(): loads the newest record and applies it to params.
void persistenceBegin();
// Web task: every second, saves a new record when one of the values changed.
void persistenceService(uint32_t nowMs);

#endif
