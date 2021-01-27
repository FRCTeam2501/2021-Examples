#include <iostream>

#include <unistd.h>
#include <signal.h>

#include "wiringPi.h"

#include "2501/ARGB.h"
#include "2501/DifferentialDrive.h"
#include "2501/Joystick.h"
#include "2501/ServoHat.h"
#include "2501/PWMSpeedController.h"
#include "2501/L298N.h"
#include "2501/Version.h"


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
PWMSpeedController *lf, *lr, *rf, *rr, *shooter;
L298N *intake;
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
	shooter = new PWMSpeedController(hat, 4U);

	drive = new DifferentialDrive(lf, lr, rf, rr);
	drive->SetLeftInverted(true);
	drive->SetRightInverted(true);

	wiringPiSetupGpio();
	intake = new L298N(hat, 5U, 24U, 25U);
}

void Enabled() {
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


int main() {
	setup_handlers();

	Setup();

	std::cout << "Booted with version " << lib_2501::VERSION << "\n";
	
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