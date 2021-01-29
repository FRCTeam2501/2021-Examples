#include "DrivetrainExampleRobot.h"


int main() {
	Joystick *stick = new Joystick();
	ServoHat *hat = new ServoHat();

	DrivetrainExampleRobot *robot = new DrivetrainExampleRobot(stick, hat);

	robot->Run();
}