// AT24C I2C EEPROM on GPIO39 (SCL) / GPIO37 (SDA), library I2C_EEPROM (Rob Tillaart).
// Backend of the persistent data storage (persistent_data_storage.h).
#ifndef EEPROM_AT24C_H
#define EEPROM_AT24C_H

#include <stddef.h>
#include <stdint.h>

// setup(): starts I2C, finds the chip at 0x50..0x57 and its size (without writing).
bool eepromBegin();
bool eepromReady();
uint32_t eepromSize();  // bytes, 0 = not found

// Read / write a byte range; writes only changed bytes and verifies them.
bool eepromRead(uint32_t address, void *data, size_t length);
bool eepromWrite(uint32_t address, const void *data, size_t length);

// Serial command "eeprom-test": writes a pattern into the last 16 bytes, reads it back and
// restores the old content.
void eepromSelfTest();

#endif
