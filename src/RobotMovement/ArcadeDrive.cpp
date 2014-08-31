/*
 * SwerveDrive.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#include <RobotMovement/ArcadeDrive.h>
#include "RobotMath.h"
#include <EventManager/Cmd.h>

#include <cmath>

ArcadeDrive::ArcadeDrive(std::shared_ptr<MotorLink> driveLeft, std::shared_ptr<MotorLink> driveRight, ListenerManager & listenerManager)
: _spdL(0),
  _spdR(0),
  _driveLeft(driveLeft),
  _driveRight(driveRight),
  _listenerManager(listenerManager),
  _cmdProcessor("TankDrive")
{

}

void ArcadeDrive::steer()
{
	_cmdProcessor.Enqueue(&ArcadeDrive::steerHandler, boost::ref(*this));
}


void ArcadeDrive::steerHandler()
{
	//threshold below which joystick movements are ignored.
	static const double thresh = 0.2;

	//read joystick values
	double x1 = std::abs(_listenerManager.getRawDouble(Listenable::JOY1X)) > thresh ? _listenerManager.getRawDouble(Listenable::JOY1X) : 0.0;
	double y1 = std::abs(_listenerManager.getRawDouble(Listenable::JOY1Y)) > thresh ? -_listenerManager.getRawDouble(Listenable::JOY1Y) : 0.0;

	_spdR = x1;
	_spdL = x1;
	_spdR += y1;
	_spdL -= y1;

	_driveLeft->setSpeed(_spdL);
	_driveRight->setSpeed(_spdR);
}
