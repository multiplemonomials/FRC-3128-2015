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

SwerveDrive::SwerveDrive()
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
_cmdProcessor("SwerveDriveCmdProcessor")
{

}

void SwerveDrive::steer(ListenerManager & listenerManager)
{
	Cmd::MakeShared(&SwerveDrive::steerHandler, boost::ref(*this), boost::ref(listenerManager));
}

SwerveDrive::~SwerveDrive()
{

}

void SwerveDrive::steerHandler(ListenerManager & listenerManager)
{
    double thresh = 0.2;

    double x1 = std::abs(listenerManager.getRawDouble(Listenable::JOY1X)) > thresh ? listenerManager.getRawDouble(Listenable::JOY1X) : 0.0;
    double y1 = std::abs(listenerManager.getRawDouble(Listenable::JOY1Y)) > thresh ? -listenerManager.getRawDouble(Listenable::JOY1Y) : 0.0;
    double x2 = std::abs(listenerManager.getRawDouble(Listenable::JOY2X)) > thresh ? listenerManager.getRawDouble(Listenable::JOY2X) : 0.0;

    vel = -(sqrt(pow(x1, 2) + pow(y1, 2)));
    rot = x2;

    if (std::abs(vel) > 0.1)
        theta = RobotMath::rTD(atan2(y1, x1)) + Global.gyr.getAngle();
    else
        vel = 0;

    xVel = vel * cos(RobotMath::dTR(theta));
    yVel = vel * sin(RobotMath::dTR(theta));

    spdL = sqrt(pow(xVel + (rot * yPosL), 2) + pow(yVel - (rot * xPosL), 2));
    angL = RobotMath::rTD(atan2(yVel + (rot * xPosL), xVel - (rot * yPosL)));

    spdB = sqrt(pow(xVel + (rot * yPosB), 2) + pow(yVel - (rot * xPosB), 2));
    angB = 180.0 / M_PI * (atan2(yVel + (rot * xPosB), xVel - (rot * yPosB)));

    spdR = sqrt(pow(xVel + (rot * yPosR), 2) + pow(yVel - (rot * xPosR), 2));
    angR = 180.0 / M_PI * (atan2(yVel + (rot * xPosR), xVel - (rot * yPosR)));

    //DebugLog.log(DebugLog.LVL_STREAM, this, "Theta: " + theta);
    //DebugLog.log(DebugLog.LVL_STREAM, this, "r: " + angR);
    //DebugLog.log(DebugLog.LVL_STREAM, this, "l: " + angL);
    //DebugLog.log(DebugLog.LVL_STREAM, this, "b: " + angB);
    //DebugLog.log(DebugLog.LVL_STREAM, this, "rot: " + rot);

    std::pair<double, double> r = optimizeSwerve(Global.rotFR.getEncoderAngle(), angR, spdR);
    std::pair<double, double> l = optimizeSwerve(Global.rotFL.getEncoderAngle(), angL, spdL);
    std::pair<double, double> b = optimizeSwerve(Global.rotBk.getEncoderAngle(), angB, spdB);

    Global.rotFR.setControlTarget(r[0]+(x1 == 0 && x2 != 0 ? 0.1 : 0));
    Global.rotFL.setControlTarget(l[0]+(x1 == 0 && x2 != 0 ? 0.1 : 0));
    Global.rotBk.setControlTarget(b[0]+(x1 == 0 && x2 != 0 ? 0.1 : 0));

    if (std::abs(r.second) > 1 || std::abs(l.second) > 1 || std::abs(b.second) > 1)
    {
        double scl = std::max(std::abs(r.second), std::max(std::abs(l.second), std::abs(b.second)));
        r.second /= scl;
        l.second /= scl;
        b.second /= scl;
    }
    Global.drvFR.setSpeed(r.second);
    Global.drvFL.setSpeed(-l.second);
    Global.drvBk.setSpeed(b.second);
}

