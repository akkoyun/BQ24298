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

	// Define Variable
	bool _Response = false;

	// Check PMIC Version
	if (I2C.Read_Register(I2C_ADDRESS, PMIC_VERSION_REGISTER) != 0x24) _Response = false;

	// Sets the charge current to 100 mA
	_Response = Set_Charge_Current(0.01);

	// End Function
	return(_Response);

}

// Buck Functions
bool BQ24298::Enable_Buck(void) {

	// Clear Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, INPUT_SOURCE_REGISTER, 7);

	// End Function
	return(true);

}
bool BQ24298::Disable_Buck(void) {

	// Set Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, INPUT_SOURCE_REGISTER, 7);

	// End Function
	return(true);

}

// OTG Functions
bool BQ24298::Disable_OTG(void) {

	// Read Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Control for Register Read
	if (_Current_PowerOn_Config_Register = 0xFF) return(false);

	// Set Register
	uint8_t _PowerOn_Config_Register;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00010000;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register, false);

	// End Function
	return(_Response);

}
bool BQ24298::Enable_OTG(void) {

	// Read Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Control for Register Read
	if (_Current_PowerOn_Config_Register = 0xFF) return(false);

	// Set Register
	uint8_t _PowerOn_Config_Register;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00100000;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register, false);

	// Command Delay
	delay(220);

	// End Function
	return(_Response);

}

