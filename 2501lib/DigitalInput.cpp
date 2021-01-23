#include "wiringPi.h"
#include "DigitalInput.h"


DigitalInput::DigitalInput(uint8_t pin) {
	wiringPiSetupGpio(); // Library looks like it has no problem being init'd multiple times.

	DigitalInput::pin = pin;
	pinMode(pin, INPUT);
}

bool DigitalInput::Get() {
	return digitalRead(pin);
}

void DigitalInput::SetPullUp() {
	pullUpDnControl(pin, PUD_UP);
}

void DigitalInput::SetPullOff() {
	pullUpDnControl(pin, PUD_OFF);
}

void DigitalInput::SetPullDown() {
	pullUpDnControl(pin, PUD_DOWN);
}
