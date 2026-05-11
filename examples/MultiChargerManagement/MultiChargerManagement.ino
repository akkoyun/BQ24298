/*
 * BQ24298 Multi-Charger Management Example
 * 
 * This example demonstrates managing multiple BQ24298 chargers on the same I2C bus
 * using multiplexer channels (up to 8 chargers per multiplexer).
 * 
 * Use case: Managing charging for multiple battery packs in:
 * - Portable power banks with multiple cells
 * - Multi-battery drone systems
 * - Solar charging systems with redundant chargers
 * 
 * Requires: I2C multiplexer (TCA9548A) connected to I2C bus
 */

#include <BQ24298.h>

// Define number of chargers and their multiplexer channels
#define NUM_CHARGERS 2
#define CHARGER_CH0  0
#define CHARGER_CH1  1

// Create array of charger objects on different multiplexer channels
BQ24298 Chargers[NUM_CHARGERS] = {
	BQ24298(true, CHARGER_CH0),  // Charger 0 on MUX channel 0
	BQ24298(true, CHARGER_CH1)   // Charger 1 on MUX channel 1
};

// Structure to track charger status
struct ChargerStatus {
	uint8_t id;
	BQ24298_Error last_error;
	uint8_t fault_register;
	float charge_voltage;
	float charge_current;
	uint8_t charge_status;
	bool is_healthy;
};

ChargerStatus charger_status[NUM_CHARGERS];

void setup() {
	Serial.begin(115200);
	delay(1000);
	
	Serial.println(F("\n=== BQ24298 Multi-Charger Management ===\n"));

	// Initialize all chargers
	Serial.println(F("Initializing chargers...\n"));

	for (uint8_t i = 0; i < NUM_CHARGERS; i++) {
		Serial.print(F("Charger "));
		Serial.print(i);
		Serial.print(F(": "));

		BQ24298_Error error = Chargers[i].Begin();
		charger_status[i].id = i;
		charger_status[i].last_error = error;
		charger_status[i].is_healthy = (error == BQ24298_Error::SUCCESS);

		if (error == BQ24298_Error::SUCCESS) {
			Serial.println(F("✓ OK"));
		} else {
			Serial.print(F("✗ ERROR: "));
			Serial.println((uint8_t)error);
		}

		delay(500);
	}

	Serial.println(F("\n✓ Initialization complete\n"));
	delay(1000);
}

void loop() {
	static unsigned long last_update = 0;
	unsigned long current_time = millis();

	// Update status every 3 seconds
	if (current_time - last_update < 3000) {
		delay(500);
		return;
	}

	last_update = current_time;

	// ============ Main Monitoring Loop ============

	Serial.print(F("\n=== Status Update ["));
	Serial.print(current_time / 1000);
	Serial.println(F("s] ===\n"));

	// Track system-wide status
	uint8_t healthy_count = 0;
	uint8_t error_count = 0;
	float total_charge_current = 0;
	float total_charge_voltage = 0;

	// ============ Poll Each Charger ============

	for (uint8_t i = 0; i < NUM_CHARGERS; i++) {
		Serial.print(F("┌─ CHARGER "));
		Serial.print(i);
		Serial.println(F(" ─────────────────────────┐"));

		// Get current status
		update_charger_status(i);

		// Display status
		print_charger_status(i);

		// Check for faults
		if (charger_status[i].fault_register != 0) {
			Serial.print(F("│ ⚠ Faults: 0x"));
			Serial.println(charger_status[i].fault_register, HEX);
			error_count++;
		}

		// Accumulate totals
		if (charger_status[i].is_healthy) {
			healthy_count++;
			total_charge_current += charger_status[i].charge_current;
			total_charge_voltage = max(total_charge_voltage, charger_status[i].charge_voltage);
		}

		Serial.println(F("└────────────────────────────────┘"));
		delay(100);
	}

	// ============ System-Wide Summary ============

	Serial.println(F("\n┏━━ SYSTEM SUMMARY ━━━━━━━━━━━━━┓"));
	Serial.print(F("┃ Healthy Chargers: "));
	Serial.print(healthy_count);
	Serial.print(F("/"));
	Serial.println(NUM_CHARGERS);

	Serial.print(F("┃ Error Count: "));
	Serial.println(error_count);

	Serial.print(F("┃ Total Current: "));
	Serial.print(total_charge_current, 2);
	Serial.println(F("A"));

	Serial.print(F("┃ Max Voltage: "));
	Serial.print(total_charge_voltage, 2);
	Serial.println(F("V"));

	// ============ Fault Management ============

	if (error_count > 0) {
		Serial.println(F("┃ ⚠ FAULTS DETECTED"));
		handle_multi_charger_faults();
	} else {
		Serial.println(F("┃ ✓ All systems nominal"));
	}

	Serial.println(F("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"));

	// ============ Reset Watchdog Timers ============

	for (uint8_t i = 0; i < NUM_CHARGERS; i++) {
		if (charger_status[i].is_healthy) {
			Chargers[i].Reset_Watchdog();
		}
	}
}

