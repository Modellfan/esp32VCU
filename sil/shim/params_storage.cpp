// SIL: instances of the signal groups that the firmware defines inside the CAN codec
// modules (986_ecu_messages.cpp, 986_vehicle_messages.cpp, tesla_sdu_messages.cpp).
// The codecs are not part of the SIL build: components are tested on the application
// level only, with their inputs and outputs set and read as parameters.
#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "tesla_sdu_messages.h"

namespace params {
EcuSignals ecu;
VehicleSignals vehicle;
TeslaSduSignals tesla_sdu;
}  // namespace params
