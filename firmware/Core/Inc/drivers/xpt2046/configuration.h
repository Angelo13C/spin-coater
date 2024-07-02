/*
 * configuration.h
 *
 *  Created on: Jun 28, 2024
 *      Author: Angelo
 */

#ifndef INC_DRIVERS_XPT2046_CONFIGURATION_H_
#define INC_DRIVERS_XPT2046_CONFIGURATION_H_

#include <cstdint>

enum class ChannelSelect : uint8_t
{
	XPosition = 0b00000101,
	YPosition = 0b00000001,
	Z1Position = 0b00000011,
	Z2Position = 0b00000100,
};

enum class Resolution : uint8_t
{
	Bit8 = 1,
	Bit12 = 0,
};

enum class ReferenceMode : uint8_t
{
	Differential = 0,
	SingleEnded = 1,
};

enum class PowerDownMode : uint8_t
{
	PowerDownBetweenConversions = 0b00,
	ReferenceOffAdcOn = 0b01,
	ReferenceOnAdcOff = 0b10,
	/// It means reference on and ADC on.
	DeviceAlwaysPowered = 0b11,
};



#endif /* INC_DRIVERS_XPT2046_CONFIGURATION_H_ */
