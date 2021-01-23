#include "L298N.h"

#include <stdlib.h>


L298N::L298N(ServoHat *hat, uint8_t channel, uint8_t in1, uint8_t in2) {
	pwm = new PWM(hat, channel);
	o1 = new DigitalOutput(in1);
	o2 = new DigitalOutput(in2);
}

L298N::L298N(ServoHat &hat, uint8_t channel, uint8_t in1, uint8_t in2) {
	pwm = new PWM(hat, channel);
	o1 = new DigitalOutput(in1);
	o2 = new DigitalOutput(in2);
}

/**
 * @brief Set the SpeedController to a speed
 * 
 * @param speed Speed to set it to
 */
void L298N::Set(double speed) {
	if(speed > 0) {
		o1->Set(true);
		o2->Set(false);
		pwm->SetPercent(speed);
	}
	else if(speed < 0) {
		o1->Set(false);
		o2->Set(true);
		pwm->SetPercent(abs(speed));
	}
	else {
		o1->Set(true);
		o2->Set(false);
		pwm->SetPercent(speed);
	}
}