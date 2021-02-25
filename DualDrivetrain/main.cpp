#include "DualDrivetrainExampleRobot.h"


int main() {
	Joystick *stick = new Joystick();
	ServoHat *hat = new ServoHat();

	DualDrivetrainExampleRobot *robot = new DualDrivetrainExampleRobot(stick, hat);

	robot->Run();
}