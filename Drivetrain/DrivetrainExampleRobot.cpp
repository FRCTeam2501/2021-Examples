#include "DrivetrainExampleRobot.h"

#include <iostream>


DrivetrainExampleRobot::DrivetrainExampleRobot(Joystick *stick, ServoHat *hat) : RobotBase(stick, hat, GAMEPAD::BUTTONS::START, GAMEPAD::BUTTONS::SELECT, 10000U) {
	rgb = new ARGB(9U);

	lf = new PWMSpeedController(hat, 12U);
	lr = new PWMSpeedController(hat, 13U);
	rf = new PWMSpeedController(hat, 14U);
	rr = new PWMSpeedController(hat, 15U);

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
	rgb->SetAll(COLORS::WHITE);
	rgb->Render();

	drive->SetLeftInverted(false);
	drive->SetRightInverted(false);

	lf->SetMaxUs(2000);
	lf->SetCenterUs(1500);
	lf->SetMinUs(1000);
}

void DrivetrainExampleRobot::RobotPeriodic() {
	// Do nothing
}

void DrivetrainExampleRobot::TeleopInit() {
	rgb->SetAll(COLORS::GREEN);
	rgb->Render();
	std::cout << "Teleop is now enabled!\n";
}

void DrivetrainExampleRobot::TeleopPeriodic() {
	drive->ArcadeDrive(stick->GetAxis(GAMEPAD::AXES::LY), stick->GetAxis(GAMEPAD::AXES::LX) * -1.0);
}

void DrivetrainExampleRobot::AutonomousInit() {
	rgb->SetAll(COLORS::BLUE);
	rgb->Render();
	std::cout << "Auto is now enabled!\n";
}

void DrivetrainExampleRobot::AutonomousPeriodic() {}

void DrivetrainExampleRobot::DisabledInit() {
	rgb->SetAll(COLORS::RED);
	rgb->Render();
	std::cout << "Robot is now disabled.\n";
}

void DrivetrainExampleRobot::DisabledPeriodic() {
	// Do nothing
}
