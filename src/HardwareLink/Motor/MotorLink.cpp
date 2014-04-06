/*
 * MotorLink.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include <HardwareLink/Motor/MotorLink.h>



MotorLink::~MotorLink()
{

}

void MotorLink::setControlTarget(double target)
{
	if (!_spdControl)
	{
		LOG_FATAL("The speed controller's target was set, but none exists.");
		return;
	}

	if (!_spdControlEnabled)
	{
		LOG_RECOVERABLE("The speed controller's target was set, but it is not enabled.");
		return;
	}


	//this function handles its own mutexing
	_spdControl->setControlTarget(target);
}

void MotorLink::setSpeedImpl(double pow)
{
	_talon->PIDWrite(pow * _speedScalar * (_motorReversed ? -1.0 : 1.0));
}

void MotorLink::setEncoderImpl(std::shared_ptr<AbstractEncoder> enc);

void MotorLink::reverseMotorImpl()
{
	_motorReversed = !_motorReversed;
}

void MotorLink::setSpeedScalarImpl(double powScl)
{
	this.speedScalar = powScl;
}

void MotorLink::startSpeedControlImpl(double target);

void MotorLink::setSpeedControllerImpl(std::shared_ptr<MotorControl> spdControl);

void MotorLink::stopSpeedControlImpl();
