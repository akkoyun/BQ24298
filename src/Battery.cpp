/* *******************************************************************************
 *  Copyright (C) 2014-2019 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun
 *	This library is a combined book of enviroment sensor library.
 *
 *	Library				: Battery Management Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *	Revision			: 1.0.0
 *	Relase				: 11.11.2020
 *
 *********************************************************************************/

#include "Arduino.h"
#include "Battery.h"
#include <Wire.h>

// Measurement Functions
bool Battery::InstantVoltage(float &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Instant Voltage Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[IV_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < IV_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00001001); // 0x09
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Measurement
		Measurement_Array[Read_ID] = (float)Measurement_Raw * 0.078125 / 1000;
			
	}
	
	// Calculate Average
	if (IV_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < IV_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / IV_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::AverageVoltage(float &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Average Voltage Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[AV_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < AV_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00011001); // 0x19

		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Measurement
		Measurement_Array[Read_ID] = (float)Measurement_Raw * 0.078125 / 1000;
			
	}
	
	// Calculate Average
	if (AV_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < AV_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / AV_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::InstantCurrent(float &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Instant Current Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[IC_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < IC_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00001010); // 0x0A
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Declare Variables
		bool _Signiture;

		// Control for Negative Value
		if ((Measurement_Raw >> 12) == 0xF) {

			Measurement_Raw = 0xFFFF - Measurement_Raw;
			_Signiture = true;

		} else {
			
			_Signiture = false;

		}

		// Calculate Data
		Measurement_Array[Read_ID] = (float)Measurement_Raw * 0.15625;
		
		// Assign Signiture
		if (_Signiture) Measurement_Array[Read_ID] *= -1;

	}
	
	// Calculate Average
	if (IC_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < IC_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / IC_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::AverageCurrent(float &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Average Current Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[AC_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < AC_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00001011); // 0x0B
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Declare Variables
		bool _Signiture;

		// Control for Negative Value
		if ((Measurement_Raw >> 12) == 0xF) {

			Measurement_Raw = 0xFFFF - Measurement_Raw;
			_Signiture = true;

		} else {
			
			_Signiture = false;

		}

		// Calculate Data
		Measurement_Array[Read_ID] = (float)Measurement_Raw * 0.15625;
		
		// Assign Signiture
		if (_Signiture) Measurement_Array[Read_ID] *= -1;

	}
	
	// Calculate Average
	if (AC_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < AC_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / AC_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::StateOfCharge(float &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 State of Charge Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[SOC_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < SOC_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00000110); // 0x06
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Measurement
		Measurement_Array[Read_ID] = (float)_Register / 256;

	}
	
	// Calculate Average
	if (SOC_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < SOC_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / SOC_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::AverageStateOfCharge(float &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Average State of Charge Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[ASOC_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < ASOC_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00001110); // 0x0E
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Measurement
		Measurement_Array[Read_ID] = (float)Measurement_Raw / 256;

	}
	
	// Calculate Average
	if (ASOC_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < ASOC_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / ASOC_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::InstantCapacity(int &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Instant Capacity Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[ICAP_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < ICAP_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00000101); // 0x05
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Measurement
		Measurement_Array[Read_ID] = (float)Measurement_Raw * (5e-3) / 0.01;

	}
	
	// Calculate Average
	if (ICAP_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < ICAP_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / ICAP_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::DesignCapacity(int &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Design Capacity Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[DCAP_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < DCAP_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00010000); // 0x10
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Measurement
		Measurement_Array[Read_ID] = (float)Measurement_Raw * (5e-3) / 0.01;

	}
	
	// Calculate Average
	if (DCAP_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < DCAP_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / DCAP_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::Temperature(float &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 IC Temperature Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[Temp_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < Temp_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00001000); // 0x08
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Declare Variables
		bool _Signiture;
		
		// Control for Negative Value
		if ((Measurement_Raw >> 12) == 0xF) {

			Measurement_Raw = 0xFFFF - Measurement_Raw;
			_Signiture = true;

		} else {
			
			_Signiture = false;

		}

		// Calculate Data
		Measurement_Array[Read_ID] = (float)Measurement_Raw / 256;
		
		// Assign Signiture
		if (_Signiture) Measurement_Array[Read_ID] *= -1;

	}
	
	// Calculate Average
	if (Temp_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < Temp_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / Temp_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::TimeToEmpty(int &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Time to Empty Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[TTE_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < TTE_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00010001); // 0x11
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Data
		Measurement_Array[Read_ID] = (float)Measurement_Raw * 5.625 / 60 / 60;

	}
	
	// Calculate Average
	if (TTE_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < TTE_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / TTE_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::TimeToFull(int &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Time to Full Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[TTF_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < TTF_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00100000); // 0x20
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Data
		Measurement_Array[Read_ID] = (float)Measurement_Raw * 5.625 / 60 / 60;

	}
	
	// Calculate Average
	if (TTF_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < TTF_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / TTF_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::Age(int &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Age Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[AGE_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < AGE_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00000111); // 0x07
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Data
		Measurement_Array[Read_ID] = (float)Measurement_Raw / 256;

	}
	
	// Calculate Average
	if (AGE_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < AGE_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / AGE_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}
bool Battery::Cycle(int &Value_) {

	/******************************************************************************
	 *	Project		: MAX17055 Cycle Read Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Define Measurement Read Array
	float Measurement_Array[CYC_Read_Count_];

	// Read Loop For Read Count
	for (uint8_t Read_ID = 0; Read_ID < CYC_Read_Count_; Read_ID++) {
	
		// Send Read Command to MAX17055
		Wire.beginTransmission(0b00110110); // 0x36
		Wire.write(0b00010111); // 0x17
		
		// Close I2C Connection
		int MAX17055_Read = Wire.endTransmission(false);
		
		// Control For Read Success
		if (MAX17055_Read != 0) {
			
			// Set Error Code
			Value_ = -101;
			
			// End Function
			return(false);
			
		}
		
		// Read Data Command to MAX17055
		Wire.requestFrom(0b00110110, 2); // 0x36
		
		// Define Data Variable
		uint8_t MAX17055_Data[2];
		
		// Read I2C Bytes
		MAX17055_Data[0] = Wire.read(); // LSB
		MAX17055_Data[1] = Wire.read(); // MSB
		
		// Combine Read Bytes
		uint16_t Measurement_Raw = ((uint16_t)MAX17055_Data[0] << 8) | (uint16_t)MAX17055_Data[1];
		
		// Calculate Data
		Measurement_Array[Read_ID] = (float)Measurement_Raw;

	}
	
	// Calculate Average
	if (CYC_Average_Type_ == 1) {
		
		// Declare Variable
		double SUM = 0;
		
		// Calculate Sum
		for (int i = 0; i < CYC_Read_Count_; i++) SUM += Measurement_Array[i];
		
		// Calculate Avg
		Value_ = SUM / CYC_Read_Count_;
		
	}	// Standart Average

	// End Function
	return(true);

}

