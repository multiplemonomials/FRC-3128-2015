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
//a reference to __start even though it is not needed
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
#ifndef HOST_BUILD //make things a little easier in the wpimock file
: public IterativeRobot
#endif
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
#ifndef HOST_BUILD
        GetWatchdog().Feed();
#endif
    }

    void AutonomousPeriodic()
    {
#ifndef HOST_BUILD
        GetWatchdog().Feed();
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    void TeleopPeriodic()
    {
#ifndef HOST_BUILD
        GetWatchdog().Feed();
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

};

#ifndef HOST_BUILD
START_ROBOT_CLASS(RobotTemplate);
#endif

#ifdef HOST_BUILD
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

	std::cout << __cplusplus;

	return 0;
}

#endif
