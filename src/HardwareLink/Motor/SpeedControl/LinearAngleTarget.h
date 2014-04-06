/*
 * LinearAngleTarget.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef LINEARANGLETARGET_H_
#define LINEARANGLETARGET_H_

#include <HardwareLink/Motor/MotorControl.h>

class LinearAngleTarget : MotorControl
{
	double _minSpeed;

	double _targetAngle;

	const double _threshold;

	double _kP;

public:

	LinearAngleTarget(double minSpeed, double threshold, double kP);

	void setControlTarget(double val);

	double speedControlStep(Time::Duration dt);

	void clearControlRun();

	bool isComplete();

	virtual ~LinearAngleTarget();
};

#endif /* LINEARANGLETARGET_H_ */
