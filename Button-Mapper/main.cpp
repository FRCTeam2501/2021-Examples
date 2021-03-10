#include <iostream>
#include <unistd.h>

#include "2501/Joystick.h"


int main() {
	std::cout << "Started button mapping program.\n";

	Joystick *stick = new Joystick();
	const uint8_t BUTTON_COUNT = stick->GetButtonCount();

	std::cout << "Detected " << +BUTTON_COUNT << " buttons\n";

	while(true) {
		// Sleep for period
		usleep(50);

		// Update joystick values
		stick->Update();

		for(uint8_t i = 0; i < BUTTON_COUNT; i++) {
			if(stick->HasButtonChanged(i)) {
				if(stick->GetButton(i)) {
					std::cout << "Button '" << +i << "' was pressed.\n";
				}
				else {
					std::cout << "Button '" << +i << "' was released.\n";
				}
			}
		}
	}
}