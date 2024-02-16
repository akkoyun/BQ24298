// Include Library
#include <BQ24298.h>

// Create Charger Object
BQ24298 Charger(true, 1);

// Setup
void setup() {

	// Start Serial
	Serial.begin(115200);

	// Start I2C Communication
	Charger.Begin();

}

// Loop
void loop() {

	// Print Diagnostic
	Serial.print(F("PG Stat             : ")); if (!Charger.PG_STAT()) {Serial.println(F("Power Not Good"));} else {Serial.println(F("Power Good"));};
	Serial.print(F("Therm Stat          : ")); if (Charger.THERM_STAT()) {Serial.println(F("Thermal Hot!")); } else {Serial.println(F("Normal"));};
	Serial.print(F("DPM Stat            : ")); if (!Charger.DPM_STAT()) {Serial.println(F("DPM Pasive")); } else {Serial.println(F("DPM Active"));};
	Serial.print(F("WatchDog Fault      : ")); if (Charger.WATCHDOG_FAULT()) {Serial.println(F("Watchdog expired")); } else {Serial.println(F("Normal"));};
	Serial.print(F("Battery OVP         : ")); if (Charger.BAT_OVP_FAULT()) {Serial.println(F("Bat OVP")); } else {Serial.println(F("Normal"));};
	Serial.print(F("System Voltage Stat : ")); if (Charger.VSYS_STAT()) {Serial.println(F("BAT < VSYSMIN")); } else {Serial.println(F("BAT > VSYSMIN"));};
	Serial.print(F("Bat LowV            : ")); if (Charger.BATLOWV()) {Serial.println(F("3V0")); } else {Serial.println(F("2V8"));};
	Serial.print(F("Bat Cold            : ")); if (Charger.BCOLD()) {Serial.println(F("BCold True")); } else {Serial.println(F("Normal"));};
	Serial.print(F("NTC Fault           : ")); if (Charger.BCOLD()) {Serial.println(F("Fault")); } else {Serial.println(F("Normal"));};
	Serial.print(F("Input Voltage Stat  : ")); 
	switch (Charger.VBUS_STAT()) {
	case 1:
		Serial.println(F("USB"));
		break;
	case 2:
		Serial.println(F("Adapter"));
		break;
	case 3:
		Serial.println(F("OTG"));
		break;
	default:
		Serial.println(F("Unknown"));
		break;
	}
	Serial.print(F("Charge Status       : "));
	switch (Charger.Charge_Status()) {
	case 1:
		Serial.println(F("Pre-Charge"));
		break;
	case 2:
		Serial.println(F("Fast Charge"));
		break;
	case 3:
		Serial.println(F("Charge Complate"));
		break;
	default:
		Serial.println(F("Not Charging"));
		break;
	}
	Serial.print(F("Fault register      : 0x")); Serial.print(Charger.Get_Fault_Register(), HEX); Serial.println("");
	Serial.println(F("----------------------------------")); 

	// Reset Watchdog
	Charger.Reset_Watchdog();

	// Delay
	delay(3000);

}