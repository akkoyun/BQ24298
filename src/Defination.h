#ifndef __BQ24298_DEFINATION__
#define __BQ24298_DEFINATION__

// Define Round Function
#ifndef Round
#define Round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#endif

//Default PMIC (BQ24298) I2C address
#define I2C_ADDRESS							0x6B

// Register address definitions
#define INPUT_SOURCE_REGISTER				0x00
#define POWERON_CONFIG_REGISTER				0x01
#define CHARGE_CURRENT_CONTROL_REGISTER		0x02
#define PRECHARGE_CURRENT_CONTROL_REGISTER	0x03
#define CHARGE_VOLTAGE_CONTROL_REGISTER		0x04
#define CHARGE_TIMER_CONTROL_REGISTER		0x05
#define THERMAL_REG_CONTROL_REGISTER		0x06
#define MISC_CONTROL_REGISTER				0x07
#define SYSTEM_STATUS_REGISTER				0x08
#define FAULT_REGISTER						0x09
#define PMIC_VERSION_REGISTER				0x0A

// Charge Fault Status
#define NO_CHARGE_FAULT						0x00 // 0b 0000 0000
#define INPUT_OVER_VOLTAGE					0x10 // 0b 0001 0000
#define THERMAL_SHUTDOWN					0x20 // 0b 0010 0000
#define CHARGE_SAFETY_TIME_EXPIRED			0x30 // 0b 0011 0000

// Termal Fault Status
#define NO_TEMPERATURE_FAULT 				0x00 // 0b 0000 0000
#define LOWER_THRESHOLD_TEMPERATURE_FAULT	0x05 // 0b 0000 0101
#define HIGHER_THRESHOLD_TEMPERATURE_FAULT	0x06 // 0b 0000 0110

// Charging status
#define NOT_CHARGING						0x00 // 0b 0000 0000
#define PRE_CHARGING						0x10 // 0b 0001 0000
#define FAST_CHARGING						0x20 // 0b 0010 0000
#define CHARGE_TERMINATION_DONE				0x30 // 0b 0011 0000

// Voltage BUS status
#define UNKNOWN_MODE						0x00 // 0b 0000 0000
#define USB_HOST_MODE						0x40 // 0b 0100 0000
#define ADAPTER_PORT_MODE					0x80 // 0b 1000 0000
#define BOOST_MODE							0xC0 // 0b 1100 0000

enum Current_Limit_mask {
    CURRENT_LIM_100 						= 0x00,
    CURRENT_LIM_150,
    CURRENT_LIM_500,
    CURRENT_LIM_900,
    CURRENT_LIM_1200,
    CURRENT_LIM_1500,
    CURRENT_LIM_2000,
    CURRENT_LIM_3000,
};
#endif
