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
#define _BQ24298_Charge_Voltage_		4.3		// Charge voltage
#define _BQ24298_Input_Current_Limit_	1.0		// Input current limit
#define _BQ24298_Input_Voltage_Limit_	5.08	// Input valtage limit
#define _BQ24298_Min_System_Voltage_	4.0		// Minimum system voltage 
#define _BQ24298_Charge_Term_Current_	0.128	// Charge termination current limit
#define _BQ24298_Watchdog_				0		// Watchdog timer value
#define _BQ24298_BatFet_Disable_		false	// Battery FET disable value
#define _BQ24298_Boost_Voltage_			4.55	// Boost voltage

class BQ24298 : public I2C_Functions {

	private:
		
		/**
		 * @brief Enable / disable buck.
		 * @param State Status
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Buck(bool State) {

			// Define Variable
			bool Stat;

			// Control State
			if (State) {

				// Set Bit
				Stat = Set_Register_Bit(0x00, 7, true);

			} else {

				// Clear Bit
				Stat = Clear_Register_Bit(0x00, 7, true);

			}

			// End Function
			return(Stat);

		}

		/**
		 * @brief Enable OTG.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool OTG(bool State) {

			// Define Variable
			bool Stat;

			// Control State
			if (State) {

				// Set Bit
				Stat = Set_Register_Bit(0x01, 5, true);

			} else {

				// Clear Bit
				Stat = Clear_Register_Bit(0x01, 5, true);

			}

			// End Function
			return(Stat);

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

			// Enable Bat FET
			bool _Response = BATFET(false);

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
			bool Response = Set_Register_Bit(0x01, 5, true); // OTG_CONFIG -> true

			// End Function
			return(Response);

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

			// Read Current Register
			uint8_t _Charge_Register = Read_Register(0x02) & 0b00000011;

			// Serial.println(_Charge_Register, BIN);

			// Handel Data
			if (_Charge_Current >= 2.048) {
				_Charge_Register |= 0b10000000;
				_Charge_Current -= 2.048;
			}
			if (_Charge_Current >= 1.024) {
				_Charge_Register |= 0b01000000;
				_Charge_Current -= 1.024;
			}
			if (_Charge_Current >= 0.512) {
				_Charge_Register |= 0b00100000;
				_Charge_Current -= 0.512;
			}
			if (_Charge_Current >= 0.256) {
				_Charge_Register |= 0b00010000;
				_Charge_Current -= 0.256;
			}
			if (_Charge_Current >= 0.128) {
				_Charge_Register |= 0b00001000;
				_Charge_Current -= 0.128;
			}
			if (_Charge_Current >= 0.064) {
				_Charge_Register |= 0b00000100;
				_Charge_Current -= 0.064;
			}

			// Serial.println(_Charge_Register, BIN);

			// Write Charge Register
			bool _Response = Write_Register(0x02, _Charge_Register, true);

			// Serial.println(_Response);

			// End Functions
			return(_Response);

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
		 * @brief Input current set function.
		 * @param _Input_Current Input current value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Input_Current_Limit(float _Input_Current) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x00) & 0b11111000;

			// Set Current Value
			if (_Input_Current == 0.100) _Input_Source_Register |= 0b00000000;
			if (_Input_Current == 0.150) _Input_Source_Register |= 0b00000001;
			if (_Input_Current == 0.500) _Input_Source_Register |= 0b00000010;
			if (_Input_Current == 0.900) _Input_Source_Register |= 0b00000011;
			if (_Input_Current == 1.000) _Input_Source_Register |= 0b00000100;
			if (_Input_Current == 1.500) _Input_Source_Register |= 0b00000101;
			if (_Input_Current == 2.000) _Input_Source_Register |= 0b00000110;
			if (_Input_Current == 3.000) _Input_Source_Register |= 0b00000111;

			// Write Voltage Register
			bool _Response = Write_Register(0x00, _Input_Source_Register, true);

			// End Functions
			return(_Response);

		}

		/**
		 * @brief Input voltage limit set function
		 * @param _Input_Voltage Input voltage limit value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Set_Input_Voltage_Limit(float _Input_Voltage) {

			// Control for Function Variable
			if (_Input_Voltage >= 5.500) _Input_Voltage = 5.500;
			if (_Input_Voltage <= 3.880) _Input_Voltage = 3.880;

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x00) & 0b10000111;

			// Set Voltage Register
			float _Voltage = _Input_Voltage - 3.880;
			if (_Voltage >= 0.640) {
				_Input_Source_Register |= 0b01000000;
				_Voltage -= 0.640;
			}
			if (_Voltage >= 0.320) {
				_Input_Source_Register |= 0b00100000;
				_Voltage -= 0.320;
			}
			if (_Voltage >= 0.160) {
				_Input_Source_Register |= 0b00010000;
				_Voltage -= 0.160;
			}
			if (_Voltage >= 0.080) {
				_Input_Source_Register |= 0b00001000;
				_Voltage -= 0.080;
			}
			// Write Voltage Register
			bool _Response = Write_Register(0x00, _Input_Source_Register, true);

			// End Functions
			return(_Response);

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
			uint8_t _Current_Register = Read_Register(0x01) & 0b11110001;

			// Set Voltage Register
			float _Voltage = _Minimum_Voltage - 3.00;
			if (_Voltage >= 0.4) {
				_Current_Register |= 0b00001000;
				_Voltage -= 0.4;
			}
			if (_Voltage >= 0.2) {
				_Current_Register |= 0b00000100;
				_Voltage -= 0.2;
			}
			if (_Voltage >= 0.1) {
				_Current_Register |= 0b00000010;
				_Voltage -= 0.1;
			}

			// Write Voltage Register
			bool _Response = Write_Register(0x01, _Current_Register, true);

			// End Functions
			return(_Response);

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
			uint8_t _Register = Read_Register(0x06) & 0b00001111;

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
			if (_PreCharge_Current >= 2.048) _PreCharge_Current = 2.048;
			if (_PreCharge_Current <= 0.128) _PreCharge_Current = 0.128;

			// Read Curent Charge Register
			uint8_t _Current_PreCharge_Current_Control_Register = Read_Register(0x03) & 0b00001111;

			// Set Current Value
			if (_PreCharge_Current == 0.128) _Current_PreCharge_Current_Control_Register |= 0b00010000;
			if (_PreCharge_Current == 0.256) _Current_PreCharge_Current_Control_Register |= 0b00100000;
			if (_PreCharge_Current == 0.384) _Current_PreCharge_Current_Control_Register |= 0b00110000;
			if (_PreCharge_Current == 0.512) _Current_PreCharge_Current_Control_Register |= 0b01000000;
			if (_PreCharge_Current == 0.768) _Current_PreCharge_Current_Control_Register |= 0b01010000;
			if (_PreCharge_Current == 0.896) _Current_PreCharge_Current_Control_Register |= 0b01100000;
			if (_PreCharge_Current == 1.024) _Current_PreCharge_Current_Control_Register |= 0b01110000;
			if (_PreCharge_Current == 1.152) _Current_PreCharge_Current_Control_Register |= 0b10000000;
			if (_PreCharge_Current == 1.280) _Current_PreCharge_Current_Control_Register |= 0b10010000;
			if (_PreCharge_Current == 1.408) _Current_PreCharge_Current_Control_Register |= 0b10100000;
			if (_PreCharge_Current == 1.536) _Current_PreCharge_Current_Control_Register |= 0b10110000;
			if (_PreCharge_Current == 1.664) _Current_PreCharge_Current_Control_Register |= 0b11000000;
			if (_PreCharge_Current == 1.792) _Current_PreCharge_Current_Control_Register |= 0b11010000;
			if (_PreCharge_Current == 1.920) _Current_PreCharge_Current_Control_Register |= 0b11100000;
			if (_PreCharge_Current == 2.048) _Current_PreCharge_Current_Control_Register |= 0b11110000;

			// Write Charge Register
			bool _Response = Write_Register(0x03, _Current_PreCharge_Current_Control_Register, true);

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
			uint8_t _Current_Data = Read_Register(0x03) & 0b11111000;

			// Set Current Register
			float _Current = _Term_Charge_Current - 0.128;
			if (_Current >= 0.512) {
				_Current_Data |= 0b00000100;
				_Current -= 0.512;
			}
			if (_Current >= 0.256) {
				_Current_Data |= 0b00000010;
				_Current -= 0.256;
			}
			if (_Current >= 0.128) {
				_Current_Data |= 0b00000001;
				_Current -= 0.128;
			}

			// Write Charge Register
			bool _Response = Write_Register(0x03, _Current_Data, true);

			// End Functions
			return(_Response);

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
		 * @brief Battery FET disable function.
		 * @param _State FET status value.
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool BATFET(bool State) {

			// Declare Variable
			bool _Response = false;

			if (!State) _Response = Clear_Register_Bit(0x07, 5, true);
			if (State) _Response = Set_Register_Bit(0x07, 5, true);

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

			// Read Curent Charge Register
			uint8_t _Current_Register = Read_Register(0x05) & 0b11001111;

			// Handle Timer
			if (_Timer == 1) { // 40 sn

				// Write Voltage Register
				bool _Response = Write_Register(0x05, (_Current_Register | 0b00010000), true);

				// End Functions
				return(_Response);

			} else if (_Timer == 2) { // 80 sn

				// Write Voltage Register
				bool _Response = Write_Register(0x05, (_Current_Register | 0b00100000), true);

				// End Functions
				return(_Response);

			} else if (_Timer == 3) { // 160 sn

				// Write Voltage Register
				bool _Response = Write_Register(0x05, (_Current_Register | 0b00110000), true);

				// End Functions
				return(_Response);

			}

			// End Functions
			return(false);

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

	public:

		/**
		 * @brief Construct a new BQ24298 object
		 * @param _Multiplexer_Enable I2C device multiplexer enable.
		 * @param _Multiplexer_Channel I2C device multiplexer channel.
		 */
		BQ24298(bool _Boot, bool _Multiplexer_Enable, uint8_t _Multiplexer_Channel) : I2C_Functions(__I2C_Addr_BQ24298__, _Multiplexer_Enable, _Multiplexer_Channel) {

			// Boot
			if (_Boot) {

				// Check PMIC Version
				uint8_t _Version = Read_Register(0x0A);

				// Control for Version
				if (_Version == 0x24) {

					// Set Charge Current
					this->Set_Charge_Current(2.048);

					// Set Charge Voltage
					this->Set_Charge_Voltage(4.208);

					// Set Charger Input Current Limit		
					this->Set_Input_Current_Limit(3.000);

					// Set Charger Input Voltage Limit
					this->Set_Input_Voltage_Limit(4.360);

					// Set Minimum System Voltage
					this->Set_Minimum_System_Voltage(3.700);

					// Set Boost Voltage
					this->Set_Boost_Voltage(4.200);

					// Set Pre Charge Current
					this->Set_PreCharge_Current(0.256);

					// Set Charge Termination Current
					this->Set_TermCharge_Current(0.128);

					// Enable Boost
					this->Enable_Boost_Mode();
					
					// Enable Charge
					this->Enable_Charge();

					// Disable Watchdog
					this->Set_Watchdog(0);

					// Disable Boost Temperature
					this->Disable_BHOT();
					
					// Disable OTG
					this->OTG(false);

				}

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
			
//			Serial.print(_Response, BIN);

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
		bool NTC_FAULT() {

			// Get Bit Value
			bool _Response_0 = Read_Register_Bit(0x09, 0);

			// Get Bit Value
			bool _Response_1 = Read_Register_Bit(0x09, 01);

			// End Function
			return (_Response_0 and _Response_1);

		}

		/**
		 * @brief Get charge current.
		 * @return float Charge current.
		 */
		float Get_Charge_Current(void) {

			// Read Current Register
			uint8_t _Charge_Register = Read_Register(0x06);

			// Declare Variable
			float _Charge_Current = 0.512;

			// Set Value
			if (bitRead(_Charge_Register, 2)) _Charge_Current += 0.064;
			if (bitRead(_Charge_Register, 3)) _Charge_Current += 0.128;
			if (bitRead(_Charge_Register, 4)) _Charge_Current += 0.256;
			if (bitRead(_Charge_Register, 5)) _Charge_Current += 0.512;
			if (bitRead(_Charge_Register, 6)) _Charge_Current += 1.024;
			if (bitRead(_Charge_Register, 7)) _Charge_Current += 2.048;

			// End Functions
			return(_Charge_Current);

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
		 * @brief Input current get function.
		 * @return float Input current value.
		 */
		float Get_Input_Current_Limit(void) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x00);

			// Set Mask
			uint8_t _Mask = _Input_Source_Register & 0b00000111;

			// Decide Response
			switch (_Mask) {
				case 0x00:
					return(0.100);
				case 0x01:
					return(0.150);
				case 0x02:
					return(0.500);
				case 0x03:
					return(0.900);
				case 0x04:
					return(1.200);
				case 0x05:
					return(1.500);
				case 0x06:
					return(2.000);
				case 0x07:
					return(3.000);
				default:
					return NAN;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Input voltage limit get function.
		 * @return float Input voltage limit value.
		 */
		float Get_Input_Voltage_Limit(void) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x00);
			
			// Set Voltage Variable
			float _Voltage = 3.88;

			// Set Value
			if (bitRead(_Input_Source_Register, 3)) _Voltage += 0.080;
			if (bitRead(_Input_Source_Register, 4)) _Voltage += 0.160;
			if (bitRead(_Input_Source_Register, 5)) _Voltage += 0.320;
			if (bitRead(_Input_Source_Register, 6)) _Voltage += 0.640;

			// End Functions
			return(_Voltage);

		}

		/**
		 * @brief Minimum input voltage get function
		 * @return float Minimum system voltage value.
		 */
		float Get_Minimum_System_Voltage(void) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x01);
			
