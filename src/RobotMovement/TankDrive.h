/*
 * SwerveDrive.h
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#ifndef TANKDRIVE_H_
#define TANKDRIVE_H_

#include <utility>
#include <Gyro.h>

#include <EventManager/CmdProcessor.h>
#include <EventManager/ListenerManager.h>
#include <HardwareLink/Motor/MotorLink.h>

class TankDrive
{
     double _spdL, _spdR;

     std::shared_ptr<MotorLink> _driveLeft;

     std::shared_ptr<MotorLink> _driveRight;

     ListenerManager & _listenerManager;

     CmdProcessor _cmdProcessor;

public:
    //queues a steer operation
    void steer();

	TankDrive(std::shared_ptr<MotorLink> driveLeft, std::shared_ptr<MotorLink> driveRight,ListenerManager & listenerManager);

	void steerHandler();

	virtual ~TankDrive(){}

};

#endif /* TANKDRIVE_H_ */
