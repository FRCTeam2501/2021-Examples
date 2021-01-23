#include <iostream>

#include <unistd.h>

#include "bcm2835.h"
#include "pca9685.h"

#include "ServoHat.h"


void ServoHat::SetInternal(uint8_t channel, uint16_t value, bool record) {
	hat->Write(channel, value);
	//std::cout << "set " << +channel << "," << +angle << "\n";

	if(record)
		values[channel] = value;
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

	hat = new PCA9685();
	hat->SetInvert(false);
	hat->SetOutDriver(true);
	hat->SetFrequency(50);

	values = new uint16_t[16];
	for(uint8_t i = 0; i < MAX_CHANNELS; i++) {
		values[i] = 90U;
	}
}

void ServoHat::Enable() {
	if(enabled)
		return;

	enabled = true;
	for(uint8_t i = 0; i < 16; i++) {
		SetInternal(i, values[i], false);
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
