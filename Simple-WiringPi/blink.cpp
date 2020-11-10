#include <wiringPi.h>
#include <iostream>

int main() {
    std::cout << "Hello world!\n";
    wiringPiSetup();
    pinMode(0, OUTPUT);
    std::cout << "Setup done.\n";

    while(true) {
        digitalWrite(0, HIGH);
        std::cout << "ON\n";
        delay(500);
        digitalWrite(0, LOW);
        std::cout << "OFF\n";
        delay(500);
    }

    return 0;
}