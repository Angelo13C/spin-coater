/*
 * driver.cpp
 *
 *  Created on: Jun 25, 2024
 *      Author: Angelo
 */

#include <drivers/mct8316z/driver.h>

MCT8316Z::MCT8316Z(MCT8316ZSpi spi, Pwm pwm, TIM_HandleTypeDef* speedFeedbackFGOUT) : spi(spi), pwm(pwm), speedFeedbackFGOUT(speedFeedbackFGOUT) {
	motorSpeedPid.setP(Parameter { .gain = 1, .limit = 1 });
	motorSpeedPid.setI(Parameter { .gain = 1, .limit = 1 });
	motorSpeedPid.setD(Parameter { .gain = 1, .limit = 1 });

}

MCT8316Z::~MCT8316Z() {
	// TODO Auto-generated destructor stub
}

void MCT8316Z::tick()
{
	uint16_t currentMotorSpeed = readSpeedFeedbackInRpm();
	float newDutyCycle = motorSpeedPid.compute(currentMotorSpeed);

	pwm.setDutyCycle(Percentage::from0To1(newDutyCycle));
}

void MCT8316Z::setSpeed(uint16_t rpm) {
	motorSpeedPid.setTheSetPoint((float) rpm);
}

uint16_t MCT8316Z::readSpeedFeedbackInRpm()
{
	uint32_t currentTimeInMillis = HAL_GetTick();
	uint32_t currentTimeInSeconds = currentTimeInMillis / 1000;
	uint16_t millisSubpart = currentTimeInMillis - currentTimeInSeconds * 1000;
	if(millisSubpart == 0)
		millisSubpart = 1;
	float percentageOfSecond = 1.0f / (((float) millisSubpart) / 1000.0f);

	// Convert the rotations per second in rotations per minute
	const uint16_t RPS_TO_RPM = 60;
	uint16_t resultWithoutFgoutSetting = (uint16_t) (percentageOfSecond * ((float) RPS_TO_RPM * __HAL_TIM_GET_COUNTER(speedFeedbackFGOUT)));

	// Reset the timer every second
	if(currentTimeInSeconds > lastTimeInSecondsOfSpeedRead)
	{
		lastTimeInSecondsOfSpeedRead = currentTimeInSeconds;
		__HAL_TIM_SET_COUNTER(speedFeedbackFGOUT, 0);
	}

	auto fgoutSetting = spi.getFGOUTFrequencyOverCommutationFrequencySetting();
	switch(fgoutSetting)
	{
		case FGOUTFrequencyOverCommutationFrequency::X0Dot25:
			return resultWithoutFgoutSetting * 4;
		case FGOUTFrequencyOverCommutationFrequency::X0Dot5:
			return resultWithoutFgoutSetting * 2;
		case FGOUTFrequencyOverCommutationFrequency::X1:
			return resultWithoutFgoutSetting;
		case FGOUTFrequencyOverCommutationFrequency::X3:
			return resultWithoutFgoutSetting / 3;
	}

	return 0;
}
