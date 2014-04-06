/*
 * LinearSpeedTarget.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef LINEARSPEEDTARGET_H_
#define LINEARSPEEDTARGET_H_

#include <HardwareLink/Motor/MotorControl.h>

/*
 * Speed controller for a motor which rotates continuously,
 * as opposed to targeting a certain angle
 *
 */
class LinearSpeedTarget : MotorControl
{
	double _tgtSpeed;

    double _dtAccum;

    double _refreshTime;

    double _lastEncoderAngle;

public:

    /**
     * tgtSpeed    target speed in deg/sec
     * refreshTime speed update rate in msec
     */
    LinearSpeedTarget(double tgtSpeed, double refreshTime);

    /**
     * Uses a default refreshTime of 50msec
     *
     * tgtSpeed target speed in deg/sec
     */
    LinearSpeedTarget(double tgtSpeed);

	virtual ~LinearSpeedTarget();

	void setControlTarget(double d);

	double speedControlStep(Time::Duration dt);

	/**
	 * Sets the speed update time in msec
	 *
	 * refreshTime speed update rate in msec
	 */
	void setRefreshTime(double refreshTime);
};

#endif /* LINEARSPEEDTARGET_H_ */