			// Set Voltage Variable
			float _Voltage = 3.00;

			// Set Value
			if (bitRead(_Input_Source_Register, 1)) _Voltage += 0.1;
			if (bitRead(_Input_Source_Register, 2)) _Voltage += 0.2;
			if (bitRead(_Input_Source_Register, 3)) _Voltage += 0.4;

			// End Functions
			return(_Voltage);

		}

		/**
		 * @brief Boost voltage get function
		 * @return float boost voltage value.
		 */
		float Get_Boost_Voltage(void) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x06);
			
			// Set Voltage Variable
			float _Voltage = 4.55;

			// Set Value
			if (bitRead(_Input_Source_Register, 4)) _Voltage += 0.064;
			if (bitRead(_Input_Source_Register, 5)) _Voltage += 0.128;
			if (bitRead(_Input_Source_Register, 6)) _Voltage += 0.256;
			if (bitRead(_Input_Source_Register, 7)) _Voltage += 0.512;

			// End Functions
			return(_Voltage);

		}

		/**
		 * @brief PreCharge current get function.
		 * @return float precharge current value.
		 */
		float Get_PreCharge_Current_Limit(void) {

			// Read Curent Charge Register
			uint8_t _Input_Source_Register = Read_Register(0x03);

			// Set Mask
			uint8_t _Mask = _Input_Source_Register & 0b11110000;

			// Decide Response
			switch (_Mask) {
				case 0b00000000:
					return(0.128);
				case 0b00010000:
					return(0.128);
				case 0b00100000:
					return(0.256);
				case 0b00110000:
					return(0.384);
				case 0b01000000:
					return(0.512);
				case 0b01010000:
					return(0.768);
				case 0b01100000:
					return(0.896);
				case 0b01110000:
					return(1.024);
				case 0b10000000:
					return(1.152);
				case 0b10010000:
					return(1.280);
				case 0b10100000:
					return(1.408);
				case 0b10110000:
					return(1.536);
				case 0b11000000:
					return(1.664);
				case 0b11010000:
					return(1.792);
				case 0b11100000:
					return(1.920);
				case 0b11110000:
					return(2.048);
				default:
					return NAN;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Charge termination current get function.
		 * @return float Charge termination current value.
		 */
		float Get_ChargeTerm_Current(void) {

			// Read Curent Charge Register
			uint8_t _Current_Data = Read_Register(0x03) & 0b11111000;

			// Set Voltage Variable
			float _Current = 0.128;

			// Set Value
			if (bitRead(_Current_Data, 0)) _Current += 0.128;
			if (bitRead(_Current_Data, 1)) _Current += 0.256;
			if (bitRead(_Current_Data, 2)) _Current += 0.512;

			// End Functions
			return(_Current);

		}

		/**
		 * @brief Thermal regulation temperature get function.
		 * @return int Regulation temperature value.
		 */
		float Get_Thermal_Regulation_Temperature() {

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
		 * @brief Get buck state
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Get_Buck_State(void) {

			// Get Bit
			bool Stat = Read_Register_Bit(0x00, 7);

			// End Function
			return(Stat);

		}

		/**
		 * @brief Get OTG state
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Get_OTG_State(void) {

			// Get Bit
			bool Stat = Read_Register_Bit(0x01, 5);

			// End Function
			return(Stat);

		}

		/**
		 * @brief Get BATFET state
		 * @return true Function success.
		 * @return false Function not success.
		 */
		bool Get_BATFET_State(void) {

			// Get Bit
			bool Stat = Read_Register_Bit(0x07, 5);

			// End Function
			return(Stat);

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
