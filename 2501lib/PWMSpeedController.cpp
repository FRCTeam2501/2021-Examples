#include "PWMSpeedController.h"


PWMSpeedController::PWMSpeedController(ServoHat *hat, uint8_t channel) {
	pwm = new PWM(hat, channel);
}

PWMSpeedController::PWMSpeedController(ServoHat &hat, uint8_t channel) {
	pwm = new PWM(hat, channel);
}

/**
 * @brief Set the SpeedController to a speed
 * 
 * @param speed Speed to set it to
 */
void PWMSpeedController::Set(double speed) {
	pwm->Set(speed);
}