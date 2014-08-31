#if 0

#include "CmdTimerPosix.h"
#include "Util/DefineZeroedStruct.h"
#include <LogMacros.h>
#include <Util/Ptr.h>
#include "CmdTimerMultiplex.h"
#include <Exception.h>

//NOTE: This file has been modified from a library written by Randall Smith

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerPosix::CmdTimerPosixImpl::TimerCreate()
{
	DEFINE_ZEROED_STRUCT(struct sigevent, mySigEvent);


    // Invoke sigev_notify_function as if it were the start function of a new thread.
    mySigEvent.sigev_notify = SIGEV_THREAD;


    // Static function shared by all instances of this class.
    mySigEvent.sigev_notify_function = CmdTimerPosix::CmdTimerPosixImpl::TimerHandler;


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

void CmdTimerPosix::CmdTimerPosixImpl::TimerSetTime
(
    time_t const &  seconds,
    long const &    nanoseconds
)
{

    DEFINE_ZEROED_STRUCT(struct itimerspec, myItimerspec);


    // Always specify a one-shot timer (no repetition).
    myItimerspec.it_interval.tv_sec = 0;
    myItimerspec.it_interval.tv_nsec = 0;


    // Load supplied time values.
    myItimerspec.it_value.tv_sec = seconds;
    myItimerspec.it_value.tv_nsec = nanoseconds;


    // Set the timer (don't bother getting the current value).
    if(timer_settime(_timerId, 0, &myItimerspec, nullptr) != 0)
    {
        LOG_FATAL("Error setting POSIX timer.  Errno = " <<  errno);
        ASSERT(false);
    }


    //LOG_DEBUG("CmdTimerPosix::CmdTimerPosixImpl::TimerSetTime() Set time to " << seconds << " seconds, " << nanoseconds << " nanoseconds.");
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

bool CmdTimerPosix::CmdTimerPosixImpl::TimerIsRunning()
{
	DEFINE_ZEROED_STRUCT(struct itimerspec, myItimerspec);

	timer_gettime(_timerId, &myItimerspec);

	// "If the value returned in itimerspec->curr_value->it_value is zero, then
    //the timer is currently disarmed."
	return (myItimerspec.it_value.tv_nsec != 0) || (myItimerspec.it_value.tv_sec != 0);
}


/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerPosix::CmdTimerPosixImpl::TimerDelete()
{
    // Tell the O/S we're done with the timer.
    if (timer_delete(_timerId) != 0)
    {
        LOG_FATAL("Error deleting POSIX timer.  Errno = " <<  errno);
        ASSERT(false);
    }
}


/*-----------------------------------------------------------------------------
    Static callback function shared by all CmdTimerPosixImpl timers.
 ----------------------------------------------------------------------------*/

void CmdTimerPosix::CmdTimerPosixImpl::TimerHandler(union sigval sigVal)
{


    CmdTimerPosixImpl * 	timerPtr((CmdTimerPosixImpl *)sigVal.sival_ptr);

    //LOG_DEBUG("CmdTimerPosixImpl::TimerHandler() firing (timerPtr = " << Ptr(timerPtr).ToString() << ").");

    //invoke the handler of that particular object
    timerPtr->operator()();
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerPosix::CmdTimerPosixImpl::operator()()
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

CmdTimerPosix::CmdTimerPosixImpl::CmdTimerPosixImpl()
: CmdTimerPosixImpl(Cmd::SharedPtr(), nullptr)
{

}

/*-----------------------------------------------------------------------------
	Base constructor
 ----------------------------------------------------------------------------*/

CmdTimerPosix::CmdTimerPosixImpl::CmdTimerPosixImpl(Cmd::SharedPtr command, CmdTimerMultiplex * multiplex)
:	_multiplex(multiplex),
    _cmdToInvoke(command),
    _timerId(nullptr)
{
    // Create the O/S timer.
    TimerCreate();

    LOG_DEBUG("CmdTimerPosixImpl() complete (this = " << Ptr(this).ToString() << ").");
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/
CmdTimerPosix::CmdTimerPosixImpl::CmdTimerPosixImpl(Cmd::SharedPtr command)
: CmdTimerPosixImpl(command, nullptr)
{

}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/
CmdTimerPosix::CmdTimerPosixImpl::CmdTimerPosixImpl(Cmd::SharedPtr command, Time::Duration timeRelative)
: CmdTimerPosixImpl(command, nullptr)
{

}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/
CmdTimerPosix::CmdTimerPosixImpl::CmdTimerPosixImpl(CmdTimerMultiplex * multiplex)
: CmdTimerPosixImpl(Cmd::SharedPtr(), multiplex)
{

}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

CmdTimerPosix::CmdTimerPosixImpl::~CmdTimerPosixImpl()
{
    // Cancel the timer (in case it's still running).
    Cancel();


    // Delete the O/S timer.
    TimerDelete();


    LOG_DEBUG("~CmdTimerPosixImpl() complete (this = " << Ptr(this).ToString() << ").");
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/
void CmdTimerPosix::CmdTimerPosixImpl::SetCmd(Cmd::SharedPtr cmdToInvoke)
{
    _cmdToInvoke = Cmd::SharedPtr(cmdToInvoke);
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void CmdTimerPosix::CmdTimerPosixImpl::StartRelative(Time::Duration invocationDelay)
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

void CmdTimerPosix::CmdTimerPosixImpl::StartAbsolute(Time::Timepoint invocationTime)
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

void CmdTimerPosix::CmdTimerPosixImpl::Cancel()
{
    // Cancel the timer.
    TimerSetTime(0, 0);
}

#endif // WIN32
