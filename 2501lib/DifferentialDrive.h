#pragma once

#include "SpeedController.h"


class DifferentialDrive {
 private:
	// Four speed controllers to use
	SpeedController *lf, *lr, *rf, *rr;
	// Invert the left or right sides
	bool invertLeft = false, invertRight = false;

	// Internal function to clamp numbers between min and max
	double Clamp(double val, double min, double max);

 public:
	/**
	 * @brief Construct a new Differential Drive object
	 * 
	 * @param lf Left front SpeedController
	 * @param lr Left rear SpeedController
	 * @param rf Right front SpeedController
	 * @param rr Right rear SpeedController
	 */
	DifferentialDrive(SpeedController *lf, SpeedController *lr, SpeedController *rf, SpeedController *rr);

	/**
	 * @brief Construct a new Differential Drive object
	 * 
	 * @param lf Left front SpeedController
	 * @param lr Left rear SpeedController
	 * @param rf Right front SpeedController
	 * @param rr Right rear SpeedController
	 */
	DifferentialDrive(SpeedController &lf, SpeedController &lr, SpeedController &rf, SpeedController &rr);

	/**
	 * @brief Drive with tank drive left and right controls
	 * 
	 * @param left Speed for left motors
	 * @param right Speed for right motors
	 */
	void TankDrive(double left, double right);

	/**
	 * @brief Drive with arcade drive forward and rotation controls.
	 * 
	 * @param forward Speed to go forwards/reverse
	 * @param rotation Speed to rotate
	 */
	void ArcadeDrive(double forward, double rotation);

	/**
	 * @brief Set the Left Inverted
	 * 
	 * @param leftInverted If the left side should be inverted
	 */
	void SetLeftInverted(bool leftInverted);

	/**
	 * @brief Get the Left Inverted
	 * 
	 * @return true for yes, false for no
	 */
	bool GetLeftInverted();

	/**
	 * @brief Set the Right Inverted
	 * 
	 * @param leftInverted If the left side should be inverted
	 */
	void SetRightInverted(bool rightInverted);

	/**
	 * @brief Get the Right Inverted
	 * 
	 * @return true for yes, false for no
	 */
	bool GetRightInverted();
};
