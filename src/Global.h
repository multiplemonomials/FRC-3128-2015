/*
 * Global.h
 *
 *  Created on: Mar 25, 2014
 *      Author: jamie
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <WPILib.h>

#include <HardwareLink/Motor/MotorLink.h>
#include <HardwareLink/Motor/SpeedControl/LinearAngleTarget.h>
#include <HardwareLink/Motor/SpeedControl/LinearSpeedTarget.h>
#include <HardwareLink/Encoder/MagneticPotEncoder.h>
#include <HardwareLink/RelayLink.h>
#include <RobotMovement/CockArm.h>
#include <EventManager/ListenerManager.h>
#include <RobotMovement/SwerveDrive.h>
#include <RobotMovement/ArcadeDrive.h>

class Global
{

public:

	std::shared_ptr<Gyro> _gyr;
	std::shared_ptr<DigitalInput> _shooterTSensor;

	std::shared_ptr<MagneticPotEncoder> _encFR;
	std::shared_ptr<MagneticPotEncoder> _encFL;
	std::shared_ptr<MagneticPotEncoder> _encBk;

	std::shared_ptr<MotorLink> _rotFR;
	std::shared_ptr<MotorLink> _rotFL;
	std::shared_ptr<MotorLink> _rotBk;
	std::shared_ptr<MotorLink> _drvFR;
	std::shared_ptr<MotorLink> _drvFL;
	std::shared_ptr<MotorLink> _drvBk;

	std::shared_ptr<MotorLink> _mShooter;
	std::shared_ptr<MotorLink> _mArmRoll;
	std::shared_ptr<MotorLink> _mArmMove;

	std::shared_ptr<CockArm> _cockShooter;

	std::shared_ptr<RelayLink> _redLights;
	std::shared_ptr<RelayLink> _blueLights;
	std::shared_ptr<RelayLink> _camLights;

	///polls controller and triggers updates when data changes
	ListenerManager _listenerManager;

	///manages all three swerve modules (one drive motor and one steering motor each)
	//std::shared_ptr<SwerveDrive> _swerveDrive;

	///controls two tank drive motors
	std::shared_ptr<ArcadeDrive> _tankDrive;

	///massive constructor which boots up the robot
	Global();

	//not assignable
	Global operator=(Global) = delete;

	//not copyable
	Global(Global&) = delete;

	void initializeRobot();

	void initializeDisabled();

	void initializeAuto();

	void initializeTeleop();

	void robotStop();

	virtual ~Global();
};

#endif /* GLOBAL_H_ */
