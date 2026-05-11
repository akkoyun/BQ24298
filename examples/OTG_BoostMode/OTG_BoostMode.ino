/*
 * BQ24298 OTG / Boost Mode Example
 * 
 * This example demonstrates how to enable OTG (On-The-Go) mode to use the BQ24298
 * as a power bank or portable charger. In OTG mode, the IC provides 5V output from
 * a Li-Ion battery to charge external devices.
 * 
 * Typical Output: 5V @ up to 500mA (depending on boost voltage/current settings)
 * Current Draw: ~100mA from battery (DC-DC converter efficiency ~80-90%)
 */

#include <BQ24298.h>

// Create Charger Object with I2C Multiplexer enabled on channel 1
BQ24298 Charger(true, 1);

void setup() {
	// Start Serial Communication
	Serial.begin(115200);
	delay(1000);
	
	Serial.println(F("\n=== BQ24298 OTG/Boost Mode Example ===\n"));

	// Initialize Charger
	BQ24298_Error error = Charger.Begin();
	if (error != BQ24298_Error::SUCCESS) {
		Serial.print(F("ERROR: Charger initialization failed with code: "));
		Serial.println((uint8_t)error);
		while (1); // Halt
	}

	Serial.println(F("✓ Charger initialized successfully"));
	
	// Enable OTG Mode (Power Bank Output)
	if (Charger.OTG(true)) {
		Serial.println(F("✓ OTG Mode ENABLED - Output 5V for external device charging"));
	} else {
		Serial.println(F("✗ Failed to enable OTG mode"));
	}

	// Optional: Configure boost voltage (4.20V to 5.51V in 64mV steps)
	// Default boost voltage should be around 4.95V-5.0V
	if (!Charger.Set_Boost_Voltage(5.0)) {
		Serial.println(F("✗ Boost voltage configuration failed"));
	}

	delay(1000);
}

void loop() {
	// Print diagnostic information every 2 seconds
	
	Serial.print(F("\n--- OTG Mode Status ---\n"));
	
	// Check if power is good
	if (Charger.PG_STAT()) {
		Serial.println(F("Power Status    : GOOD (5V available)"));
	} else {
		Serial.println(F("Power Status    : NOT GOOD (5V insufficient)"));
	}

	// Check thermal status
	if (Charger.THERM_STAT()) {
		Serial.println(F("⚠ Thermal Stat  : THERMAL SHUTDOWN"));
	} else {
		Serial.println(F("Thermal Stat    : NORMAL"));
	}

	// Check OTG mode status
	if (Charger.Get_OTG_State()) {
		Serial.println(F("OTG Mode        : ACTIVE"));
	} else {
		Serial.println(F("OTG Mode        : DISABLED"));
	}

	// Check DPM (Dynamic Power Management) status
	if (Charger.DPM_STAT()) {
		Serial.println(F("DPM Status      : ACTIVE (Current Limiting)"));
	} else {
		Serial.println(F("DPM Status      : PASSIVE"));
	}

	// Check VSYS status
	if (!Charger.VSYS_STAT()) {
		Serial.println(F("VSYS Status     : BATTERY GOOD (>VSYSMIN)"));
	} else {
		Serial.println(F("⚠ VSYS Status   : BATTERY CRITICAL (<VSYSMIN)"));
	}

	// Check for any faults
	uint8_t faults = Charger.Get_Fault_Register();
	if (faults != 0) {
		Serial.print(F("⚠ Faults Detected: 0x"));
		Serial.println(faults, HEX);
		
		if (Charger.WATCHDOG_FAULT()) Serial.println(F("  - Watchdog expired"));
		if (Charger.BAT_OVP_FAULT()) Serial.println(F("  - Battery OVP"));
		if (Charger.NTC_FAULT()) Serial.println(F("  - NTC thermistor fault"));
	} else {
		Serial.println(F("Faults          : NONE"));
	}

	// Display current boost voltage (if available)
	Serial.print(F("Boost Voltage   : "));
	Serial.print(Charger.Get_Boost_Voltage(), 3);
	Serial.println(F("V"));

	// Wait before next update
	delay(2000);
}
