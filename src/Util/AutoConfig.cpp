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

#define SPINLOCK_UNTIL(expression) if(expression){break;}else{boost::this_thread::sleep(Time::Milliseconds(75));}

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

	_cmdProcessor.EnqueueLambda(new Cmd::Functor([&]()
	{
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
	}));

	_cmdProcessor.EnqueueLambda(new Cmd::Functor([&]() //stop motors and wait a little
	{
		while(true)
		{
	        global._drvBk->setSpeed(0);
	        global._drvFL->setSpeed(0);
	        global._drvFR->setSpeed(0);

	        SPINLOCK_UNTIL(Time::GetLocalTime() >= sequenceStartTime + Time::Seconds(2))
		}

	}));

	_cmdProcessor.EnqueueLambda(new Cmd::Functor([&]() //Stop the arm cocking event
	{
        global._cockShooter->cancel();
	}));

	_cmdProcessor.EnqueueLambda(new Cmd::Functor([&]() //Run arm until cocked
	{
		while(true)
		{
			global._mShooter->setSpeed(-1.0);

			SPINLOCK_UNTIL(global._shooterTSensor.get())
		}

	}));

	_cmdProcessor.EnqueueLambda(new Cmd::Functor([&]() //Re-enable the arm cocking for teleop
	{
        global._cockShooter->start();
	}));

	_cmdProcessor.EnqueueLambda(new Cmd::Functor([&]()//Drive forward into goal
	{
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
	}));

	_cmdProcessor.EnqueueLambda(new Cmd::Functor([&]()//Stop motors
	{
		global._drvBk->setSpeed(0);
		global._drvFL->setSpeed(0);
		global._drvFR->setSpeed(0);
	}));

	_cmdProcessor.EnqueueLambda(new Cmd::Functor([&]()//Do a dance
	{
		lightChange(Options::Alliance::BLUE, global._redLights, global._blueLights);
		boost::this_thread::sleep(Time::Seconds(1));

		lightChange(Options::Alliance::RED, global._redLights, global._blueLights);
		boost::this_thread::sleep(Time::Seconds(1));

	}));


}
