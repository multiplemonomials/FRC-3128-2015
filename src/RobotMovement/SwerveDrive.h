/*
 * SwerveDrive.h
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#ifndef SWERVEDRIVE_H_
#define SWERVEDRIVE_H_

#include <utility>

#include <EventManager/CmdProcessor.h>
#include <EventManager/ListenerManager.h>
#include <HardwareLink/Motor/MotorLink.h>

class SwerveDrive
{
	 double dimConst = 10;
     double xPosL = -12.77374 / dimConst, xPosR = 12.77374 / dimConst, xPosB = 0.0 / dimConst;
     double yPosL = 7.375 / dimConst, yPosR = 7.375 / dimConst, yPosB = -14.75 / dimConst;
     double vel, theta, rot, xVel, yVel;
     double spdL, spdR, spdB;
     double angL, angR, angB;

     std::shared_ptr<GyroLink> _gyr;

     std::shared_ptr<MotorLink> _rotFR;

     std::shared_ptr<MotorLink> _rotFL;

     std::shared_ptr<MotorLink> _rotBk;

     std::shared_ptr<MotorLink> _drvFR;

     std::shared_ptr<MotorLink> _drvFL;

     std::shared_ptr<MotorLink> _drvBk;

     //serializes calls to steer() and runs them off the calling thread
     CmdProcessor _cmdProcessor;

     //function that does the actual steering
     void steerHandler(ListenerManager & listenerManager);

public:

    static std::pair<double, double> optimizeSwerve(double ang1, double ang2, double vel);

    //queues a steer operation
    void steer(ListenerManager & listnenerManager);

	SwerveDrive();

	virtual ~SwerveDrive();
};

#endif /* SWERVEDRIVE_H_ */
