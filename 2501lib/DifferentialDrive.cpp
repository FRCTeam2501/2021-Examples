#include <algorithm>
#include <cmath>

#include "DifferentialDrive.h"


double DifferentialDrive::Clamp(double val, double min, double max) {
	return std::min(std::max(val, min), max);
}

DifferentialDrive::DifferentialDrive(SpeedController *lf, SpeedController *lr, SpeedController *rf, SpeedController *rr) {
	DifferentialDrive::lf = lf;
	DifferentialDrive::lr = lr;
	DifferentialDrive::rf = rf;
	DifferentialDrive::rr = rr;
}

DifferentialDrive::DifferentialDrive(SpeedController &lf, SpeedController &lr, SpeedController &rf, SpeedController &rr) {
	DifferentialDrive::lf = &lf;
	DifferentialDrive::lr = &lr;
	DifferentialDrive::rf = &rf;
	DifferentialDrive::rr = &rr;
}

void DifferentialDrive::TankDrive(double left, double right) {
	if(invertLeft)
		left *= -1.0;
	if(invertRight)
		right *= -1.0;

	DifferentialDrive::lf->Set(left);
	DifferentialDrive::lr->Set(left);
	DifferentialDrive::rf->Set(right);
	DifferentialDrive::rr->Set(right);
}

void DifferentialDrive::ArcadeDrive(double forward, double rotation) {
	forward = Clamp(forward, -1.0, 1.0);
	rotation = Clamp(rotation, -1.0, 1.0);

	double max = std::copysign(std::max(std::abs(forward), std::abs(rotation)), forward);
	double left, right;

	if(forward >= 0.0) {
		// First quadrant, else second quadrant
		if (rotation >= 0.0) {
			left = max;
			right = forward - rotation;
		} else {
			left = forward + rotation;
			right = max;
		}
	}
	else {
		// Third quadrant, else fourth quadrant
		if (rotation >= 0.0) {
			left = forward + rotation;
			right = max;
		} else {
			left = max;
			right = forward - rotation;
		}
	}
	left = Clamp(left, -1.0, 1.0);
	right = Clamp(right, -1.0, 1.0);

	TankDrive(left, right);
}

void DifferentialDrive::SetLeftInverted(bool leftInverted) {
	invertLeft = leftInverted;
}

bool DifferentialDrive::GetLeftInverted() {
	return invertLeft;
}

void DifferentialDrive::SetRightInverted(bool rightInverted) {
	invertRight = rightInverted;
}

bool DifferentialDrive::GetRightInverted() {
	return invertRight;
}
