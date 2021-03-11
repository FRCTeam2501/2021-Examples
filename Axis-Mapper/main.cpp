#include <iostream>
#include <unistd.h>

#include "2501/Joystick.h"


int main() {
	std::cout << "Started axis mapping program.\n";

	Joystick *stick = new Joystick();
	const uint8_t AXIS_COUNT = stick->GetAxisCount();
	int16_t axisValues[AXIS_COUNT];

	std::cout << "Detected " << +AXIS_COUNT << " axis\n";

	while(true) {
		// Sleep for period
		usleep(50);

		// Update joystick values
		stick->Update();

		for(uint8_t i = 0; i < AXIS_COUNT; i++) {
			int16_t last = axisValues[i];
			axisValues[i] = stick->GetRawAxis(i);
			if(abs(last - axisValues[i]) > 2048) {
				std::cout << "Axis '" << +i << "' was moved.\n";
			}
		}
	}
}