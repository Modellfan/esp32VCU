// Service: GVRET (SavvyCAN) over UART0. The CAN task only queues frames;
// the service task writes them to the serial port, so a slow host never blocks CAN timing.
#ifndef GVRET_BRIDGE_H
#define GVRET_BRIDGE_H

#include <ACAN2515.h>

constexpr int GVRET_BUS_TX = 0;  // frames sent by the VCU (ECU emulation)

void gvretBridgeBegin();
// CAN task side: non-blocking, drops the frame when the queue is full.
void gvretBridgeObserve(const CANMessage &frame, int bus);
// Service side: handle host commands and forward queued frames.
void gvretBridgeService();
uint32_t gvretBridgeDropped();

#endif
