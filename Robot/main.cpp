#include <iostream>

#include <unistd.h>
#include <signal.h>

#include "2501/ARGB.h"
#include "2501/DifferentialDrive.h"
#include "2501/Joystick.h"
#include "2501/ServoHat.h"
#include "2501/SpeedController.h"


static ARGB *rgb;
static bool running = true;

static void ctrl_c_handler(int signum) {
	(void) (signum);
	running = false;
	std::cout << "\nStopped\n";
}

static void setup_handlers(void) {
	struct sigaction sa;
	sa.sa_handler = ctrl_c_handler;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}


Joystick *stick;
ServoHat *hat;
SpeedController *lf, *lr, *rf, *rr, *shooter;
DifferentialDrive *drive;
bool startWasPressed = false;
bool aWasPressed = false;

void Setup() {
	stick = new Joystick(0U);
	if(!stick->IsOpen()) {
		exit(-255);
	}

	hat = new ServoHat();
	lf = new SpeedController(hat, 0U),
	lr = new SpeedController(hat, 2U),
	rf = new SpeedController(hat, 1U),
	rr = new SpeedController(hat, 3U),
	shooter = new SpeedController(hat, 4U);
	drive = new DifferentialDrive(lf, lr, rf, rr);
	drive->SetLeftInverted(true);
	drive->SetRightInverted(true);

	rgb = new ARGB();
	rgb->Set(COLORS::BLUE);
}

void Enabled() {
	// Deal with drive
	drive->ArcadeDrive(stick->GetAxis(GAMEPAD::AXES::LY), stick->GetAxis(GAMEPAD::AXES::LX) * -1.0);

	// Deal with shooter
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


int main() {
	setup_handlers();

	Setup();
	
	while(running) {
		// Sleep for 1ms
		usleep(1000U);


		// Update joystick values
		stick->Update();


		// Deal with start button
		if(stick->GetButton(GAMEPAD::BUTTONS::START) && !startWasPressed) {
			if(!hat->IsEnabled()) {
				hat->Enable();
				rgb->Set(COLORS::GREEN);
				std::cout << "Robot is now enabled!\n";
			}
			else {
				hat->Disable();
				rgb->Set(COLORS::RED);
				std::cout << "Robot is now disabled.\n";
			}
			startWasPressed = true;
		}
		else if(!stick->GetButton(GAMEPAD::BUTTONS::START) && startWasPressed) {
			startWasPressed = false;
		}
		
		if(hat->IsEnabled()) {
			Enabled();
		}
	}

	delete rgb;
	return 0;
}