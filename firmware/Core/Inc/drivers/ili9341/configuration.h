/*
 * configuration.h
 *
 *  Created on: Jun 16, 2024
 *      Author: Angelo
 */

#ifndef INC_ILI9341_CONFIGURATION_H_
#define INC_ILI9341_CONFIGURATION_H_


enum class Orientation
{
	Landscape = 0b00101000,
	LandscapeFlipped = 0b11101000,
	Portrait = 0b01001000,
	PortraitFlipped = 0b10001000,
};

enum class BitsPerPixel
{
	Bits16 = 0b01010101,
	Bits18 = 0b01100110,
};


#endif /* INC_ILI9341_CONFIGURATION_H_ */
