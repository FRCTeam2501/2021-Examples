#include "MotorScanner.h"


int main() {
	Joystick *stick = new Joystick();
	ServoHat *hat = new ServoHat();

	MotorScanner *robot = new MotorScanner(stick, hat);

	robot->Run();
}