#pragma once

#include <cstdint>


class DigitalInput {
 private:
	uint8_t pin;

 public:
	DigitalInput(uint8_t pin);

	bool Get();

	void SetPullUp();
	void SetPullOff();
	void SetPullDown();
};
