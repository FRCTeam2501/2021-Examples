#pragma once

#include "pca9685.h"


class PWM; // Class prototype


class ServoHat {
 private:
	// Lower level object to talk with the servo controller
	PCA9685 *hat;
	// Array to store the values of the servos
	uint16_t *values;
	// If the hat is enabled
	bool enabled = false;

	// Internal function to set a channel to an internal value
	void SetInternal(uint8_t channel, uint16_t value, bool record);

 protected:
	// Allow PWM to access the channels
	friend class PWM;
	// We have a max of 16 channels
	const static uint8_t MAX_CHANNELS = 16;

	/**
	 * @brief Set the Raw value
	 *
	 * @param channel Channel to set
	 * @param value Value to set channel to
	 */
	void SetRaw(uint8_t channel, uint16_t value);

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
