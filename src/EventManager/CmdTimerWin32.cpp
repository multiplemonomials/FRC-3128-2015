/*
 * CmdTimerWin32.cpp
 *
 *  Created on: Aug 18, 2014
 *      Author: jamie
 */

#include <EventManager/CmdTimerWin32.h>
#include <LogMacros.h>
#include <Exception.h>
#include <Util/Ptr.h>

UINT_PTR CmdTimerWin32::CmdTimerWin32Impl::guaranteedUnusedID = 1;

std::set<UINT_PTR> CmdTimerWin32::CmdTimerWin32Impl::_usedIDs({0});

std::map<UINT_PTR, std::shared_ptr<CmdTimerWin32::CmdTimerWin32Impl>> CmdTimerWin32::CmdTimerWin32Impl::_idToTimerMap{};

std::mutex CmdTimerWin32::CmdTimerWin32Impl::_mapMutex{};

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::TimerSetTime(UINT const & milliseconds)
{
	HWND window = GetForegroundWindow();

	if(_timerID == 0)
	{
		//create a new timer
		std::unique_lock<std::mutex>(_mapMutex);

		//the second argument doesn't matter so long as it is not already a used ID
		_timerID = SetTimer(window, guaranteedUnusedID, milliseconds, &CmdTimerWin32::CmdTimerWin32Impl::TimerHandler);

		LOG_DEBUG("Generated timer with id " << _timerID)

		//step 1: mark this as used
		_usedIDs.insert(_timerID);

		//step 2: make sure that the guarenteed unique ID is not the one that was taken
		while(_usedIDs.find(guaranteedUnusedID) != _usedIDs.end())
		{
			++guaranteedUnusedID;
		}

		//step 3: register this as the timer for the ID
		_idToTimerMap.insert(std::pair<UINT_PTR, std::shared_ptr<CmdTimerWin32::CmdTimerWin32Impl>>(_timerID, shared_from_this()));
	}
	else
	{
		SetTimer(window, _timerID, milliseconds, &CmdTimerWin32::CmdTimerWin32Impl::TimerHandler);
	}

	if(_timerID == 0)
	{
		DWORD lastError = GetLastError();
		LOG_RECOVERABLE("Failed to create Win32 Timer: error code " << lastError << ": " << strerror(lastError));
	}
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::Cancel()
{
    // Tell the O/S we're done with the timer.
    if(KillTimer(nullptr, _timerID))
    {
    	LOG_FATAL("Failed to delete timer with ID" << _timerID)
    }

    //remove from our database

	std::unique_lock<std::mutex> lock(_mapMutex);
    _idToTimerMap.erase(_timerID);

    _usedIDs.erase(_timerID);

    _timerID = 0;
}


/*-----------------------------------------------------------------------------
    Static callback function shared by all CmdTimerWin32Impl timers.
 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::TimerHandler(HWND windowHandle, UINT uMsg, UINT_PTR idEvent, DWORD elapsedTime)
{
	std::shared_ptr<CmdTimerWin32Impl> 	timerPtr;

	try
	{
		std::unique_lock<std::mutex> lock(_mapMutex);
		timerPtr = _idToTimerMap.at(idEvent);
	}
	catch(std::out_of_range & error)
	{
		LOG_UNUSUAL("Couldn't look up timer id " << idEvent << " from map, ignoring event")
		return;
	}

    //LOG_DEBUG("CmdTimerWin32Impl::TimerHandler() firing (timerID = " << timerPtr->_timerID << ").");

    //invoke the handler of that particular object
    timerPtr->operator()();
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::operator()()
{
	if(_multiplex == nullptr)
	{
		// Invoke the command.
		try
		{
			(*_cmdToInvoke)();
		}
		catch(std::exception & error)
		{
			LOG_RECOVERABLE("Error processsing event: " << error.what())
		}
	}
	else
	{
		_multiplex->onTimerFire(this, _cmdToInvoke);
	}

}

/*-----------------------------------------------------------------------------
	constructors
 ----------------------------------------------------------------------------*/

// Ctor.
CmdTimerWin32::CmdTimerWin32Impl::CmdTimerWin32Impl(Cmd::SharedPtr command, Time::Duration timeRelative)
:	_multiplex(),
    _cmdToInvoke(command),
    _timerID(0)
{
    // Create the O/S timer.
    StartRelative(timeRelative);

    LOG_DEBUG("CmdTimerWin32Impl() complete (this = " << Ptr(this).ToString() << ").");
}

// Ctor.
CmdTimerWin32::CmdTimerWin32Impl::CmdTimerWin32Impl(Cmd::SharedPtr command)
:	_multiplex(),
    _cmdToInvoke(command),
    _timerID(0)
{
    LOG_DEBUG("CmdTimerWin32Impl() complete (this = " << Ptr(this).ToString() << ").");
}

// Ctor.
CmdTimerWin32::CmdTimerWin32Impl::CmdTimerWin32Impl()
:	_multiplex(),
    _cmdToInvoke(),
    _timerID(0)
{
    LOG_DEBUG("CmdTimerWin32Impl() complete (this = " << Ptr(this).ToString() << ").");
}

// Ctor.
CmdTimerWin32::CmdTimerWin32Impl::CmdTimerWin32Impl(Cmd::SharedPtr command, std::shared_ptr<CmdTimerMultiplex> multiplex)
:	_multiplex(multiplex),
    _cmdToInvoke(command),
    _timerID(0)
{
    LOG_DEBUG("CmdTimerWin32Impl() complete (this = " << Ptr(this).ToString() << ").");
}
/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

CmdTimerWin32::CmdTimerWin32Impl::~CmdTimerWin32Impl()
{
    // Delete the O/S timer.
    Cancel();


    LOG_DEBUG("~CmdTimerWin32Impl() complete (this = " << Ptr(this).ToString() << ").");
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/
void CmdTimerWin32::CmdTimerWin32Impl::SetCmd(Cmd::SharedPtr cmdToInvoke)
{
    _cmdToInvoke = Cmd::SharedPtr(cmdToInvoke);
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::StartRelative(Time::Duration invocationDelay)
{
    // Expiration time must not be in the past.
    ASSERT(invocationDelay >= Time::Duration(0, 0, 0, 0));

    //Must have a command to execute
    ASSERT(_cmdToInvoke);


    // Set the timer.
    // Note: fractional_seconds() returns microseconds--convert here to nanoseconds.
    TimerSetTime(invocationDelay.total_milliseconds());
}


/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::StartAbsolute(Time::Timepoint invocationTime)
{
    // Get the current time.
    Time::Timepoint currentTime(Time::GetTime());


    // Compute the relative delay.
    Time::Duration delay(invocationTime - currentTime);


    // Set the timer.
    StartRelative(delay);
}

