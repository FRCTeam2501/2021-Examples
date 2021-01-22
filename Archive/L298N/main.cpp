#include <iostream>
#include <unistd.h>

#include "wiringPi.h"
#include "bcm2835.h"
#include "pca9685.h"


constexpr uint16_t MAX_12_BIT = 0x0FFF;
constexpr uint16_t HALF_12_BIT = MAX_12_BIT / 2;
constexpr uint16_t ZERO = 0x0000;

constexpr uint8_t PIN = 5U;


int main() {
	std::cout << "Hello world!\n";

	if (bcm2835_init() != 1) {
		fprintf(stderr, "bcm2835_init() failed\n");
		return -2;
	}

	PCA9685 hat;
	std::cout << "1\n";

	hat.SetInvert(false);
	hat.SetOutDriver(true);
	hat.SetFrequency(50);
	std::cout << "1.1\n";

	wiringPiSetupGpio();
	std::cout << "2\n";


	pinMode(24, OUTPUT);
	std::cout << "3\n";
	pinMode(25, OUTPUT);
	std::cout << "4\n";

	std::cout << "Setup done.\n";


	while(true) {
		std::cout << "Forward 100%\n";
		digitalWrite(24, HIGH);
		digitalWrite(25, LOW);
		hat.SetFullOn(PIN, true);
		sleep(5U);

		std::cout << "Forward 50%\n";
		digitalWrite(24, HIGH);
		digitalWrite(25, LOW);
		hat.Write(PIN, HALF_12_BIT);
		sleep(5U);

		std::cout << "0%\n";
		digitalWrite(24, HIGH);
		digitalWrite(25, LOW);
		hat.SetFullOff(PIN, true);
		sleep(5U);

		std::cout << "Reverse 50%\n";
		digitalWrite(24, LOW);
		digitalWrite(25, HIGH);
		hat.Write(PIN, HALF_12_BIT);
		sleep(5U);

		std::cout << "Reverse 100%\n";
		digitalWrite(24, LOW);
		digitalWrite(25, HIGH);
		hat.SetFullOn(PIN, true);
		sleep(5U);
	}

	return 0;
}