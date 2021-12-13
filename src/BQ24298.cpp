/* *******************************************************************************
 *  Copyright (C) 2014-2019 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun
 *	This library is a combined book of enviroment sensor library.
 *
 *	Library				: Battery Charger Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *	Revision			: 1.0.0
 *	Relase				: 21.11.2021
 *
 *********************************************************************************/

#include "BQ24298.h"

// Set Functions
void BQ24298::Read_Registers(void) {

	/******************************************************************************
	 *	Project		: BQ24298 Read All Registers Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Read Register 00
	REG00 = Register_Read(0x00);

	// Read Register 01
	REG01 = Register_Read(0x01);

	// Read Register 02
	REG02 = Register_Read(0x02);

	// Read Register 03
	REG03 = Register_Read(0x03);

	// Read Register 04
	REG04 = Register_Read(0x04);

	// Read Register 05
	REG05 = Register_Read(0x05);

	// Read Register 06
	REG06 = Register_Read(0x06);

	// Read Register 07
	REG07 = Register_Read(0x07);

	// Read Register 08
	REG08 = Register_Read(0x08);

	// Read Register 09
	REG09 = Register_Read(0x09);

	// Read Register 0A
	REG0A = Register_Read(0x0A);


}
void BQ24298::Set_Registers(void) {

	/******************************************************************************
	 *	Project		: BQ24298 Set All Registers Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Set Registers
	//if (Register_Write(0x00, REG00_Default) != true) Serial.println("!!");
	if (Register_Write(0x01, REG01_Default) != true) Serial.println("!!");
	//if (Register_Write(0x02, REG02_Default) != true) Serial.println("!!");
	//if (Register_Write(0x03, REG03_Default) != true) Serial.println("!!");
	//if (Register_Write(0x04, REG04_Default) != true) Serial.println("!!");
	//if (Register_Write(0x05, REG05_Default) != true) Serial.println("!!");
	//if (Register_Write(0x06, REG06_Default) != true) Serial.println("!!");
	//if (Register_Write(0x07, REG07_Default) != true) Serial.println("!!");

}

// I2C Functions
uint8_t BQ24298::Register_Read(uint8_t _Register) {

	/******************************************************************************
	 *	Project		: BQ24298 Read Register Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Connect to Charger
	Wire.beginTransmission(0x6B);
	
	// Set Register Read
	Wire.write(_Register);

	// End Connection
	Wire.endTransmission();

	// Connect to Charger
	Wire.beginTransmission(0x6B);

	// Request Data
	Wire.requestFrom(0x6B, 0x01);

	// Read Register
	uint8_t _Value = Wire.read();
	
	// End Connection
	Wire.endTransmission(true);
	
	// delay
	delay(5);

	// Return Value
	return(_Value);

}
bool BQ24298::Register_Write(uint8_t _Register, uint8_t _Value) {

	/******************************************************************************
	 *	Project		: BQ24298 Write Register Function
	 *	Developer	: Mehmet Gunce Akkoyun (akkoyun@me.com)
	 *	Revision	: 01.00.00
	 ******************************************************************************/

	// Send Write Command to BQ24298
	Wire.beginTransmission(0x6B);
	
	// Set Address
	Wire.write(_Register);

	// Send Data
	Wire.write(_Value);
	
	// Close I2C Connection
	uint8_t BQ24298_Write = Wire.endTransmission(true);
	
	// Control For Write Success
	if (BQ24298_Write != 0) return (false);
	
	// Software Delay
	delay(5);

}

BQ24298 Charger;