// Charge Functions
bool BQ24298::Enable_Charge(void) {

	// Read Curent Power On Config Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Control for Register Read
	if (_Current_PowerOn_Config_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0xCF;

	// Write Charge Register
	I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register | 0x10, false);

	// Read Curent Timer Control Register
	uint8_t _Current_Timer_Control_Register = I2C.Read_Register(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Timer_Control_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _Timer_Control_Register = _Current_Timer_Control_Register & 0x7F;

	// Write Timer Register
	I2C.Write_Register(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER, _Timer_Control_Register | 0x80, false);

	// Read Curent Misc Control Register
	uint8_t _Current_Misc_Control_Register = I2C.Read_Register(I2C_ADDRESS, MISC_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Misc_Control_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _Misc_Control_Register = _Current_Misc_Control_Register & 0xFC;

	// Write Misc Register
	I2C.Write_Register(I2C_ADDRESS, MISC_CONTROL_REGISTER, _Misc_Control_Register | 0x03, false);

	// Enable Bat FET
	bool _Response = Enable_BATFET();

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
	I2C.Clear_Register_Bit(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER, 7);

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
	I2C.Set_Register_Bit(I2C_ADDRESS, POWERON_CONFIG_REGISTER, 5);

	// Set Charge Termination Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER, 7);

	// Read Curent Misc Control Register
	uint8_t _Current_Misc_Control_Register = I2C.Read_Register(I2C_ADDRESS, MISC_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Misc_Control_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _Misc_Control_Register = _Current_Misc_Control_Register | 0x03;

	// Write Charge Register
	I2C.Write_Register(I2C_ADDRESS, MISC_CONTROL_REGISTER, _Misc_Control_Register | 0x10, false);

	// Command Delay
	delay(500);

	// End Function
	return(true);

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

	// Control for Register Read
	if (_Current_PowerOn_Config_Register = 0xFF) return(false);

	// Set Register
	uint8_t _PowerOn_Config_Register;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
	_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00010000;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _PowerOn_Config_Register, false);

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
    if (_Charge_Current > 3.008) _Charge_Current = 3.008;
    if (_Charge_Current < 0.512) _Charge_Current = 0.512;

	// Read Curent Charge Register
	uint8_t _Current_Charge_Register = I2C.Read_Register(I2C_ADDRESS, CHARGE_CURRENT_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Charge_Register = 0xFF) return(false);
	
	// Set Mask
	uint8_t _Mask = _Current_Charge_Register & 0x01;

	// Set Charge Register
	uint8_t _Charge_Register = (Round(((_Charge_Current - 0.512) / 0.016)) & 0xFC) | _Mask;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, CHARGE_CURRENT_CONTROL_REGISTER, _Charge_Register, false);

	// End Functions
	return(_Response);

}
float BQ24298::Get_Charge_Current(void) {

	// Read Curent Charge Register
	uint8_t _Current_Charge_Current_Control_Register = I2C.Read_Register(I2C_ADDRESS, CHARGE_CURRENT_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Charge_Current_Control_Register = 0xFF) return(NAN);
	
	// Set Mask
	uint8_t _Mask = _Current_Charge_Current_Control_Register & 0xFC;

	// Set Charge Register
	float _Charge_Current = (_Mask * 0.016f) + 0.512f;

	// End Functions
	return(_Charge_Current);

}
bool BQ24298::Set_Charge_Voltage(float _Charge_Voltage) {

	// Control for Function Variable
    if (_Charge_Voltage > 4.512) _Charge_Voltage = 4.512;
    if (_Charge_Voltage < 3.504) _Charge_Voltage = 3.504;

	// Read Register
	uint8_t _Current_Charge_Voltage_Config_Register = I2C.Read_Register(I2C_ADDRESS, CHARGE_VOLTAGE_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Charge_Voltage_Config_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _Mask = _Current_Charge_Voltage_Config_Register & 0x03;

	// Set Voltage Register
	uint8_t _Voltage_Register = (Round(((_Charge_Voltage - 3.504) / 0.004)) & 0xFC) | _Mask;

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, CHARGE_VOLTAGE_CONTROL_REGISTER, _Voltage_Register, false);

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

	// Control for Register Read
	if (_Input_Source_Register = 0xFF) return(false);
	
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
	bool _Response = I2C.Write_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER, _Current_Value | _Mask, false);

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
    if (_Input_Voltage > 5.008) _Input_Voltage = 5.008;
    if (_Input_Voltage < 3.880) _Input_Voltage = 3.880;

	// Read Curent Charge Register
	uint8_t _Input_Source_Register = I2C.Read_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER);

	// Control for Register Read
	if (_Input_Source_Register = 0xFF) return(false);
	
	// Set Mask
	uint8_t _Mask = _Input_Source_Register & 0x87;

	// Set Voltage Register
	uint8_t _Voltage_Register = (Round(((_Input_Voltage - 3.880) * 100)) & 0x78) | _Mask;

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, INPUT_SOURCE_REGISTER, _Voltage_Register, false);

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
    if (_Minimum_Voltage > 3.7) _Minimum_Voltage = 3.7;
    if (_Minimum_Voltage < 3.0) _Minimum_Voltage = 3.0;

	// Read Register
	uint8_t _Current_PowerOn_Config_Register = I2C.Read_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER);

	// Control for Register Read
	if (_Current_PowerOn_Config_Register = 0xFF) return(false);

	// Set Mask
	uint8_t _Mask = _Current_PowerOn_Config_Register & 0xF0;

	// Set Voltage Register
	uint8_t _Voltage_Register = (Round(((_Minimum_Voltage - 3.0) * 10) * 2) + 1) | _Mask;

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, POWERON_CONFIG_REGISTER, _Voltage_Register, false);

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
    if (_Term_Charge_Current > 2.048) _Term_Charge_Current = 2.048;
    if (_Term_Charge_Current < 0.128) _Term_Charge_Current = 0.128;

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, PRECHARGE_CURRENT_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);
	
	// Set Mask
	uint8_t _Mask = _Current_Data & 0xF0;

	// Set Charge Register
	uint8_t _Data = (Round(((_Term_Charge_Current - 0.128f) / 0.128f)) & 0x0F) | _Mask;

	// Write Charge Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, PRECHARGE_CURRENT_CONTROL_REGISTER, _Data, false);

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
bool BQ24298::Enable_BATFET(void) {

	// Set BatFet Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 5);

	// End Function
	return(true);

}
bool BQ24298::Disable_BATFET(void) {

	// Set BatFet Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 5);

	// End Function
	return(true);

}

// DPDM Functions
bool BQ24298::Disable_DPDM() {

	// Set BatFet Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 7);

	// End Function
	return(true);

}
bool BQ24298::Enable_DPDM() {

	// Set BatFet Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 7);

	// End Function
	return(true);

}

// Bat Fault Interrupt Functions
bool BQ24298::Enable_BatFault_INT(){

	// Set BatFet Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 0);

	// End Function
	return(true);

}
bool BQ24298::Disable_BatFault_INT() {

	// Set BatFet Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 0);

	// End Function
	return(true);

}

// Charge Fault Interrupt Functions
bool BQ24298::Enable_ChargeFault_INT() {

	// Set BatFet Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 1);

	// End Function
	return(true);

}
bool BQ24298::Disable_ChargeFault_INT() {

	// Set BatFet Bit
	I2C.Clear_Register_Bit(I2C_ADDRESS, MISC_CONTROL_REGISTER, 1);

	// End Function
	return(true);

}
uint8_t BQ24298::Get_Charge_Fault() {

	// Read Curent Charge Register
	uint8_t _Current_Fault_Register = I2C.Read_Register(I2C_ADDRESS, FAULT_REGISTER);

	// Control for Register Read
	if (_Current_Fault_Register = 0xFF) return(NAN);

	// Set Data
	uint8_t _Fault_Register = _Current_Fault_Register & 0x30;

	// End Functions
	return(_Fault_Register);

}

// Watchdog Functions
bool BQ24298::Reset_Watchdog() {

	// Set Bit
	I2C.Set_Register_Bit(I2C_ADDRESS, POWERON_CONFIG_REGISTER, 6);

	// End Function
	return(true);

}
bool BQ24298::Disable_Watchdog(void) {

	// Read Curent Charge Register
	uint8_t _Current_Charge_Timer_Control_Register = I2C.Read_Register(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER);

	// Control for Register Read
	if (_Current_Charge_Timer_Control_Register = 0xFF) return(NAN);
	
	// Set Data
	uint8_t _Charge_Timer_Control_Register = _Current_Charge_Timer_Control_Register & 0b11001110;

	// Write Voltage Register
	bool _Response = I2C.Write_Register(I2C_ADDRESS, CHARGE_TIMER_CONTROL_REGISTER, _Charge_Timer_Control_Register, false);

	// End Functions
	return(_Response);

}

// Control Functions
bool BQ24298::is_Power_Good(void) {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, SYSTEM_STATUS_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);

	// Control for Bit
	if (_Current_Data & 0b00000100) return(true);
	
	// End Function
	return(false);

}
bool BQ24298::is_Hot(void) {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, SYSTEM_STATUS_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);

	// Control for Bit
	if (_Current_Data & 0b00000010) return(true);
	
	// End Function
	return(false);

}
bool BQ24298::is_Batt_Connected(void) {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, SYSTEM_STATUS_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);

	// Control for Bit
	if ((_Current_Data & 0x08) == 0x08) return(true);
	
	// End Function
	return(false);

}
bool BQ24298::is_Watchdog_Expired(void) {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, FAULT_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);

	// Control for Bit
	if ((_Current_Data & 0x80)) return(true);
	
	// End Function
	return(false);

}
bool BQ24298::is_Battery_In_Over_Voltage(void) {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, FAULT_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);

	// Control for Bit
	if ((_Current_Data & 0x08)) return(true);
	
	// End Function
	return(false);

}
bool BQ24298::Can_Run_On_Battery() {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, SYSTEM_STATUS_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);

	// Control for Bit
	if ((_Current_Data & 0x01)) return(true);
	
	// End Function
	return(false);

}
uint8_t BQ24298::Has_Battery_Temperature_Fault() {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, FAULT_REGISTER);

	// Control for Register Read
	if (_Current_Data = 0xFF) return(false);

	// Control for OTG
	if (!Disable_OTG()) return(false);

	// End Function
	return (_Current_Data & 0x07);

}

// Other Functions
int BQ24298::USBmode() {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, SYSTEM_STATUS_REGISTER);

	// End Function
	return (_Current_Data & 0xC0);

}
int BQ24298::Charge_Status() {

	// Read Curent Charge Register
	uint8_t _Current_Data = I2C.Read_Register(I2C_ADDRESS, SYSTEM_STATUS_REGISTER);

	// End Function
	return (_Current_Data & 0x30);

}

BQ24298 Charger;
