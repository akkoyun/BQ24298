/* *******************************************************************************
 *  Copyright (C) 2014-2019 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun
 *	This library is a combined book of enviroment sensor library.
 *
 *	Library				: Battery Charger Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#ifndef __BQ24298__
#define __BQ24298__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define Round Function
#ifndef BQ_Round
#define BQ_Round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#endif

// Define I2C Functions Library
#ifndef __I2C_Functions__
#include <I2C_Functions.h>
#endif

class BQ24298 {

	public:

		// ************************************************************
		// Public Variables
		// ************************************************************

		// ************************************************************
		// Public Functions
		// ************************************************************

		bool		begin(void);
		bool 		Set_Charge_Current(float _Charge_Current);
		bool 		Enable_Charge(void);
		bool 		Disable_Charge(void);
		bool 		Enable_Boost_Mode(void);
		bool 		Disable_Boost_Mode(void);
		bool 		Set_Input_Voltage_Limit(float _Input_Voltage);
		float 		Get_Input_Voltage_Limit(void);
		bool 		Set_Input_Current_Limit(float _Input_Current);
		float 		Get_Input_Current_Limit(void);
		uint8_t 	Read_Input_Source_Register(void);
		bool 		Enable_Buck(void);
		bool 		Disable_Buck(void);
		uint8_t 	Read_Power_ON_Register(void);
		bool 		Enable_Charging(void);
		bool 		Disable_Charging(void);
		bool 		Disable_OTG(void);
		bool 		Enable_OTG(void);
		bool 		Reset_Watchdog(void);
		bool 		Set_Minimum_System_Voltage(float _Minimum_Voltage);
		float 		Get_Minimum_System_Voltage(void);
		bool 		Set_Boost_Voltage(float _Boost_Voltage);
		float 		Get_Charge_Current(void);
		bool 		Set_PreCharge_Current(float _PreCharge_Current);
		float 		Get_PreCharge_Current(void);
		bool 		Set_TermCharge_Current(float _Term_Charge_Current);
		float 		Get_TermCharge_Current(void);
		bool 		Set_Charge_Voltage(float _Charge_Voltage);
		float 		Get_Charge_Voltage(void);
		uint8_t 	Read_ChargeTerm_Register(void);
		bool 		Set_Watchdog(uint8_t _Timer);
		bool 		Set_Thermal_Regulation_Temperature(uint8_t _Temperature);
		int 		Get_Thermal_Regulation_Temperature(void);
		bool 		Disable_DPDM(void);
		bool 		Enable_DPDM(void);
		bool 		BATFET_Disable_Bit(bool _State);
		bool 		ChargeFault_INT(bool _State);
		bool 		Enable_BatFault_INT(void);
		bool 		Disable_BatFault_INT(void);
		uint8_t 	Read_OpControl_Register(void);
		uint8_t		VBUS_STAT(void);
		uint8_t		Charge_Status(void);
		bool 		PG_STAT(void);
		bool 		THERM_STAT(void);
		bool 		VSYS_STAT(void);
		bool 		DPM_STAT(void);
		uint8_t 	Read_System_Status_Register(void);
		uint8_t 	Read_Fault_Register(void);
		uint8_t		Get_Charge_Fault(void);
		uint8_t		NTC_FAULT(void);
		bool 		WATCHDOG_FAULT(void);
		bool 		BAT_OVP_FAULT(void);
		uint8_t 	Get_Version(void);
		bool 		BATLOWV(void);
		bool 		BCOLD(void);
		bool 		Disable_BHOT(void);
		uint8_t 	Get_Fault_Register(void);

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
