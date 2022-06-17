#include <BQ24298.h>

void setup() {

	// Start Serial
	Serial.begin(115200);

}

void loop() {

	// Create Charger Object
	BQ24298 Charger(true, 5);

	// Print Diagnostic
	Serial.print("PG Stat             : "); if (!Charger.PG_STAT()) {Serial.println("Power Not Good");} else {Serial.println("Power Good");};
	Serial.print("Therm Stat          : "); if (Charger.THERM_STAT()) {Serial.println("Thermal Hot!"); } else {Serial.println("Normal");};
	Serial.print("DPM Stat            : "); if (!Charger.DPM_STAT()) {Serial.println("DPM Pasive"); } else {Serial.println("DPM Active");};
	Serial.print("WatchDog Fault      : "); if (Charger.WATCHDOG_FAULT()) {Serial.println("Watchdog expired"); } else {Serial.println("Normal");};
	Serial.print("Battery OVP         : "); if (Charger.BAT_OVP_FAULT()) {Serial.println("Bat OVP"); } else {Serial.println("Normal");};
	Serial.print("System Voltage Stat : "); if (Charger.VSYS_STAT()) {Serial.println("BAT < VSYSMIN"); } else {Serial.println("BAT > VSYSMIN");};
	Serial.print("Bat LowV            : "); if (Charger.BATLOWV()) {Serial.println("3V0"); } else {Serial.println("2V8");};
	Serial.print("Bat Cold            : "); if (Charger.BCOLD()) {Serial.println("BCold True"); } else {Serial.println("Normal");};
	Serial.print("NTC Fault           : "); if (Charger.BCOLD()) {Serial.println("Fault"); } else {Serial.println("Normal");};
	Serial.print("Input Voltage Stat  : "); 
	switch (Charger.VBUS_STAT()) {
	case 1:
		Serial.println("USB");
		break;
	case 2:
		Serial.println("Adapter");
		break;
	case 3:
		Serial.println("OTG");
		break;
	default:
		Serial.println("Unknown");
		break;
	}
	Serial.print("Charge Status       : "); 
	switch (Charger.Charge_Status()) {
	case 1:
		Serial.println("Pre-Charge");
		break;
	case 2:
		Serial.println("Fast Charge");
		break;
	case 3:
		Serial.println("Charge Complate");
		break;
	default:
		Serial.println("Not Charging");
		break;
	}
	Serial.print("Fault register      : 0x"); Serial.print(Charger.Get_Fault_Register(), HEX); Serial.println("");
	








	Serial.println("----------------------------------"); 

	delay(3000);

}