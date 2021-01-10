#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

int main() {
    std::cout << "Hello world!\n";
    wiringPiSetup();
    
    softPwmCreate(0, 0, 50);
    std::cout << "Setup done.\n";

    while(true) {
        softPwmWrite(0, 5);
        std::cout << "0%\n";
        delay(5000);
        softPwmWrite(0, 7);
        std::cout << "50%\n";
        delay(5000);
        softPwmWrite(0, 10);
        std::cout << "100%\n";
        delay(5000);
    }

    return 0;
}