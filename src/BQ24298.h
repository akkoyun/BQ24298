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

#ifndef __BQ24298__
#define __BQ24298__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define Wire Library
#ifndef __Wire__
#include <Wire.h>
#endif

class BQ24298
{
public:

	// ************************************************************
	// Public Variables
	// ************************************************************

	String Firmware 					= "01.00.00";
	
	uint8_t	REG00						= 0x00;
	uint8_t	REG01						= 0x00;
	uint8_t	REG02						= 0x00;
	uint8_t	REG03						= 0x00;
	uint8_t	REG04						= 0x00;
	uint8_t	REG05						= 0x00;
	uint8_t	REG06						= 0x00;
	uint8_t	REG07						= 0x00;
	uint8_t	REG08						= 0x00;
	uint8_t	REG09						= 0x00;
	uint8_t	REG0A						= 0x00;

	uint8_t	REG00_Default				= 0x7F;	// 0x37
	uint8_t	REG01_Default				= 0x1A;	// 0x1B [1A]
	uint8_t	REG02_Default				= 0x90;	// 0x60
	uint8_t	REG03_Default				= 0x11;	// 0x11
	uint8_t	REG04_Default				= 0xB2;	// 0xB2
	uint8_t	REG05_Default				= 0xC6;	// 0xDC [06]
	uint8_t	REG06_Default				= 0x73;	// 0x73
	uint8_t	REG07_Default				= 0x4B;	// 0x4B

	// ************************************************************
	// Public Functions
	// ************************************************************
	
	uint8_t 	Read_Register(uint8_t _Register);
	void 		Read_Registers(void);
	void 		Set_Registers(void);

	// I2C Functions
	uint8_t		Register_Read(uint8_t _Register);
	bool 		Register_Write(uint8_t _Register, uint8_t _Value);

private:

	// ************************************************************
	// Private Variables
	// ************************************************************
	

	// ************************************************************
	// Private Functions
	// ************************************************************


};

extern BQ24298 Charger;

#endif /* defined(__BQ24298__) */
