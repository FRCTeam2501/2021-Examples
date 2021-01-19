#pragma once

#include "pca9685servo.h"


class SpeedController; // Class prototype


class ServoHat {
 private:
	// Lower level object to talk with the servo controller
	PCA9685Servo *hat;
	// Array to store the angles of the servos
	uint8_t *angles;
	// If the hat is enabled
	bool enabled = false;

	// Internal function to convert a double to an angle
	uint8_t DoubleToAngle(double d);

	// Internal function to set a channel to an angle
	void SetInternal(uint8_t channel, uint8_t angle, bool record);

 protected:
	// Allow SpeedControllers to access the channels
	friend class SpeedController;
	// We have a max of 16 channels
	const static uint8_t MAX_CHANNELS = 16;

	/**
	 * @brief Set a channel to a speed
	 * 
	 * @param channel Channel to set
	 * @param speed Speed to set to
	 */
	void Set(uint8_t channel, double speed);

	/**
	 * @brief Set a channel to an angle
	 * 
	 * @param channel Channel to set
	 * @param angle Angle to set to
	 */
	void SetAngle(uint8_t channel, uint8_t angle);

 public:
	/**
	 * @brief Construct a new Servo Hat object
	 */
	ServoHat();

	/**
	 * @brief Enable the Servo Hat
	 */
	void Enable();

	/**
	 * @brief Disable the Servo Hat
	 */
	void Disable();

	/**
	 * @brief Check if the Servo Hat is Enabled
	 * 
	 * @return true if enabled, false if disabled
	 */
	bool IsEnabled();
};
