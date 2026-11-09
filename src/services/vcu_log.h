// Diagnostic text log on UART0 (COM10), shared with GVRET: text only while GVRET is not
// in binary mode (VCU-DEC-010).
#ifndef VCU_LOG_H
#define VCU_LOG_H

void vcuLogBegin();
void vcuLogf(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif
