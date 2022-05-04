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

// Define Charger Parameters
#define _BQ24298_Input_Current_Limit_	3.0		// Input current limit
#define _BQ24298_Input_Voltage_Limit_	5.08	// Input valtage limit
#define _BQ24298_Min_System_Voltage_	4.0		// Minimum system voltage 
#define _BQ24298_Charge_Voltage_		4.3		// Charge voltage
#define _BQ24298_Charge_Current_		2		// Charge current limit
#define _BQ24298_Charge_Term_Current_	0.128	// Charge termination current limit
#define _BQ24298_Watchdog_				0		// Watchdog timer value
#define _BQ24298_BatFet_Disable_		false	// Battery FET disable value
#define _BQ24298_Boost_Voltage_			4.55	// Boost voltage

class BQ24298 : public I2C_Functions {

	private:

		/**
		 * @brief Enable buck.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Enable_Buck(void) {

			// Clear Bit
			Clear_Register_Bit(0x00, 7, true);

			// End Function
			return(true);

		}

		/**
		 * @brief Disable buck.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Disable_Buck(void) {

			// Set Bit
			Set_Register_Bit(0x00, 7, true);

			// End Function
			return(true);

		}

		/**
		 * @brief Enable OTG.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Enable_OTG(void) {

			// Read Register
			uint8_t _Current_PowerOn_Config_Register = Read_Register(0x01);

			// Set Register
			uint8_t _PowerOn_Config_Register;
			_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
			_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00100000;

			// Write Charge Register
			bool _Response = Write_Register(0x01, _PowerOn_Config_Register, true);

			// Command Delay
			delay(220);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Disable OTG.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Disable_OTG(void) {

			// Read Register
			uint8_t _Current_PowerOn_Config_Register = Read_Register(0x01);

			// Set Register
			uint8_t _PowerOn_Config_Register;
			_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
			_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00010000;

			// Write Charge Register
			bool _Response = Write_Register(0x01, _PowerOn_Config_Register, true);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Enable charge.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Enable_Charge(void) {

			// Set Charge Bits
			Set_Register_Bit(0x01, 4, true);
			Set_Register_Bit(0x05, 7, true);
			Set_Register_Bit(0x07, 0, true);
			Set_Register_Bit(0x07, 1, true);

			// Enable Bat FET
			bool _Response = BATFET_Disable_Bit(false);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Disable charge.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Disable_Charge(void) {

			// Read Curent Power On Config Register
			uint8_t _Current_PowerOn_Config_Register = Read_Register(0x01);

			// Control for Register Read
			if (_Current_PowerOn_Config_Register == 0xFF) return(false);

			// Set Mask
			uint8_t _PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0xCF;

			// Write Charge Register
			Write_Register(0x01, _PowerOn_Config_Register, false);

			// Set Charge Termination Bit
			Clear_Register_Bit(0x05, 7, true);

			// Read Curent Power On Config Register
			uint8_t _Current_Misc_Control_Register = Read_Register(0x07);

			// Control for Register Read
			if (_Current_Misc_Control_Register == 0xFF) return(false);

			// Set Mask
			uint8_t _Misc_Control_Register = _Current_Misc_Control_Register & 0xFC;

			// Write Charge Register
			bool _Response = Write_Register(0x07, _Misc_Control_Register, false);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Enable boost mode.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Enable_Boost_Mode(void) {

			// Set OTG Config Bit
			bool _Response1 = Set_Register_Bit(0x01, 5, true);

			// Set Charge Termination Bit
			bool _Response2 = Clear_Register_Bit(0x05, 7, true);

			// Read Curent Misc Control Register
			uint8_t _Current_Misc_Control_Register = Read_Register(0x07);

			// Set Mask
			uint8_t _Misc_Control_Register = _Current_Misc_Control_Register | 0x03;

			// Write Charge Register
			bool _Response3 = Write_Register(0x07, _Misc_Control_Register | 0x08, true);

			// Command Delay
			delay(500);

			// End Function
			return(_Response1 & _Response2 & _Response3);

		}

		/**
		 * @brief Disable boost mode.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Disable_Boost_Mode(void) {

			// Read Curent Misc Control Register
			uint8_t _Current_Power_On_Control_Register = Read_Register(0x01);

			// Control for Register Read
			if (_Current_Power_On_Control_Register == 0xFF) return(false);

			// Set Mask
			uint8_t _Power_On_Control_Register = _Current_Power_On_Control_Register & 0xCF;

			// Write Charge Register
			bool _Response = Write_Register(0x01, _Power_On_Control_Register | 0x10, false);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Enable charging.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Enable_Charging(void) {

			// Read Register
			uint8_t _Current_PowerOn_Config_Register = Read_Register(0x01);

			// Set Register
			uint8_t _PowerOn_Config_Register;
			_PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0b11001111;
			_PowerOn_Config_Register = _Current_PowerOn_Config_Register | 0b00010000;

			// Write Charge Register
			bool _Response = Write_Register(0x01, _PowerOn_Config_Register, true);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Disable charging.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Disable_Charging(void) {

			// Read Register
			uint8_t _Current_PowerOn_Config_Register = Read_Register(0x01);

			// Control for Register Read
			if (_Current_PowerOn_Config_Register == 0xFF) return(false);

			// Set Register
			uint8_t _PowerOn_Config_Register = _Current_PowerOn_Config_Register & 0xCF;

			// Write Charge Register
			bool _Response = Write_Register(0x01, _PowerOn_Config_Register, false);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Set charge current.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Charge_Current(float _Charge_Current) {

			// Control for Function Variable
			if (_Charge_Current >= 3.008) _Charge_Current = 3.008;
			if (_Charge_Current <= 0.512) _Charge_Current = 0.512;

			// Read Curent Charge Register
			uint8_t _Current_Data = Read_Register(0x02);

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
			bool _Response = Write_Register(0x02, _Charge_Register, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Get charge current.
		 * @return float Charge current.
		 */
		float Get_Charge_Current(void) {

			// Declare Variable
			float _Charge_Current = 0.512;

			// Read Curent Charge Register
			uint8_t _Current_Data = Read_Register(0x02);

			// Set Value
			if (bitRead(_Current_Data, 2) == true) _Charge_Current += 0.064;
			if (bitRead(_Current_Data, 3) == true) _Charge_Current += 0.128;
			if (bitRead(_Current_Data, 4) == true) _Charge_Current += 0.256;
			if (bitRead(_Current_Data, 5) == true) _Charge_Current += 0.512;
			if (bitRead(_Current_Data, 6) == true) _Charge_Current += 1.024;
			if (bitRead(_Current_Data, 7) == true) _Charge_Current += 2.048;

			// End Functions
			return(_Charge_Current);

		}

		/**
		 * @brief Set charge voltage.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Charge_Voltage(float _Charge_Voltage) {

			// Control for Function Variable
			if (_Charge_Voltage >= 4.512) _Charge_Voltage = 4.512;
			if (_Charge_Voltage <= 3.504) _Charge_Voltage = 3.504;

			// Read Register
			uint8_t _Current_Register = Read_Register(0x04);

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
			bool _Response = Write_Register(0x04, _Voltage_Register, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Get charge voltage.
		 * @return float Charge voltage.
		 */
		float Get_Charge_Voltage(void) {

			// Read Curent Charge Register
			uint8_t _Current_Charge_Voltage_Control_Register = Read_Register(0x04);

			// Control for Register Read
			if (_Current_Charge_Voltage_Control_Register == 0xFF) return(NAN);
			
			// Set Mask
			uint8_t _Mask = _Current_Charge_Voltage_Control_Register & 0xFC;

			// Set Charge Register
			float _Charge_Voltage_Control_Register = (_Mask * 0.004f) + 3.504f;

			// End Functions
			return(_Charge_Voltage_Control_Register);

		}

		/**
		 * @brief Input current set function.
		 * @param _Input_Current Input current value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Input_Current_Limit(float _Input_Current) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x00);

			// Set Mask
			uint8_t _Mask = _Input_Source_Register & 0xF8;

			// Set Current Value
			uint8_t _Current_Value = 0x00;

			// Set Current Value
			if (_Input_Current > 0.015) _Current_Value = 0x01;
			if (_Input_Current >= 0.5) _Current_Value = 0x02;
			if (_Input_Current >= 0.9) _Current_Value = 0x03;
			if (_Input_Current >= 1.2) _Current_Value = 0x04;
			if (_Input_Current >= 1.5) _Current_Value = 0x05;
			if (_Input_Current >= 2.0) _Current_Value = 0x06;
			if (_Input_Current >= 3.0) _Current_Value = 0x07;

			// Write Voltage Register
			bool _Response = Write_Register(0x00, _Current_Value | _Mask, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Input current get function.
		 * @return float Input current value.
		 */
		float Get_Input_Current_Limit(void) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x00);

			// Control for Register Read
			if (_Input_Source_Register == 0xFF) return(NAN);
			
			// Set Mask
			uint8_t _Mask = _Input_Source_Register & 0x07;

			// Decide Response
			switch (_Mask) {
				case 0x00:
					return 0.1;
				case 0x01:
					return 0.015;
				case 0x02:
					return 0.5;
				case 0x03:
					return 0.9;
				case 0x04:
					return 1.2;
				case 0x05:
					return 1.5;
				case 0x06:
					return 2.0;
				case 0x07:
					return 3.0;
				default:
					return NAN;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Input voltage limit set function
		 * @param _Input_Voltage Input voltage limit value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Input_Voltage_Limit(float _Input_Voltage) {

			// Control for Function Variable
			if (_Input_Voltage >= 5.080) _Input_Voltage = 5.080;
			if (_Input_Voltage <= 3.880) _Input_Voltage = 3.880;

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x00);

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
			bool _Response = Write_Register(0x00, _Voltage_Register, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Input voltage limit get function.
		 * @return float Input voltage limit value.
		 */
		float Get_Input_Voltage_Limit(void) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x00);

			// Control for Register Read
			if (_Input_Source_Register == 0xFF) return(NAN);
			
			// Set Mask
			uint8_t _Mask = _Input_Source_Register & 0x78;

			// Set Charge Register
			uint8_t _Voltage_Register = (_Mask / 100.0f) + 3.880;

			// End Functions
			return(_Voltage_Register);

		}

		/**
		 * @brief Minimum input voltage set function
		 * @param _Minimum_Voltage Minimum system voltage value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Minimum_System_Voltage(float _Minimum_Voltage) {

			// Control for Function Variable
			if (_Minimum_Voltage >= 3.7) _Minimum_Voltage = 3.7;
			if (_Minimum_Voltage <= 3.0) _Minimum_Voltage = 3.0;

			// Read Register
			uint8_t _Current_Register = Read_Register(0x01);

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
			bool _Response = Write_Register(0x01, _Voltage_Register, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Minimum input voltage get function
		 * @return float Minimum system voltage value.
		 */
		float Get_Minimum_System_Voltage(void) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x01);

			// Control for Register Read
			if (_Input_Source_Register == 0xFF) return(NAN);
			
			// Set Mask
			uint8_t _Mask = _Input_Source_Register & 0x0E;

			// Set Charge Register
			uint8_t _Voltage_Register = (_Mask / 20.0f) + 3.0f;

			// End Functions
			return(_Voltage_Register);

		}

		/**
		 * @brief Boost voltage set function
		 * @param _Boost_Voltage Boost voltage value
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Boost_Voltage(float _Boost_Voltage) {

			// Control for Function Variable
			if (_Boost_Voltage >= 4.550) _Boost_Voltage = 4.550;
			if (_Boost_Voltage <= 4.998) _Boost_Voltage = 4.998;

			// Read Register
			uint8_t _Register = Read_Register(0x06);

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
			bool _Response = Write_Register(0x06, _Register, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Precharge current set function
		 * @param _PreCharge_Current Precharge current value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_PreCharge_Current(float _PreCharge_Current) {

			// Control for Function Variable
			if (_PreCharge_Current > 2.048) _PreCharge_Current = 2.048;
			if (_PreCharge_Current < 0.128) _PreCharge_Current = 0.128;

			// Read Curent Charge Register
			uint8_t _Current_PreCharge_Current_Control_Register = Read_Register(0x03);

			// Control for Register Read
			if (_Current_PreCharge_Current_Control_Register == 0xFF) return(false);
			
			// Set Mask
			uint8_t _Mask = _Current_PreCharge_Current_Control_Register & 0x0F;

			// Set Charge Register
			uint8_t _Charge_Register = (BQ_Round(((_PreCharge_Current - 0.128f) / 0.008f)) & 0xF0) | _Mask;

			// Write Charge Register
			bool _Response = Write_Register(0x03, _Charge_Register, false);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Precharge current get function.
		 * @return float Precharge current value.
		 */
		float Get_PreCharge_Current(void) {

			// Read Curent Charge Register
			uint8_t _Current_PreCharge_Current_Control_Register = Read_Register(0x03);

			// Control for Register Read
			if (_Current_PreCharge_Current_Control_Register == 0xFF) return(NAN);
			
			// Set Mask
			uint8_t _Mask = _Current_PreCharge_Current_Control_Register & 0xF0;

			// Set Charge Register
			float _Charge_Current = (_Mask * 0.008f) + 0.128f;

			// End Functions
			return(_Charge_Current);

		}

		/**
		 * @brief Charge termination current set function.
		 * @param _Term_Charge_Current Charge termination current value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_TermCharge_Current(float _Term_Charge_Current) {

			// Control for Function Variable
			if (_Term_Charge_Current >= 1.024) _Term_Charge_Current = 1.024;
			if (_Term_Charge_Current <= 0.128) _Term_Charge_Current = 0.128;

			// Read Curent Charge Register
			uint8_t _Current_Data = Read_Register(0x03);

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
			bool _Response = Write_Register(0x03, _Mask, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Charge termination current get function.
		 * @return float Charge termination current value.
		 */
		float Get_TermCharge_Current(void) {

			// Read Curent Charge Register
			uint8_t _Current_Data = Read_Register(0x03);

			// Control for Register Read
			if (_Current_Data == 0xFF) return(NAN);
			
			// Set Mask
			uint8_t _Mask = _Current_Data & 0x0F;

			// Set Charge Register
			float _Data = (_Mask * 0.128f) + 0.128f;

			// End Functions
			return(_Data);

		}

		/**
		 * @brief Thermal regulation temperature set function.
		 * @param _Temperature Regulation temperature value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Thermal_Regulation_Temperature(uint8_t _Temperature) {

			// Control for Function Variable
			if (_Temperature > 120) _Temperature = 120;
			if (_Temperature < 60) _Temperature = 60;

			// Read Curent Charge Register
			uint8_t _Current_Data = Read_Register(0x06);

			// Control for Register Read
			if (_Current_Data == 0xFF) return(false);
			
			// Set Mask
			uint8_t _Mask = _Current_Data & 0xFC;

			// Set Charge Register
			uint8_t _Data = (BQ_Round(((_Temperature - 60) / 20)) & 0x03) | _Mask;

			// Write Charge Register
			bool _Response = Write_Register(0x06, _Data, false);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Thermal regulation temperature get function.
		 * @return int Regulation temperature value.
		 */
		int Get_Thermal_Regulation_Temperature() {

			// Read Curent Charge Register
			uint8_t _Current_Data = Read_Register(0x06);

			// Control for Register Read
			if (_Current_Data == 0xFF) return(0);
			
			// Set Mask
			uint8_t _Mask = _Current_Data & 0x03;

			// Set Charge Register
			float _Data = (_Mask * 20) + 60;

			// End Functions
			return(_Data);

		}

		/**
		 * @brief Battery FET disable function.
		 * @param _State FET status value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool BATFET_Disable_Bit(bool _State) {

			// Declare Variable
			bool _Response = false;

			if (!_State) _Response = Clear_Register_Bit(0x07, 5, true);
			if (_State) _Response = Set_Register_Bit(0x07, 5, true);

			// End Function
			return(_Response);

		}

		/**
		 * @brief DPDM disable function.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Disable_DPDM() {

			// Set BatFet Bit
			Clear_Register_Bit(0x07, 7, true);

			// End Function
			return(true);

		}

		/**
		 * @brief DPDM enable function.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Enable_DPDM() {

			// Set BatFet Bit
			Set_Register_Bit(0x07, 7, true);

			// End Function
			return(true);

		}

		/**
		 * @brief Battery fault interrupt enable function.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Enable_BatFault_INT(){

			// Set BatFet Bit
			Set_Register_Bit(0x07, 0, true);

			// End Function
			return(true);

		}

		/**
		 * @brief Battery fault interrupt disable function.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Disable_BatFault_INT() {

			// Set BatFet Bit
			Clear_Register_Bit(0x07, 0, true);

			// End Function
			return(true);

		}

		/**
		 * @brief Charge fault interrupt set function.
		 * @param _State Interrupt status value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool ChargeFault_INT(bool _State) {

			// Declare variable
			bool _Response;

			// Enable Interrupt
			if (_State == true) {

				// Set Bit
				_Response = Set_Register_Bit(0x07, 1, true);

			}
			
			// Disable Interrupt
			if (_State == false) {

				// Clear Bit
				_Response = Clear_Register_Bit(0x07, 1, true);

			}


			// End Function
			return(_Response);

		}

		/**
		 * @brief Charge fault get function.
		 * @return uint8_t Fault status value.
		 */
		uint8_t Get_Charge_Fault(void) {

			// Read Curent Charge Register
			uint8_t _Current_Fault_Register = Read_Register(0x09);

			// Set Data
			uint8_t _Fault_Register = _Current_Fault_Register & 0x30;

			// End Functions
			return(_Fault_Register);

		}

		/**
		 * @brief Reset watchdog function.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Reset_Watchdog(void) {

			// Set Bit
			Set_Register_Bit(0x01, 6, true);

			// End Function
			return(true);

		}

		/**
		 * @brief Watchdog timer set function.
		 * @param _Timer Watchdog timer value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Watchdog(uint8_t _Timer) {

			// Declare Register
			uint8_t _Register;

			// Read Curent Charge Register
			uint8_t _Current_Register = Read_Register(0x05);

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
			bool _Response = Write_Register(0x05, _Register, true);

			// End Functions
			return(_Response);

		}

	public:

		/**
		 * @brief Construct a new BQ24298 object
		 * @param _Multiplexer_Enable I2C device multiplexer enable.
		 * @param _Multiplexer_Channel I2C device multiplexer channel.
		 */
		BQ24298(bool _Multiplexer_Enable, uint8_t _Multiplexer_Channel) : I2C_Functions(__I2C_Addr_BQ24298__, _Multiplexer_Enable, _Multiplexer_Channel) {

			// Check PMIC Version
			uint8_t _Version = Read_Register(0x0A);

			// Control for Version
			if (_Version == 0x24) {

				// Set Charger Input Current Limit		
				this->Set_Input_Current_Limit(_BQ24298_Input_Current_Limit_);
				
				// Set Charger Input Voltage Limit
				this->Set_Input_Voltage_Limit(_BQ24298_Input_Voltage_Limit_);
				
				// Set Minimum System Voltage
				this->Set_Minimum_System_Voltage(_BQ24298_Min_System_Voltage_);

				// Set Charge Voltage
				this->Set_Charge_Voltage(_BQ24298_Charge_Voltage_);
				
				// Disable Boost Temperature
				this->Disable_BHOT();

				// Set Charge Current
				this->Set_Charge_Current(_BQ24298_Charge_Current_);

				// Set Charge Termination Current
				this->Set_TermCharge_Current(_BQ24298_Charge_Term_Current_);

				// Disable Watchdog
				this->Set_Watchdog(_BQ24298_Watchdog_);
				
				// Enable BatFet
				this->BATFET_Disable_Bit(_BQ24298_BatFet_Disable_);

				// Enable Charge
				this->Enable_Charge();

				// Set Boost Voltage
				this->Set_Boost_Voltage(_BQ24298_Boost_Voltage_);

				// Enable Boost
				this->Enable_Boost_Mode();

				// Disable OTG
				this->Disable_OTG();

			}
		
		}

		/**
		 * @brief Power good control function.
		 * @return true Control function ok.
		 * @return false Control function fail.
		 */
		bool PG_STAT(void) {

			// Get Bit Value
			bool _Response = Read_Register_Bit(0x08, 2);
			
			// End Function
			return(_Response);

		}

		/**
		 * @brief Thermal status control function.
		 * @return true Control function ok.
		 * @return false Control function fail.
		 */
		bool THERM_STAT(void) {

			// Get Bit Value
			bool _Response = Read_Register_Bit(0x08, 1);
			
			// End Function
			return(_Response);

		}

		/**
		 * @brief DPM status control function.
		 * @return true Control function ok.
		 * @return false Control function fail.
		 */
		bool DPM_STAT(void) {

			// Get Bit Value
			bool _Response = Read_Register_Bit(0x08, 3);
			
			// End Function
			return(_Response);

		}

		/**
		 * @brief Watchdog fault control function.
		 * @return true Control function ok.
		 * @return false Control function fail.
		 */
		bool WATCHDOG_FAULT(void) {

			// Get Bit Value
			bool _Response = Read_Register_Bit(0x09, 7);
			
			// End Function
			return(_Response);

		}

		/**
		 * @brief OVP control function.
		 * @return true Control function ok.
		 * @return false Control function fail.
		 */
		bool BAT_OVP_FAULT(void) {

			// Get Bit Value
			bool _Response = Read_Register_Bit(0x09, 3);
			
			// End Function
			return(_Response);

		}

		/**
		 * @brief Status control function.
		 * @return true Control function ok.
		 * @return false Control function fail.
		 */
		bool VSYS_STAT(void) {

			// Get Bit Value
			bool _Response = Read_Register_Bit(0x08, 0);
			
			// End Function
			return(_Response);

		}

		/**
		 * @brief Low voltage control function.
		 * @return true Control function ok.
		 * @return false Control function fail.
		 */
		bool BATLOWV(void) {

			// Get Bit Value
			bool _Response = Read_Register_Bit(0x04, 1);
			
			// End Function
			return(_Response);

		}

		/**
		 * @brief Battery low temperature control function.
		 * @return true Control function ok.
		 * @return false Control function fail.
		 */
		bool BCOLD(void) {

			// Get Bit Value
			bool _Response = Read_Register_Bit(0x02, 1);
			
			// End Function
			return(_Response);

		}

		/**
		 * @brief Battery NTC fault get function.
		 * @return uint8_t NTC fault value.
		 */
		uint8_t NTC_FAULT() {

			// Read Curent Charge Register
			uint8_t _Current_Data = Read_Register(0x09);

			// Control for Register Read
			if (_Current_Data == 0xFF) return(false);

			// Control for OTG
			if (!Disable_OTG()) return(false);

			// End Function
			return (_Current_Data & 0x07);

		}

		/**
		 * @brief Battery voltage bus status function.
		 * @return uint8_t VBus status value.
		 */
		uint8_t VBUS_STAT(void) {

			// Declare Status Variable
			uint8_t _Status;

			// Read Curent Charge Register
			uint8_t _Register = Read_Register(0x08);

			// Handle Data
			if (bitRead(_Register,7) == 0 and bitRead(_Register,6) == 0) _Status = 0;
			if (bitRead(_Register,7) == 0 and bitRead(_Register,6) == 1) _Status = 1;
			if (bitRead(_Register,7) == 1 and bitRead(_Register,6) == 0) _Status = 2;
			if (bitRead(_Register,7) == 1 and bitRead(_Register,6) == 1) _Status = 3;

			// End Function
			return (_Status);

		}

		/**
		 * @brief Battery charge status control function.
		 * @return uint8_t Charge status value.
		 */
		uint8_t Charge_Status(void) {

			// Declare Status Variable
			uint8_t _Status;

			// Read Curent Charge Register
			uint8_t _Register = Read_Register(0x08);

			// Handle Data
			if (bitRead(_Register,5) == 0 and bitRead(_Register,4) == 0) _Status = 0;
			if (bitRead(_Register,5) == 0 and bitRead(_Register,4) == 1) _Status = 1;
			if (bitRead(_Register,5) == 1 and bitRead(_Register,4) == 0) _Status = 2;
			if (bitRead(_Register,5) == 1 and bitRead(_Register,4) == 1) _Status = 3;

			// End Function
			return (_Status);

		}

		/**
		 * @brief Disable battery over heat fault function.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Disable_BHOT(void) {

			// Declare Register
			uint8_t _Register;

			// Read Curent Charge Register
			uint8_t _Current_Register = Read_Register(0x06);

			// Handle Timer
			_Register = _Current_Register | 0b00001111;

			// Write Voltage Register
			bool _Response = Write_Register(0x06, _Register, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Battery fault status get function.
		 * @return uint8_t Fault status value.
		 */
		uint8_t Get_Fault_Register(void) {

			// Read Curent Charge Register
			uint8_t _Fault_Register = Read_Register(0x09);

			// End Functions
			return(_Fault_Register);

		}

};

#endif /* defined(__BQ24298__) */
