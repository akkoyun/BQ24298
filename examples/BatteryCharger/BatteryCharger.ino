#include <BQ24298.h>

void setup() {

	// Start Serial
	Serial.begin(115200);

	// Control for Charger
	if (!Charger.begin()) {
		Serial.println("Failed to initialize PMIC!");
		while (1);
	}

	// Set the input current limit to 2 A and the overload input voltage to 3.88 V
	if (!Charger.Set_Input_Current_Limit(2.0)) Serial.println("Error in set input current limit");

	if (!Charger.Set_Input_Voltage_Limit(3.88)) Serial.println("Error in set input voltage limit");

	// set the minimum voltage used to feeding the module embed on Board
	if (!Charger.Set_Minimum_System_Voltage(3.8)) Serial.println("Error in set minimum system volage");

	// Set the desired charge voltage to 4.11 V
	if (!Charger.Set_Charge_Voltage(4.2)) Serial.println("Error in set charge volage");

  	// Set the charge current to 375 mA
	// the charge current should be defined as maximum at (C for hour)/2h
	// to avoid battery explosion (for example for a 750 mAh battery set to 0.375 A)
	if (!Charger.Set_Charge_Current(2)) Serial.println("Error in set charge current");

	Serial.println("Initialization done!");

}

void loop() {

	// Enable the Charger
	if (!Charger.Enable_Charge()) Serial.println("Error enabling Charge mode");

	// canRunOnBattery() returns true if the battery voltage is < the minimum
	// systems voltage
	if (Charger.Can_Run_On_Battery()) {
    
		// loop until charge is done
    	while (Charger.Charge_Status() != CHARGE_TERMINATION_DONE) delay(1000);

		// Disable the charger and loop forever
		Serial.println("Disable Charge mode");
    
		if (!Charger.Disable_Charge()) Serial.println("Error disabling Charge mode");
    
		while (1);
    
	}

	delay(100);

}