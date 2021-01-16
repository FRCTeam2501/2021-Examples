#include <iostream>

#include <unistd.h>

#include "bcm2835.h"
#include "pca9685servo.h"

#include "ServoHat.h"


uint8_t ServoHat::DoubleToAngle(double d) {
	return (d + 1) * 90.0;
}

void ServoHat::SetInternal(uint8_t channel, uint8_t angle, bool record) {
	hat->SetAngle(channel, angle);
	//std::cout << "set " << +channel << "," << +angle << "\n";


	if(record)
		angles[channel] = angle;
}

void ServoHat::Set(uint8_t channel, double speed) {
	if(!enabled || channel >= MAX_CHANNELS)
		return;

	SetInternal(channel, DoubleToAngle(speed), true);
}

void ServoHat::SetAngle(uint8_t channel, uint8_t angle) {
	if(!enabled || channel >= MAX_CHANNELS)
		return;

	SetInternal(channel, angle, true);
}

ServoHat::ServoHat() {
	if (getuid() != 0) {
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		exit(-1);
	}

	if (bcm2835_init() != 1) {
		fprintf(stderr, "bcm2835_init() failed\n");
		exit(-2);
	}

	hat = new PCA9685Servo();
	hat->SetLeftUs(1050);
	hat->SetCenterUs(1550);
	hat->SetRightUs(2050);

	angles = new uint8_t[16];
	for(uint8_t i = 0; i < MAX_CHANNELS; i++) {
		angles[i] = 90U;
	}
}

void ServoHat::Enable() {
	if(enabled)
		return;

	enabled = true;
	for(uint8_t i = 0; i < 16; i++) {
		SetInternal(i, angles[i], false);
	}
}

void ServoHat::Disable() {
	if(!enabled)
		return;

	enabled = false;
	for(uint8_t i = 0; i < 16; i++) {
		SetInternal(i, 90U, false);
	}
}

bool ServoHat::IsEnabled() {
	return enabled;
}
