/*
 * Piston.h
 *
 *  Created on: Apr 3, 2014
 *      Author: jamie
 */

#ifndef PISTON_H_
#define PISTON_H_

#include <Solenoid.h>

#include <memory>

class Piston
{
	std::shared_ptr<Solenoid> _solAPtr;
	std::shared_ptr<Solenoid> _solBPtr;

	bool _inversion = false;
public:

	//convienience constructor that sets the state of the solenoid
	Piston(std::shared_ptr<Solenoid> solAPtr, std::shared_ptr<Solenoid> solBPtr, bool solStateA, bool solStateB);

	//creates piston from the two solenoids that control it
	Piston(std::shared_ptr<Solenoid> solAPtr, std::shared_ptr<Solenoid> solBPtr);

	void invertPiston();

	void lockPiston();

	void unlockPiston();

	void setPistonOn();

	void setPistonOff();

	void invertSolenoids();

	virtual ~Piston();
};

#endif /* PISTON_H_ */
