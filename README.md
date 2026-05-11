# BQ24298 Charger Library v3.0.1

Arduino library for the Texas Instruments BQ24298 single-cell Li-Ion/Li-Polymer charger and power-path management IC.

## Features

- I2C-based control via external I2C_Functions dependency
- Buck/boost and OTG control
- Charge current/voltage configuration
- Input current/voltage limit configuration
- Watchdog, thermal and fault handling helpers
- Register definitions centralized in Definitions.h

## Dependencies

- I2C_Functions library

## Installation

### Arduino Library Manager

Search for BQ24298 and install.

### PlatformIO

Add to lib_deps:

```ini
akkoyun/BQ24298@^3.0.1
```

## Quick Start

```cpp
#include <BQ24298.h>

BQ24298 Charger(true, 1); // multiplexer enabled, channel 1

void setup() {
  Serial.begin(115200);


  BQ24298_Error result = Charger.Begin();
  if (result != BQ24298_Error::SUCCESS) {
    Serial.print("Init failed, code: ");
    Serial.println((uint8_t)result);
    while (1) {}
  }

  Charger.Set_Charge_Current(2.048);
  Charger.Set_Charge_Voltage(4.208);
}

void loop() {
  Serial.print("VBUS: ");
  Serial.println(Charger.VBUS_STAT());
  delay(1000);
}
```

## API Notes

- Begin returns BQ24298_Error.
- SUCCESS means initialization completed.
- INVALID_VERSION means unexpected device ID.
- DEVICE_NOT_FOUND means I2C read failed during identification.

## Examples

- examples/BatteryCharger
- examples/OTG_BoostMode
- examples/ThermalManagement
- examples/LowPowerErrorHandling
- examples/MultiChargerManagement

## License

MIT
