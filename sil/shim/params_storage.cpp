// SIL: instances of the signal groups that the firmware defines inside modules that are not
// part of the SIL build (CAN codecs, battery codec, fuel gauge driver with the X9C103S library).
// Components are tested on the application level only, with their inputs and outputs set and
// read as parameters.
#include "986_ecu_messages.h"
#include "986_vehicle_messages.h"
#include "tesla_sdu_messages.h"
#include "battery_messages.h"
#include "digipot_control.h"

namespace params {
EcuSignals ecu;
EcuOverrides ecu_override;
VehicleSignals vehicle;
TeslaSduSignals tesla_sdu;
BatterySignals battery;
ChargingSignals charging;
DigipotSignals digipot;
}  // namespace params
