#include <BQ24298.h>

void setup() {

	// Start Serial
	Serial.begin(115200);

	// Set Multiplexer
	I2C.Set_Multiplexer(0x70, 5);

	// Control for Charger
	Charger.begin();
//	if (!Charger.begin()) {
//		Serial.println("Failed to initialize PMIC!");
//		while (1);
//	}

}

void loop() {


	//if (Charger.Can_Run_On_Battery()) Serial.println(Charger.Charge_Status(), HEX);

	if (!Charger.PG_STAT()) Serial.println("Power Not Good");
	if (Charger.THERM_STAT()) Serial.println("Thermal Hot!");
	if (!Charger.DPM_STAT()) Serial.println("DPM False");
	if (Charger.WATCHDOG_FAULT()) Serial.println("Watchdog expired");
	if (Charger.VSYS_STAT() == 1) {Serial.println("BAT < VSYSMIN");} else {Serial.println("BAT > VSYSMIN");}
	
	Serial.println(Charger.Get_Fault_Register(), HEX);

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

	delay(1000);

}