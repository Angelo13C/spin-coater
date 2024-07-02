/*
 * driver.cpp
 *
 *  Created on: Jun 28, 2024
 *      Author: Angelo
 */

#include "drivers/xpt2046/driver.h"

XPT2046::XPT2046(SPI_HandleTypeDef* spi, Pin csPin)
	: XPT2046(spi, csPin, Resolution::Bit12, ReferenceMode::Differential, PowerDownMode::PowerDownBetweenConversions)
{ }

XPT2046::XPT2046(SPI_HandleTypeDef* spi, Pin csPin, Resolution resolution, ReferenceMode referenceMode, PowerDownMode powerDownMode)
	: spi(spi), csPin(csPin), resolution(resolution), referenceMode(referenceMode), powerDownMode(powerDownMode)
{ }

XPT2046::~XPT2046() {
	// TODO Auto-generated destructor stub
}

bool XPT2046::readSingleTouchSample(uint16_t& x, uint16_t& y)
{
	auto read_axis = [&](uint16_t& axis, ChannelSelect channel_select, uint16_t validStart, uint16_t validEnd) {
		auto response = sendCommand(channel_select);
		if(response >= validStart && response <= validEnd)
		{
			axis = response;
			return true;
		}
		else
			return false;
	};

	const uint16_t VALID_X_START = 10;
	const uint16_t VALID_X_END = 4095;
	const uint16_t VALID_Y_START = 0;
	const uint16_t VALID_Y_END = 4090;
	bool is_touching = (read_axis)(x, ChannelSelect::XPosition, VALID_X_START, VALID_X_END);
	if(is_touching)
		is_touching = (read_axis)(y, ChannelSelect::YPosition, VALID_Y_START, VALID_Y_END);

	return is_touching;
}

uint16_t XPT2046::sendCommand(ChannelSelect channelSelect)
{
	uint8_t command = 0b10000000
		| (((uint8_t) channelSelect) << 4)
		| (((uint8_t) resolution) << 3)
		| (((uint8_t) referenceMode) << 2)
		| ((uint8_t) powerDownMode);
	uint8_t response[2] = { 0, 0 };

	HAL_GPIO_WritePin(csPin.port, csPin.pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi, &command, 1, 100);
	HAL_SPI_Receive(spi, response, 2, 100);
	HAL_GPIO_WritePin(csPin.port, csPin.pin, GPIO_PIN_SET);

	uint16_t responseAsNumber = (response[0] << 8) | response[1];
	responseAsNumber = responseAsNumber >> 3;
	return responseAsNumber;
}

