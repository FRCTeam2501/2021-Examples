#include <iostream>

#include "PWM.h"


constexpr uint16_t MAX_12_BIT = 0x0FFF;

uint16_t PWM::DoubleToValue(double d) {
	if (d <= -1.0) {
		return servo_minCount;
	}
	else if (d == 0.0) {
		return servo_centerCount;
	}
	else if (d >= 1.0) {
		return servo_maxCount;
	}
	else if (d > 0.0) {
		const uint16_t nCount = servo_centerCount + d * (servo_maxCount - servo_centerCount);
		return nCount;
	}
	else {
		const uint16_t nCount = servo_centerCount + d * (servo_centerCount - servo_minCount);
		return nCount;
	}
}

double PWM::AngleToDouble(uint8_t angle) {
	return (angle - 90) / 90.0;
}

PWM::PWM(ServoHat *hat, uint8_t channel) {
	if(channel > ServoHat::MAX_CHANNELS) {
		std::cerr << "Channel number is larger than " << ServoHat::MAX_CHANNELS << "\n";
		exit(-1);
	}

	PWM::hat = hat;
	PWM::channel = channel;

	SetMaxUs(2050U);
	SetCenterUs(1550U);
	SetMinUs(1050U);
}

PWM::PWM(ServoHat &hat, uint8_t channel) {
	if(channel > ServoHat::MAX_CHANNELS) {
		std::cerr << "Channel number is larger than " << ServoHat::MAX_CHANNELS << "\n";
		exit(-1);
	}

	PWM::hat = &hat;
	PWM::channel = channel;

	SetMaxUs(2050U);
	SetCenterUs(1550U);
	SetMinUs(1050U);
}

void PWM::Set(double speed) {
	hat->SetInternal(channel, DoubleToValue(speed), true);
}

void PWM::SetAngle(uint8_t angle) {
	Set(AngleToDouble(angle));
}

void PWM::SetPercent(double p) {
	hat->SetInternal(channel, p * MAX_12_BIT, true);
}

void PWM::SetMaxUs(uint16_t max_us) {
	servo_maxCount = (.5 + ((204.8 * max_us) / 1000));
}

void PWM::SetCenterUs(uint16_t center_us) {
	servo_centerCount = (.5 + ((204.8 * center_us) / 1000));
}

void PWM::SetMinUs(uint16_t min_us) {
	servo_minCount = (.5 + ((204.8 * min_us) / 1000));
}
