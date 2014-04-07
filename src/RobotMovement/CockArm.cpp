/*
 * CockArm.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include <RobotMovement/CockArm.h>
#include <Options.h>
#include <EventManager/Cmd.h>
#include <Util/Time.h>

CockArm::CockArm(std::shared_ptr<DigitalInput> shooterTSensor, std::shared_ptr<MotorLink> mShooter)
:_warned(false),
 _cockArmActive(true),
 _shooterTSensor(shooterTSensor),
 _mShooter(mShooter),
 _mutex(),
 _timer(Cmd::MakeShared(&CockArm::execute, boost::ref(*this)))
{

}

CockArm::~CockArm()
{

}

void CockArm::execute()
{
	boost::unique_lock<boost::mutex>(_mutex);

	if(!_cockArmActive || Options::instance()._armEnabled)
	{
		return;
	}

	if(!_shooterTSensor->Get()) //touch sensor pressed
	{
		_mShooter->setSpeed(-1.0);
	}
	else
	{
		_mShooter->setSpeed(0);
	}

	_timer.StartRelative(Time::Milliseconds(200));
}

void CockArm::start()
{
	if(!(_timer.TimerIsRunning()))
	{
		_timer.StartRelative(Time::Milliseconds(200));
	}
}

void CockArm::cancel()
{
	if(_timer.TimerIsRunning())
	{
		_timer.Cancel();
	}
}

void CockArm::stopArmCock()
{
	boost::unique_lock<boost::mutex>(_mutex);
	_cockArmActive = false;
}

void CockArm::startArmCock()
{
	boost::unique_lock<boost::mutex>(_mutex);
	_cockArmActive = true;

}
