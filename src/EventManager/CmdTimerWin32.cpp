/*
 * CmdTimerWin32.cpp
 *
 *  Created on: Aug 18, 2014
 *      Author: jamie
 */

#include <EventManager/CmdTimerWin32.h>

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::TimerCreate()
{
    // Invoke sigev_notify_function as if it were the start function of a new thread.
    mySigEvent.sigev_notify = SIGEV_THREAD;


    // Static function shared by all instances of this class.
    mySigEvent.sigev_notify_function = CmdTimerWin32::CmdTimerWin32Impl::TimerHandler;


    // This value will be returned to TimerHandler() when the timer expires.
    mySigEvent.sigev_value.sival_ptr = this;


    // Create the POSIX timer, placing the ID in _timerid.
    if(timer_create(CLOCK_REALTIME, &mySigEvent, &_timerId) != 0)
    {
        LOG_FATAL("Error creating POSIX timer.  Errno = " <<  errno);
        ASSERT(false);
    }
}


/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::TimerSetTime
(
    time_t const &  seconds,
    long const &    nanoseconds
)
{

	//the second argument doesn't matter so long as it is not already a used ID
    SetTimer(nullptr, 9544);


    // Set the timer (don't bother getting the current value).
    if(timer_settime(_timerId, 0, &myItimerspec, nullptr) != 0)
    {
        LOG_FATAL("Error setting POSIX timer.  Errno = " <<  errno);
        ASSERT(false);
    }


    //LOG_DEBUG("CmdTimerWin32::CmdTimerWin32Impl::TimerSetTime() Set time to " << seconds << " seconds, " << nanoseconds << " nanoseconds.");
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

bool CmdTimerWin32::CmdTimerWin32Impl::TimerIsRunning()
{
	DEFINE_ZEROED_STRUCT(struct itimerspec, myItimerspec);

	timer_gettime(_timerId, &myItimerspec);

	// "If the value returned in itimerspec->curr_value->it_value is zero, then
    //the timer is currently disarmed."
	return (myItimerspec.it_value.tv_nsec != 0) || (myItimerspec.it_value.tv_sec != 0);
}


/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::TimerDelete()
{
    // Tell the O/S we're done with the timer.
    if (timer_delete(_timerId) != 0)
    {
        LOG_FATAL("Error deleting POSIX timer.  Errno = " <<  errno);
        ASSERT(false);
    }
}


/*-----------------------------------------------------------------------------
    Static callback function shared by all CmdTimerWin32Impl timers.
 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::TimerHandler(union sigval sigVal)
{


    CmdTimerWin32Impl * 	timerPtr((CmdTimerWin32Impl *)sigVal.sival_ptr);

    //LOG_DEBUG("CmdTimerWin32Impl::TimerHandler() firing (timerPtr = " << Ptr(timerPtr).ToString() << ").");

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

 ----------------------------------------------------------------------------*/

CmdTimerWin32::CmdTimerWin32Impl::CmdTimerWin32Impl()
: CmdTimerWin32Impl(Cmd::SharedPtr(), nullptr)
{

}

/*-----------------------------------------------------------------------------
	Base constructor
 ----------------------------------------------------------------------------*/

CmdTimerWin32::CmdTimerWin32Impl::CmdTimerWin32Impl(Cmd::SharedPtr command, CmdTimerMultiplex * multiplex)
:	_multiplex(multiplex),
    _cmdToInvoke(command),
    _timerId(nullptr)
{
    // Create the O/S timer.
    TimerCreate();

    LOG_DEBUG("CmdTimerWin32Impl() complete (this = " << Ptr(this).ToString() << ").");
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/
CmdTimerWin32::CmdTimerWin32Impl::CmdTimerWin32Impl(Cmd::SharedPtr command)
: CmdTimerWin32Impl(command, nullptr)
{

}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/
CmdTimerWin32::CmdTimerWin32Impl::CmdTimerWin32Impl(CmdTimerMultiplex * multiplex)
: CmdTimerWin32Impl(Cmd::SharedPtr(), multiplex)
{

}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

CmdTimerWin32::CmdTimerWin32Impl::~CmdTimerWin32Impl()
{
    // Cancel the timer (in case it's still running).
    Cancel();


    // Delete the O/S timer.
    TimerDelete();


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
    TimerSetTime(invocationDelay.total_seconds(), (invocationDelay.fractional_seconds() * 1000));
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

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerWin32::CmdTimerWin32Impl::Cancel()
{
    // Cancel the timer.
    TimerSetTime(0, 0);
}
