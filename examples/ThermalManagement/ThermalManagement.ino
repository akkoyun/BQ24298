/*
 * BQ24298 Thermal Management Example
 * 
 * This example demonstrates thermal monitoring and management:
 * - Continuous temperature monitoring via NTC thermistor
 * - Dynamic charge current/voltage adjustment based on temperature
 * - Over-temperature fault detection and handling
 * - Thermal regulation configuration
 * 
 * Typical NTC values: 10kΩ @ 25°C (Beta 3470K for typical components)
 */

#include <BQ24298.h>

// Create Charger Object
BQ24298 Charger(true, 1);

// Thermal thresholds (Celsius)
#define TEMP_COLD       0    // Precharge region (0°C - 10°C)
#define TEMP_COOL      10    // Reduced charge current
#define TEMP_NORMAL    25    // Nominal operation
#define TEMP_WARM      40    // Thermal limiting begins
#define TEMP_HOT       55    // Aggressive current reduction
#define TEMP_CRITICAL  60    // Thermal shutdown (device-specific)
#define TEMP_EMERGENCY 120   // Emergency stop

// Charge current levels for different temperature ranges (in Amps)
#define I_PRECHARGE   0.256  // Cold battery pre-charge
#define I_REDUCED     1.0    // Reduced current mode
#define I_NORMAL      2.048  // Full charge current
#define I_THERMAL     0.5    // Thermal limiting mode
#define I_EMERGENCY   0.256  // Emergency mode only

void setup() {
	Serial.begin(115200);
	delay(1000);
	
	Serial.println(F("\n=== BQ24298 Thermal Management Example ===\n"));

	// Initialize Charger
	BQ24298_Error error = Charger.Begin();
	if (error != BQ24298_Error::SUCCESS) {
		Serial.print(F("ERROR: Initialization failed: "));
		Serial.println((uint8_t)error);
		while (1);
	}

	Serial.println(F("✓ Charger initialized\n"));

	// Configure Thermal Regulation Threshold
	// Options: 60°C, 80°C, 100°C, 120°C
	if (Charger.Set_Thermal_Regulation_Temperature(80)) {
		Serial.println(F("✓ Thermal regulation threshold: 80°C"));
	}

	// Enable Battery Fault Interrupts (optional)
	Charger.Enable_BatFault_INT();

	delay(1000);
}

void loop() {
	Serial.print(F("\n--- Thermal Monitoring ("));
	Serial.print(millis() / 1000);
	Serial.println(F("s) ---"));

	// ============ Thermal Status Check ============
	
	// Check if thermal sensor is active
	if (Charger.THERM_STAT()) {
		Serial.println(F("⚠ WARNING: Thermal shutdown active!"));
		Serial.println(F("  → Disabling charge until cool-down"));
		Charger.Disable_Charge();
		delay(5000);
		return;
	}

	// Check NTC (thermistor) fault
	if (Charger.NTC_FAULT()) {
		Serial.println(F("✗ ERROR: NTC thermistor fault detected!"));
		Serial.println(F("  → Check thermistor connection/resistance"));
		Charger.Disable_Charge();
		delay(5000);
		return;
	}

	// Check battery cold condition
	if (Charger.BCOLD()) {
		Serial.println(F("ℹ Battery cold (<10°C detected)"));
		Serial.println(F("  → Applying pre-charge current"));
		Charger.Set_PreCharge_Current(I_PRECHARGE);
		Charger.Enable_Charge();
	} else {
		Serial.println(F("✓ Battery temperature: OK"));
	}

	// Check battery low voltage
	if (Charger.BATLOWV()) {
		Serial.println(F("ℹ Battery low voltage: <2.8V"));
		Charger.Set_PreCharge_Current(I_PRECHARGE);
	}

	// ============ Display Thermal Info ============

	Serial.print(F("\nThermal Regulation Temp: "));
	Serial.print(Charger.Get_Thermal_Regulation_Temperature());
	Serial.println(F("°C"));

	Serial.print(F("Charge Current: "));
	Serial.print(Charger.Get_Charge_Current(), 3);
	Serial.println(F("A"));

	Serial.print(F("Charge Voltage: "));
	Serial.print(Charger.Get_Charge_Voltage(), 3);
	Serial.println(F("V"));

	// ============ Adaptive Charging Based on Temperature ============
	// (Requires external temperature sensor or estimation)
	// This is a placeholder for temperature-based logic
	
	/*
	float battery_temperature = read_external_temp_sensor();
	
	if (battery_temperature < TEMP_COLD) {
		// Disable charging in extreme cold
		Charger.Disable_Charge();
		Serial.println(F("Charging disabled: Temperature too low"));
	} 
	else if (battery_temperature < TEMP_COOL) {
		// Pre-charge only
		Charger.Set_Charge_Current(I_PRECHARGE);
		Serial.println(F("Pre-charge mode: Cold temperature"));
	}
	else if (battery_temperature < TEMP_WARM) {
		// Normal charging
		Charger.Set_Charge_Current(I_NORMAL);
		Serial.println(F("Charge mode: Normal"));
	}
	else if (battery_temperature < TEMP_HOT) {
		// Thermal limiting
		Charger.Set_Charge_Current(I_THERMAL);
		Serial.println(F("Charge mode: Thermal limiting"));
	}
	else if (battery_temperature < TEMP_CRITICAL) {
		// Emergency mode
		Charger.Set_Charge_Current(I_EMERGENCY);
		Serial.println(F("Charge mode: Emergency (high temp)"));
	}
	else {
		// Thermal shutdown
		Charger.Disable_Charge();
		Serial.println(F("Charging disabled: Temperature critical!"));
	}
	*/

	// ============ Fault Monitoring ============
	
	uint8_t faults = Charger.Get_Fault_Register();
	if (faults != 0) {
		Serial.println(F("\n⚠ Faults detected:"));
		if (Charger.WATCHDOG_FAULT()) {
			Serial.println(F("  - Watchdog timer expired"));
			Charger.Reset_Watchdog();
		}
		if (Charger.BAT_OVP_FAULT()) {
			Serial.println(F("  - Battery over-voltage protection"));
		}
	}

	// Update watchdog timer periodically
	Charger.Reset_Watchdog();

	delay(3000);
}

// Placeholder for external temperature sensor reading
/*
float read_external_temp_sensor() {
	// Example: Read from analog sensor on A0
	// int raw = analogRead(A0);
	// float temp = (raw / 1024.0) * 125.0 - 40; // For TMP36 sensor
	// return temp;
	return 25.0;
}
*/
