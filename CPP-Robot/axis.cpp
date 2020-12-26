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

class SpeedController; // Class prototype

class ServoHat {
 private:
	PCA9685Servo *hat;
	uint8_t *angles;
	bool enabled = false;

	uint8_t doubleToAngle(double d) {
		return (d + 1) * 90.0;
	}

	void setInternal(uint8_t channel, uint8_t angle, bool record) {
		hat->SetAngle(channel, angle);
		//std::cout << "set " << +channel << "," << +angle << "\n";


		if(record)
			angles[channel] = angle;
	}

 protected:
	friend class SpeedController;
	const static uint8_t MAX_CHANNELS = 16;

	void set(uint8_t channel, double speed) {
		if(!enabled || channel >= MAX_CHANNELS)
			return;

		setInternal(channel, doubleToAngle(speed), true);
	}

	void setAngle(uint8_t channel, uint8_t angle) {
		if(!enabled || channel >= MAX_CHANNELS)
			return;

		setInternal(channel, angle, true);
	}

 public:
	ServoHat() {
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

	void enable() {
		if(enabled)
			return;

		enabled = true;
		for(uint8_t i = 0; i < 16; i++) {
			setInternal(i, angles[i], false);
		}
	}

	void disable() {
		if(!enabled)
			return;

		enabled = false;
		for(uint8_t i = 0; i < 16; i++) {
			setInternal(i, 90U, false);
		}
	}

	bool isEnabled() {
		return enabled;
	}
};

class SpeedController {
 private:
	ServoHat *hat;
	uint8_t channel;

 public:
	SpeedController(ServoHat *hat, uint8_t channel) {
		if(channel > ServoHat::MAX_CHANNELS) {
			std::cerr << "Channel number is larger than " << ServoHat::MAX_CHANNELS << "\n";
			exit(-3);
		}

		SpeedController::hat = hat;
		SpeedController::channel = channel;
	}

	void set(double speed) {
		hat->set(channel, speed);
	}

	void setAngle(uint8_t angle) {
		hat->setAngle(channel, angle);
	}
};

class DifferentialDrive {
 private:
	SpeedController *lf, *lr, *rf, *rr;
	bool invertLeft = false, invertRight = false;

	double clamp(double val, double min, double max) {
		return std::min(std::max(val, min), max);
	}

 public:
	DifferentialDrive(SpeedController *lf, SpeedController *lr, SpeedController *rf, SpeedController *rr) {
		DifferentialDrive::lf = lf;
		DifferentialDrive::lr = lr;
		DifferentialDrive::rf = rf;
		DifferentialDrive::rr = rr;
	}

	void tankDrive(double left, double right) {
		if(invertLeft)
			left *= -1.0;
		if(invertRight)
			right *= -1.0;

		DifferentialDrive::lf->set(left);
		DifferentialDrive::lr->set(left);
		DifferentialDrive::rf->set(right);
		DifferentialDrive::rr->set(right);
	}

	void arcadeDrive(double forward, double rotation) {
		forward = clamp(forward, -1.0, 1.0);
		rotation = clamp(rotation, -1.0, 1.0);

		double max = std::copysign(std::max(std::abs(forward), std::abs(rotation)), forward);
		double left, right;

		if(forward >= 0.0) {
			// First quadrant, else second quadrant
			if (rotation >= 0.0) {
				left = max;
				right = forward - rotation;
			} else {
				left = forward + rotation;
				right = max;
			}
		}
		else {
			// Third quadrant, else fourth quadrant
			if (rotation >= 0.0) {
				left = forward + rotation;
				right = max;
			} else {
				left = max;
				right = forward - rotation;
			}
		}
		left = clamp(left, -1.0, 1.0);
		right = clamp(right, -1.0, 1.0);

		tankDrive(left, right);
	}

	void setLeftInverted(bool leftInverted) {
		invertLeft = leftInverted;
	}

	bool getLeftInverted() {
		return invertLeft;
	}

	void setRightInverted(bool rightInverted) {
		invertRight = rightInverted;
	}

	bool getRightInverted() {
		return invertRight;
	}
};



int main() {
	ServoHat *hat = new ServoHat();
	SpeedController *lf = new SpeedController(hat, 0U),
					*lr = new SpeedController(hat, 2U),
					*rf = new SpeedController(hat, 1U),
					*rr = new SpeedController(hat, 3U),
					*shooter = new SpeedController(hat, 4U);
	DifferentialDrive *drive = new DifferentialDrive(lf, lr, rf, rr);
	Joystick *stick = new Joystick();

	if(!stick->isOpen()) {
		return -255;
	}
	drive->setLeftInverted(true);
	drive->setRightInverted(true);


	bool startWasPressed = false;

	bool aWasPressed = false;
	double shooterSpeed = 0.5;
	constexpr double SHOOTER_ADJUST = 0.05;
	bool dyWasUp = false;
	bool dyWasDown = false;
	
	while(true) {
		// Sleep for 1ms
		usleep(1000);


		// Update joystick values
		stick->update();


		// Deal with start button
		if(stick->getButton(GAMEPAD::BUTTONS::START) && !startWasPressed) {
			if(!hat->isEnabled()) {
				hat->enable();
				std::cout << "Robot is now enabled!\n";
			}
			else {
				hat->disable();
				std::cout << "Robot is now disabled.\n";
			}
			startWasPressed = true;
		}
		else if(!stick->getButton(GAMEPAD::BUTTONS::START) && startWasPressed) {
			startWasPressed = false;
		}


		// Deal with drive
		if(hat->isEnabled()) {
			drive->arcadeDrive(stick->getAxis(GAMEPAD::AXES::LY), stick->getAxis(GAMEPAD::AXES::LX) * -1.0);
		}


		// Deal with shooter
		if(hat->isEnabled()) {
			// Button to run
			if(stick->getButton(GAMEPAD::BUTTONS::A) && !aWasPressed) {
				shooter->set(shooterSpeed);
				std::cout << "Shooter: ON\n";
				aWasPressed = true;
			}
			else if(!stick->getButton(GAMEPAD::BUTTONS::A) && aWasPressed) {
				shooter->set(0.0);
				std::cout << "Shooter: OFF\n";
				aWasPressed = false;
			}

			// Speed up
			if(stick->getAxis(GAMEPAD::AXES::DY) > 0.5 && !dyWasUp) {
				shooterSpeed = std::min(shooterSpeed + SHOOTER_ADJUST, 1.0);
				dyWasUp = true;
				dyWasDown = false;
			}
			else if(stick->getAxis(GAMEPAD::AXES::DY) < 0.5 && dyWasUp) {
				dyWasUp = false;
			}

			// Speed down
			if(stick->getAxis(GAMEPAD::AXES::DY) < -0.5 && !dyWasDown) {
				shooterSpeed = std::max(shooterSpeed - SHOOTER_ADJUST, 0.0);
				shooterSpeed -= SHOOTER_ADJUST;
				dyWasUp = false;
				dyWasDown = true;
			}
			else if(stick->getAxis(GAMEPAD::AXES::DY) > -0.5 && dyWasDown) {
				dyWasDown = false;
			}
		}
	}
	return 0;
}