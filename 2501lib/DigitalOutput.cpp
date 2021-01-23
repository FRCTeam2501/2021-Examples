#include "wiringPi.h"
#include "DigitalOutput.h"


DigitalOutput::DigitalOutput(uint8_t pin) {
	wiringPiSetupGpio(); // Library looks like it has no problem being init'd multiple times.

	DigitalOutput::pin = pin;
	pinMode(pin, OUTPUT);
}

void DigitalOutput::Set(bool value) {
	digitalWrite(pin, value);
}
