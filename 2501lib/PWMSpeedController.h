#pragma once

#include "SpeedController.h"
#include "PWM.h"


class PWMSpeedController : public SpeedController {
 private:
	PWM *pwm;

 public:
	PWMSpeedController(ServoHat *hat, uint8_t channel);

	PWMSpeedController(ServoHat &hat, uint8_t channel);

	/**
	 * @brief Set the SpeedController to a speed
	 * 
	 * @param speed Speed to set it to
	 */
	void Set(double speed);
};