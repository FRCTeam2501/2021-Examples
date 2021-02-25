#include "DualDrivetrainExampleRobot.h"

#include <iostream>


DualDrivetrainExampleRobot::DualDrivetrainExampleRobot(Joystick *stick, ServoHat *hat) : RobotBase(stick, hat, 10000U) {
	rgb = new ARGB(9U);

	l = new PWMSpeedController(hat, 0U);
	r = new PWMSpeedController(hat, 1U);

	drive = new DifferentialDrive(l, r);
}

void DualDrivetrainExampleRobot::Shutdown() {
	delete rgb;

	//delete lf;
	//delete lr;
	//delete rf;
	//delete rr;

	//delete drive;
}

void DualDrivetrainExampleRobot::RobotInit() {
	rgb->SetAll(COLORS::BLUE);
	rgb->Render();

	drive->SetLeftInverted(true);
	drive->SetRightInverted(true);
}

void DualDrivetrainExampleRobot::RobotPeriodic() {
	// Do nothing
}

void DualDrivetrainExampleRobot::TeleopInit() {
	rgb->SetAll(COLORS::GREEN);
	rgb->Render();
	std::cout << "Robot is now enabled!\n";
}

void DualDrivetrainExampleRobot::TeleopPeriodic() {
	drive->ArcadeDrive(stick->GetAxis(GAMEPAD::AXES::LY), stick->GetAxis(GAMEPAD::AXES::LX) * -1.0);
}

void DualDrivetrainExampleRobot::DisabledInit() {
	rgb->SetAll(COLORS::RED);
	rgb->Render();
	std::cout << "Robot is now disabled.\n";
}

void DualDrivetrainExampleRobot::DisabledPeriodic() {
	// Do nothing
}
