#include "PrototypeRobot.h"

#include <iostream>
#include <chrono>

#include "wiringPi.h"


PrototypeRobot::PrototypeRobot(Joystick *stick, ServoHat *hat) : RobotBase(stick, hat, GAMEPAD::BUTTONS::START, GAMEPAD::BUTTONS::SELECT, 10000U) {
	rgb = new ARGB(9U);

	lf = new PWMSpeedController(hat, 0U);
	lr = new PWMSpeedController(hat, 2U);
	rf = new PWMSpeedController(hat, 1U);
	rr = new PWMSpeedController(hat, 3U);

	drive = new DifferentialDrive(lf, lr, rf, rr);

	shooter = new PWMSpeedController(hat, 6U);
	hoodAdjust = new Servo(hat, 7U);

	wiringPiSetupGpio();
	intake = new L298N(hat, 5U, 24U, 25U);
}

void PrototypeRobot::Shutdown() {
	delete rgb;

	//delete lf;
	//delete lr;
	//delete rf;
	//delete rr;

	//delete drive;

	//delete shooter;
	//delete hoodAdjust;
}

void PrototypeRobot::RobotInit() {
	rgb->SetAll(COLORS::WHITE);
	rgb->Render();

	drive->SetLeftInverted(true);
	drive->SetRightInverted(true);
}

void PrototypeRobot::RobotPeriodic() {
	// Do nothing
}

void PrototypeRobot::TeleopInit() {
	rgb->SetAll(COLORS::GREEN);
	rgb->Render();
	std::cout << "Teleop is now enabled!\n";
}

void PrototypeRobot::TeleopPeriodic() {
    // Deal with drive
	drive->ArcadeDrive(stick->GetAxis(GAMEPAD::AXES::LY), stick->GetAxis(GAMEPAD::AXES::LX) * -1.0);

	// Deal with shooter
	if(stick->HasButtonChanged(GAMEPAD::BUTTONS::A)) {
		if(stick->GetButton(GAMEPAD::BUTTONS::A)) {
			shooter->Set(0.5);
			std::cout << "Shooter: ON\n";
		}
		else {
			shooter->Set(0.0);
			std::cout << "Shooter: OFF\n";
		}
	}

	//Deal with shooter hood adjust
	if(stick->HasButtonChanged(GAMEPAD::BUTTONS::RT)) {
		if(stick->GetButton(GAMEPAD::BUTTONS::RT)) {
			uint8_t angle = hoodAdjust->GetAngle();
			if(angle >= 175)
				angle = 180;
			else
				angle += 5;

			hoodAdjust->SetAngle(angle);
			std::cout << "Hood is now at: " << +angle << "\n";
		}
	}
	else if(stick->HasButtonChanged(GAMEPAD::BUTTONS::RB)) {
		if(stick->GetButton(GAMEPAD::BUTTONS::RB)) {
			uint8_t angle = hoodAdjust->GetAngle();
			if(angle <= 5)
				angle = 0;
			else
				angle -= 5;

			hoodAdjust->SetAngle(angle);
			std::cout << "Hood is now at: " << +angle << "\n";
		}
	}

	// Deal with "intake"
	if(stick->HasButtonChanged(GAMEPAD::BUTTONS::LB)) {
		if(stick->GetButton(GAMEPAD::BUTTONS::LB)) {
			intake->Set(1.0);
			std::cout << "Intake: Forward\n";
		}
		else {
			intake->Set(0.0);
			std::cout << "Intake: OFF\n";
		}
	}
	else if(stick->HasButtonChanged(GAMEPAD::BUTTONS::LT)) {
		if(stick->GetButton(GAMEPAD::BUTTONS::LT)) {
			intake->Set(-1.0);
			std::cout << "Intake: Reverse\n";
		}
		else {
			intake->Set(0.0);
			std::cout << "Intake: OFF\n";
		}
	}
}

void PrototypeRobot::AutonomousInit() {
	rgb->SetAll(COLORS::BLUE);
	rgb->Render();

	std::cout << "Auto is now enabled!\n";

	autoCycles = 0;
}

void PrototypeRobot::AutonomousPeriodic() {
	if(autoCycles == 0) {
		// For the first cycle in auto, start moving
		std::cout << "[Auto]\t" << "Started driving.\n";
		drive->ArcadeDrive(-0.5, 0.0);
	}
	else if(autoCycles == 150) {
		// For the 150th cycle in auto, stop moving

		/**
		 * Since we told RobotBase that we want each loop to be 10,000us or 10ms, the robot will run at 100 loops per second.
		 * Therefore the 150th loop will be at 1.5 seconds into auto.
		 */

		drive->ArcadeDrive(0.0, 0.0);
		std::cout << "[Auto]\t" << "Stopped driving.\n";
	}


	// Increment our counter of how many cycles have happened in auto
	autoCycles++;
}

void PrototypeRobot::DisabledInit() {
	rgb->SetAll(COLORS::RED);
	rgb->Render();
	std::cout << "Robot is now disabled.\n";
}

void PrototypeRobot::DisabledPeriodic() {
	// Do nothing
}
