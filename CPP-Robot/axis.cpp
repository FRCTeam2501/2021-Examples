#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdint>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>

#include "bcm2835.h"
#include "pca9685servo.h"


// Gamepad button & axis mapping
namespace GAMEPAD {
	namespace BUTTONS {
		constexpr uint8_t A = 0;
		constexpr uint8_t B = 1;
		constexpr uint8_t X = 3;
		constexpr uint8_t Y = 4;
		constexpr uint8_t LT = 6;
		constexpr uint8_t RT = 7;
		constexpr uint8_t LB = 8;
		constexpr uint8_t RB = 9;
		constexpr uint8_t SELECT = 10;
		constexpr uint8_t START = 11;
		constexpr uint8_t LA = 13;
		constexpr uint8_t RA = 14;
	}
	namespace AXES {
		constexpr uint8_t LX = 0;
		constexpr uint8_t LY = 1;
		constexpr uint8_t RX = 2;
		constexpr uint8_t RY = 3;
		constexpr uint8_t DX = 6;
		constexpr uint8_t DY = 7;

		// Note: while the bumpers can be read as axis, the library is inconsistent at reading their value.  Your best bet is to use them as buttons.
		constexpr uint8_t RB = 4;
		constexpr uint8_t LB = 5;
	}
}

class Joystick {
 private:
	int fd = -1;
	bool *btns;
	uint8_t btnCount;
	int16_t *axes;
	uint8_t axisCount;

 public:
	Joystick() {
		// TODO: Allow different js numbers for multi-joystick usage
		fd = open("/dev/input/js0", (O_RDONLY | O_NONBLOCK));
		if(fd == -1) {
			std::cerr << "Error opening joystick on js0!\n";
		}
		//std::cout << "Opened joystick with fd: " << fd << "\n";


		// Get button count, setup button statuses
		ioctl(fd, JSIOCGBUTTONS, &btnCount);
		//std::cout << "Joystick has " << +btnCount << " buttons\n";
		btns = new bool[btnCount];
		for(uint8_t i = 0; i < btnCount; i++) {
			btns[i] = false;
		}


		// Get axis count, setup axis statuses
		ioctl(fd, JSIOCGAXES, &axisCount);
		axes = new int16_t[axisCount];
		for(uint8_t i = 0; i < axisCount; i++) {
			axes[i] = 0;
		}
	}

	bool isOpen() {
		return fd != -1;
	}

	bool update() {
		if(!isOpen())
			return false;

		struct js_event event;
		if(read(fd, &event, sizeof(event)) > 0) {
			switch(event.type) {
				case JS_EVENT_BUTTON:
					//std::cout << "Button " << +event.number << " event " << event.value << "\n";
					btns[event.number] = event.value;
					break;
				case JS_EVENT_AXIS:
					//std::cout << "Axis " << +event.number << " event " << event.value << "\n";
					axes[event.number] = event.value;
					break;
				case JS_EVENT_INIT:
					std::cout << "Init event\n";
					break;
				default:
					std::cout << "Other event\n";
					break;
			}
			return true;
		}
		else
			return false;
	}

	uint8_t getButtonCount() {
		if(!isOpen())
			return 0;

		return btnCount;
	}

	bool getButton(uint8_t button) {
		if(!isOpen())
			return false;

		if(button >= btnCount) {
			return false;
		}

		return btns[button];
	}

	uint8_t getAxisCount() {
		if(!isOpen())
			return 0;

		return axisCount;
	}

	int16_t getRawAxis(uint8_t axis) {
		if(!isOpen())
			return false;

		if(axis >= axisCount) {
			return false;
		}

		return axes[axis];
	}

	double getAxis(uint8_t axis) {
		if(!isOpen())
			return false;

		if(axis >= axisCount) {
			return false;
		}

		return axes[axis] / 32767.0;
	}
};


uint8_t doubleToAngle(double d) {
	return (d + 1) * 90.0;
}

double clamp(double val, double min, double max) {
	return std::min(std::max(val, min), max);
}

void arcadeDrive(double y, double x, double &left, double &right) {
	y = clamp(y, -1.0, 1.0);
	x = clamp(x, -1.0, 1.0);

	double max = std::copysign(std::max(std::abs(y), std::abs(x)), y);

	if(y >= 0.0) {
		// First quadrant, else second quadrant
		if (x >= 0.0) {
			left = max;
			right = y - x;
		} else {
			left = y + x;
			right = max;
		}
  	}
	else {
		// Third quadrant, else fourth quadrant
		if (x >= 0.0) {
			left = y + x;
			right = max;
		} else {
			left = max;
			right = y - x;
		}
	}
	left = clamp(left, -1.0, 1.0);
	right = clamp(right, -1.0, 1.0);
}


int main() {
	if (getuid() != 0) {
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}

	if (bcm2835_init() != 1) {
		fprintf(stderr, "bcm2835_init() failed\n");
		return -2;
	}

	PCA9685Servo servo;
	servo.SetLeftUs(1050);
	servo.SetRightUs(2050);



	bool startWasPressed = false;
	bool isEnabled = false;

	Joystick *stick;
	stick = new Joystick();

	if(!stick->isOpen()) {
		return -1;
	}

	while(true) {
		// Sleep for 1ms
		usleep(1000);

		// Update joystick values
		stick->update();


		// Deal with start button
		if(stick->getButton(GAMEPAD::BUTTONS::START) && !startWasPressed) {
			isEnabled = !isEnabled;

			if(isEnabled)
				std::cout << "Robot is now enabled!\n";
			else {
				std::cout << "Robot is now disabled.\n";
				servo.SetAngle(CHANNEL(0), ANGLE(90));
				servo.SetAngle(CHANNEL(1), ANGLE(90));
				servo.SetAngle(CHANNEL(2), ANGLE(90));
				servo.SetAngle(CHANNEL(3), ANGLE(90));
			}
		}
		startWasPressed = stick->getButton(GAMEPAD::BUTTONS::START);


		// Deal with drive
		if(isEnabled) {
			double left, right;
			arcadeDrive(stick->getAxis(GAMEPAD::AXES::LY), stick->getAxis(GAMEPAD::AXES::LX), left, right);

			servo.SetAngle(CHANNEL(0), ANGLE(doubleToAngle(left)));
			servo.SetAngle(CHANNEL(1), ANGLE(doubleToAngle(right)));
			servo.SetAngle(CHANNEL(2), ANGLE(doubleToAngle(left)));
			servo.SetAngle(CHANNEL(3), ANGLE(doubleToAngle(right)));
			std::cout << "left: " << left << ", right: " << right << "\n";
		}
	}
	return 0;
}