/*
 * pin.h
 *
 *  Created on: Jun 13, 2024
 *      Author: Angelo
 */

#ifndef INC_PIN_H_
#define INC_PIN_H_

#include "stm32f4xx_hal.h"

/**
 * @struct Color565
 * @brief A structure to represent a 16-bit color value in 5-6-5 RGB format.
 */
struct Color565
{
	uint16_t value;
};

#endif /* INC_PIN_H_ */
