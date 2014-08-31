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

class ArcadeDrive
{
     double _spdL, _spdR;

     std::shared_ptr<MotorLink> _driveLeft;

     std::shared_ptr<MotorLink> _driveRight;

     ListenerManager & _listenerManager;

     CmdProcessor _cmdProcessor;

public:
    //queues a steer operation
    void steer();

	ArcadeDrive(std::shared_ptr<MotorLink> driveLeft, std::shared_ptr<MotorLink> driveRight,ListenerManager & listenerManager);

	void steerHandler();

	virtual ~ArcadeDrive(){}

};

#endif /* TANKDRIVE_H_ */
