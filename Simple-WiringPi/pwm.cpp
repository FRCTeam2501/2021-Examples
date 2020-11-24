#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

int main() {
    std::cout << "Hello world!\n";
    wiringPiSetup();
    
    softPwmCreate(0, 0, 100);
    std::cout << "Setup done.\n";

    while(true) {
        softPwmWrite(0, 75);
        std::cout << "75%\n";
        delay(5000);
        softPwmWrite(0, 50);
        std::cout << "50%\n";
        delay(5000);
        softPwmWrite(0, 25);
        std::cout << "25%\n";
        delay(5000);
    }

    return 0;
}