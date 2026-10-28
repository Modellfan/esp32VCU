// Diagnostic log on the native USB port (USB-Serial-JTAG). UART0 (Serial) carries only
// GVRET binary data, so log text never corrupts the SavvyCAN stream (proposal 8).
#ifndef VCU_LOG_H
#define VCU_LOG_H

void vcuLogBegin();
void vcuLogf(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif
