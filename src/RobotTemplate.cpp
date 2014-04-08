//============================================================================
// Name        : RobotTemplate.cpp
// Author      : FRC Team 3128
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <iostream>
#include <initializer_list>
#include <memory>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <wpilib/WPILib.h>

#include <LogMacros.h>
#include "Global.h"
#include <EzLogger/LogCore.h>
#include <EzLogger/output/acceptors/BasicAcceptor.h>
#include <EzLogger/output/formatters/CondensedFormatter.h>
#include <EzLogger/output/writers/BasicWriter.h>
#include <EzLogger/output/LogOutput.h>

struct RobotTemplate// : public IterativeRobot
{
	bool autonomousHasBeenInit = false;
	bool teleopHasBeenInit = false;

	Global _global;

	void RobotInit()
	{
		LOG_INFO("Welcome to the FRC Team 3128 Event System Version 2!")
		_global.initializeRobot();
	}

	void DisabledInit()
	{
		_global.initializeDisabled();
	}

    void AutonomousInit()
    {
        if(!autonomousHasBeenInit)
        {
            _global._listenerManager.removeAllListeners();
        	_global.initializeAuto();
            autonomousHasBeenInit = true;
            teleopHasBeenInit = false;
        }
    }

    void TeleopInit()
    {
        if(!teleopHasBeenInit)
        {
        	_global._listenerManager.removeAllListeners();
        	_global.initializeTeleop();
            teleopHasBeenInit = true;
            autonomousHasBeenInit = false;
        }
    }

    void DisabledPeriodic()
    {
        //GetWatchdog().Feed();
    }

    void AutonomousPeriodic()
    {
    	//GetWatchdog().Feed();
        boost::this_thread::sleep(boost::posix_time::milliseconds(150));
    }

    void TeleopPeriodic()
    {
    	//GetWatchdog().Feed();
        boost::this_thread::sleep(boost::posix_time::milliseconds(150));
    }

};


int main()
{

	LogCore::instance().addOutput("stdout", std::make_shared<LogOutput<BasicAcceptor, CondensedFormatter, BasicWriter>>());

	RobotTemplate robotTemplate;

	robotTemplate.RobotInit();

	std::cout << "*** Main: Autonomous Running..." << std::endl;
	robotTemplate.AutonomousInit();
	robotTemplate.AutonomousPeriodic();
	std::cin.get();

	std::cout << "*** Main: Disabled Running..." << std::endl;
	robotTemplate.DisabledInit();
	robotTemplate.DisabledPeriodic();

	std::cout << "*** Main: Teleop Running..." << std::endl;
	robotTemplate.TeleopInit();
	robotTemplate.TeleopPeriodic();
	std::cin.get();

	return 0;
}