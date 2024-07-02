/*
 * pid.h
 *
 *  Created on: Jun 18, 2024
 *      Author: Angelo
 */

// PID controller implementation inspired by the "pid" crate from Rust.
// Original project: https://crates.io/crates/pid
// Licensed under either the MIT License or the Apache License 2.0.

#ifndef INC_UTILS_PID_H_
#define INC_UTILS_PID_H_

#include <limits>

struct Parameter
{
	float gain;
	float limit;

	float apply_limit(float value)
	{
		if(value > limit)
			return limit;
		else if(value < -limit)
			return -limit;
		return value;
	}
};

class Pid {
public:
	Pid(float setpoint, float outputLimit) : setpoint(setpoint), outputLimit(outputLimit) { }
	virtual ~Pid() { }

	inline void setP(Parameter p) { paramP = p; }
	inline void setI(Parameter i) { paramI = i; }
	inline void setD(Parameter d) { paramD = d; }

	inline void setTheSetPoint(float setpoint) { this->setpoint = setpoint; }

	float compute(float measurement)
	{
		float error = setpoint - measurement;

		float p = error * paramP.gain;
		p = paramP.apply_limit(p);

		integralTerm += error * paramI.gain;
		integralTerm = paramI.apply_limit(integralTerm);

		float d = 0;
		if(previousMeasurement != NO_MEASUREMENT)
		{
			d = -(measurement - previousMeasurement) * paramD.gain;
			d = paramD.apply_limit(d);
		}

		previousMeasurement = measurement;

		float output = p + integralTerm + d;
		output = Parameter { .limit = outputLimit }.apply_limit(output);

		return output;
	}

private:
    float setpoint;
    float outputLimit;

    Parameter paramP = Parameter { .gain = 0, .limit = 0 };
    Parameter paramI = Parameter { .gain = 0, .limit = 0 };
    Parameter paramD = Parameter { .gain = 0, .limit = 0 };

    float integralTerm = 0;

    static constexpr float NO_MEASUREMENT = std::numeric_limits<float>::infinity();
	float previousMeasurement = NO_MEASUREMENT;
};

#endif /* INC_UTILS_PID_H_ */
