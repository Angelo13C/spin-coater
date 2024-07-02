/*
 * driver.h
 *
 *  Created on: Jun 25, 2024
 *      Author: Angelo
 */

#ifndef INC_DRIVERS_MCT8316Z_DRIVER_H_
#define INC_DRIVERS_MCT8316Z_DRIVER_H_

#include "spi.h"
#include "../../utils/pid.h"
#include "../../utils/pwm.h"

/**
 * @class MCT8316Z
 * @brief An ESC motor driver for the MCT8316Z motor controller.
 *
 * This class provides the interface and functionality for controlling an
 * ESC motor using the MCT8316Z motor controller. It uses SPI for communication
 * and PWM for motor control.
 */
class MCT8316Z {
public:
    /**
     * @brief Constructor for MCT8316Z class.
     *
     * Initializes the motor driver with the given SPI, PWM, and timer handler for speed feedback.
     *
     * @param spi SPI interface for communication with the motor controller.
     * @param pwm PWM interface for controlling the motor speed.
     * @param speedFeedbackFGOUT Timer handler for speed feedback.
     */
	MCT8316Z(MCT8316ZSpi spi, Pwm pwm, TIM_HandleTypeDef* speedFeedbackFGOUT);
	virtual ~MCT8316Z();

    /**
     * @brief Function to be called periodically for updating motor control.
     *
     * This function handles the periodic tasks needed for motor control,
     * such as reading speed feedback and adjusting the motor speed.
     */
	void tick();

    /**
     * @brief Sets the motor speed in RPM.
     *
     * This function sets the desired motor speed in revolutions per minute (RPM).
     *
     * @param rpm Desired motor speed in RPM.
     */
	void setSpeed(uint16_t rpm);

private:
    /**
     * @brief Reads the current motor speed from the feedback in RPM.
     *
     * This function reads the speed feedback and converts it to RPM.
     *
     * @return Current motor speed in RPM.
     */
	uint16_t readSpeedFeedbackInRpm();

private:
	MCT8316ZSpi spi;  ///< SPI interface for communication with the motor controller.
	Pid motorSpeedPid = Pid(0, 1); ///< PID controller for motor speed control.
	Pwm pwm;  ///< PWM interface for controlling the motor speed.

	TIM_HandleTypeDef* speedFeedbackFGOUT;  ///< Timer handler for speed feedback.
	uint32_t lastTimeInSecondsOfSpeedRead = 0;  ///< Last timestamp of speed feedback read.
};

#endif /* INC_DRIVERS_MCT8316Z_DRIVER_H_ */
