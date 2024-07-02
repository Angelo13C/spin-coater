/*
 * MCT8316Z.cpp
 *
 *  Created on: Jun 13, 2024
 *      Author: Angelo
 */

#include "drivers/mct8316z/spi.h"

constexpr RegisterField Control_Register_8::FGOUT_SEL;

MCT8316ZSpi::MCT8316ZSpi(Pin csPin, SPI_HandleTypeDef* spi) : csPin(csPin), spi(spi) {
	// TODO Auto-generated constructor stub

}

MCT8316ZSpi::~MCT8316ZSpi() {
	// TODO Auto-generated destructor stub
}

static bool has_even_parity(uint16_t value){
	uint16_t count = 0;

	const uint8_t NUMBER_OF_BITS_IN_A_BYTE = 8;
    for(uint8_t i = 0; i < sizeof(value) * NUMBER_OF_BITS_IN_A_BYTE; i++)
    {
        if(value & (1 << i))
        	count++;
    }

    return !(count % 2);
}

ReceivedPacket MCT8316ZSpi::sendRaw(bool write, uint8_t address, uint8_t data)
{
	uint16_t bufferToSend = ((uint8_t) write) << 15 | address << 9 | data;
	if(!has_even_parity(bufferToSend))
		bufferToSend |= 1 << 8;

	uint8_t receiveBuffer[2];
	HAL_GPIO_WritePin(csPin.port, csPin.pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi, (uint8_t*) &bufferToSend, 2, 100);
	HAL_SPI_Receive(spi, receiveBuffer, 2, 100);
	HAL_GPIO_WritePin(csPin.port, csPin.pin, GPIO_PIN_SET);

	return ReceivedPacket
	{
		.status = receiveBuffer[0],
		.data = receiveBuffer[1]
	};
}

