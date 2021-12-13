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

#ifndef __MAX17055__
#define __MAX17055__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define Wire Library
#ifndef __Wire__
#include <Wire.h>
#endif

class MAX17055
{
public:

	// ************************************************************
	// Public Variables
	// ************************************************************

	String Firmware 					= "01.00.02";
	
	// ************************************************************
	// Public Functions
	// ************************************************************

	bool Begin(const int DesignCapacity_);
	float InstantVoltage(void);
	float AverageVoltage(void);
	float InstantCurrent(void);
	float AverageCurrent(void);
	float StateOfCharge(void);
	float AverageStateOfCharge(void);
	int InstantCapacity(void);
	int DesignCapacity(void);
	float Temperature(void);
	int TimeToEmpty(void);
	int TimeToFull(void);
	int Age(void);
	int Cycle(void);

private:

	// ************************************************************
	// Private Constants
	// ************************************************************

	// Instant Voltage
	const uint8_t 	IV_Read_Count_ 		= 5;
	const uint8_t 	IV_Average_Type_ 	= 1;
	
	// Average Voltage
	const uint8_t 	AV_Read_Count_ 		= 1;
	const uint8_t 	AV_Average_Type_ 	= 1;
	
	// Instant Current
	const uint8_t 	IC_Read_Count_ 		= 5;
	const uint8_t 	IC_Average_Type_ 	= 1;
	
	// Average Current
	const uint8_t 	AC_Read_Count_ 		= 1;
	const uint8_t 	AC_Average_Type_ 	= 1;
	
	// State of Charge
	const uint8_t 	SOC_Read_Count_ 	= 5;
	const uint8_t 	SOC_Average_Type_ 	= 1;

	// Average State of Charge
	const uint8_t 	ASOC_Read_Count_ 	= 1;
	const uint8_t 	ASOC_Average_Type_ 	= 1;

	// Instant Capacity
	const uint8_t 	ICAP_Read_Count_ 	= 1;
	const uint8_t 	ICAP_Average_Type_ 	= 1;

	// Design Capacity
	const uint8_t 	DCAP_Read_Count_ 	= 1;
	const uint8_t 	DCAP_Average_Type_ 	= 1;

	// IC Temperature
	const uint8_t 	Temp_Read_Count_ 	= 5;
	const uint8_t 	Temp_Average_Type_ 	= 1;

	// Time to Empty
	const uint8_t 	TTE_Read_Count_ 	= 1;
	const uint8_t 	TTE_Average_Type_ 	= 1;

	// Time to Full
	const uint8_t 	TTF_Read_Count_ 	= 1;
	const uint8_t 	TTF_Average_Type_ 	= 1;

	// Age
	const uint8_t 	AGE_Read_Count_ 	= 1;
	const uint8_t 	AGE_Average_Type_ 	= 1;

	// Cycle
	const uint8_t 	CYC_Read_Count_ 	= 1;
	const uint8_t 	CYC_Average_Type_ 	= 1;

};

extern MAX17055 Gauge;

#endif /* defined(__MAX17055__) */
