/*
 * Global.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: jamie
 */

#include <Global.h>
#include <Options.h>
#include <EzLogger/LogCore.h>
#include <EzLogger/output/acceptors/BasicAcceptor.h>
#include <EzLogger/output/formatters/CondensedFormatter.h>
#include <EzLogger/output/writers/BasicWriter.h>
#include <EzLogger/output/LogOutput.h>
#include <Util/AutoConfig.h>
#include <HardwareLink/LightChangeEvent.h>
#include <memory>


Global::Global()
:_gyr(std::make_shared<Gyro>(1, 1)),
_shooterTSensor(std::make_shared<DigitalInput>(4, 4)),
_encFR(std::make_shared<MagneticPotEncoder>(-60, 1, 2)),
_encFL(std::make_shared<MagneticPotEncoder>(17, 1, 3)),
_encBk(std::make_shared<MagneticPotEncoder>(-35, 1, 4)),
_rotFR(std::make_shared<MotorLink>("rotFr", std::make_shared<Talon>(6, 8), _encFR, std::make_shared<LinearAngleTarget>(.40, 4, 0.005))), //OFFSET: -55 DEG
_rotFL(std::make_shared<MotorLink>("rotFL", std::make_shared<Talon>(6, 9), _encFL, std::make_shared<LinearAngleTarget>(.40, 4, 0.005))), //OFFSET: -18 DEG
_rotBk(std::make_shared<MotorLink>("rotBk", std::make_shared<Talon>(6, 7), _encBk, std::make_shared<LinearAngleTarget>(.40, 4, 0.005))), //OFFSET: -10 DEG
_drvFR(std::make_shared<MotorLink>("drvFr", std::make_shared<Talon>(6, 1))),
_drvFL(std::make_shared<MotorLink>("drvFl", std::make_shared<Talon>(6, 2))),
_drvBk(std::make_shared<MotorLink>("drvBk", std::make_shared<Talon>(6, 3))),
_mShooter(std::make_shared<MotorLink>("mShooter", std::make_shared<Talon>(6, 4))),
_mArmRoll(std::make_shared<MotorLink>("mArmRoll", std::make_shared<Talon>(6, 5))),
_mArmMove(std::make_shared<MotorLink>("mArmMove", std::make_shared<Talon>(6, 6))),
_cockShooter(std::make_shared<CockArm>(_shooterTSensor, _mShooter)),
_redLights(std::make_shared<RelayLink>(std::make_shared<Relay>(4, 1))),
_blueLights(std::make_shared<RelayLink>(std::make_shared<Relay>(4, 2))),
_camLights(std::make_shared<RelayLink>(std::make_shared<Relay>(4, 3))),
_listenerManager(std::make_shared<Joystick>(Options::instance()._controllerPort)),
_tankDrive(std::make_shared<ArcadeDrive>(_drvFL, _drvFR, _listenerManager))
//_swerveDrive(new SwerveDrive(_gyr, _rotFR, _rotFL, _rotBk, _drvFR, _drvFL, _drvBk, _listenerManager))
{

}

Global::~Global()
{

}

void Global::initializeRobot()
{
	_redLights->setOff();
	_blueLights->setOff();

	_rotBk->startSpeedControl(90);
	_rotFL->startSpeedControl(90);
	_rotFR->startSpeedControl(90);

	//Disabled for simulation builds, it needs to be done in main
#ifndef HOST_BUILD
	LogCore::instance().addOutput("stdout", std::make_shared<LogOutput<BasicAcceptor, CondensedFormatter, BasicWriter>>());
#endif
}

void Global::initializeDisabled()
{
    _redLights->setOff();
    _blueLights->setOff();
}

void Global::initializeAuto()
{
	std::thread(&AutoConfig::initialize, boost::ref(*this)).detach();
}

void Global::initializeTeleop()
{
    //_rotBk->startSpeedControl(90);
    //_rotFL->startSpeedControl(90);
    //_rotFR->startSpeedControl(90);

    lightChange(_redLights, _blueLights);

    _listenerManager.addListener(Listenable::XDOWN, Cmd::MakeShared(&Gyro::Reset, boost::ref(*_gyr)));

    _listenerManager.addListener(Listenable::ADOWN, Cmd::MakeShared(&CockArm::cancel, boost::ref(*_cockShooter)));
    _listenerManager.addListener(Listenable::ADOWN, Cmd::MakeShared(&MotorLink::setSpeed, boost::ref(*_mShooter), -1));

    _listenerManager.addListener(Listenable::AUP, Cmd::MakeShared(&CockArm::start, boost::ref(*_cockShooter)));
    _listenerManager.addListener(Listenable::AUP, Cmd::MakeShared(&MotorLink::setSpeed, boost::ref(*_mShooter), 0));

    _listenerManager.addListener(Listenable::BDOWN, Cmd::MakeShared(&CockArm::cancel, boost::ref(*_cockShooter)));
    _listenerManager.addListener(Listenable::BDOWN, Cmd::MakeShared(&MotorLink::setSpeed, boost::ref(*_mShooter), -1));

    _listenerManager.addListener(Listenable::BUP, Cmd::MakeShared(&CockArm::start, boost::ref(*_cockShooter)));
    _listenerManager.addListener(Listenable::BUP, Cmd::MakeShared(&MotorLink::setSpeed, boost::ref(*_mShooter), 0));

    _listenerManager.addListener(Listenable::LBDOWN, Cmd::MakeShared(&MotorLink::setSpeed, boost::ref(*_mArmMove), 0.1));
    _listenerManager.addListener(Listenable::LBDOWN, Cmd::MakeShared(&MotorLink::setSpeed, boost::ref(*_mArmRoll), 0.1));

    _listenerManager.addListener(Listenable::RBDOWN, Cmd::MakeShared(&MotorLink::setSpeed, boost::ref(*_mArmMove), -0.1));
    _listenerManager.addListener(Listenable::RBDOWN, Cmd::MakeShared(&MotorLink::setSpeed, boost::ref(*_mArmRoll), -0.1));

    _listenerManager.addListener(Listenable::BACKDOWN, Cmd::MakeShared(&CockArm::stopArmCock, boost::ref(*_cockShooter)));

    _listenerManager.addListener(Listenable::STARTDOWN, Cmd::MakeShared(&CockArm::startArmCock, boost::ref(*_cockShooter)));

    _listenerManager.addListener(Listenable::JOY1X, Cmd::MakeShared(&ArcadeDrive::steer, boost::ref(*_tankDrive)));
    _listenerManager.addListener(Listenable::JOY1Y, Cmd::MakeShared(&ArcadeDrive::steer, boost::ref(*_tankDrive)));

    //_listenerManager.addListener(Listenable::JOY1X, Cmd::MakeShared(&SwerveDrive::steer, boost::ref(*_swerveDrive)));
    //_listenerManager.addListener(Listenable::JOY1Y, Cmd::MakeShared(&SwerveDrive::steer, boost::ref(*_swerveDrive)));
    //_listenerManager.addListener(Listenable::JOY2X, Cmd::MakeShared(&SwerveDrive::steer, boost::ref(*_swerveDrive)));
}

void Global::robotStop()
{

}


