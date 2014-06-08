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
#include <wpilib/WPILib.h>


//The _start function is needed because of a bug in the ppc toolchain's linker script, which generates
//a reference to __start even though it is not needed
extern "C" {
void _start() {
   //output an error message?
}
}

void __assert(const char* message)
{
	std::cerr << "Assert Failed: " << message << std::endl;
}

struct RobotTemplate
#ifndef HOST_BUILD //make things a little easier in the wpimock file by not extending IterativeRobot
: public IterativeRobot
#endif
{
	bool autonomousHasBeenInit = false;
	bool teleopHasBeenInit = false;


	void RobotInit()
	{
		std::cout << "Welcome to the FRC Team 3128 Event System Version 2!" << std::endl;
	}

	void DisabledInit()
	{
	}

    void AutonomousInit()
    {
        if(!autonomousHasBeenInit)
        {
            autonomousHasBeenInit = true;
            teleopHasBeenInit = false;
        }
    }

    void TeleopInit()
    {
        if(!teleopHasBeenInit)
        {
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
    }

    void TeleopPeriodic()
    {
#ifndef HOST_BUILD
        GetWatchdog().Feed();
#endif
    }

};

#ifndef HOST_BUILD
START_ROBOT_CLASS(RobotTemplate);
#endif

#ifdef HOST_BUILD
int main()
{

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

#endif
