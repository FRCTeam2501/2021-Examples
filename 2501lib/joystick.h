#pragma once

#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>


class Joystick {
 private:
	int fd = -1;
	bool *btns;
	uint8_t btnCount;
	int16_t *axes;
	uint8_t axisCount;

 public:
	Joystick();

	bool IsOpen();

	bool Update();

	uint8_t GetButtonCount();

	bool GetButton(uint8_t button);

	uint8_t GetAxisCount();

	int16_t GetRawAxis(uint8_t axis);

	double GetAxis(uint8_t axis);
};
