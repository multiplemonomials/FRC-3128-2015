/*
 * LinearAngleTarget.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include <cmath>

#include <HardwareLink/Motor/SpeedControl/LinearAngleTarget.h>
#include <Exception.h>
#include <RobotMovement/RobotMath.h>

LinearAngleTarget::~LinearAngleTarget()
{

}

LinearAngleTarget::LinearAngleTarget(double minSpeed, double threshold, double kP)
:_minSpeed(std::abs(minSpeed)),
_targetAngle(),
_threshold(threshold),
_kP(kP)
{
	if (!RobotMath::isValidPower(minSpeed))
	{
		THROW_EXCEPTION("The minimum power is incorrect!");
	}
}

void LinearAngleTarget::setControlTargetImpl(double & val)
{
	_targetAngle = (fmod(val, 180.0) == 0 ? _targetAngle : val);
}

double LinearAngleTarget::speedControlStep(Time::Duration dt)
{
	double error = RobotMath::angleDistance(getLinkedEncoderAngle(), _targetAngle);
	double sgn = RobotMath::sgn(error);
	double pGain = sgn*(std::abs(error))*((1- _minSpeed)/90.0) + _minSpeed;
	pGain = (std::abs(pGain) > _minSpeed ? pGain : RobotMath::getMotorDirToTarget(getLinkedEncoderAngle(), _targetAngle) * _minSpeed);

	if(std::abs(error) < _threshold) {return 0;}
	return pGain;
}

void LinearAngleTarget::clearControlRunImpl() {}

bool LinearAngleTarget::isComplete()
{
	double x = std::abs(RobotMath::angleDistance(getLinkedEncoderAngle(), _targetAngle));
	return x < _threshold;
}
