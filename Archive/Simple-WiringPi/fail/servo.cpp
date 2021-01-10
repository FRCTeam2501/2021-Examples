#include <wiringPi.h>
#include <softServo.h>
#include <iostream>

int main() {
    std::cout << "Hello world!\n";
    wiringPiSetup();
    
    softServoSetup(0, -1, -1, -1, -1, -1, -1, -1);
    std::cout << "Setup done.\n";

    while(true) {
        softServoWrite(0, -250);
        std::cout << "75%\n";
        delay(5000);
        softServoWrite(0, 500);
        std::cout << "50%\n";
        delay(5000);
        softServoWrite(0, 1250);
        std::cout << "25%\n";
        delay(5000);
    }

    return 0;
}