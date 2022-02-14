#include <BQ24298.h>

int usb_mode = 0x00;

void setup() {

	// Start Serial
	Serial.begin(115200);

	// Control for Charger
	if (!Charger.begin()) {
		Serial.println("Failed to initialize PMIC!");
		while (1);
	}

	// Enable boost mode, this mode allows using the board as host to
	// connect a guest device such as a keyboard
	if (!Charger.Enable_Boost_Mode()) Serial.println("Error enabling Boost Mode");
	
	Serial.println("Initialization Done!");

}

void loop() {

	int actual_mode = Charger.USBmode();

	if (actual_mode != usb_mode) {
    
		usb_mode = actual_mode;
    
		if (actual_mode == 0xC0) {

		Serial1.println("Boost mode status enabled");

    }

  }
  
}