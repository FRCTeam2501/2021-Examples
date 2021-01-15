#pragma once

#include "SpeedController.h"


class DifferentialDrive {
 private:
	SpeedController *lf, *lr, *rf, *rr;
	bool invertLeft = false, invertRight = false;

	double Clamp(double val, double min, double max);

 public:
	DifferentialDrive(SpeedController *lf, SpeedController *lr, SpeedController *rf, SpeedController *rr);

	DifferentialDrive(SpeedController &lf, SpeedController &lr, SpeedController &rf, SpeedController &rr);

	void TankDrive(double left, double right);

	void ArcadeDrive(double forward, double rotation);

	void SetLeftInverted(bool leftInverted);

	bool GetLeftInverted();

	void SetRightInverted(bool rightInverted);

	bool GetRightInverted();
};
