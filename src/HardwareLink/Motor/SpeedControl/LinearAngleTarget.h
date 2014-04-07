/*
 * LinearAngleTarget.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef LINEARANGLETARGET_H_
#define LINEARANGLETARGET_H_

#include <HardwareLink/Motor/MotorControl.h>

class LinearAngleTarget : public MotorControl
{
	double _minSpeed;

	double _targetAngle;

	const double _threshold;

	double _kP;

	virtual void setControlTargetImpl(double & val);

	virtual double speedControlStep(Time::Duration dt);

public:

	virtual void clearControlRunImpl();

	LinearAngleTarget(double minSpeed, double threshold, double kP);

	virtual bool isComplete();

	virtual ~LinearAngleTarget();
};

#endif /* LINEARANGLETARGET_H_ */
