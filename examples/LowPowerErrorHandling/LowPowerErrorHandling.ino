/*
 * BQ24298 Low Power Mode & Error Handling Example
 * 
 * This example demonstrates:
 * - Comprehensive error handling using error codes
 * - Low-power sleep mode configuration
 * - Watchdog timer management for long-term operation
 * - Safe shutdown and recovery procedures
 * 
 * Ultra-optimized for minimal power consumption and reliable long-term operation
 */

#include <BQ24298.h>

// Create Charger Object
BQ24298 Charger(true, 1);

// Error handling flags
volatile bool g_charger_error = false;
volatile uint8_t g_last_error_code = 0;

void setup() {
	Serial.begin(115200);
	delay(1000);
	
	Serial.println(F("\n=== BQ24298 Low Power & Error Handling ===\n"));

	// ============ Initialization with Error Handling ============
	
	BQ24298_Error init_result = Charger.Begin();
	
	if (init_result != BQ24298_Error::SUCCESS) {
		handle_initialization_error(init_result);
		// Don't proceed if initialization failed
		while (1) {
			delay(5000);
			Serial.println(F("Retrying initialization..."));
			init_result = Charger.Begin();
			if (init_result == BQ24298_Error::SUCCESS) break;
		}
	}

	Serial.println(F("✓ Charger initialized successfully\n"));

	// ============ Low Power Configuration ============
	
	// Set minimal charge current for low-power operation
	if (!Charger.Set_Charge_Current(0.512)) {
		Serial.println(F("✗ Failed to set charge current"));
	} else {
		Serial.println(F("✓ Charge current set to 512mA (low power)"));
	}

	// Set charge voltage to nominal Li-Ion
	if (!Charger.Set_Charge_Voltage(4.208)) {
		Serial.println(F("✗ Failed to set charge voltage"));
	} else {
		Serial.println(F("✓ Charge voltage set to 4.208V"));
	}

	// Enable Watchdog for safety (prevents runaway charging)
	// Options: 0=disabled, 1=40s, 2=80s, 3=160s
	if (!Charger.Set_Watchdog(2)) {  // 80 second timeout
		Serial.println(F("✗ Failed to set watchdog timer"));
	} else {
		Serial.println(F("✓ Watchdog enabled: 80s timeout"));
	}

	// Disable unnecessary features for power saving
	Charger.Disable_DPDM();  // Dynamic Power Path Management off
	Serial.println(F("✓ DPDM disabled (power saving)"));

	delay(2000);
}

void loop() {
	static unsigned long last_update = 0;
	static unsigned long last_watchdog_reset = 0;
	static uint8_t error_count = 0;

	unsigned long current_time = millis();

	// ============ Periodic Status Check (Every 1 second) ============
	if (current_time - last_update >= 1000) {
		last_update = current_time;

		// Query charger status
		if (!perform_safe_status_check()) {
			error_count++;
			if (error_count > 3) {
				Serial.println(F("\n✗ Multiple errors detected - entering recovery mode"));
				enter_recovery_mode();
				error_count = 0;
			}
		} else {
			error_count = 0;  // Reset error counter on success
		}
	}

	// ============ Watchdog Reset (Every 60 seconds) ============
	if (current_time - last_watchdog_reset >= 60000) {
		last_watchdog_reset = current_time;
		
		if (!Charger.Reset_Watchdog()) {
			Serial.println(F("⚠ Warning: Watchdog reset failed"));
		} else {
			Serial.println(F("✓ Watchdog reset"));
		}
	}

	delay(500);
}

/**
 * @brief Perform safe status check with error handling
 * @return true if status check successful, false on error
 */
