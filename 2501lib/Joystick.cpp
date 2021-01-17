#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#include "Joystick.h"


Joystick::Joystick(std::string deviceID) {
	init(deviceID);
}

Joystick::Joystick(uint8_t deviceID) {
	std::string id = "/dev/input/js" + std::to_string(deviceID);
	init(id);
}

Joystick::Joystick() {
	init("/dev/input/js0");
}

void Joystick::init(std::string deviceID) {
	//std::cout << "New device: " << deviceID << "\n";
	fd = open(deviceID.c_str(), (O_RDONLY | O_NONBLOCK));
	if(fd == -1) {
		std::cerr << "Error opening joystick on '" << deviceID << "'!\n";
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

bool Joystick::IsOpen() {
	return fd != -1;
}

bool Joystick::Update() {
	if(!IsOpen())
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
				//std::cout << "Init event\n";
				break;
			default:
				//std::cout << "Other event\n";
				break;
		}
		return true;
	}
	else
		return false;
}

uint8_t Joystick::GetButtonCount() {
	if(!IsOpen())
		return 0;

	return btnCount;
}

bool Joystick::GetButton(uint8_t button) {
	if(!IsOpen())
		return false;

	if(button >= btnCount) {
		return false;
	}

	return btns[button];
}

uint8_t Joystick::GetAxisCount() {
	if(!IsOpen())
		return 0;

	return axisCount;
}

int16_t Joystick::GetRawAxis(uint8_t axis) {
	if(!IsOpen())
		return false;

	if(axis >= axisCount) {
		return false;
	}

	return axes[axis];
}

double Joystick::GetAxis(uint8_t axis) {
	if(!IsOpen())
		return false;

	if(axis >= axisCount) {
		return false;
	}

	return axes[axis] / 32767.0;
}
