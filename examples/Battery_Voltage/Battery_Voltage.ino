#include "Battery.h"

// Define Library
Battery Battery;

void setup() {
  
  // Serial Communication Start
  Serial.begin(115200);
  
  // Start I2C
  Wire.begin();
  
  // Header
  Serial.println("     Battery Voltage      ");
  Serial.println("--------------------------");

}

void loop() {
  
  // Define Sensor Variables
  float _Measurement;
  unsigned long Time;

  // Set Start Time
  Time = millis();

  // Measure
  Battery.InstantVoltage(_Measurement);

  // Calculate Delta Time
  int DT = millis() - Time;
    
  // Serial Print Data
  Serial.print("Value         : "); Serial.print(_Measurement, 3); Serial.println(" V");
  Serial.print("Function Time : "); Serial.print(DT); Serial.println(" mS");
  Serial.println("--------------------------");
  
  // Loop Delay
  delay(1000);

}
