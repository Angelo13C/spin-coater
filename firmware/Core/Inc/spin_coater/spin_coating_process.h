/*
 * spin_coating_process.h
 *
 *  Created on: Jun 17, 2024
 *      Author: Angelo
 */

#ifndef INC_SPIN_COATER_SPIN_COATING_PROCESS_H_
#define INC_SPIN_COATER_SPIN_COATING_PROCESS_H_

#include "stm32f4xx_hal.h"

/**
 * @brief Structure representing a process point with duration and speed.
 */
struct ProcessPoint
{
    uint16_t durationInSeconds;  ///< Duration of the process point in seconds.
    uint16_t speedInRpm;         ///< Speed of the process point in RPM.
};

constexpr size_t POINTS_COUNT = 5;  ///< Maximum number of process points.

/**
 * @brief Structure holding an array of ProcessPoint and the count of used points.
 */
struct ProcessPoints
{
    ProcessPoint points[POINTS_COUNT];  ///< Array of process points.
    uint8_t usedPointsCount;            ///< Number of used process points.
};

/**
 * @brief Class managing the spin coating process with points stored in flash memory.
 */
class SpinCoatingProcess {
public:
    /**
     * @brief Constructs a SpinCoatingProcess object.
     *
     * @param startFlashAddress Start address in flash memory for storing points.
     */
	SpinCoatingProcess(uint32_t startFlashAddress);
	virtual ~SpinCoatingProcess();

	/**
     * @brief Adds a process point to the spin coating process.
     *
     * @param point ProcessPoint to add.
     * @return true if adding the point was successful, false otherwise.
     */
	bool addPointToProcess(ProcessPoint point);

    /**
     * @brief Retrieves the speed in RPM at a given time in the process.
     *
     * @param timeInSeconds Time in seconds from the start of the process.
     * @param hasProcessEnded Flag indicating if the process has ended.
     * @return Speed in RPM at the specified time.
     */
	uint16_t getSpeedInRpmAtTime(uint16_t timeInSeconds, bool& hasProcessEnded) const;

    /**
     * @brief Retrieves a pointer to the process points.
     *
     * @return Pointer to the ProcessPoints object.
     */
	inline ProcessPoints* getPoints() { return &points; }

private:
	void savePointsToFlash() const;

	uint32_t localAddressToFlashAddress(uint32_t localAddress) const;
	uint32_t pointIndexToFlashAddress(uint8_t pointIndex) const;

private:
	uint32_t startFlashAddress;

	ProcessPoints points;
};

#endif /* INC_SPIN_COATER_SPIN_COATING_PROCESS_H_ */
