/*
 * RobotMath.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#include <RobotMovement/RobotMath.h>

#include <cmath>

RobotMath::~RobotMath()
{

}

double RobotMath::normalizeAngle(double angle)
{
	double theta = fmod((fmod(angle, 360) + 360), 360);
	if (theta > 180) return (theta - 360);
	return theta;
}


 double RobotMath::angleDistance(double angle1, double angle2)
 {
	double dist = normalizeAngle(angle2) - normalizeAngle(angle1);
	if (std::abs(dist) > 180)
	{
		double sgnval = sgn(dist);
		return -sgnval * (360 - std::abs(dist));
	}
	return dist;
}

double RobotMath::sgn(double n)
{
	return std::abs(n) / n;
}


bool RobotMath::isValidPower(double pow)
{
	return (pow >= -1 && pow <= 1);
}

double RobotMath::makeValidPower(double pow)
{
	return (pow < -1 ? -1 : (pow > 1 ? 1 : pow));
}

MotorDir RobotMath::getMotorDirToTarget(double currentAngle, double targetAngle)
{
	currentAngle = normalizeAngle(currentAngle);
	targetAngle = normalizeAngle(targetAngle);
	int retDir = 1 * (std::abs(currentAngle - targetAngle) > 180 ? 1 : -1) * (currentAngle - targetAngle < 0 ? -1 : 1);

	//MotorDir is syntactic sugar; could just return retDir
	if (currentAngle - targetAngle == 0 || currentAngle - targetAngle == 180) return MotorDir::NONE;
	return (retDir == 1 ? MotorDir::CW : MotorDir::CCW);
}
