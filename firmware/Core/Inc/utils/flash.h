/*
 * flash.h
 *
 *  Created on: Jun 17, 2024
 *      Author: Angelo
 */

#ifndef INC_UTILS_FLASH_H_
#define INC_UTILS_FLASH_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash_ex.h"

namespace flash
{
	uint32_t read(uint32_t flashAddress)
	{
		return * (uint32_t*) flashAddress;
	}

	void erase()
	{
		HAL_FLASH_Unlock();
		FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_1);
		HAL_FLASH_Lock();
	}

	void program(uint32_t flashAddress, uint32_t* data, size_t dataSize)
	{
		HAL_FLASH_Unlock();

		for(size_t i = 0; i < dataSize; i++)
		{
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddress + i * 4, (uint64_t) (*(data + i)));
		}

		HAL_FLASH_Lock();
	}
}


#endif /* INC_UTILS_FLASH_H_ */