bool perform_safe_status_check() {
	static unsigned long check_count = 0;
	static unsigned long last_print = 0;

	check_count++;

	// Only print status every 5 checks (~5 seconds)
	if (millis() - last_print < 5000) {
		return true;
	}

	last_print = millis();

	Serial.print(F("\n[Check #"));
	Serial.print(check_count);
	Serial.println(F("]"));

	// ============ Power Good Status ============
	bool pg_stat = Charger.PG_STAT();
	Serial.print(F("Power Good          : "));
	Serial.println(pg_stat ? F("✓ YES") : F("✗ NO"));

	// ============ Thermal Status ============
	bool thermal = Charger.THERM_STAT();
	if (thermal) {
		Serial.println(F("✗ THERMAL SHUTDOWN ACTIVE"));
		return false;
	}

	// ============ Fault Detection ============
	uint8_t faults = Charger.Get_Fault_Register();
	if (faults != 0) {
		Serial.print(F("Faults Detected     : 0x"));
		Serial.println(faults, HEX);
		
		// Detailed fault analysis
		if (Charger.WATCHDOG_FAULT()) {
			Serial.println(F("  → Watchdog expired (communication timeout)"));
			g_last_error_code = (uint8_t)BQ24298_Error::WATCHDOG_EXPIRED;
			return false;
		}
		if (Charger.BAT_OVP_FAULT()) {
			Serial.println(F("  → Battery over-voltage protection triggered"));
			g_last_error_code = (uint8_t)BQ24298_Error::BATTERY_OVP;
			return false;
		}
		if (Charger.NTC_FAULT()) {
			Serial.println(F("  → NTC thermistor fault"));
			g_last_error_code = (uint8_t)BQ24298_Error::NTC_FAULT;
			return false;
		}
	} else {
		Serial.println(F("Faults              : ✓ NONE"));
	}

	// ============ Charging Status ============
	uint8_t chrg_status = Charger.Charge_Status();
	Serial.print(F("Charging Status     : "));
	switch (chrg_status) {
		case 0: Serial.println(F("Not charging")); break;
		case 1: Serial.println(F("Pre-charge")); break;
		case 2: Serial.println(F("Fast charge")); break;
		case 3: Serial.println(F("Charge termination")); break;
		default: Serial.println(F("Unknown")); return false;
	}

	// ============ Input Source Status ============
	uint8_t vbus = Charger.VBUS_STAT();
	Serial.print(F("Input Source        : "));
	switch (vbus) {
		case 0: Serial.println(F("No input")); break;
		case 1: Serial.println(F("USB (5V)")); break;
		case 2: Serial.println(F("Adapter (>5V)")); break;
		case 3: Serial.println(F("OTG mode")); break;
		default: Serial.println(F("Unknown")); return false;
	}

	// ============ Voltage/Current Monitoring ============
	Serial.print(F("Charge Voltage      : "));
	Serial.print(Charger.Get_Charge_Voltage(), 3);
	Serial.println(F("V"));

	Serial.print(F("Charge Current      : "));
	Serial.print(Charger.Get_Charge_Current(), 3);
	Serial.println(F("A"));

	// ============ System Voltage Status ============
	if (!Charger.VSYS_STAT()) {
		Serial.println(F("System Voltage      : ✓ Good (>VSYSMIN)"));
	} else {
		Serial.println(F("System Voltage      : ⚠ Low (<VSYSMIN)"));
	}

	return true;  // All checks passed
}

/**
 * @brief Enter recovery mode after detecting errors
 */
void enter_recovery_mode() {
	Serial.println(F("\n=== ENTERING RECOVERY MODE ===\n"));

	// Disable all charging
	Charger.Disable_Charge();
	Serial.println(F("✓ Charging disabled"));

	// Wait 5 seconds
	delay(5000);

	// Attempt reinitialization
	Serial.println(F("Attempting re-initialization..."));
	BQ24298_Error result = Charger.Begin();

	if (result == BQ24298_Error::SUCCESS) {
		Serial.println(F("✓ Recovery successful - resuming normal operation"));
	} else {
		Serial.print(F("✗ Recovery failed - error code: "));
		Serial.println((uint8_t)result);
		
		// Detailed error explanation
		print_error_details(result);
		
		// Stay in recovery attempt loop
		while (true) {
			Serial.println(F("Retrying in 10 seconds..."));
			delay(10000);
			result = Charger.Begin();
			if (result == BQ24298_Error::SUCCESS) break;
		}
	}

	Serial.println(F("\n=== EXITING RECOVERY MODE ===\n"));
}

/**
 * @brief Print detailed error information
 */
void print_error_details(BQ24298_Error error) {
	Serial.print(F("Error Details: "));
	
	switch (error) {
		case BQ24298_Error::SUCCESS:
			Serial.println(F("No error"));
			break;
		case BQ24298_Error::DEVICE_NOT_FOUND:
			Serial.println(F("Device not detected on I2C bus"));
			Serial.println(F("  → Check I2C connections, I2C address, pull-ups"));
			break;
		case BQ24298_Error::INVALID_VERSION:
			Serial.println(F("Device version mismatch"));
			Serial.println(F("  → Expected 0x24, verify IC part number"));
			break;
		case BQ24298_Error::INVALID_PARAMETER:
			Serial.println(F("Invalid parameter range"));
			break;
		case BQ24298_Error::I2C_ERROR:
			Serial.println(F("I2C communication error"));
			Serial.println(F("  → Check I2C bus, reduce clock speed"));
			break;
		case BQ24298_Error::REGISTER_READ_FAILED:
			Serial.println(F("Register read failed"));
			break;
		case BQ24298_Error::REGISTER_WRITE_FAILED:
			Serial.println(F("Register write failed"));
			break;
		case BQ24298_Error::INITIALIZATION_FAILED:
			Serial.println(F("Initialization sequence failed"));
			break;
		case BQ24298_Error::THERMAL_FAULT:
			Serial.println(F("Thermal shutdown active"));
			break;
		case BQ24298_Error::BATTERY_OVP:
			Serial.println(F("Battery over-voltage detected"));
			break;
		case BQ24298_Error::WATCHDOG_EXPIRED:
			Serial.println(F("Watchdog timer expired"));
			break;
		case BQ24298_Error::NTC_FAULT:
			Serial.println(F("NTC thermistor fault"));
			break;
		default:
			Serial.println(F("Unknown error"));
	}
}

/**
 * @brief Handle initialization errors gracefully
 */
void handle_initialization_error(BQ24298_Error error) {
	Serial.println(F("\n✗ INITIALIZATION FAILED\n"));
	print_error_details(error);
	Serial.println();
}
