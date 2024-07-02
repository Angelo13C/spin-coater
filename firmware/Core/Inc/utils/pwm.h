/*
 * pwm.h
 *
 *  Created on: Jun 25, 2024
 *      Author: Angelo
 */

#ifndef INC_UTILS_PWM_H_
#define INC_UTILS_PWM_H_


#include "stm32f4xx_hal.h"
#include "percentage.h"

struct Pwm
{
	TIM_HandleTypeDef* htim;
	uint32_t channel;

	void setDutyCycle(Percentage dutyCycle)
	{
	    __HAL_TIM_SET_COMPARE(htim, channel, htim->Init.Period * dutyCycle.into0To1());
	}
};


#endif /* INC_UTILS_PWM_H_ */
