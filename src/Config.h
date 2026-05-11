#ifndef __BQ24298_CONFIG__
#define __BQ24298_CONFIG__

	// Include Arduino Library
	#ifndef Arduino_h
		#include <Arduino.h>
	#endif

	// BQ24298 Configuration Namespace
	namespace BQ24298_Config {

		// ==================== Charge Parameters ====================

        // Charge current in mA (multiply by 0.001 to get Amps)
		constexpr uint16_t Charge_Current_mA = 2048;  // 2.048A

        // Charge voltage in mV (multiply by 0.001 to get Volts)
		constexpr uint16_t Charge_Voltage_mV = 4208;  // 4.208V

        // Input current limit in mA
		constexpr uint16_t Input_Current_Limit_mA = 3000;  // 3.0A

        // Input voltage limit in mV
		constexpr uint16_t Input_Voltage_Limit_mV = 5080;  // 5.08V

        // Minimum system voltage in mV
		constexpr uint16_t Min_System_Voltage_mV = 3800;  // 3.8V

        // Boost mode voltage in mV
		constexpr uint16_t Boost_Voltage_mV = 4200;  // 4.20V

        // Pre-charge current in mA
		constexpr uint16_t PreCharge_Current_mA = 256;  // 0.256A

        // Charge termination current in mA
		constexpr uint16_t Charge_Term_Current_mA = 128;  // 0.128A

        // Watchdog timer (0=disabled, 1=40s, 2=80s, 3=160s)
		constexpr uint8_t Watchdog_Timer = 0;  // Disabled

	}

	// For backward compatibility with existing code
	#define _BQ24298_Charge_Current_ (BQ24298_Config::Charge_Current_mA * 0.001f)
	#define _BQ24298_Charge_Voltage_ (BQ24298_Config::Charge_Voltage_mV * 0.001f)
	#define _BQ24298_Input_Current_Limit_ (BQ24298_Config::Input_Current_Limit_mA * 0.001f)
	#define _BQ24298_Input_Voltage_Limit_ (BQ24298_Config::Input_Voltage_Limit_mV * 0.001f)
	#define _BQ24298_Min_System_Voltage_ (BQ24298_Config::Min_System_Voltage_mV * 0.001f)
	#define _BQ24298_Boost_Voltage_ (BQ24298_Config::Boost_Voltage_mV * 0.001f)
	#define _BQ24298_PreCharge_Current_ (BQ24298_Config::PreCharge_Current_mA * 0.001f)
	#define _BQ24298_Charge_Term_Current_ (BQ24298_Config::Charge_Term_Current_mA * 0.001f)
	#define _BQ24298_Watchdog_ BQ24298_Config::Watchdog_Timer

	// Default I2C address for BQ24298 (ADDR pin low)
	#define __I2C_Addr_BQ24298__ 0x6B  // Default I2C address (ADDR pin low)

#endif /* __BQ24298_CONFIG__ */