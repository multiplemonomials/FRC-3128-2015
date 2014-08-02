/*
 * MotorControl.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include "MotorControl.h"
#include <Util/Time.h>
#include "MotorLink.h"


MotorControl::~MotorControl()
{

}

void MotorControl::setControlledMotor(MotorLink * m)
{
	std::unique_lock<std::mutex>();
	_controlledMotor = m;
}


void MotorControl::setControlTarget(double & val)
{
	std::unique_lock<std::mutex>();
	setControlTargetImpl(val);
}

Time::Timepoint MotorControl::getLastRuntime()
{
	return _lastRuntime;
}

Time::Duration MotorControl::getLastRuntimeDist()
{
	return Time::Timepoint(Time::GetLocalTime()) - _lastRuntime;
}

double MotorControl::getLinkedEncoderAngle()
{
	return _controlledMotor->getEncoderAngle();
}

void MotorControl::cancel()
{
	std::unique_lock<std::mutex>();
	_speedSetScheduler.Cancel();
}

double MotorControl::getLinkedMotorSpeed()
{
	return _controlledMotor->getSpeed();
}

void MotorControl::start()
{
	//TODO configuration setting
	_speedSetScheduler.StartRelative(Time::Milliseconds(100));
}

void MotorControl::execute()
{
	std::unique_lock<std::mutex>(_mutex);


	//TODO This logic was modified because it seemed bugged.  Find out if it was a bug or a feature.
	if(isComplete())
	{
		_controlledMotor->setSpeedForce(0.0);
	}
	else
	{
		_controlledMotor->setSpeedForce(speedControlStep(getLastRuntimeDist()));
	}

	_lastRuntime = Time::Timepoint(Time::GetLocalTime());

	//TODO configuration setting
	_speedSetScheduler.StartRelative(Time::Milliseconds(100));
}

void MotorControl::clearControlRun()
{
	std::unique_lock<std::mutex>(_mutex);
	clearControlRunImpl();
}