/**
 * @brief Update status for a specific charger
 */
void update_charger_status(uint8_t charger_id) {
	if (charger_id >= NUM_CHARGERS) return;

	ChargerStatus &status = charger_status[charger_id];

	// Read all status registers
	status.fault_register = Chargers[charger_id].Get_Fault_Register();
	status.charge_voltage = Chargers[charger_id].Get_Charge_Voltage();
	status.charge_current = Chargers[charger_id].Get_Charge_Current();
	status.charge_status = Chargers[charger_id].Charge_Status();

	// Update health status
	status.is_healthy = (status.fault_register == 0) && !Chargers[charger_id].THERM_STAT();
}

/**
 * @brief Display charger status in formatted output
 */
void print_charger_status(uint8_t charger_id) {
	ChargerStatus &status = charger_status[charger_id];

	// Health indicator
	Serial.print(F("│ Status: "));
	Serial.println(status.is_healthy ? F("✓ HEALTHY") : F("✗ FAULT"));

	// Voltage and Current
	Serial.print(F("│ Voltage: "));
	Serial.print(status.charge_voltage, 3);
	Serial.print(F("V  Current: "));
	Serial.print(status.charge_current, 3);
	Serial.println(F("A"));

	// Charging Mode
	Serial.print(F("│ Mode: "));
	switch (status.charge_status) {
		case 0: Serial.println(F("Not charging")); break;
		case 1: Serial.println(F("Pre-charge")); break;
		case 2: Serial.println(F("Fast charge")); break;
		case 3: Serial.println(F("Termination")); break;
		default: Serial.println(F("Unknown"));
	}

	// Power status
	Serial.print(F("│ Power: "));
	if (Chargers[charger_id].PG_STAT()) {
		Serial.println(F("Good"));
	} else {
		Serial.println(F("⚠ Not good"));
	}
}

/**
 * @brief Handle faults across multiple chargers
 */
void handle_multi_charger_faults() {
	Serial.println(F("\nFault Analysis:"));

	for (uint8_t i = 0; i < NUM_CHARGERS; i++) {
		if (charger_status[i].fault_register != 0) {
			Serial.print(F("  Charger "));
			Serial.print(i);
			Serial.print(F(": "));

			if (Chargers[i].WATCHDOG_FAULT()) {
				Serial.println(F("Watchdog expired"));
				Chargers[i].Reset_Watchdog();
			} else if (Chargers[i].BAT_OVP_FAULT()) {
				Serial.println(F("Battery OVP"));
				Chargers[i].Disable_Charge();
			} else if (Chargers[i].NTC_FAULT()) {
				Serial.println(F("NTC fault"));
				Chargers[i].Disable_Charge();
			} else if (Chargers[i].THERM_STAT()) {
				Serial.println(F("Thermal shutdown"));
				Chargers[i].Disable_Charge();
			} else {
				Serial.println(F("Unknown fault"));
			}
		}
	}
}

/**
 * @brief Set identical charging parameters across all chargers
 */
void set_all_chargers_current(float current_amps) {
	for (uint8_t i = 0; i < NUM_CHARGERS; i++) {
		if (charger_status[i].is_healthy) {
			if (!Chargers[i].Set_Charge_Current(current_amps)) {
				Serial.print(F("✗ Failed to set current for charger "));
				Serial.println(i);
			}
		}
	}
}

/**
 * @brief Enable charging on all chargers
 */
void enable_all_chargers() {
	for (uint8_t i = 0; i < NUM_CHARGERS; i++) {
		if (charger_status[i].is_healthy) {
			Chargers[i].Enable_Charge();
		}
	}
}

/**
 * @brief Disable charging on all chargers
 */
void disable_all_chargers() {
	for (uint8_t i = 0; i < NUM_CHARGERS; i++) {
		Chargers[i].Disable_Charge();
	}
}

/**
 * @brief Get total system charging power
 */
float get_total_charging_power() {
	float total_power = 0;
	for (uint8_t i = 0; i < NUM_CHARGERS; i++) {
		if (charger_status[i].is_healthy) {
			total_power += charger_status[i].charge_voltage * charger_status[i].charge_current;
		}
	}
	return total_power;
}
