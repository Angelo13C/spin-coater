/*
 * Keypad.h
 *
 *  Created on: Jun 12, 2024
 *      Author: Angelo
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <array>
#include <functional>

#include "stm32f4xx_hal.h"
#include "utils/pin.h"

/**
 * @struct ButtonAddress
 * @brief A structure to represent the address of a button in the keypad.
 */
struct ButtonAddress
{
    size_t row;    ///< Row index of the button.
    size_t column; ///< Column index of the button.
};

/**
 * @class Keypad
 * @brief A template class to handle a matrix keypad.
 * @tparam ROWS Number of rows in the keypad.
 * @tparam COLUMNS Number of columns in the keypad.
 */
template <size_t ROWS, size_t COLUMNS>
class Keypad {
public:
    /**
     * @brief Constructor to initialize the keypad with row and column pins.
     * @param rowsPins Array of pins corresponding to the rows.
     * @param columnsPins Array of pins corresponding to the columns.
     */
	Keypad(std::array<Pin, ROWS> rowsPins, std::array<Pin, COLUMNS> columnsPins) : rowsPins(rowsPins), columnsPins(columnsPins) {}
	virtual ~Keypad() {}

    /**
	* @brief Function to get the currently pressed buttons.
	* @param buttonPressedFn A callback function to handle the pressed button addresses.
	*/
	void getPressedButtons(std::function<void (const ButtonAddress)> buttonPressedFn) const
	{
		for(size_t r = 0; r < ROWS; r++)
		{
			Pin currentRow = rowsPins[r];
			HAL_GPIO_WritePin(currentRow.port, currentRow.pin, GPIO_PIN_SET);
			for(size_t c = 0; c < COLUMNS; c++)
			{
				Pin currentColumn = columnsPins[c];
				if(HAL_GPIO_ReadPin(currentColumn.port, currentColumn.pin))
					buttonPressedFn(ButtonAddress {r, c});
			}
			HAL_GPIO_WritePin(currentRow.port, currentRow.pin, GPIO_PIN_RESET);
		}
	}

private:
    std::array<Pin, ROWS> rowsPins;
    std::array<Pin, COLUMNS> columnsPins;
};

#endif /* KEYPAD_H_ */
