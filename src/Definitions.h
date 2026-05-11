#ifndef __BQ24298_DEFINITIONS__
#define __BQ24298_DEFINITIONS__

	// Include Arduino Library
	#ifndef Arduino_h
		#include <Arduino.h>
	#endif

	// Register addresses
	namespace BQ24298_Reg {

		// Input Source Control Register
		constexpr uint8_t INPUT_SOURCE_CTRL = 0x00;

		// Power-On Configuration Register
		constexpr uint8_t POWER_ON_CONFIG = 0x01;

		// Charge Current Control Register
		constexpr uint8_t CHARGE_CURRENT_CTRL = 0x02;

		// Pre-charge / Termination Current Register
		constexpr uint8_t PRECHARGE_TERM_CURRENT = 0x03;

		// Charge Voltage Control Register
		constexpr uint8_t CHARGE_VOLTAGE_CTRL = 0x04;

		// Charge Termination / Timer Control Register
		constexpr uint8_t CHARGE_TERM_TIMER_CTRL = 0x05;

		// Misc Operation Control Register
		constexpr uint8_t MISC_OPERATION_CTRL = 0x06;

		// System VBAT Monitor Register
		constexpr uint8_t SYSTEM_VBAT_MONITOR = 0x07;

		// System Status Register (read-only)
		constexpr uint8_t SYSTEM_STATUS = 0x08;

		// Fault Register (read-only)
		constexpr uint8_t FAULT = 0x09;

		// Vendor / Part / Revision Status Register (read-only)
		constexpr uint8_t VENDOR_PART_REV = 0x0A;

	}

	// Bit positions for each register (used for bit manipulation)
	namespace BQ24298_Bit {

		// Input Source Control (0x00) bits
		constexpr uint8_t INPUT_CURRENT_LIMIT_START = 0;		// bits 2:0
		constexpr uint8_t INPUT_VOLTAGE_LIMIT_START = 3;		// bits 6:3
		constexpr uint8_t EN_HIZ = 7;							// bit 7

		// Power-On Config (0x01) bits
		constexpr uint8_t CHG_CONFIG = 4;						// bits 5:4
		constexpr uint8_t OTG_ENABLE = 5;						// bit 5
		constexpr uint8_t WATCHDOG_TIMER_START = 4;				// bits 5:4
		constexpr uint8_t WATCHDOG_RESET = 6;					// bit 6
		constexpr uint8_t VSYSMIN_START = 1;					// bits 3:1

		// Charge Current (0x02) bits
		constexpr uint8_t CHARGE_CURRENT_START = 2;				// bits 7:2

		// Pre-charge/Term Current (0x03) bits
		constexpr uint8_t PRECHARGE_CURRENT_START = 4;			// bits 7:4
		constexpr uint8_t TERM_CURRENT_START = 0;				// bits 2:0

		// Charge Voltage (0x04) bits
		constexpr uint8_t CHARGE_VOLTAGE_START = 2;				// bits 7:2
		constexpr uint8_t BATLOWV = 1;							// bit 1

		// Termination/Timer Control (0x05) bits
		constexpr uint8_t CHG_TERM_EN = 7;						// bit 7
		constexpr uint8_t WATCHDOG_START = 4;					// bits 5:4

		// Misc Operation (0x06) bits
		constexpr uint8_t BOOST_VOLTAGE_START = 4;				// bits 7:4
		constexpr uint8_t THERMAL_REG_START = 0;				// bits 1:0
		constexpr uint8_t DPDM_EN = 7;							// bit 7
		constexpr uint8_t BATFET_DISABLE = 5;					// bit 5
		constexpr uint8_t BATFET_INT_EN = 0;					// bit 0
		constexpr uint8_t CHG_FAULT_INT_EN = 1;					// bit 1

		// System Status (0x08) bits (read-only)
		constexpr uint8_t VSYS_STAT = 0;						// bit 0
		constexpr uint8_t THERM_STAT = 1;						// bit 1
		constexpr uint8_t PG_STAT = 2;							// bit 2
		constexpr uint8_t DPM_STAT = 3;							// bit 3
		constexpr uint8_t CHG_STAT_START = 4;					// bits 5:4
		constexpr uint8_t VBUS_STAT_START = 6;					// bits 7:6

		// Fault Register (0x09) bits (read-only)
		constexpr uint8_t NTC_FAULT_START = 0;					// bits 1:0
		constexpr uint8_t BAT_FAULT = 3;						// bit 3
		constexpr uint8_t CHG_FAULT_START = 4;					// bits 5:4
		constexpr uint8_t WATCHDOG_FAULT = 7;					// bit 7

	}

	// I2C communication parameters
	namespace BQ24298_I2C {

		// Default I2C address (ADDR pin low)
		constexpr uint8_t ADDR_DEFAULT = 0x6B;

		// Alternative I2C address (ADDR pin high)
		constexpr uint8_t ADDR_ALT = 0x6A;

		// Expected device version register 0x0A
		constexpr uint8_t DEVICE_VERSION = 0x24;

	}

	// Error codes for BQ24298 operations	
	enum class BQ24298_Error : uint8_t {
		SUCCESS = 0,					// Operation successful
		DEVICE_NOT_FOUND = 1,			// BQ24298 not detected on I2C bus
		INVALID_VERSION = 2,			// Device version mismatch (expected 0x24)
		INVALID_PARAMETER = 3,			// Parameter value out of valid range
		I2C_ERROR = 4,					// I2C bus communication error
		REGISTER_READ_FAILED = 5,		// Failed to read device register
		REGISTER_WRITE_FAILED = 6,		// Failed to write device register
		INITIALIZATION_FAILED = 7,		// Device initialization sequence failed
		THERMAL_FAULT = 8,				// Thermal shutdown condition detected
		BATTERY_OVP = 9,				// Battery over-voltage protection triggered
		WATCHDOG_EXPIRED = 10,			// Watchdog timer expired (no I2C activity)
		NTC_FAULT = 11,					// NTC thermistor fault detected
		CHARGE_FAULT = 12,				// Charger fault detected
		INPUT_FAULT = 13,				// Input source fault
		UNSPECIFIED_ERROR = 255			// Unknown or unspecified error
	};

	// Charge status enumeration for user-friendly status reporting	
	enum class BQ24298_ChargeStatus : uint8_t {
		NOT_CHARGING = 0,				// Not charging
		PRE_CHARGE = 1,					// Pre-charge mode (cold/weak battery)
		FAST_CHARGE = 2,				// Fast charge mode
		CHARGE_TERMINATION = 3			// Charge termination (complete)
	};

	// Thermal regulation temperature options
	enum class BQ24298_VbusStatus : uint8_t {
		UNKNOWN = 0,					// Unknown or no input
		USB_INPUT = 1,					// USB input (5V)
		ADAPTER_INPUT = 2,				// Adapter input (>5V)
		OTG_MODE = 3					// OTG mode (output)
	};

	// Thermal regulation temperature options
	enum class BQ24298_ThermalReg : uint8_t {
		TEMP_60C = 0,					// Thermal regulation at 60°C
		TEMP_80C = 1,					// Thermal regulation at 80°C
		TEMP_100C = 2,					// Thermal regulation at 100°C
		TEMP_120C = 3					// Thermal regulation at 120°C
	};

	// Watchdog timer options
	enum class BQ24298_WatchdogTimer : uint8_t {
		DISABLED = 0,					// Watchdog disabled
		TIMEOUT_40S = 1,				// 40 second timeout
		TIMEOUT_80S = 2,				// 80 second timeout
		TIMEOUT_160S = 3				// 160 second timeout
	};

	// Input current limit table (PROGMEM)
	static const uint16_t BQ24298_InputCurrent_mA_PROGMEM[] PROGMEM = {
		100,		// 0b000: 100 mA
		150,		// 0b001: 150 mA
		500,		// 0b010: 500 mA
		900,		// 0b011: 900 mA
		1200,		// 0b100: 1.2 A
		1500,		// 0b101: 1.5 A
		2000,		// 0b110: 2.0 A
		3000		// 0b111: 3.0 A
	};

	// Pre-charge Current Table (PROGMEM)
	static const uint16_t BQ24298_PrechargeCurrent_mA_PROGMEM[] PROGMEM = {
		128,		// 0b0000: 128 mA
		128,		// 0b0001: 128 mA (duplicate)
		256,		// 0b0010: 256 mA
		384,		// 0b0011: 384 mA
		512,		// 0b0100: 512 mA
		768,		// 0b0101: 768 mA
		896,		// 0b0110: 896 mA
		1024,		// 0b0111: 1024 mA
		1152,		// 0b1000: 1152 mA
		1280,		// 0b1001: 1280 mA
		1408,		// 0b1010: 1408 mA
		1536,		// 0b1011: 1536 mA
		1664,		// 0b1100: 1664 mA
		1792,		// 0b1101: 1792 mA
		1920,		// 0b1110: 1920 mA
		2048		// 0b1111: 2048 mA
	};

	// Input Voltage Limit Table (PROGMEM)
	static const uint16_t BQ24298_InputVoltage_mV_PROGMEM[] PROGMEM = {
		3880,		// 0b0000: 3.88V
		3960,		// 0b0001: 3.96V
		4040,		// 0b0010: 4.04V
		4120,		// 0b0011: 4.12V
		4200,		// 0b0100: 4.20V
		4280,		// 0b0101: 4.28V
		4360,		// 0b0110: 4.36V
		4440,		// 0b0111: 4.44V
		4520,		// 0b1000: 4.52V
		4600,		// 0b1001: 4.60V
		4680,		// 0b1010: 4.68V
		4760,		// 0b1011: 4.76V
		4840,		// 0b1100: 4.84V
		4920,		// 0b1101: 4.92V
		5000,		// 0b1110: 5.00V
		5080		// 0b1111: 5.08V
	};

	// Charge Voltage Table (PROGMEM)
	static const uint16_t BQ24298_ChargeVoltage_mV_PROGMEM[] PROGMEM = {
		3504, 3520, 3536, 3552, 3568, 3584, 3600, 3616,
		3632, 3648, 3664, 3680, 3696, 3712, 3728, 3744,
		3760, 3776, 3792, 3808, 3824, 3840, 3856, 3872,
		3888, 3904, 3920, 3936, 3952, 3968, 3984, 4000,
		4016, 4032, 4048, 4064, 4080, 4096, 4112, 4128,
		4144, 4160, 4176, 4192, 4208, 4224, 4240, 4256,
		4272, 4288, 4304, 4320, 4336, 4352, 4368, 4384,
		4400, 4416, 4432, 4448, 4464, 4480, 4496, 4512
	};

	// Timing parameters for I2C communication and device operation
	namespace BQ24298_Timing {

		// Delay between I2C retries
		constexpr uint16_t I2C_RETRY_DELAY_MS = 10;

		// Max I2C retry attempts
		constexpr uint8_t I2C_MAX_RETRIES = 3;

		// Delay after initialization
		constexpr uint16_t INIT_SEQUENCE_DELAY_MS = 100;

		// Reset watchdog every 60s
		constexpr uint16_t WATCHDOG_RESET_INTERVAL_MS = 60000;

		// Status check interval
		constexpr uint16_t STATUS_UPDATE_INTERVAL_MS = 1000;

	}

	// Threshold values for input validation and error handling
	namespace BQ24298_Threshold {

		// Charge Current Range (mA)
		constexpr uint16_t CHARGE_CURRENT_MIN = 512;       // Min: 512 mA
		constexpr uint16_t CHARGE_CURRENT_MAX = 3584;      // Max: 3.584 A
		constexpr uint8_t CHARGE_CURRENT_STEP = 64;        // Step: 64 mA

		// Charge Voltage Range (mV)
		constexpr uint16_t CHARGE_VOLTAGE_MIN = 3504;      // Min: 3.504V
		constexpr uint16_t CHARGE_VOLTAGE_MAX = 4208;      // Max: 4.208V (Li-Ion nominal)
		constexpr uint8_t CHARGE_VOLTAGE_STEP = 16;        // Step: 16 mV

		// Input Current Range (mA)
		constexpr uint16_t INPUT_CURRENT_MIN = 100;        // Min: 100 mA
		constexpr uint16_t INPUT_CURRENT_MAX = 3000;       // Max: 3000 mA

		// Input Voltage Range (mV)
		constexpr uint16_t INPUT_VOLTAGE_MIN = 3880;       // Min: 3.88V
		constexpr uint16_t INPUT_VOLTAGE_MAX = 5080;       // Max: 5.08V
		constexpr uint8_t INPUT_VOLTAGE_STEP = 80;         // Step: 80 mV

		// System Voltage Range (mV)
		constexpr uint16_t VSYSMIN_MIN = 3000;             // Min: 3.0V
		constexpr uint16_t VSYSMIN_MAX = 3700;             // Max: 3.7V
		constexpr uint8_t VSYSMIN_STEP = 100;              // Step: 100 mV

		// Boost Voltage Range (mV)
		constexpr uint16_t BOOST_VOLTAGE_MIN = 4550;       // Min: 4.55V
		constexpr uint16_t BOOST_VOLTAGE_MAX = 5510;       // Max: 5.51V
		constexpr uint16_t BOOST_VOLTAGE_STEP = 64;        // Step: 64 mV

		// Temperature Thresholds (°C)
		constexpr uint8_t THERMAL_REG_MIN = 60;            // Min: 60°C
		constexpr uint8_t THERMAL_REG_MAX = 120;           // Max: 120°C

	}

#endif /* __BQ24298_DEFINITIONS__ */
