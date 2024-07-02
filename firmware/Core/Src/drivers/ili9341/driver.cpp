/*
 * ili9341.cpp
 *
 *  Created on: Jun 16, 2024
 *      Author: Angelo
 */

#include "../Inc/drivers/ili9341/driver.h"
#include "../../../Inc/utils/endianness.h"

ILI9341::ILI9341(SPI_HandleTypeDef* spi, Pin csPin, Pin dataCommandPin, Pin resetPin, Pin backlightPin, Orientation defaultOrientation)
	: spi(spi), csPin(csPin), dataCommandPin(dataCommandPin), resetPin(resetPin), backlightPin(backlightPin)
{
	HAL_GPIO_WritePin(backlightPin.port, backlightPin.pin, GPIO_PIN_SET);

	hardware_reset();

	HAL_Delay(5);

	send_command(Command::SoftwareReset);

	HAL_Delay(120);

	set_orientation(defaultOrientation);

	set_bits_per_pixel(BitsPerPixel::Bits16);

	send_command(Command::SleepOut);

	HAL_Delay(5);

	send_command(Command::DisplayOn);
}

ILI9341::~ILI9341() {
	// TODO Auto-generated destructor stub
}


void ILI9341::hardware_reset()
{
	HAL_GPIO_WritePin(resetPin.port, resetPin.pin, GPIO_PIN_RESET);

	HAL_Delay(1);

	HAL_GPIO_WritePin(resetPin.port, resetPin.pin, GPIO_PIN_SET);
}

void ILI9341::set_orientation(Orientation orientation)
{
	send_command(Command::MemoryAccessControl);
	inner_send_data((uint8_t*) &orientation, 1);
}

void ILI9341::set_bits_per_pixel(BitsPerPixel bitsPerPixel)
{
	send_command(Command::PixelFormatSet);
	inner_send_data((uint8_t*) &bitsPerPixel, 1);
}

void ILI9341::set_window(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
{
	auto send_axis = [&](Command command, uint16_t start, uint16_t end)
	{
		send_command(command);
		uint32_t data = ((uint32_t) start) << 16 | (uint32_t) end;
		data = reverseEndianness(data);
		inner_send_data((uint8_t*) &data, 4);
	};

	send_axis(Command::ColumnAddressSet, startX, endX);
	send_axis(Command::PageAddressSet, startY, endY);
}

void ILI9341::send_colors(uint8_t* colorsData, size_t size)
{
	send_command(Command::MemoryWrite);
	inner_send_data(colorsData, size);
}


void ILI9341::send_command(Command command)
{
	inner_send_command((uint8_t*) &command, 1);
}

void ILI9341::inner_send_data(uint8_t* buffer, size_t size)
{
	HAL_GPIO_WritePin(dataCommandPin.port, dataCommandPin.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(csPin.port, csPin.pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi, buffer, size, 100);
	HAL_GPIO_WritePin(csPin.port, csPin.pin, GPIO_PIN_SET);
}

void ILI9341::inner_send_command(uint8_t* buffer, size_t size)
{
	HAL_GPIO_WritePin(dataCommandPin.port, dataCommandPin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(csPin.port, csPin.pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi, buffer, size, 100);
	HAL_GPIO_WritePin(csPin.port, csPin.pin, GPIO_PIN_SET);
}
