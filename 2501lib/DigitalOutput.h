#pragma once

#include <cstdint>


class DigitalOutput {
 private:
	uint8_t pin;

 public:
	DigitalOutput(uint8_t pin);

	void Set(bool value);
};
