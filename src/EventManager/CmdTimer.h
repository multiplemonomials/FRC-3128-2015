#ifndef CMD_TIMER_H
#define CMD_TIMER_H

#include <boost/date_time.hpp>
#include "Cmd.h"
#include <Util/Time.h>



/*-----------------------------------------------------------------------------
    Base class for the timer objects' impl private classes
 ----------------------------------------------------------------------------*/
class CmdTimerImpl
{

public:
	virtual ~CmdTimerImpl()
	{

	}
};

/*-----------------------------------------------------------------------------
    Base class for a hierearchy of objects that provide "timer" functions.

    Invokes a Cmd at a specified future time unless cancelled or
    destroyed.
 ----------------------------------------------------------------------------*/
class CmdTimer
{
public:

    // Virtual in this base class.
    virtual ~CmdTimer()
    {
        // Empty.
    }


    // Specify the Cmd to execute when the timer expires.
    virtual void SetCmd(Cmd::SharedPtr cmdToInvoke) = 0;

    // Return the Cmd to execute when the timer expires.
    virtual Cmd::SharedPtr GetCmd() = 0;

    // Return a constant pointer to the timer's impl
    virtual CmdTimerImpl * const GetImpl() = 0;

    // Starts the timer and sets it to trigger at a relative time
    virtual void StartRelative(Time::Duration invocationDelay) = 0;


    // Starts the timer to trigger at an absolute time
    virtual void StartAbsolute(Time::Timepoint invocationTime) = 0;


    // Turns off the timer.  You CAN restart it with Start*() and have it trigger.
    virtual void Cancel() = 0;
};

#endif /* CMD_TIMER_H */
