/*
 * ili9341.h
 *
 *  Created on: Jun 16, 2024
 *      Author: Angelo
 */

#ifndef INC_ILI9341_DRIVER_H_
#define INC_ILI9341_DRIVER_H_


#include "stm32f4xx_hal.h"

#include "../../utils/pin.h"
#include "../../utils/color.h"
#include "commands.h"
#include "configuration.h"

/**
 * @class ILI9341
 * @brief A class to interface with the ILI9341 display driver.
 */
class ILI9341 {
public:
	ILI9341() {}

    /**
     * @brief Parameterized constructor.
     * @param spi The SPI handle.
     * @param csPin The chip select pin.
     * @param dataCommandPin The data/command control pin.
     * @param resetPin The reset pin.
     * @param backlightPin The backlight control pin.
     * @param defaultOrientation The default orientation of the display.
     */
	ILI9341(SPI_HandleTypeDef* spi, Pin csPin, Pin dataCommandPin, Pin resetPin, Pin backlightPin, Orientation defaultOrientation);
	virtual ~ILI9341();

    /**
     * @brief Perform a hardware reset on the display.
     */
	void hardware_reset();

    /**
     * @brief Set the orientation of the display.
     * @param orientation The orientation to set.
     */
	void set_orientation(Orientation orientation);
    /**
     * @brief Set the bits per pixel for the display.
     * @param bitsPerPixel The bits per pixel setting.
     */
	void set_bits_per_pixel(BitsPerPixel bitsPerPixel);
    /**
     * @brief Set the active window on the display.
     * @param startX The starting X coordinate.
     * @param startY The starting Y coordinate.
     * @param endX The ending X coordinate.
     * @param endY The ending Y coordinate.
     */
	void set_window(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY);

    /**
     * @brief Send color data to the display.
     * @param colorsData Pointer to the color data.
     * @param size The size of the color data.
     */
	void send_colors(uint8_t* colorsData, size_t size);

    /**
     * @brief Send a command to the display.
     * @param command The command to send.
     */
	void send_command(Command command);

private:
    /**
     * @brief Internal method to send data to the display.
     * @param data Pointer to the data.
     * @param size The size of the data.
     */
	void inner_send_data(uint8_t* data, size_t size);
    /**
     * @brief Internal method to send a command to the display.
     * @param data Pointer to the command data.
     * @param size The size of the command data.
     */
	void inner_send_command(uint8_t* data, size_t size);

private:
	SPI_HandleTypeDef* spi; ///< The SPI handle.

    Pin csPin;              ///< The chip select pin.
    Pin dataCommandPin;     ///< The data/command control pin.
    Pin resetPin;           ///< The reset pin.
    Pin backlightPin;       ///< The backlight control pin.
};


#endif /* INC_ILI9341_DRIVER_H_ */
