#pragma once

#include "ServoHat.h"

class SpeedController {
 private:
	ServoHat *hat;
	uint8_t channel;

 public:
	SpeedController(ServoHat *hat, uint8_t channel);

	SpeedController(ServoHat &hat, uint8_t channel);

	void Set(double speed);

	void SetAngle(uint8_t angle);
};
