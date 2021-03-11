#include <iostream>

#include <unistd.h>

#include "2501/Joystick.h"
#include "2501/PWM.h"
#include "2501/ServoHat.h"



int main() {
	Joystick *stick = new Joystick();
	ServoHat *hat = new ServoHat();

	PWM *ch1 = new PWM(hat, 8U),
		*ch2 = new PWM(hat, 9U),
		*ch3 = new PWM(hat, 10U);

	uint8_t state = 0;

	while(true) {
		stick->Update();

		if(stick->HasButtonChanged(GAMEPAD::BUTTONS::START)) {
			if(stick->GetButton(GAMEPAD::BUTTONS::START)) {
				if(hat->IsEnabled()) {
					std::cout << "Disabled\n";
					hat->Disable();
				}
				else {
					std::cout << "Enabled\n";
					hat->Enable();
				}
			}
		}

		if(hat->IsEnabled()) {
			if(stick->HasButtonChanged(GAMEPAD::BUTTONS::A)) {
				if(stick->GetButton(GAMEPAD::BUTTONS::A)) {
					state++;
					if(state == 3) {
						state = 0;
					}

					switch(state) {
						case 0:
							ch1->SetAngle(0U);
							ch2->Set(-1.0);
							ch3->SetPercent(0.0);
							break;
						case 1:
							ch1->SetAngle(90U);
							ch2->Set(0.0);
							ch3->SetPercent(0.5);
							break;
						case 2:
							ch1->SetAngle(180U);
							ch2->Set(1.0);
							ch3->SetPercent(1.0);
							break;
						default:
							state = 0;
							break;
					}
				}
			}
		}

		usleep(1000);
	}
}
