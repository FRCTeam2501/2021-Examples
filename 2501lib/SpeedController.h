#pragma once

#include "ServoHat.h"

class SpeedController {
 private:
	// Servo Hat to control the lower level
	ServoHat *hat;
	// Channel to use
	uint8_t channel;

 public:
	/**
	 * @brief Construct a new Speed Controller object
	 * 
	 * @param hat Servo Hat object to use
	 * @param channel Channel to use
	 */
	SpeedController(ServoHat *hat, uint8_t channel);

	/**
	 * @brief Construct a new Speed Controller object
	 * 
	 * @param hat Servo Hat object to use
	 * @param channel Channel to use
	 */
	SpeedController(ServoHat &hat, uint8_t channel);

	/**
	 * @brief Set the SpeedController to a speed
	 * 
	 * @param speed Speed to set it to
	 */
	void Set(double speed);

	/**
	 * @brief Set the SpeedController to an angle
	 * 
	 * @param angle Angle to set it to
	 */
	void SetAngle(uint8_t angle);
};
