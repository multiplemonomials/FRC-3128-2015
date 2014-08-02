/*
 * AutoConfig.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include <Util/AutoConfig.h>
#include <EventManager/CmdProcessor.h>
#include <EventManager/CmdTimerMultiplex.h>
#include <Global.h>
#include <HardwareLink/LightChangeEvent.h>
#include <Options.h>
#include <LogMacros.h>
#include <chrono>

#define SPINLOCK_UNTIL(expression) if(expression){break;}else{std::this_thread::sleep_for(std::chrono::milliseconds(100));}

CmdProcessor AutoConfig::_cmdProcessor("AutoConfigCmdProcessor");

AutoConfig::AutoConfig()
{

}

AutoConfig::~AutoConfig()
{

}

void AutoConfig::initialize(Global & global)
{

	Time::Timepoint sequenceStartTime(Time::GetLocalTime());

	LOG_INFO("Starting Autonomous Event Sequence...")
	while(true)
	{
		global._rotBk->setControlTarget(90);
		global._rotFL->setControlTarget(90);
		global._rotFR->setControlTarget(90);

		global._drvBk->setSpeed(1.0);
		global._drvFL->setSpeed(1.0);
		global._drvFR->setSpeed(1.0);

		SPINLOCK_UNTIL(Time::GetLocalTime() >= sequenceStartTime + Time::Milliseconds(750))
	}

	//stop motors and wait a little
	while(true)
	{
		global._drvBk->setSpeed(0);
		global._drvFL->setSpeed(0);
		global._drvFR->setSpeed(0);

		SPINLOCK_UNTIL(Time::GetLocalTime() >= sequenceStartTime + Time::Seconds(2))
	}

	//Stop the arm cocking event
	global._cockShooter->cancel();

	while(true)
	{
		global._mShooter->setSpeed(-1.0);

		SPINLOCK_UNTIL(global._shooterTSensor.get())
	}

	//Re-enable the arm cocking for teleop
	global._cockShooter->start();

	//Drive forward into goal
	Time::Timepoint desiredFinishTime(Time::GetLocalTime() + Time::Milliseconds(2500));
	while(true)
	{
		global._rotBk->setControlTarget(90+global._gyr->GetAngle());
		global._rotFL->setControlTarget(90+global._gyr->GetAngle());
		global._rotFR->setControlTarget(90+global._gyr->GetAngle());

		global._drvBk->setSpeed(1.0);
		global._drvFL->setSpeed(1.0);
		global._drvFR->setSpeed(1.0);

		SPINLOCK_UNTIL(Time::GetLocalTime() >= desiredFinishTime)
	}

	//Stop motors
	global._drvBk->setSpeed(0);
	global._drvFL->setSpeed(0);
	global._drvFR->setSpeed(0);

	//Do a dance
	lightChange(Options::Alliance::BLUE, global._redLights, global._blueLights);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	lightChange(Options::Alliance::RED, global._redLights, global._blueLights);
	std::this_thread::sleep_for(std::chrono::seconds(1));


}
