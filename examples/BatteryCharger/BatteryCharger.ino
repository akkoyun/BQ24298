#include <BQ24298.h>

void setup() {

	// Start Serial
	Serial.begin(115200);

}

void loop() {

	// Create Charger Object
	BQ24298 Charger(true, 5);

	Serial.print("PG Stat             : "); if (!Charger.PG_STAT()) Serial.print("Power Not Good"); Serial.println("");
	Serial.print("Therm Stat          : "); if (Charger.THERM_STAT()) Serial.print("Thermal Hot!"); Serial.println("");
	Serial.print("DPM Stat            : "); if (!Charger.DPM_STAT()) Serial.print("DPM False"); Serial.println("");
	Serial.print("WatchDog Fault      : "); if (Charger.WATCHDOG_FAULT()) Serial.print("Watchdog expired"); Serial.println("");
	Serial.print("System Voltage Stat : "); if (Charger.VSYS_STAT() == 1) {Serial.print("BAT < VSYSMIN");} else {Serial.print("BAT > VSYSMIN");} Serial.println("");
	Serial.print("Fault register      : 0x"); Serial.print(Charger.Get_Fault_Register(), HEX); Serial.println("");

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

	Serial.println("----------------------------------"); 

	delay(3000);

}