#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

int main() {
    std::cout << "Hello world!\n";
    wiringPiSetup();
    
    softPwmCreate(0, 0, 100);
    std::cout << "Setup done.\n";

    while(true) {
        softPwmWrite(0, 100);
        std::cout << "ON\n";
        delay(500);
        softPwmWrite(0, 50);
        std::cout << "HALF\n";
        delay(500);
        softPwmWrite(0, 0);
        std::cout << "OFF\n";
        delay(500);
    }

    return 0;
}