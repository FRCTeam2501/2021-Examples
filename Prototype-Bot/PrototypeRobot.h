#include "2501/RobotBase.h"

#include "2501/ARGB.h"
#include "2501/DifferentialDrive.h"
#include "2501/PWMSpeedController.h"
#include "2501/L298N.h"


class PrototypeRobot : public RobotBase {
 private:
	PWMSpeedController *lf, *lr, *rf, *rr, *shooter;
	L298N *intake;
	DifferentialDrive *drive;
	ARGB *rgb;

	uint16_t autoCycles;

 public:
	PrototypeRobot(Joystick *stick, ServoHat *hat);

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
