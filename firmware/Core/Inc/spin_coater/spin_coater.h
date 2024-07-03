/*
 * spin_coater.h
 *
 *  Created on: Jun 16, 2024
 *      Author: Angelo
 */

#ifndef SRC_SPIN_COATER_SPIN_COATER_H_
#define SRC_SPIN_COATER_SPIN_COATER_H_

#include "drivers/mct8316z/driver.h"
#include "drivers/keypad.h"
#include "spin_coating_process.h"
#include "display.h"
#include <limits>
#include <functional>

static constexpr uint32_t START_FLASH_ADDRESS = 0x080E0000;
static constexpr uint32_t SPIN_COATING_NOT_STARTED = std::numeric_limits<uint32_t>::max();

/**
 * @brief Class representing a Spin Coater device.
 */
class SpinCoater {
public:
    /**
     * @brief Constructs a SpinCoater object with keypad, motor driver, and display.
     *
     * @param keypad Keypad used for user interaction.
     * @param motorDriver Motor driver (MCT8316Z) used for controlling spin coating.
     * @param display Display used for user interface.
     */
	SpinCoater(Keypad<1, 1> keypad, MCT8316Z motorDriver, Display display);
	virtual ~SpinCoater();

    /**
     * @brief Creates a SpinCoater object with default configuration.
     *
     * @param mct8316zSpiHandle SPI handle for MCT8316Z driver.
     * @param ili9341SpiHandle SPI handle for ILI9341 display.
     * @param motorPwmTimer PWM timer handle for motor speed control.
     * @param motorSpeedFeedbackTimer Timer handle for motor speed feedback.
     * @return SpinCoater object with default configuration.
     */
	static SpinCoater withDefaultConfiguration(SPI_HandleTypeDef* mct8316zSpiHandle, SPI_HandleTypeDef* ili9341SpiHandle, SPI_HandleTypeDef* xpt2046SpiHandle, TIM_HandleTypeDef* motorPwmTimer, TIM_HandleTypeDef* motorSpeedFeedbackTimer);

    /**
     * @brief Performs periodic tasks for the SpinCoater device.
     */
	void tick();

    /**
     * @brief Starts the spin coating process.
     */
	inline void startSpinCoating() { spinCoatingStartTime = HAL_GetTick(); }
private:
    /**
     * @brief Finishes the spin coating process.
     */
	inline void finishSpinCoating() { spinCoatingStartTime = SPIN_COATING_NOT_STARTED; }

    /**
     * @brief Checks if the spin coating process is currently active.
     *
     * @return true if spin coating is active, false otherwise.
     */
	inline bool isSpinCoating() { return spinCoatingStartTime == SPIN_COATING_NOT_STARTED;  }

private:
	Keypad<1, 1> keypad;
	uint16_t* keypadNumber = nullptr;

	MCT8316Z motorDriver;

	Display display;

	SpinCoatingProcess process = SpinCoatingProcess(START_FLASH_ADDRESS);
	uint32_t spinCoatingStartTime = SPIN_COATING_NOT_STARTED;

	std::function<void()> onStartButtonPressedFn;
	std::function<void()> onStopButtonPressedFn;
};

#endif /* SRC_SPIN_COATER_SPIN_COATER_H_ */
