#include "MotorScanner.h"

#include <iostream>




MotorScanner::MotorScanner(Joystick *stick, ServoHat *hat) : RobotBase(stick, hat, CONFIG::START_BUTTON, CONFIG::SELECT_BUTTON, 10000U) {
	rgb = new ARGB(9U);

	for(uint8_t i = 0; i < CONFIG::MOTOR_COUNT; i++) {
		motors[i] = new PWMSpeedController(hat, i);
	}
}

void MotorScanner::Shutdown() {
	delete rgb;

	//delete lf;
	//delete lr;
	//delete rf;
	//delete rr;

	//delete drive;
}

void MotorScanner::RobotInit() {
	rgb->SetAll(COLORS::WHITE);
	rgb->Render();
}

void MotorScanner::RobotPeriodic() {
	// do nothing
}

void MotorScanner::TeleopInit() {
	rgb->SetAll(COLORS::GREEN);
	rgb->Render();
	std::cout << "Teleop is now enabled!\n";

	teleopState = 0;
}

void MotorScanner::TeleopPeriodic() {
	uint8_t lastState = teleopState;

	if(stick->HasButtonChanged(CONFIG::FORWARD_BUTTON)) {
		if(stick->GetButton(CONFIG::FORWARD_BUTTON)) {
			if(teleopState == (CONFIG::MOTOR_COUNT * 2)) {
				teleopState = 0;
			}
			else {
				teleopState++;
			}
		}
	}
	else if(stick->HasButtonChanged(CONFIG::REVERSE_BUTTON)) {
		if(stick->GetButton(CONFIG::REVERSE_BUTTON)) {
			if(teleopState == 0) {
				teleopState = (CONFIG::MOTOR_COUNT * 2);
			}
			else {
				teleopState--;
			}
		}
	}

	if(teleopState != lastState) {
		uint8_t motor = (teleopState - 1) / 2;

		uint8_t lastMotor = (lastState - 1) / 2;
		if(lastMotor != motor) {
			std::cout << "Turning motor " << +lastMotor << " OFF.\n";
			motors[lastMotor]->Set(0.0);
		}

		uint8_t subState = (teleopState - 1) % 2;
		switch(subState) {
			case 0:
				std::cout << "Turning motor " << +motor << " FORWARD.\n";
				motors[motor]->Set(CONFIG::FORWARD_SPEED);
				break;
			case 1:
				std::cout << "Turning motor " << +motor << " REVERSE.\n";
				motors[motor]->Set(CONFIG::REVERSE_SPEED);
				break;
			default:
				std::cout << "Turning all motors OFF.\n";
				for(uint8_t i = 0; i < CONFIG::MOTOR_COUNT; i++) {
					motors[i]->Set(0.0);
				}
				break;
		}
	}
}

void MotorScanner::AutonomousInit() {
	rgb->SetAll(COLORS::BLUE);
	rgb->Render();
	std::cout << "Auto is now enabled!\n";

	autoCycles = 0;
}

void MotorScanner::AutonomousPeriodic() {
	if(autoCycles == CONFIG::CALIBRATION_COUNT * CONFIG::MOTOR_COUNT) {
		std::cout << "[Auto]\t" << "Calibration finished!\n";
	}
	else if(autoCycles > CONFIG::CALIBRATION_COUNT * CONFIG::MOTOR_COUNT) {
		// Do nothing
	}
	else {
		uint8_t motor = (autoCycles / CONFIG::CALIBRATION_COUNT);
		if(autoCycles % CONFIG::CALIBRATION_COUNT == 0) {
			std::cout << "[Auto]\t" << "Started calibrating motor " << +motor << ".\n";
			motors[motor]->Set(1.0);
		}
		else if(autoCycles % CONFIG::CALIBRATION_COUNT == CONFIG::CALIBRATION_COUNT / 3) {
			motors[motor]->Set(1.0);
		}
		else if(autoCycles % CONFIG::CALIBRATION_COUNT == CONFIG::CALIBRATION_COUNT * 2 / 3) {
			motors[motor]->Set(0.0);
			std::cout << "[Auto]\t" << "Ended calibrating motor " << +motor << ".\n";
		}
	}
	autoCycles++;
}

void MotorScanner::DisabledInit() {
	rgb->SetAll(COLORS::RED);
	rgb->Render();
	std::cout << "Robot is now disabled.\n";
}

void MotorScanner::DisabledPeriodic() {
	// Do nothing
}
