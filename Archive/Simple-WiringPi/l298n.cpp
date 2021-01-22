#include <wiringPi.h>
#include <iostream>

int main() {
	std::cout << "Hello world!\n";

	wiringPiSetupGpio();
	pinMode(24, OUTPUT);
	pinMode(25, OUTPUT);

	std::cout << "Setup done.\n";

	digitalWrite(24, HIGH);
	digitalWrite(25, LOW);

	while(true) {
		delay(5000);
	}

	return 0;
}