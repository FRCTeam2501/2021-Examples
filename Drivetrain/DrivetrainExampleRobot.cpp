#include "DrivetrainExampleRobot.h"

#include <iostream>


DrivetrainExampleRobot::DrivetrainExampleRobot(Joystick *stick, ServoHat *hat) : RobotBase(stick, hat, 10000U) {
	rgb = new ARGB(9U);

	lf = new PWMSpeedController(hat, 0U);
	lr = new PWMSpeedController(hat, 2U);
	rf = new PWMSpeedController(hat, 1U);
	rr = new PWMSpeedController(hat, 3U);

	drive = new DifferentialDrive(lf, lr, rf, rr);
}

void DrivetrainExampleRobot::Shutdown() {
	delete rgb;

	//delete lf;
	//delete lr;
	//delete rf;
	//delete rr;

	//delete drive;
}

void DrivetrainExampleRobot::RobotInit() {
	rgb->SetAll(COLORS::BLUE);
	rgb->Render();

	drive->SetLeftInverted(true);
	drive->SetRightInverted(true);
}

void DrivetrainExampleRobot::RobotPeriodic() {
	// Do nothing
}

void DrivetrainExampleRobot::TeleopInit() {
	rgb->SetAll(COLORS::GREEN);
	rgb->Render();
	std::cout << "Robot is now enabled!\n";
}

void DrivetrainExampleRobot::TeleopPeriodic() {
	drive->ArcadeDrive(stick->GetAxis(GAMEPAD::AXES::LY), stick->GetAxis(GAMEPAD::AXES::LX) * -1.0);
}

void DrivetrainExampleRobot::DisabledInit() {
	rgb->SetAll(COLORS::RED);
	rgb->Render();
	std::cout << "Robot is now disabled.\n";
}

void DrivetrainExampleRobot::DisabledPeriodic() {
	// Do nothing
}
