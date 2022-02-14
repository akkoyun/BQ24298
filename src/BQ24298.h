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

// Define Library Structures
#ifndef __BQ24298_CONFIG__
#include <Config.h>
#endif

class BQ24298 {

	public:

		// ************************************************************
		// Public Variables
		// ************************************************************

		// ************************************************************
		// Public Functions
		// ************************************************************

		bool 		begin(void);
		bool 		Set_Charge_Current(float current);
		bool 		Enable_Charge(void);
		bool 		Disable_Charge(void);
		bool 		Enable_BATFET(void);
		bool 		Enable_Boost_Mode(void);
		bool 		Disable_Boost_Mode(void);
		bool 		Set_Input_Voltage_Limit(float voltage);
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
		bool 		Set_Minimum_System_Voltage(float voltage);
		float 		Get_Minimum_System_Voltage(void);
		bool 		Set_Charge_Current(float current);
		float 		Get_Charge_Current(void);
		bool 		Set_PreCharge_Current(float current);
		float 		Get_PreCharge_Current(void);
		bool 		Set_TermCharge_Current(float current);
		float 		Get_TermCharge_Current(void);
		bool 		Set_Charge_Voltage(float voltage);
		float 		Get_Charge_Voltage(void);
		uint8_t 	Read_ChargeTerm_Register(void);
		bool 		Disable_Watchdog(void);
		bool 		Set_Thermal_Regulation_Temperature(uint8_t degree);
		int 		Get_Thermal_Regulation_Temperature(void);
		bool 		Disable_DPDM(void);
		bool 		Enable_DPDM(void);
		bool 		Enable_BATFET(void);
		bool 		Disable_BATFET(void);
		bool 		Enable_ChargeFault_INT(void);
		bool 		Disable_ChargeFault_INT(void);
		bool 		Enable_BatFault_INT(void);
		bool 		Disable_BatFault_INT(void);
		uint8_t 	Read_OpControl_Register(void);
		int 		USBmode(void);
		int 		Charge_Status(void);
		bool 		is_Power_Good(void);
		bool 		is_Hot(void);
		bool 		Can_Run_On_Battery(void);
		bool 		is_Batt_Connected(void);
		uint8_t 	Read_System_Status_Register(void);
		uint8_t 	Read_Fault_Register(void);
		uint8_t		Get_Charge_Fault(void);
		uint8_t		Has_Battery_Temperature_Fault(void);
		bool 		is_Watchdog_Expired(void);
		bool 		is_Battery_In_Over_Voltage(void);
		uint8_t 	Get_Version(void);

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
