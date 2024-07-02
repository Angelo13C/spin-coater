/*
 * driver.h
 *
 *  Created on: Jun 28, 2024
 *      Author: Angelo
 */

#ifndef INC_DRIVERS_XPT2046_DRIVER_H_
#define INC_DRIVERS_XPT2046_DRIVER_H_

#include "configuration.h"
#include "../../utils/pin.h"

/**
 * @class XPT2046
 * @brief A class to interface with the XPT2046 touch controller.
 */
class XPT2046 {
public:
	XPT2046() {}
    /**
     * @brief Constructor to initialize XPT2046 with SPI and CS pin (with default settings).
     * @param spi Pointer to the SPI handle.
     * @param csPin Chip select pin.
     */
	XPT2046(SPI_HandleTypeDef* spi, Pin csPin);
    /**
     * @brief Constructor to initialize XPT2046 with SPI, CS pin, and additional settings.
     * @param spi Pointer to the SPI handle.
     * @param csPin Chip select pin.
     * @param resolution Resolution setting.
     * @param referenceMode Reference mode setting.
     * @param powerDownMode Power down mode setting.
     */
	XPT2046(SPI_HandleTypeDef* spi, Pin csPin, Resolution resolution, ReferenceMode referenceMode, PowerDownMode powerDownMode);
	virtual ~XPT2046();

    /**
     * @brief Set the resolution of the XPT2046.
     * @param resolution The resolution to set.
     */
	inline void setResolution(Resolution resolution) { this->resolution = resolution; }
    /**
     * @brief Set the reference mode of the XPT2046.
     * @param referenceMode The reference mode to set.
     */
	inline void setReferenceMode(ReferenceMode referenceMode) { this->referenceMode = referenceMode; }
    /**
     * @brief Set the power down mode of the XPT2046.
     * @param powerDownMode The power down mode to set.
     */
	inline void setPowerDownMode(PowerDownMode powerDownMode) { this->powerDownMode = powerDownMode; }

	 /**
	 * @brief Read a single touch sample from the XPT2046.
	 * @param x Reference to store the X coordinate of the touch.
	 * @param y Reference to store the Y coordinate of the touch.
	 * @return True if a touch is detected, false otherwise.
	 */
	bool readSingleTouchSample(uint16_t& x, uint16_t& y);

    /**
     * @brief Read the current touch position, averaging multiple samples.
     * @tparam N_SAMPLES Number of samples to average.
     * @param x Reference to store the X coordinate of the touch.
     * @param y Reference to store the Y coordinate of the touch.
     * @return True if a touch is detected, false otherwise.
     */
	template <size_t N_SAMPLES>
	bool readCurrentTouchPosition(uint16_t& x, uint16_t& y)
	{
		uint16_t xSamples[N_SAMPLES] = {};
		uint16_t ySamples[N_SAMPLES] = {};

		for(size_t i = 0; i < N_SAMPLES; i++)
		{
			bool isTouching = readSingleTouchSample(xSamples[i], ySamples[i]);

			if(!isTouching)
				return false;
		}
		auto calculateMean = [](uint16_t& axis, uint16_t samples[N_SAMPLES]) {
			uint32_t sum = 0;
			for(size_t i = 0; i < N_SAMPLES; i++)
				sum += samples[i];
			axis = sum / N_SAMPLES;
		};

		(calculateMean)(x, xSamples);
		(calculateMean)(y, ySamples);

		return true;
	}

private:
    /**
     * @brief Send a command to the XPT2046.
     * @param channelSelect The channel to select for the command.
     * @return The response from the XPT2046.
     */
	uint16_t sendCommand(ChannelSelect channelSelect);

private:
	SPI_HandleTypeDef* spi;  ///< Pointer to the SPI handle.
	Pin csPin;               ///< Chip select pin.

	// Configuration
	Resolution resolution;
	ReferenceMode referenceMode;
	PowerDownMode powerDownMode;
};


#endif /* INC_DRIVERS_XPT2046_DRIVER_H_ */
