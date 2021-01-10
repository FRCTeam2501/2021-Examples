#include <iostream>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>


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

int main() {
	Joystick *stick;
	stick = new Joystick();

	while(true) {
		// Sleep for 1ms
		usleep(1000);

		// Update joystick values
		stick->update();

		/*std::cout
			<< "A: " << stick->getButton(GAMEPAD::BUTTONS::A) << "\t"
			<< "B: " << stick->getButton(GAMEPAD::BUTTONS::B) << "\t"
			<< "X: " << stick->getButton(GAMEPAD::BUTTONS::X) << "\t"
			<< "Y: " << stick->getButton(GAMEPAD::BUTTONS::Y) << "\t"
			<< "LT: " << stick->getButton(GAMEPAD::BUTTONS::LT) << "\t"
			<< "RT: " << stick->getButton(GAMEPAD::BUTTONS::RT) << "\t"
			<< "LB: " << stick->getButton(GAMEPAD::BUTTONS::LB) << "\t"
			<< "RB: " << stick->getButton(GAMEPAD::BUTTONS::RB) << "\t"
			<< "Select: " << stick->getButton(GAMEPAD::BUTTONS::SELECT) << "\t"
			<< "Start: " << stick->getButton(GAMEPAD::BUTTONS::START) << "\t"
			<< "LA: " << stick->getButton(GAMEPAD::BUTTONS::LA) << "\t"
			<< "RA: " << stick->getButton(GAMEPAD::BUTTONS::RA) << "\t"
			<< "\n";*/
		
		/*std::cout
			<< "LX: " << stick->getRawAxis(GAMEPAD::AXES::LX) << "\t"
			<< "LY: " << stick->getRawAxis(GAMEPAD::AXES::LY) << "\t"
			<< "RX: " << stick->getRawAxis(GAMEPAD::AXES::RX) << "\t"
			<< "RY: " << stick->getRawAxis(GAMEPAD::AXES::RY) << "\t"
			<< "RB: " << stick->getRawAxis(GAMEPAD::AXES::RB) << "\t"
			<< "LB: " << stick->getRawAxis(GAMEPAD::AXES::LB) << "\t"
			<< "DX: " << stick->getRawAxis(GAMEPAD::AXES::DX) << "\t"
			<< "DY: " << stick->getRawAxis(GAMEPAD::AXES::DY) << "\t"
			<< "\n";*/

		/*std::cout
			<< "LX: " << stick->getAxis(GAMEPAD::AXES::LX) << "\t"
			<< "LY: " << stick->getAxis(GAMEPAD::AXES::LY) << "\t"
			<< "RX: " << stick->getAxis(GAMEPAD::AXES::RX) << "\t"
			<< "RY: " << stick->getAxis(GAMEPAD::AXES::RY) << "\t"
		//	<< "RB: " << stick->getAxis(GAMEPAD::AXES::RB) << "\t"
		//	<< "LB: " << stick->getAxis(GAMEPAD::AXES::LB) << "\t"
			<< "DX: " << stick->getAxis(GAMEPAD::AXES::DX) << "\t"
			<< "DY: " << stick->getAxis(GAMEPAD::AXES::DY) << "\t"
			<< "\n";*/
	}
    return 0;
}