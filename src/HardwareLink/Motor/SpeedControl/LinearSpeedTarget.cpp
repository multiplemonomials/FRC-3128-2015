/*
 * LinearSpeedTarget.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include <HardwareLink/Motor/SpeedControl/LinearSpeedTarget.h>
#include <RobotMovement/RobotMath.h>

LinearSpeedTarget::LinearSpeedTarget(double tgtSpeed, double refreshTime)
:_tgtSpeed(tgtSpeed),
_dtAccum(refreshTime),
_refreshTime(50),
_lastEncoderAngle(-1)
{

}

LinearSpeedTarget::~LinearSpeedTarget()
{

}

 LinearSpeedTarget::LinearSpeedTarget(double tgtSpeed)
: LinearSpeedTarget(tgtSpeed, 0.050)
{

}

void LinearSpeedTarget::setControlTargetImpl(double & d)
{
	_tgtSpeed = d;
	_lastEncoderAngle = getLinkedEncoderAngle();
}

double LinearSpeedTarget::speedControlStep(Time::Duration dt)
{
	_dtAccum += dt.total_milliseconds();

	if(_dtAccum < _refreshTime)
	{
		return getLinkedMotorSpeed();
	}

	// Power * (current rate of change) / (target rate of change) => pow * (deg/sec) / (deg/sec) => pow
	double retVal = RobotMath::makeValidPower((getLinkedMotorSpeed() / _tgtSpeed) * ((getLinkedEncoderAngle() - _lastEncoderAngle)/_dtAccum));
	_dtAccum = 0;
	_lastEncoderAngle = getLinkedEncoderAngle();
	return retVal;
}


void LinearSpeedTarget::setRefreshTime(double refreshTime)
{
	_refreshTime = refreshTime/1000.0;
}

void LinearSpeedTarget::clearControlRunImpl()
{
	_tgtSpeed = 0; _lastEncoderAngle = getLinkedEncoderAngle();
}

bool LinearSpeedTarget::isComplete()
{
	return false;
}
