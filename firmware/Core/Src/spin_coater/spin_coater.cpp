/*
 * spin_coater.cpp
 *
 *  Created on: Jun 16, 2024
 *      Author: Angelo
 */

#include "../../Inc/spin_coater/spin_coater.h"

template <size_t ROWS, size_t COLUMNS>
static void getPressedDigit(const Keypad<ROWS, COLUMNS>& keypad, std::function<void (int8_t)> digitPressedFn)
{
  keypad.getPressedButtons([&](ButtonAddress pressedButton) {
	  if(pressedButton.row < 4)
		  digitPressedFn(1 + pressedButton.column + pressedButton.row * 3);
	  else if(pressedButton.column != 0 && pressedButton.column != 2)
		  digitPressedFn(0);
  });
}

SpinCoater::SpinCoater(Keypad<1, 1> keypad, MCT8316Z motorDriver, Display display)
	: keypad(keypad), motorDriver(motorDriver), display(display),
	  onStartButtonPressedFn([this] { this->startSpinCoating(); }),
	  onStopButtonPressedFn([this] { this->finishSpinCoating(); })
{
	display.configure(process.getPoints(), onStartButtonPressedFn, onStopButtonPressedFn, &keypadNumber);
}

SpinCoater::~SpinCoater()
{
	// TODO Auto-generated destructor stub
}

SpinCoater SpinCoater::withDefaultConfiguration(SPI_HandleTypeDef* mct8316zSpiHandle, SPI_HandleTypeDef* ili9341SpiHandle, SPI_HandleTypeDef* xpt2046SpiHandle, TIM_HandleTypeDef* motorPwmTimer, TIM_HandleTypeDef* motorSpeedFeedbackTimer)
{
	// TODO: choose which pins to use for the things connected to the microcontroller. For now I've just put GPIOF and GPIO_PIN_0 as a placeholder.

	Keypad<1, 1> keypad(std::array<Pin, 1>{{
		GPIOF,
		GPIO_PIN_0
	}}, std::array<Pin, 1>{{
		GPIOF,
		GPIO_PIN_0
	}});

	const Pin mct8316zCsPin = { .port = GPIOF, .pin = GPIO_PIN_0 };
	MCT8316ZSpi mct8316zSpi(mct8316zCsPin, mct8316zSpiHandle);
	Pwm motorSpeedPwm = { .htim = motorPwmTimer, .channel = TIM_CHANNEL_1 };
	MCT8316Z motorDriver(mct8316zSpi, motorSpeedPwm, motorSpeedFeedbackTimer);

	const Pin ili9341CsPin = { .port = GPIOF, .pin = GPIO_PIN_0 };
	const Pin ili9341DCPin = { .port = GPIOF, .pin = GPIO_PIN_0 };
	const Pin ili9341ResetPin = { .port = GPIOF, .pin = GPIO_PIN_0 };
	const Pin ili9341BacklightPin = { .port = GPIOF, .pin = GPIO_PIN_0 };
	ILI9341 ili9341(ili9341SpiHandle, ili9341CsPin, ili9341DCPin, ili9341ResetPin, ili9341BacklightPin, Orientation::Landscape);

	const Pin xpt2046CsPin = { .port = GPIOF, .pin = GPIO_PIN_0 };
	XPT2046 xpt2046(xpt2046SpiHandle, xpt2046CsPin);

	Display display(ili9341, xpt2046);

	return SpinCoater(keypad, motorDriver, display);
}

void SpinCoater::tick()
{
	display.tick();

	if(isSpinCoating())
	{
		uint32_t spinCoatingTime = HAL_GetTick() - spinCoatingStartTime;
		bool hasProcessEnded = false;
		uint16_t targetSpeed = process.getSpeedInRpmAtTime(spinCoatingTime / 1000, hasProcessEnded);

		motorDriver.setSpeed(targetSpeed);

		if(hasProcessEnded)
			finishSpinCoating();
	}
	else
	{
		if(keypadNumber != nullptr)
		{
			getPressedDigit(keypad, [&](int8_t pressedDigit) {
				*keypadNumber *= 10;
				*keypadNumber += pressedDigit;

				this->display.refreshLabel();
			});
		}
	}

	motorDriver.tick();
}
