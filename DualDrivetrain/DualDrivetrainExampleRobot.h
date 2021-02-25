#include "2501/RobotBase.h"

#include "2501/ARGB.h"
#include "2501/DifferentialDrive.h"
#include "2501/PWMSpeedController.h"


class DualDrivetrainExampleRobot : public RobotBase {
 private:
	PWMSpeedController *l, *r;
	DifferentialDrive *drive;
	ARGB *rgb;

 public:
	DualDrivetrainExampleRobot(Joystick *stick, ServoHat *hat);

	void Shutdown() override;
	
	void RobotInit() override;
	void RobotPeriodic() override;

	void TeleopInit() override;
	void TeleopPeriodic() override;

	void DisabledInit() override;
	void DisabledPeriodic() override;
};
