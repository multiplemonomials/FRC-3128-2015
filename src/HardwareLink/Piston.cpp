/*
 * Piston.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: jamie
 */

#include <HardwareLink/Piston.h>
#include <LogMacros.h>

Piston::Piston(std::shared_ptr<Solenoid> solAPtr, std::shared_ptr<Solenoid> solBPtr, bool solStateA, bool solStateB)
:Piston(solAPtr, solBPtr)
{
	solAPtr->Set(solStateA);

	solBPtr->Set(solStateB);
}

Piston::Piston(std::shared_ptr<Solenoid> solAPtr, std::shared_ptr<Solenoid> solBPtr)
:_solAPtr(solAPtr),
 _solBPtr(solBPtr)
{

}

Piston::~Piston()
{

}

void Piston::invertPiston()
{
	_inversion = !_inversion;
}

void Piston::lockPiston()
{
	_solAPtr->Set(true);
	_solBPtr->Set(true);
	LOG_DEBUG("Piston set to locked state");
}

void Piston::unlockPiston()
{
	_solAPtr->Set(false);
	_solBPtr->Set(false);
	LOG_DEBUG("Piston set to unlocked state");
}

void Piston::setPistonOn()
{
	_solAPtr->Set(true ^ _inversion);
	_solBPtr->Set(false ^ _inversion);
	LOG_DEBUG("Solenoid set to on state");
}

void Piston::setPistonOff()
{
	_solAPtr->Set(false ^ _inversion);
	_solBPtr->Set(true ^ _inversion);
	LOG_DEBUG("Solenoid set to off state");
}

void Piston::invertSolenoids()
{
	_solAPtr->Set(!_solAPtr->Get());
	_solBPtr->Set(!_solBPtr->Get());
	LOG_DEBUG("Solenoid set to flip-state " << _solAPtr->Get() << ", " <<_solBPtr->Get());
}

