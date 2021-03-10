#include "2501/RobotBase.h"

#include "2501/ARGB.h"
#include "2501/DifferentialDrive.h"
#include "2501/PWMSpeedController.h"


class DrivetrainExampleRobot : public RobotBase {
 private:
	PWMSpeedController *lf, *lr, *rf, *rr;
	DifferentialDrive *drive;
	ARGB *rgb;

 public:
	DrivetrainExampleRobot(Joystick *stick, ServoHat *hat);

	void Shutdown() override;
	
	void RobotInit() override;
	void RobotPeriodic() override;

	void TeleopInit() override;
	void TeleopPeriodic() override;

	void AutonomousInit() override;
	void AutonomousPeriodic() override;

	void DisabledInit() override;
	void DisabledPeriodic() override;
};
