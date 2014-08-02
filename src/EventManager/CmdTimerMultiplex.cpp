/*
 * CmdTimerMultiplex.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: jamie
 */

#include "CmdTimerMultiplex.h"

#include "CmdProcessor.h"
#include "CmdTimerPosix.h"
#include <LogMacros.h>
#include <Util/Ptr.h>

CmdTimerMultiplex::CmdTimerMultiplex()
:_timerMap(),
 _mutex()
{

}

void CmdTimerMultiplex::startTimer(Time::Duration time, Cmd::SharedPtr command)
{
	std::shared_ptr<CmdTimer> timer = std::make_shared<CmdTimerPosix>();
	timer->SetCmd(command);
	timer->StartRelative(time);

	boost::unique_lock<boost::mutex> lock(_mutex);
	_timerMap[timer->GetImpl()] = timer;
}

void CmdTimerMultiplex::startTimer(Time::Timepoint time, Cmd::SharedPtr command)
{
	std::shared_ptr<CmdTimer> timer = std::make_shared<CmdTimerPosix>();
	timer->SetCmd(command);
	timer->StartAbsolute(time);

	boost::unique_lock<boost::mutex> lock(_mutex);
	_timerMap[timer->GetImpl()] = timer;
}

//Called by timers when they go off.
//Inserts timer at front of processing queue
void CmdTimerMultiplex::onTimerFire(CmdTimerImpl * timerPtr, Cmd::SharedPtr cmdToInvoke)
{
	try
	{
		(*cmdToInvoke)();
	}
	catch(std::exception & error)
	{
		LOG_RECOVERABLE("Error processsing event: " << error.what())
	}

	boost::unique_lock<boost::mutex> lock(_mutex);

	if(_timerMap.erase(timerPtr))
	{
		LOG_RECOVERABLE("Couldn't erase timer with pointer " << Ptr(timerPtr).ToString() << "from timer map")
	}
}

//Dtor.
CmdTimerMultiplex::~CmdTimerMultiplex()
{
	//Empty
}

