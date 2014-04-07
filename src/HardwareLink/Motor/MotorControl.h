/*
 * MotorControl.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

#include <boost/thread.hpp>

#include <Util/Time.h>
#include "MotorLink.h"
#include <EventManager/CmdTimerPosix.h>


/*
 * Base class for the two motor speed controllers, LinearAngleTarget
 * and LinearSpeedTarget
 */
class MotorControl
{
protected:

    Time::Timepoint _lastRuntime;

    MotorLink * _controlledMotor;

    boost::mutex _mutex;

    //timer object to regularly call execute()
    CmdTimerPosix _speedSetScheduler;

public:

    MotorControl()
	:_lastRuntime(),
	_controlledMotor(nullptr),
	_mutex(),
	_speedSetScheduler(Cmd::MakeShared(&MotorControl::execute, boost::ref(*this)))
    {

    }

    virtual ~MotorControl()
    {

    }

    void setControlledMotor(MotorLink * m)
    {
    	boost::unique_lock<boost::mutex>();
    	_controlledMotor = m;
    }

    //abstract functions that are implemented by its subclasses
protected:
    //sets the target... parameter(it differs by base class) for the motor.
    //following two are called through one of the base class's functions so they can be mutexed
    virtual void setControlTargetImpl(double & val) = 0;
public:
    virtual void clearControlRunImpl() = 0;

protected:
    virtual double speedControlStep(Time::Duration) = 0;

public:
    virtual bool isComplete() = 0;

    void setControlTarget(double & val)
    {
    	boost::unique_lock<boost::mutex>();
    	setControlTargetImpl(val);
    }

    /**
     *
     * @return the last runtime in system clock milliseconds
     */
    Time::Timepoint getLastRuntime()
    {
    	return _lastRuntime;
    }

    /**
     *
     * @return how long ago the event was last called (used for dT)
     */
    Time::Duration getLastRuntimeDist()
    {
    	return Time::Timepoint(Time::GetLocalTime()) - _lastRuntime;
    }

    /**
     *
     * @return the encoder value of the controlled motor
     */
    double getLinkedEncoderAngle()
    {
    	return _controlledMotor->getEncoderAngle();
    }

    //stops the speed-setting loop, allowing you to control the motor
    //via something else
    void cancel()
    {
    	boost::unique_lock<boost::mutex>();
    	_speedSetScheduler.Cancel();
    }

    /**
     *
     * @return the linked motor's speed
     */
    double getLinkedMotorSpeed()
    {
    	return _controlledMotor->getSpeed();
    }

    void start()
    {
    	//TODO configuration setting
    	_speedSetScheduler.StartRelative(Time::Milliseconds(100));
    }

    void execute()
    {
        boost::unique_lock<boost::mutex>(_mutex);

        if(isComplete())
        {
        	_controlledMotor->setSpeed(0.0);
        }
        else
        {
        	_controlledMotor->setSpeed(speedControlStep(getLastRuntimeDist()));
        }

        _lastRuntime = Time::Timepoint(Time::GetLocalTime());

        //TODO configuration setting
        _speedSetScheduler.StartRelative(Time::Milliseconds(100));
    }

    virtual void clearControlRun()
    {
    	boost::unique_lock<boost::mutex>(_mutex);
    	clearControlRunImpl();
    }
};



#endif /* MOTORCONTROL_H_ */
