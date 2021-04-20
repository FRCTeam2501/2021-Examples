#include "2501/RobotBase.h"

#include "2501/ARGB.h"
#include "2501/DifferentialDrive.h"
#include "2501/PWMSpeedController.h"


/*
 ********** NOTE: YOU ONLY NEED TO EDIT THE CONFIG VALUES **********
 */
namespace CONFIG {
		// This most likely does not need editing
	constexpr uint8_t MOTOR_COUNT = 16U;
		// Start button to enable the code
	constexpr uint8_t START_BUTTON = GAMEPAD::BUTTONS::START;
		// Select button to start auto (not used)
	constexpr uint8_t SELECT_BUTTON = GAMEPAD::BUTTONS::SELECT;
		// Button to go forward in teleopState
	constexpr uint8_t FORWARD_BUTTON = GAMEPAD::BUTTONS::Y;
		// Button to go backwards in teleopState
	constexpr uint8_t REVERSE_BUTTON = GAMEPAD::BUTTONS::A;
		// Speed to run motors at (forwards)
	constexpr double FORWARD_SPEED = 0.5;
		// Speed to run motors at (reverse)
	constexpr double REVERSE_SPEED = FORWARD_SPEED * -1.0;
		// Number of auto cycles for each calibration period, 1 cycle = 10ms
	constexpr uint32_t CALIBRATION_COUNT = 78;
}

class MotorScanner : public RobotBase {
 private:
	PWMSpeedController *motors[CONFIG::MOTOR_COUNT];
	ARGB *rgb;

	uint8_t teleopState;
	uint32_t autoCycles;

 public:
	MotorScanner(Joystick *stick, ServoHat *hat);

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
