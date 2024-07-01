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
 * @struct Pin
 * @brief A structure to represent a GPIO pin with its port and pin number.
 */
struct Pin
{
	GPIO_TypeDef* port; ///< The GPIO port of the pin.
	uint16_t pin;       ///< The pin number.
};

#endif /* INC_PIN_H_ */
