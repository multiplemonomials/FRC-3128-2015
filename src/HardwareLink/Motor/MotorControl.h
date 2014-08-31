/*
 * MotorControl.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

#include <Util/Time.h>
#include "MotorLink.h"
#include <mutex>
#include <EventManager/CmdTimerSelector.h>

/*
 * Base class for the two motor speed controllers, LinearAngleTarget
 * and LinearSpeedTarget
 */
class MotorControl
{
protected:

    Time::Timepoint _lastRuntime;

    MotorLink * _controlledMotor;

    std::mutex _mutex;

    //timer object to regularly call execute()
    PlatformCmdTimer _speedSetScheduler;

public:

    MotorControl()
	:_lastRuntime(),
	_controlledMotor(nullptr),
	_mutex(),
	_speedSetScheduler(Cmd::MakeShared(&MotorControl::execute, boost::ref(*this)))
    {

    }

    virtual ~MotorControl();

    void setControlledMotor(MotorLink * m);

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

    void setControlTarget(double & val);

    /**
     *
     * @return the last runtime in system clock milliseconds
     */
    Time::Timepoint getLastRuntime();

    /**
     *
     * @return how long ago the event was last called (used for dT)
     */
    Time::Duration getLastRuntimeDist();

    /**
     *
     * @return the encoder value of the controlled motor
     */
    double getLinkedEncoderAngle();

    //stops the speed-setting loop, allowing you to control the motor
    //via something else
    void cancel();

    /**
     *
     * @return the linked motor's speed
     */
    double getLinkedMotorSpeed();

    void start();

    void execute();

    virtual void clearControlRun();
};



#endif /* MOTORCONTROL_H_ */
