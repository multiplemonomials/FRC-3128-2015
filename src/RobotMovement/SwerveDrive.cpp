/*
 * SwerveDrive.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#include <RobotMovement/SwerveDrive.h>
#include "RobotMath.h"
#include <EventManager/Cmd.h>

#include <cmath>

std::pair<double, double> SwerveDrive::optimizeSwerve(double ang1, double ang2, double vel)
{
	double a = RobotMath::angleDistance(ang2, ang1);

	std::pair<double, double> returnVals;
	if (std::abs(a) > 90)
	{
		returnVals.first = ang2 + 180.0;
		returnVals.second = -vel;
	}
	else
	{
		returnVals.first = ang2;
		returnVals.second = vel;
	}
	return returnVals;
}

SwerveDrive::SwerveDrive(std::shared_ptr<Gyro> gyr, std::shared_ptr<MotorLink> rotFR,
			std::shared_ptr<MotorLink> rotFL, std::shared_ptr<MotorLink> rotBk,
			std::shared_ptr<MotorLink> drvFR, std::shared_ptr<MotorLink> drvFL,
			std::shared_ptr<MotorLink> drvBk, ListenerManager & listenerManager)
:vel(),
theta(),
rot(),
xVel(),
yVel(),
spdL(),
spdR(),
spdB(),
angL(),
angR(),
angB(),
_gyr(gyr),
_rotFR(rotFR),
_rotFL(rotFL),
_rotBk(rotBk),
_drvFR(drvFR),
_drvFL(drvFL),
_drvBk(drvBk),
_listenerManager(listenerManager),
_cmdProcessor("SwerveDriveCmdProcessor")
{

}

void SwerveDrive::steer()
{
	_cmdProcessor.Enqueue(&SwerveDrive::steerHandler, boost::ref(*this));
}

SwerveDrive::~SwerveDrive()
{

}

void SwerveDrive::steerHandler()
{
    double thresh = 0.2;

    double x1 = std::abs(_listenerManager.getRawDouble(Listenable::JOY1X)) > thresh ? _listenerManager.getRawDouble(Listenable::JOY1X) : 0.0;
    double y1 = std::abs(_listenerManager.getRawDouble(Listenable::JOY1Y)) > thresh ? -_listenerManager.getRawDouble(Listenable::JOY1Y) : 0.0;
    double x2 = std::abs(_listenerManager.getRawDouble(Listenable::JOY2X)) > thresh ? _listenerManager.getRawDouble(Listenable::JOY2X) : 0.0;

    vel = -(sqrt(pow(x1, 2) + pow(y1, 2)));
    rot = x2;

    if (std::abs(vel) > 0.1)
    {
        theta = RobotMath::rTD(atan2(y1, x1)) + _gyr->GetAngle();
    }
    else
        vel = 0;

    xVel = vel * cos(RobotMath::dTR(theta));
    yVel = vel * sin(RobotMath::dTR(theta));

    spdL = sqrt(pow(xVel + (rot * yPosL), 2) + pow(yVel - (rot * xPosL), 2));
    angL = RobotMath::rTD(atan2(yVel + (rot * xPosL), xVel - (rot * yPosL)));

    spdB = sqrt(pow(xVel + (rot * yPosB), 2) + pow(yVel - (rot * xPosB), 2));
    angB = RobotMath::rTD(atan2(yVel + (rot * xPosB), xVel - (rot * yPosB)));

    spdR = sqrt(pow(xVel + (rot * yPosR), 2) + pow(yVel - (rot * xPosR), 2));
    angR = RobotMath::rTD(atan2(yVel + (rot * xPosR), xVel - (rot * yPosR)));

    std::pair<double, double> r = optimizeSwerve(_rotFR->getEncoderAngle(), angR, spdR);
    std::pair<double, double> l = optimizeSwerve(_rotFL->getEncoderAngle(), angL, spdL);
    std::pair<double, double> b = optimizeSwerve(_rotBk->getEncoderAngle(), angB, spdB);

    _rotFR->setControlTarget(r.first +(x1 == 0 && x2 != 0 ? 0.1 : 0));
    _rotFL->setControlTarget(l.first +(x1 == 0 && x2 != 0 ? 0.1 : 0));
    _rotBk->setControlTarget(b.first +(x1 == 0 && x2 != 0 ? 0.1 : 0));

    if (std::abs(r.second) > 1 || std::abs(l.second) > 1 || std::abs(b.second) > 1)
    {
        double scl = std::max(std::abs(r.second), std::max(std::abs(l.second), std::abs(b.second)));
        r.second /= scl;
        l.second /= scl;
        b.second /= scl;
    }
    _drvFR->setSpeed(r.second);
    _drvFL->setSpeed(-l.second);
    _drvBk->setSpeed(b.second);
}

