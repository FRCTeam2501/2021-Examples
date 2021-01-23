#pragma once

#include "SpeedController.h"
#include "PWM.h"
#include "DigitalOutput.h"


class L298N : public SpeedController {
 private:
	PWM *pwm;
	DigitalOutput *o1, *o2;

 public:
	L298N(ServoHat *hat, uint8_t pwm_channel, uint8_t in1, uint8_t in2);

	L298N(ServoHat &hat, uint8_t pwm_channel, uint8_t in1, uint8_t in2);

	/**
	 * @brief Set the SpeedController to a speed
	 * 
	 * @param speed Speed to set it to
	 */
	void Set(double speed);
};