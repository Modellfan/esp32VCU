# CAN Signal Reference (User Provided)

## CAN ID 0x280  Engine_1 (ECU)

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Engine Speed (RPM) | 16 | 16 | Intel | U16 | 0.25 | 0 | rpm | DBC, More infos, Teensy | All sources agree |
| Throttle Position | 40 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | - |
| Pedal Position 1 | 32 | 8 | Intel | U8 | 1 | - | % | DBC | Not used in Teensy |
| Pedal Position 2 | 56 | 8 | Intel | U8 | 1 | - | % | DBC | Not used in Teensy |

## CAN ID 0x289  EngineTemp (ECU)

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Coolant Temperature | 8 | 8 | Intel | U8 | 0.75 | -48 | degC | DBC, More infos | Physical ECU value |
| Coolant Level Switch | 16 | 1 | Intel | Bool | 1 | 0 | - | DBC | - |
| Cruise Control Active | 22 | 1 | Intel | Bool | 1 | 0 | - | DBC | - |

Note: Instrument cluster temperature gauge uses non-linear / modified scaling (display behavior).

## CAN ID 0x298  Engine_2 (ECU)

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Inlet Air Temperature | 8 | 8 | Intel | U8 | 0.75 | -48 | degC | DBC, More infos | - |
| Throttle Position 1 | 16 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | Turbo? Unsure |
| Throttle Position 2 | 32 | 8 | Intel | U8 | 0.390625 | 0 | % | DBC, More infos | Turbo? Unsure |
| Barometric Pressure | 40 | 8 | Intel | U8 | 5 | 0 | mbar | More infos | Turbo? Unsure |
| Vehicle Speed | 56 | 8 | Intel | U8 | ? | 0 | ? | More infos | Scaling unknown |

## CAN ID 0x2A8  Vehicle Speed (ABS / PSM)

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Wheel Speed FL | 1 | 15 | Intel | U15 | 0.01 | 0 | km/h | More infos | Legacy definition |
| Wheel Speed FR | 17 | 15 | Intel | U15 | 0.01 | 0 | km/h | More infos | Legacy definition |
| Wheel Speed RL | 33 | 15 | Intel | U15 | 0.01 | 0 | km/h | More infos | Legacy definition |
| Wheel Speed RR | 49 | 15 | Intel | U15 | 0.01 | 0 | km/h | More infos | Legacy definition |
| Wheel Speed FL | 0 | 16 | Intel | U16 | 0.005218 | -1.6338 | km/h | DBC | Newer ABS version |
| Wheel Speed FR | 16 | 16 | Intel | U16 | 0.005218 | -1.6338 | km/h | DBC | Newer ABS version |
| Wheel Speed RL | 32 | 16 | Intel | U16 | 0.005218 | -1.6338 | km/h | DBC | Newer ABS version |
| Wheel Speed RR | 48 | 16 | Intel | U16 | 0.005218 | -1.6338 | km/h | DBC | Newer ABS version |

Likely dependent on ABS generation.

## CAN ID 0x1A0  ABS_1 (ABS / PSM)

| Signal | Start | Len | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| TCS Intervention | 0 | 1 | Bool | 1 | 0 | - | DBC, More infos | - |
| MSR Request | 1 | 1 | Bool | 1 | 0 | - | DBC | - |
| ABS Intervention | 2 | 1 | Bool | 1 | 0 | - | DBC, More infos | - |
| ABD Intervention | 3 | 1 | Bool | 1 | 0 | - | DBC, More infos | - |
| FDR Intervention | 4 | 1 | Bool | 1 | 0 | - | DBC, More infos | - |
| ASR Control Mode | 5 | 2 | Enum | - | - | - | More infos | Shift / downshift / inhibit |
| ABS Warning Lamp | 8 | 1 | Bool | - | - | - | More infos | Offset unknown |
| Brake Warning Lamp | 10 | 1 | Bool | - | - | - | More infos | Offset unknown |
| Brake Switch | 11 | 1 | Bool | 1 | 0 | - | More infos | - |
| Brake Switch (Inverted?) | 12 | 1 | Bool | 1 | 0 | - | More infos | Possibly inverted |
| Handbrake Switch | 15 | 1 | Bool | 1 | 0 | - | More infos | - |
| Vehicle Reference Speed | 17 | 16 | U16 | 0.01 | 0 | km/h | More infos | Tire circumference dependent |
| TCS Intervention Slow | 32 | 8 | U8 | 0.39 | 0 | % | DBC, More infos | - |
| TCS Intervention Fast | 40 | 8 | U8 | 0.39 | 0 | % | DBC, More infos | - |
| Intervention Torque | 48 | 8 | U8 | 0.39 | 0 | % | More infos | Conflicts with Lateral G |
| Lateral Acceleration (FDR) | 48 | 8 | U8 | 0.01 | -1.27 | g | More infos | FDR only |

Note: Start bit 48 is reused depending on system variant.

## CAN ID 0x4E0  EngineStat (ECU)

| Signal | Start | Len | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Check Engine Light | 0 | 1 | Bool | 1 | 0 | - | DBC, More infos | Also bit 1 |
| Reduced Power | 3 | 1 | Bool | 1 | 0 | - | DBC, More infos | - |
| Fan Error | 4 | 1 | Bool | 1 | 0 | - | DBC, More infos | - |
| Fuel Used | 16 | 16 | U16 | 10 | 0 | - | DBC | Scaling unclear |
| Boost Pressure | 32 | 8 | U8 | 10 | 0 | - | More infos | Turbo only |
| Oil Temperature | 40 | 8 | U8 | 0.75 | -48 | degC | DBC, More infos | - |

## CAN ID 0x520  Instrument Cluster 2

| Signal | Start | Len | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Ambient Temperature | 24 | 8 | U8 | 0.5 | -40 | degC | DBC, More infos | - |
| Oil Pressure | 32 | 8 | U8 | 0.04 | - | bar | DBC, More infos | - |
| Light Dimmer | 8 | 8 | U8 | 1 | 0 | - | DBC | - |
| Counter / Unknown | 40 | 8 | U8 | 1 | 0 | - | More infos | Unknown purpose |

## CAN ID 0x00C0  Steering Angle Sensor

| Signal | Start | Len | Endian | Type | Factor | Offset | Unit | Source | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Steering Angle | 4 | 12 | Intel | Signed | 0.390625 | 0 | deg | More infos | Older definition |
| Steering Angle | 0 | 16 | Intel | I16 | 0.0773 | -1.1668 | deg | DBC | Higher resolution |
