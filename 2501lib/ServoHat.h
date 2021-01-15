#pragma once

#include "pca9685servo.h"


class SpeedController; // Class prototype


class ServoHat {
 private:
	PCA9685Servo *hat;
	uint8_t *angles;
	bool enabled = false;

	uint8_t DoubleToAngle(double d);

	void SetInternal(uint8_t channel, uint8_t angle, bool record);

 protected:
	friend class SpeedController;
	const static uint8_t MAX_CHANNELS = 16;

	void Set(uint8_t channel, double speed);

	void SetAngle(uint8_t channel, uint8_t angle);

 public:
	ServoHat();

	void Enable();

	void Disable();

	bool IsEnabled();
};
