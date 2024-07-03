/*
 * spin_coating_process.cpp
 *
 *  Created on: Jun 17, 2024
 *      Author: Angelo
 */

#include "../../Inc/spin_coater/spin_coating_process.h"
#include "../../Inc/utils/flash.h"

#include <algorithm>

SpinCoatingProcess::SpinCoatingProcess(uint32_t startFlashAddress) : startFlashAddress(startFlashAddress) {
	points.usedPointsCount = (uint8_t) flash::read(localAddressToFlashAddress(0));
	for(uint8_t i = 0; i < points.usedPointsCount; i++)
	{
		uint32_t readValue = flash::read(pointIndexToFlashAddress(i));
		points.points[i] = ProcessPoint
		{
			.durationInSeconds = (uint16_t) readValue,
			.speedInRpm = (uint16_t) (readValue >> 16),
		};
	}
}

SpinCoatingProcess::~SpinCoatingProcess() {
	// TODO Auto-generated destructor stub
}

void SpinCoatingProcess::savePointsToFlash() const
{
	uint32_t pointsToSaveCount = (uint32_t) points.usedPointsCount;
	flash::program(localAddressToFlashAddress(0), &pointsToSaveCount, 1);

	for(uint8_t i = 0; i < points.usedPointsCount; i++)
	{
		uint32_t valueToWrite = (((uint32_t) points.points[i].speedInRpm) << 16) + (uint32_t) points.points[i].durationInSeconds;
		flash::program(pointIndexToFlashAddress(i), &valueToWrite, 1);
	}
}

uint16_t SpinCoatingProcess::getSpeedInRpmAtTime(uint16_t timeInSeconds, bool& hasProcessEnded) const
{
	hasProcessEnded = false;

	uint16_t cumulatedTime = 0;
	for(uint8_t i = 0; i < points.usedPointsCount; i++)
	{
		cumulatedTime += points.points[i].durationInSeconds;
		if(cumulatedTime > timeInSeconds)
			return points.points[i].speedInRpm;
	}

	hasProcessEnded = true;
	return 0;
}

uint32_t SpinCoatingProcess::localAddressToFlashAddress(uint32_t localAddress) const
{
	return startFlashAddress + localAddress;
}

bool SpinCoatingProcess::addPointToProcess(ProcessPoint point)
{
	if(points.usedPointsCount >= POINTS_COUNT)
		return false;

	points.points[points.usedPointsCount] = point;
	points.usedPointsCount++;
	return true;
}

uint32_t SpinCoatingProcess::pointIndexToFlashAddress(uint8_t pointIndex) const
{
	return localAddressToFlashAddress((1 + (uint32_t) pointIndex) * 4);
}
