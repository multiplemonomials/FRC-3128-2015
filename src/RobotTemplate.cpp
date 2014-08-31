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
#include <boost/date_time.hpp>
#include <WPILib.h>
#include <thread>

#include <LogMacros.h>
#include "Global.h"
#include <EzLogger/LogCore.h>
#include <EzLogger/output/acceptors/BasicAcceptor.h>
#include <EzLogger/output/formatters/CondensedFormatter.h>
#include <EzLogger/output/writers/BasicWriter.h>
#include <EzLogger/output/LogOutput.h>

//The _start function is needed because of a bug in the ppc toolchain's linker script, which generates
//a reference to _start even though it is not needed
#ifdef __VXWORKS__
extern "C" {
void _start() {
   //output an error message?
}
}

void __assert(const char* message)
{
	std::cerr << "Assert Failed: " << message << std::endl;
}

#endif

struct RobotTemplate
: public IterativeRobot
{
	bool autonomousHasBeenInit = false;
	bool teleopHasBeenInit = false;

	Global _global;

	void RobotInit()
	{
		std::cout << "Welcome to the FRC Team 3128 System Version 2!" << std::endl;
		_global.initializeRobot();
	}

	void DisabledInit()
	{
		std::cout << "Initializing Disabled" << std::endl;
		_global.initializeDisabled();
	}

    void AutonomousInit()
    {
        if(!autonomousHasBeenInit)
        {
    		std::cout << "Initializing Auto" << std::endl;

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
    		std::cout << "Initializing Teleop" << std::endl;

        	_global._listenerManager.removeAllListeners();
        	_global.initializeTeleop();
            teleopHasBeenInit = true;
            autonomousHasBeenInit = false;
        }
    }

    void DisabledPeriodic()
    {
        GetWatchdog().Feed();
    }

    void AutonomousPeriodic()
    {
        GetWatchdog().Feed();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    void TeleopPeriodic()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

};

START_ROBOT_CLASS(RobotTemplate);
