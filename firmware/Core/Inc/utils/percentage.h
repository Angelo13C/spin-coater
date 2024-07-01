/*
 * percentage.h
 *
 *  Created on: Jun 25, 2024
 *      Author: Angelo
 */

#ifndef INC_UTILS_PERCENTAGE_H_
#define INC_UTILS_PERCENTAGE_H_


struct Percentage
{
public:
	inline static Percentage from0To1(float value) { return Percentage(value); }
	inline static Percentage from0To100(float value) { return Percentage::from0To1(value / 100); }

	inline float into0To1() { return value; }

private:
	Percentage(float value) : value(value) {}

private:
	float value;
};


#endif /* INC_UTILS_PERCENTAGE_H_ */
