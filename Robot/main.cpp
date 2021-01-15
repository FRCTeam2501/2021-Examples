#include <iostream>

#include <unistd.h>

#include "2501/DifferentialDrive.h"
#include "2501/Joystick.h"
#include "2501/ServoHat.h"
#include "2501/SpeedController.h"


int main() {
	ServoHat *hat = new ServoHat();
	SpeedController *lf = new SpeedController(hat, 0U),
					*lr = new SpeedController(hat, 2U),
					*rf = new SpeedController(hat, 1U),
					*rr = new SpeedController(hat, 3U),
					*shooter = new SpeedController(hat, 4U);
	DifferentialDrive *drive = new DifferentialDrive(lf, lr, rf, rr);
	Joystick *stick = new Joystick();


	if(!stick->IsOpen()) {
		return -255;
	}
	drive->SetLeftInverted(true);
	drive->SetRightInverted(true);


	bool startWasPressed = false;
	bool aWasPressed = false;
	
	while(true) {
		// Sleep for 1ms
		usleep(1000U);


		// Update joystick values
		stick->Update();


		// Deal with start button
		if(stick->GetButton(GAMEPAD::BUTTONS::START) && !startWasPressed) {
			if(!hat->IsEnabled()) {
				hat->Enable();
				std::cout << "Robot is now enabled!\n";
			}
			else {
				hat->Disable();
				std::cout << "Robot is now disabled.\n";
			}
			startWasPressed = true;
		}
		else if(!stick->GetButton(GAMEPAD::BUTTONS::START) && startWasPressed) {
			startWasPressed = false;
		}


		// Deal with drive
		if(hat->IsEnabled()) {
			drive->ArcadeDrive(stick->GetAxis(GAMEPAD::AXES::LY), stick->GetAxis(GAMEPAD::AXES::LX) * -1.0);
		}


		// Deal with shooter
		if(hat->IsEnabled()) {
			// Button to run
			if(stick->GetButton(GAMEPAD::BUTTONS::A) && !aWasPressed) {
				shooter->Set(0.5);
				std::cout << "Shooter: ON\n";
				aWasPressed = true;
			}
			else if(!stick->GetButton(GAMEPAD::BUTTONS::A) && aWasPressed) {
				shooter->Set(0.0);
				std::cout << "Shooter: OFF\n";
				aWasPressed = false;
			}
		}
	}
	return 0;
}