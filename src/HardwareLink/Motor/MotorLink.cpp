/*
 * MotorLink.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include <HardwareLink/Motor/MotorLink.h>
#include "MotorControl.h"


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

void MotorLink::setEncoderImpl(std::shared_ptr<AbstractEncoder> enc)
{
	if(_encoder)
	{
		LOG_RECOVERABLE("MotorLink: The encoder has been changed when one already existed.");
	}
	_encoder = enc;
}

void MotorLink::reverseMotorImpl()
{
	_motorReversed = !_motorReversed;
}

void MotorLink::setSpeedScalarImpl(double powScl)
{
	_speedScalar = powScl;
}

void MotorLink::startSpeedControlImpl(double target)
{
	_spdControl->clearControlRun();
	_spdControl->setControlTarget(target);
	_spdControl->setControlledMotor(this);
	_spdControl->start();
	_spdControlEnabled = true;
}


void MotorLink::setSpeedControllerImpl(std::shared_ptr<MotorControl> spdControl)
{
	if(_spdControlEnabled)
	{
		_spdControlEnabled = false;
		_spdControl->cancel();
		LOG_RECOVERABLE("The speed controller was changed when one was running.");
	}

	_spdControl = spdControl;
}

void MotorLink::stopSpeedControlImpl()
{
	_talon->Set(0);
	_spdControl->cancel();
	_spdControlEnabled = false;
}

void MotorLink::setSpeed(double pow)
{
    if(_spdControlEnabled)
    {
        _spdControl->cancel();
        _spdControlEnabled = false;
        LOG_UNUSUAL("The motor power was set from outside the speed controller, so the controller was canceled.");
    }

	_cmdProcessor.Enqueue(&MotorLink::setSpeedImpl, boost::ref(*this), pow);
}
