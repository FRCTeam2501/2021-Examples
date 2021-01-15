#include <iostream>

#include "SpeedController.h"


SpeedController::SpeedController(ServoHat *hat, uint8_t channel) {
	if(channel > ServoHat::MAX_CHANNELS) {
		std::cerr << "Channel number is larger than " << ServoHat::MAX_CHANNELS << "\n";
		exit(-3);
	}

	SpeedController::hat = hat;
	SpeedController::channel = channel;
}

SpeedController::SpeedController(ServoHat &hat, uint8_t channel) {
	SpeedController(&hat, channel);
}

void SpeedController::Set(double speed) {
	hat->Set(channel, speed);
}

void SpeedController::SetAngle(uint8_t angle) {
	hat->SetAngle(channel, angle);
}
