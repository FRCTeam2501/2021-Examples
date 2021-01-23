#pragma once

#include "ServoHat.h"


class PWM {
 private:
	// Servo Hat to control the lower level
    ServoHat *hat;
	// Channel to use
	uint8_t channel;

    uint16_t servo_minCount = 0;
	uint16_t servo_centerCount = 0;
	uint16_t servo_maxCount = 0;

	// Internal function to convert a double to an internal value
	uint16_t DoubleToValue(double d);

	// Internal function to convert an angle to double
	double AngleToDouble(uint8_t angle);

 public:
	/**
	 * @brief Construct a new PWM object
	 * 
	 * @param hat Servo Hat object to use
	 * @param channel Channel to use
	 */
    PWM(ServoHat *hat, uint8_t channel);

	/**
	 * @brief Construct a new PWM object
	 * 
	 * @param hat Servo Hat object to use
	 * @param channel Channel to use
	 */
    PWM(ServoHat &hat, uint8_t channel);

    /**
	 * @brief Set the channel to a speed
	 *
	 * @param speed Speed to set to
	 */
	void Set(double speed);

	/**
	 * @brief Set the channel to an angle
	 *
	 * @param angle Angle to set to
	 */
	void SetAngle(uint8_t angle);

	/**
	 * @brief Set the raw percent
	 * 
	 * @param p Percent duty cycle to set to
	 */
	void SetPercent(double p);

	/**
	 * @brief Set the Raw value
	 *
	 * @param value Value to set channel to
	 */
	void SetRaw(uint16_t value);

	void SetMaxUs(uint16_t max_us);
	void SetCenterUs(uint16_t center_us);
	void SetMinUs(uint16_t max_us);
};
