//============================================================================
// Name        : FRC-3128-2015.cpp
// Author      : FRC Team 3128
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <initializer_list>
#include <memory>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <wpilib/WPILib.h>
#include <LogMacros.h>
#include "Global.h"

struct RobotTemplate : public IterativeRobot
{
	bool autonomousHasBeenInit = false;
	bool teleopHasBeenInit = false;

	void RobotInit()
	{
		LOG_INFO("Welcome to the FRC Team 3128 Event System!")
		Global::initializeRobot();
	}

	void DisabledInit()
	{
		Global::initializeDisabled();
	}

    void AutonomousInit()
    {
        if(!autonomousHasBeenInit)
        {
            //TODO EventManager.dropAllEvents();
            //TODO ListenerManager.dropAllListeners();
            Global::initializeAuto();
            autonomousHasBeenInit = true;
            teleopHasBeenInit = false;
        }
    }

    void TeleopInit()
    {
        if(!teleopHasBeenInit)
        {
            //TODO EventSequencer.stopAllSequencers();
            //TODO EventManager.dropAllEvents();
            //TODO ListenerManager.dropAllListeners();
            Global::initializeTeleop();
            teleopHasBeenInit = true;
            autonomousHasBeenInit = false;
        }
    }

    void DisabledPeriodic()
    {
        GetWatchdog().Feed();
        // TODO if(Constants.EVENT_PROCESS_WHILE_DISABLED) EventManager.processEvents();
    }

    void AutonomousPeriodic()
    {
    	GetWatchdog().Feed();
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }

    void TeleopPeriodic()
    {
    	GetWatchdog().Feed();
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }

};


int main()
{

	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!

	return 0;
}
