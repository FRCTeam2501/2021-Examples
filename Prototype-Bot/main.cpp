#include "PrototypeRobot.h"


int main() {
	Joystick *stick = new Joystick();
	ServoHat *hat = new ServoHat();

	PrototypeRobot *robot = new PrototypeRobot(stick, hat);

	robot->Run();
}
