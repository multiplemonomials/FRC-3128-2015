/*
 * RobotMath.h
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#ifndef ROBOTMATH_H_
#define ROBOTMATH_H_

#include <math.h>
#include <boost/math/constants/constants.hpp>

#include <HardwareLink/Motor/MotorDir.h>

class RobotMath
{
public:
	virtual ~RobotMath();

	//returns -1.0 if the argument is negative, ot 1.0 if the argument is positive
	static double sgn(double n);

	//returns whether or not the power is valid
	static bool isValidPower(double pow);

	/**
	 * Limits the angle to between 0 and 359 degrees for all math. All angles
	 * should be normalized before use.
	*/
	static double normalizeAngle(double angle);

	/**
	 * Finds the shortest distance between two angles.
	 */
	static double angleDistance(double angle1, double angle2);

	/**
	 * Makes the provided power into a valid power level.
	 */
	static double makeValidPower(double pow);

	/**
	 * Determines the appropriate direction for a motor to turn.
	 * @param currentAngle the current angle of the motor
	 * @param targetAngle the target angle of the motor
	 */
	static MotorDir getMotorDirToTarget(double currentAngle, double targetAngle);

	/**
	 * Convert degrees to radians
	 * @param angle degrees
	 * @return radians
	 */
	 static double dTR(double angle)
	 {
		 return boost::math::constants::pi<double>() * angle / 180.0;
	 }

	/**
	 * Convert radians to degrees
	 *
	 * @param rad radians
	 * @return degrees
	 */
	 static double rTD(double rad)
	 {
		 return rad * (180.0 / boost::math::constants::pi<double>());
	 }
};

#endif /* ROBOTMATH_H_ */
