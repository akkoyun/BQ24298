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

#ifndef __Battery__
#define __Battery__

#include <Arduino.h>
#include <Wire.h>

class Battery
{
public:
	
	// ************************************************************
	// Public Functions
	// ************************************************************

	bool Begin(int DesignCapacity_);
	float InstantVoltage(float &Value_);
	float AverageVoltage(float &Value_);
	float InstantCurrent(float &Value_);
	float AverageCurrent(float &Value_);
	float StateOfCharge(float &Value_);
	float AverageStateOfCharge(float &Value_);
	int InstantCapacity(int &Value_);
	int DesignCapacity(int &Value_);
	float Temperature(float &Value_);
	int TimeToEmpty(int &Value_);
	int TimeToFull(int &Value_);
	int Age(int &Value_);
	int Cycle(int &Value_);

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

#endif /* defined(__Battery__) */
