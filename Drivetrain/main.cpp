#include <iostream>

#include <unistd.h>
#include <signal.h>

#include "wiringPi.h"

#include "2501/ARGB.h"
#include "2501/DifferentialDrive.h"
#include "2501/Joystick.h"
#include "2501/ServoHat.h"
#include "2501/PWMSpeedController.h"


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
PWMSpeedController *lf, *lr, *rf, *rr;
DifferentialDrive *drive;

void Setup() {
	stick = new Joystick(0U);
	if(!stick->IsOpen()) {
		exit(-255);
	}

	rgb = new ARGB(9U);
	rgb->SetAll(COLORS::BLUE);
	rgb->Render();

	hat = new ServoHat();

	lf = new PWMSpeedController(hat, 0U),
	lr = new PWMSpeedController(hat, 2U),
	rf = new PWMSpeedController(hat, 1U),
	rr = new PWMSpeedController(hat, 3U),

	drive = new DifferentialDrive(lf, lr, rf, rr);
	drive->SetLeftInverted(true);
	drive->SetRightInverted(true);
}

void Enabled() {
	// Deal with drive
	drive->ArcadeDrive(stick->GetAxis(GAMEPAD::AXES::LY), stick->GetAxis(GAMEPAD::AXES::LX) * -1.0);
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
		if(stick->HasButtonChanged(GAMEPAD::BUTTONS::START) && stick->GetButton(GAMEPAD::BUTTONS::START)) {
			// Start button was pressed
			if(!hat->IsEnabled()) {
				hat->Enable();
				rgb->SetAll(COLORS::GREEN);
				rgb->Render();
				std::cout << "Robot is now enabled!\n";
			}
			else {
				hat->Disable();
				rgb->SetAll(COLORS::RED);
				rgb->Render();
				std::cout << "Robot is now disabled.\n";
			}
		}
		
		
		if(hat->IsEnabled()) {
			Enabled();
		}
	}

	delete rgb;
	return 0;
}