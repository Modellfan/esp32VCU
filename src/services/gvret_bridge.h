// Service: GVRET (SavvyCAN) over UART0. The receive loop and the ECU task only queue frames;
// the service task writes them to the serial port, so a slow host never blocks CAN timing.
#ifndef GVRET_BRIDGE_H
#define GVRET_BRIDGE_H

#include <ACAN2515.h>

// Bus numbers in SavvyCAN: 0-3 = CAN0-CAN3, 4 = frames sent by the VCU (ECU emulation).
constexpr int GVRET_BUS_CAN0 = 0;  // battery / BMS (TWAI)
constexpr int GVRET_BUS_CAN1 = 1;  // vehicle
constexpr int GVRET_BUS_CAN2 = 2;  // Tesla SDU
constexpr int GVRET_BUS_CAN3 = 3;  // spare
constexpr int GVRET_BUS_TX = 4;    // sent by the VCU
constexpr int GVRET_BUS_COUNT = 5;

void gvretBridgeBegin();
// CAN task side: non-blocking, drops the frame when the queue is full.
void gvretBridgeObserve(const CANMessage &frame, int bus);
// Service side: handle host commands and forward queued frames.
void gvretBridgeService();
uint32_t gvretBridgeDropped();
// true once the host switched GVRET to binary output (0xE7); the text log is silent then.
bool gvretBridgeBinary();

#endif
