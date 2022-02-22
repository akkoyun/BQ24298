/* *******************************************************************************
 *  Copyright (C) 2014-2019 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun
 *	This library is a combined book of enviroment sensor library.
 *
 *	Library				: Battery Charger Library
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include "BQ24298.h"

bool BQ24298::begin(void) {

	// Check PMIC Version
	uint8_t _Version = I2C.Read_Register(I2C_ADDRESS, PMIC_VERSION_REGISTER);

	// Control for Version
	if (_Version == 0x24) {

		// Set Charger Input Current Limit		
		Charger.Set_Input_Current_Limit(3.0);
		
		// Set Charger Input Voltage Limir
		Charger.Set_Input_Voltage_Limit(5.08);
		
		// Set Minimum System Voltage
		Charger.Set_Minimum_System_Voltage(4.0);

		// Set Charge Voltage
		Charger.Set_Charge_Voltage(4.3);
		
		// Disable Boost Temperature
		Disable_BHOT();

		// Set Charge Current
		Charger.Set_Charge_Current(2);

		// Set Charge Termination Current
		Charger.Set_TermCharge_Current(0.128);

		// Disable Watchdog
		Charger.Set_Watchdog(0);
		
		// Enable BatFet
		Charger.BATFET_Disable_Bit(false);

		// Enable Charge
		Charger.Enable_Charge();

		// Set Boost Voltage
		Charger.Set_Boost_Voltage(4.55);

		// Enable Boost
		Charger.Enable_Boost_Mode();

		// Disable OTG
		Charger.Disable_OTG();

		// End Function
		return(true);

	}
	
	// End Function
	return(false);

}

// Buck Functions
bool BQ24298::Enable_Buck(void) {

	// Clear Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, INPUT_SOURCE_REGISTER, 7, true);

	// End Function
	return(true);

}
bool BQ24298::Disable_Buck(void) {

	// Set Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, INPUT_SOURCE_REGISTER, 7, true);

	// End Function
	return(true);

}

// OTG Functions
bool BQ24298::Disable_OTG(void) {

	// Read Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Set Register
	uint8_t _PowerOn_Config_Register;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00010000;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register, true);

	// End Function
	return(_Response);

}
bool BQ24298::Enable_OTG(void) {

	// Read Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Set Register
	uint8_t _PowerOn_Config_Register;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00100000;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register, true);

	// Command Delay
	delay(220);

	// End Function
	return(_Response);

}

// Charge Functions
bool BQ24298::Enable_Charge(void) {

	I2C.Set_Register_Bit(I2C_ADDRESS, POWERON_CONFIG_REGISTER, 4, true);
	I2C.Set_Register_Bit(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER, 7, true);
	I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 0, true);
	I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 1, true);

	// Enable Bat FET
	bool _Response = BATFET_Disable_Bit(false);

	// End Function
	return(_Response);

}
bool BQ24298::Disable_Charge(void) {

	// Read Curent Power On Config Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Control for Register Read
	if (_Current_PowerOn_Config_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0xCF;

	// Write Charge Register
	I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register, false);

	// Set Charge Termination Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER, 7, true);

	// Read Curent Power On Config Register
	uint8_t _Current_Misc_Control_Register = I2C.Read_Register(I2C_ADDRESS, MISC_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Misc_Control_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _Misc_Control_Register = _Current_Misc_Control_Register & 0xFC;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, MISC_CONTROL_REGISTER, _Misc_Control_Register, false);

	// End Function
	return(_Response);

}

// Boost Mode Functions
bool BQ24298::Enable_Boost_Mode(void) {

	// Set OTG Config Bit
	bool _Response1 = I2C.Set_Register_Bit(I2C_ADDRESS, POWERON_CONFIG_REGISTER, 5, true);

	// Set Charge Termination Bit
	bool _Response2 = I2C.Clear_Register_Bit(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER, 7, true);




	// Read Curent Misc Control Register
	uint8_t _Current_Misc_Control_Register = I2C.Read_Register(I2C_ADDRESS, MISC_CONTROL_REGISTER);

	// Set Mask
	uint8_t _Misc_Control_Register = _Current_Misc_Control_Register | 0x03;

	// Write Charge Register
	bool _Response3 = I2C.Write_Register(I2C_ADDRESS, MISC_CONTROL_REGISTER, _Misc_Control_Register | 0x08, true);

	// Command Delay
	delay(500);

	// End Function
	return(_Response1 & _Response2 & _Response3);

}
bool BQ24298::Disable_Boost_Mode(void) {

	// Read Curent Misc Control Register
	uint8_t _Current_Power_On_Control_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Control for Register Read
	if (_Current_Power_On_Control_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _Power_On_Control_Register = _Current_Power_On_Control_Register & 0xCF;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _Power_On_Control_Register | 0x10, false);

	// End Function
	return(_Response);

}

// Enable Charging Functions
bool BQ24298::Enable_Charging() {

	// Read Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Set Register
	uint8_t _PowerOn_Config_Register;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00010000;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register, true);

	// End Function
	return(_Response);

}
bool BQ24298::Disable_Charging() {

	// Read Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Control for Register Read
	if (_Current_PowerOn_Config_Register = 0xFF) return(false);

	// Set Register
	uint8_t _PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0xCF;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register, false);

	// End Function
	return(_Response);

}

// Charge Voltage / Current Functions
bool BQ24298::Set_Charge_Current(float _Charge_Current) {

	// Control for Function Variable
    if (_Charge_Current >= 3.008) _Charge_Current = 3.008;
    if (_Charge_Current <= 0.512) _Charge_Current = 0.512;

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, CHARGE_CURRENT_CONTROL_REGISTER);

	// Define Charge Current Register
	uint8_t _Charge_Register = _Current_Data & 0x03;

	// Handel Data
	float _Current = _Charge_Current - 0.512;
	if (_Current >= 2048) {
		_Charge_Register |= 0b10000000;
		_Current -= 2048;
	}
	if (_Current >= 1024) {
		_Charge_Register |= 0b01000000;
		_Current -= 1024;
	}
	if (_Current >= 512) {
		_Charge_Register |= 0b00100000;
		_Current -= 512;
	}
	if (_Current >= 256) {
		_Charge_Register |= 0b00010000;
		_Current -= 256;
	}
	if (_Current >= 128) {
		_Charge_Register |= 0b00001000;
		_Current -= 128;
	}
	if (_Current >= 64) {
		_Charge_Register |= 0b00000100;
		_Current -= 64;
	}

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, CHARGE_CURRENT_CONTROL_REGISTER, _Charge_Register, true);

	// End Functions
	return(_Response);

}
float BQ24298::Get_Charge_Current(void) {

	// Declare Variable
	float _Charge_Current = 0.512;

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, CHARGE_CURRENT_CONTROL_REGISTER);

	// Set Value
	if (bitRead(_Current_Data, 2) == true) _Current_Data += 0.064;
	if (bitRead(_Current_Data, 3) == true) _Current_Data += 0.128;
	if (bitRead(_Current_Data, 4) == true) _Current_Data += 0.256;
	if (bitRead(_Current_Data, 5) == true) _Current_Data += 0.512;
	if (bitRead(_Current_Data, 6) == true) _Current_Data += 1.024;
	if (bitRead(_Current_Data, 7) == true) _Current_Data += 2.048;

	// End Functions
	return(_Current_Data);

}
bool BQ24298::Set_Charge_Voltage(float _Charge_Voltage) {

	// Control for Function Variable
    if (_Charge_Voltage >= 4.512) _Charge_Voltage = 4.512;
    if (_Charge_Voltage <= 3.504) _Charge_Voltage = 3.504;

	// Read Register
	uint8_t _Current_Register = I2C.Read_Register(I2C_ADDRESS, CHARGE_VOLTAGE_CONTROL_REGISTER);

	// Set Mask
	uint8_t _Voltage_Register = _Current_Register & 0x03;

	// Set Voltage Register
	float _Voltage = _Charge_Voltage - 3.504;
	if (_Voltage >= 0.512) {
		_Voltage_Register |= 0b10000000;
		_Voltage -= 0.512;
	}
	if (_Voltage >= 0.256) {
		_Voltage_Register |= 0b01000000;
		_Voltage -= 0.256;
	}
	if (_Voltage >= 0.128) {
		_Voltage_Register |= 0b00100000;
		_Voltage -= 0.128;
	}
	if (_Voltage >= 0.064) {
		_Voltage_Register |= 0b00010000;
		_Voltage -= 0.064;
	}
	if (_Voltage >= 0.032) {
		_Voltage_Register |= 0b00001000;
		_Voltage -= 0.032;
	}
	if (_Voltage >= 0.016) {
		_Voltage_Register |= 0b00000100;
		_Voltage -= 0.016;
	}

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, CHARGE_VOLTAGE_CONTROL_REGISTER, _Voltage_Register, true);

	// End Functions
	return(_Response);

}
float BQ24298::Get_Charge_Voltage(void) {

	// Read Curent Charge Register
	uint8_t _Current_Charge_Voltage_Control_Register = I2C.Read_Register(I2C_ADDRESS, CHARGE_VOLTAGE_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Charge_Voltage_Control_Register = 0xFF) return(NAN);
	
	// Set Mask
	uint8_t _Mask = _Current_Charge_Voltage_Control_Register & 0xFC;

	// Set Charge Register
	float _Charge_Voltage_Control_Register = (_Mask * 0.004f) + 3.504f;

	// End Functions
	return(_Charge_Voltage_Control_Register);

}

// Input Voltage / Current Functions
bool BQ24298::Set_Input_Current_Limit(float _Input_Current) {

	// Read Curent Charge Register
	uint8_t _Input_Source_Register = I2C.Read_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER);

	// Set Mask
	uint8_t _Mask = _Input_Source_Register & 0xF8;

	// Set Current Value
	uint8_t _Current_Value = CURRENT_LIM_100;

	// Set Current Value
	if (_Input_Current > 0.015) _Current_Value = CURRENT_LIM_150;
	if (_Input_Current >= 0.5) _Current_Value = CURRENT_LIM_500;
	if (_Input_Current >= 0.9) _Current_Value = CURRENT_LIM_900;
	if (_Input_Current >= 1.2) _Current_Value = CURRENT_LIM_1200;
	if (_Input_Current >= 1.5) _Current_Value = CURRENT_LIM_1500;
	if (_Input_Current >= 2.0) _Current_Value = CURRENT_LIM_2000;
	if (_Input_Current >= 3.0) _Current_Value = CURRENT_LIM_3000;

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER, _Current_Value | _Mask, true);

	// End Functions
	return(_Response);

}
float BQ24298::Get_Input_Current_Limit(void) {

	// Read Curent Charge Register
	uint8_t _Input_Source_Register = I2C.Read_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER);

	// Control for Register Read
	if (_Input_Source_Register = 0xFF) return(NAN);
	
	// Set Mask
	uint8_t _Mask = _Input_Source_Register & 0x07;

	// Decide Response
    switch (_Mask) {
        case CURRENT_LIM_100:
            return 0.1;
        case CURRENT_LIM_150:
            return 0.015;
        case CURRENT_LIM_500:
            return 0.5;
        case CURRENT_LIM_900:
            return 0.9;
        case CURRENT_LIM_1200:
            return 1.2;
        case CURRENT_LIM_1500:
            return 1.5;
        case CURRENT_LIM_2000:
            return 2.0;
        case CURRENT_LIM_3000:
            return 3.0;
        default:
            return NAN;
    }

	// End Function
	return(0);

}
bool BQ24298::Set_Input_Voltage_Limit(float _Input_Voltage) {

	// Control for Function Variable
    if (_Input_Voltage >= 5.080) _Input_Voltage = 5.080;
    if (_Input_Voltage <= 3.880) _Input_Voltage = 3.880;

	// Read Curent Charge Register
	uint8_t _Input_Source_Register = I2C.Read_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER);

	// Set Mask
	uint8_t _Voltage_Register = _Input_Source_Register & 0x87;

	// Set Voltage Register
	float _Voltage = _Input_Voltage - 3.880;
	if (_Voltage >= 0.640) {
		_Voltage_Register |= 0b01000000;
		_Voltage -= 0.640;
	}
	if (_Voltage >= 0.320) {
		_Voltage_Register |= 0b00100000;
		_Voltage -= 0.320;
	}
	if (_Voltage >= 0.160) {
		_Voltage_Register |= 0b00010000;
		_Voltage -= 0.160;
	}
	if (_Voltage >= 0.080) {
		_Voltage_Register |= 0b00001000;
		_Voltage -= 0.080;
	}

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER, _Voltage_Register, true);

	// End Functions
	return(_Response);

}
float BQ24298::Get_Input_Voltage_Limit(void) {

	// Read Curent Charge Register
	uint8_t _Input_Source_Register = I2C.Read_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER);

	// Control for Register Read
	if (_Input_Source_Register = 0xFF) return(NAN);
	
	// Set Mask
	uint8_t _Mask = _Input_Source_Register & 0x78;

	// Set Charge Register
	uint8_t _Voltage_Register = (_Mask / 100.0f) + 3.880;

	// End Functions
	return(_Voltage_Register);

}

// System Voltage Functions
bool BQ24298::Set_Minimum_System_Voltage(float _Minimum_Voltage) {

	// Control for Function Variable
    if (_Minimum_Voltage >= 3.7) _Minimum_Voltage = 3.7;
    if (_Minimum_Voltage <= 3.0) _Minimum_Voltage = 3.0;

	// Read Register
	uint8_t _Current_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Set Mask
	uint8_t _Voltage_Register = _Current_Register & 0xF1;

	// Set Voltage Register
	float _Voltage = _Minimum_Voltage - 3.00;
	if (_Voltage >= 0.4) {
		_Voltage_Register |= 0b00001000;
		_Voltage -= 0.4;
	}
	if (_Voltage >= 0.2) {
		_Voltage_Register |= 0b00000100;
		_Voltage -= 0.2;
	}
	if (_Voltage >= 0.1) {
		_Voltage_Register |= 0b00000010;
		_Voltage -= 0.1;
	}

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _Voltage_Register, true);

	// End Functions
	return(_Response);

}
float BQ24298::Get_Minimum_System_Voltage(void) {

	// Read Curent Charge Register
	uint8_t _Input_Source_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Control for Register Read
	if (_Input_Source_Register = 0xFF) return(NAN);
	
	// Set Mask
	uint8_t _Mask = _Input_Source_Register & 0x0E;

	// Set Charge Register
	uint8_t _Voltage_Register = (_Mask / 20.0f) + 3.0f;

	// End Functions
	return(_Voltage_Register);

}

// Boost Voltage
bool BQ24298::Set_Boost_Voltage(float _Boost_Voltage) {

	// Control for Function Variable
    if (_Boost_Voltage >= 4.550) _Boost_Voltage = 4.550;
    if (_Boost_Voltage <= 4.998) _Boost_Voltage = 4.998;

	// Read Register
	uint8_t _Register = I2C.Read_Register(I2C_ADDRESS, THERMAL_REG_CONTROL_REGISTER);

	// Set Mask
	_Register = _Register & 0xF0;

	// Set Voltage Register
	float _Voltage = _Boost_Voltage - 4.55;
	if (_Voltage >= 0.512) {
		_Register |= 0b10000000;
		_Voltage -= 0.512;
	}
	if (_Voltage >= 0.256) {
		_Register |= 0b01000000;
		_Voltage -= 0.256;
	}
	if (_Voltage >= 0.128) {
		_Register |= 0b00100000;
		_Voltage -= 0.128;
	}
	if (_Voltage >= 0.064) {
		_Register |= 0b00010000;
		_Voltage -= 0.064;
	}

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, THERMAL_REG_CONTROL_REGISTER, _Register, true);

	// End Functions
	return(_Response);

}

// System PreCharge Current Functions
bool BQ24298::Set_PreCharge_Current(float _PreCharge_Current) {

	// Control for Function Variable
    if (_PreCharge_Current > 2.048) _PreCharge_Current = 2.048;
    if (_PreCharge_Current < 0.128) _PreCharge_Current = 0.128;

	// Read Curent Charge Register
	uint8_t _Current_PreCharge_Current_Control_Register = I2C.Read_Register(I2C_ADDRESS, PRECHARGE_CURRENT_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_PreCharge_Current_Control_Register = 0xFF) return(false);
	
	// Set Mask
	uint8_t _Mask = _Current_PreCharge_Current_Control_Register & 0x0F;

	// Set Charge Register
	uint8_t _Charge_Register = (Round(((_PreCharge_Current - 0.128f) / 0.008f)) & 0xF0) | _Mask;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, PRECHARGE_CURRENT_CONTROL_REGISTER, _Charge_Register, false);

	// End Functions
	return(_Response);

}
float BQ24298::Get_PreCharge_Current(void) {

	// Read Curent Charge Register
	uint8_t _Current_PreCharge_Current_Control_Register = I2C.Read_Register(I2C_ADDRESS, PRECHARGE_CURRENT_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_PreCharge_Current_Control_Register = 0xFF) return(NAN);
	
	// Set Mask
	uint8_t _Mask = _Current_PreCharge_Current_Control_Register & 0xF0;

	// Set Charge Register
	float _Charge_Current = (_Mask * 0.008f) + 0.128f;

	// End Functions
	return(_Charge_Current);

}

// Termination Charge Current
bool BQ24298::Set_TermCharge_Current(float _Term_Charge_Current) {

	// Control for Function Variable
    if (_Term_Charge_Current >= 1.024) _Term_Charge_Current = 1.024;
    if (_Term_Charge_Current <= 0.128) _Term_Charge_Current = 0.128;

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, PRECHARGE_CURRENT_CONTROL_REGISTER);

	// Set Mask
	uint8_t _Mask = _Current_Data & 0xF0;

	// Set Current Register
	float _Current = _Term_Charge_Current - 0.128;
	if (_Current >= 0.512) {
		_Mask |= 0b00000100;
		_Current -= 0.512;
	}
	if (_Current >= 0.256) {
		_Mask |= 0b00000010;
		_Current -= 0.256;
	}
	if (_Current >= 0.128) {
		_Mask |= 0b00000001;
		_Current -= 0.128;
	}

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, PRECHARGE_CURRENT_CONTROL_REGISTER, _Mask, true);

	// End Functions
	return(_Response);

}
float BQ24298::Get_TermCharge_Current(void) {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, PRECHARGE_CURRENT_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(NAN);
	
	// Set Mask
	uint8_t _Mask = _Current_Data & 0x0F;

	// Set Charge Register
	float _Data = (_Mask * 0.128f) + 0.128f;

	// End Functions
	return(_Data);

}

// Thermal Regulation Temperature Functions
bool BQ24298::Set_Thermal_Regulation_Temperature(uint8_t _Temperature) {

	// Control for Function Variable
    if (_Temperature > 120) _Temperature = 120;
    if (_Temperature < 60) _Temperature = 60;

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, THERMAL_REG_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);
	
	// Set Mask
	uint8_t _Mask = _Current_Data & 0xFC;

	// Set Charge Register
	uint8_t _Data = (Round(((_Temperature - 60) / 20)) & 0x03) | _Mask;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, THERMAL_REG_CONTROL_REGISTER, _Data, false);

	// End Functions
	return(_Response);

}
int BQ24298::Get_Thermal_Regulation_Temperature() {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, THERMAL_REG_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(NAN);
	
	// Set Mask
	uint8_t _Mask = _Current_Data & 0x03;

	// Set Charge Register
	float _Data = (_Mask * 20) + 60;

	// End Functions
	return(_Data);

}

// Bat FET Functions
bool BQ24298::BATFET_Disable_Bit(bool _State) {

	// Declare Variable
	bool _Response = false;

	if (!_State) _Response = I2C.Clear_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 5, true);
	if (_State) _Response = I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 5, true);

	// End Function
	return(_Response);

}

// DPDM Functions
bool BQ24298::Disable_DPDM() {

	// Set BatFet Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 7, true);

	// End Function
	return(true);

}
bool BQ24298::Enable_DPDM() {

	// Set BatFet Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 7, true);

	// End Function
	return(true);

}

// Bat Fault Interrupt Functions
bool BQ24298::Enable_BatFault_INT(){

	// Set BatFet Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 0, true);

	// End Function
	return(true);

}
bool BQ24298::Disable_BatFault_INT() {

	// Set BatFet Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 0, true);

	// End Function
	return(true);

}

// Charge Fault Interrupt Functions
bool BQ24298::ChargeFault_INT(bool _State) {

	// Declare variable
	bool _Response;

	// Enable Interrupt
	if (_State == true) {

		// Set Bit
		_Response = I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 1, true);

	}
	
	// Disable Interrupt
	if (_State == false) {

		// Clear Bit
		_Response = I2C.Clear_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 1, true);

	}


	// End Function
	return(_Response);

}
uint8_t BQ24298::Get_Charge_Fault(void) {

	// Read Curent Charge Register
	uint8_t _Current_Fault_Register = I2C.Read_Register(I2C_ADDRESS, FAULT_REGISTER);

	// Set Data
	uint8_t _Fault_Register = _Current_Fault_Register & 0x30;

	// End Functions
	return(_Fault_Register);

}

// Watchdog Functions
bool BQ24298::Reset_Watchdog(void) {

	// Set Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, POWERON_CONFIG_REGISTER, 6, true);

	// End Function
	return(true);

}
bool BQ24298::Set_Watchdog(uint8_t _Timer) {

	// Declare Register
	uint8_t _Register;

	// Read Curent Charge Register
	uint8_t _Current_Register = I2C.Read_Register(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER);

	// Handle Timer
	if (_Timer == 0) { // Disable
		_Register = _Current_Register & 0b11001111;
	}
	if (_Timer == 1) { // 40 sn
		_Register = _Current_Register & 0b11001111;
		_Register = _Current_Register | 0b00010000;
	}
	if (_Timer == 2) { // 80 sn
		_Register = _Current_Register & 0b11001111;
		_Register = _Current_Register | 0b00100000;
	}
	if (_Timer == 3) { // 160 sn
		_Register = _Current_Register & 0b11001111;
		_Register = _Current_Register | 0b00110000;
	}

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER, _Register, true);

	// End Functions
	return(_Response);

}

// Control Functions
bool BQ24298::PG_STAT(void) {

	// Get Bit Value
	bool _Response = I2C.Read_Register_Bit(I2C_ADDRESS, SYSTEM_STATUS_REGISTER, 2);
	
	// End Function
	return(_Response);

}
bool BQ24298::THERM_STAT(void) {

	// Get Bit Value
	bool _Response = I2C.Read_Register_Bit(I2C_ADDRESS, SYSTEM_STATUS_REGISTER, 1);
	
	// End Function
	return(_Response);

}
bool BQ24298::DPM_STAT(void) {

	// Get Bit Value
	bool _Response = I2C.Read_Register_Bit(I2C_ADDRESS, SYSTEM_STATUS_REGISTER, 3);
	
	// End Function
	return(_Response);

}
bool BQ24298::WATCHDOG_FAULT(void) {

	// Get Bit Value
	bool _Response = I2C.Read_Register_Bit(I2C_ADDRESS, FAULT_REGISTER, 7);
	
	// End Function
	return(_Response);

}
bool BQ24298::BAT_OVP_FAULT(void) {

	// Get Bit Value
	bool _Response = I2C.Read_Register_Bit(I2C_ADDRESS, FAULT_REGISTER, 3);
	
	// End Function
	return(_Response);

}
bool BQ24298::VSYS_STAT(void) {

	// Get Bit Value
	bool _Response = I2C.Read_Register_Bit(I2C_ADDRESS, SYSTEM_STATUS_REGISTER, 0);
	
	// End Function
	return(_Response);

}
uint8_t BQ24298::NTC_FAULT() {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, FAULT_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);

	// Control for OTG
	if (!Disable_OTG()) return(false);

	// End Function
	return (_Current_Data & 0x07);

}
uint8_t BQ24298::VBUS_STAT(void) {

	// Declare Status Variable
	uint8_t _Status;

	// Read Curent Charge Register
	uint8_t _Register = I2C.Read_Register(I2C_ADDRESS, SYSTEM_STATUS_REGISTER);

	// Handle Data
	if (bitRead(_Register,7) == 0 and bitRead(_Register,6) == 0) _Status = 0;
	if (bitRead(_Register,7) == 0 and bitRead(_Register,6) == 1) _Status = 1;
	if (bitRead(_Register,7) == 1 and bitRead(_Register,6) == 0) _Status = 2;
	if (bitRead(_Register,7) == 1 and bitRead(_Register,6) == 1) _Status = 3;

	// End Function
	return (_Status);

}
uint8_t BQ24298::Charge_Status(void) {

	// Declare Status Variable
	uint8_t _Status;

	// Read Curent Charge Register
	uint8_t _Register = I2C.Read_Register(I2C_ADDRESS, SYSTEM_STATUS_REGISTER);

	// Handle Data
	if (bitRead(_Register,5) == 0 and bitRead(_Register,4) == 0) _Status = 0;
	if (bitRead(_Register,5) == 0 and bitRead(_Register,4) == 1) _Status = 1;
	if (bitRead(_Register,5) == 1 and bitRead(_Register,4) == 0) _Status = 2;
	if (bitRead(_Register,5) == 1 and bitRead(_Register,4) == 1) _Status = 3;

	// End Function
	return (_Status);

}
bool BQ24298::BATLOWV(void) {

	// Get Bit Value
	bool _Response = I2C.Read_Register_Bit(I2C_ADDRESS, CHARGE_VOLTAGE_CONTROL_REGISTER, 1);
	
	// End Function
	return(_Response);

}
bool BQ24298::BCOLD(void) {

	// Get Bit Value
	bool _Response = I2C.Read_Register_Bit(I2C_ADDRESS, CHARGE_CURRENT_CONTROL_REGISTER, 1);
	
	// End Function
	return(_Response);

}
bool BQ24298::Disable_BHOT(void) {

	// Declare Register
	uint8_t _Register;

	// Read Curent Charge Register
	uint8_t _Current_Register = I2C.Read_Register(I2C_ADDRESS, THERMAL_REG_CONTROL_REGISTER);

	// Handle Timer
	_Register = _Current_Register | 0b00001111;

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, THERMAL_REG_CONTROL_REGISTER, _Register, true);

	// End Functions
	return(_Response);

}

uint8_t BQ24298::Get_Fault_Register(void) {

	// Read Curent Charge Register
	uint8_t _Fault_Register = I2C.Read_Register(I2C_ADDRESS, FAULT_REGISTER);

	// End Functions
	return(_Fault_Register);

}

BQ24298 Charger;
