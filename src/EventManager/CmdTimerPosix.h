#ifndef CMD_TIMER_POSIX_H
#define CMD_TIMER_POSIX_H

#include <csignal>
#include <ctime>
#include <signal.h>
#include <time.h>
#include <map>
#include <boost/date_time.hpp>
#include <boost/thread/mutex.hpp>

#include "CmdTimer.h"
#include "Cmd.h"
#include <Util/Time.h>

//NOTE: This file has been modified from a library written by Randall Smith

/*-----------------------------------------------------------------------------
    Invokes a Cmd at a specified future time unless cancelled or
    destroyed.

    Built over the POSIX timer_create(), etc., services, so it's Unix specific.
    See, e.g.: http://man7.org/linux/man-pages/man2/timer_create.2.html

    Note: Only a certain number of timers may be created per process. See
    http://man7.org/linux/man-pages/man2/setrlimit.2.html for more info.
 ----------------------------------------------------------------------------*/

class CmdTimerMultiplex;

class CmdTimerPosix : public CmdTimer
{
    /*-----------------------------------------------------------------------------
         Implementation of the CmdTimerPosix class.  CmdTimerPosix objects have shared
         pointers to one of these guys.
     ----------------------------------------------------------------------------*/

    class CmdTimerPosixImpl : public CmdTimerImpl
    {


        //multiplex to report to when timer goes off
        //can be nullptr, which means no multiplex
        CmdTimerMultiplex * 								_multiplex;

        Cmd::SharedPtr										_cmdToInvoke;

    private:

        // Handle to our timer instance being maintained by the O/S.
        timer_t                                             _timerId;


        // Create an O/S timer (wrap Linux gunk).
        void TimerCreate();


        // Set an O/S timer (wrap Linux gunk).
        // Timer expires this many seconds/nanoseconds from now.  Providing zero
        // values cancels the timer.
        void TimerSetTime
        (
            time_t const &  seconds,
            long const &    nanoseconds
        );


        // Create an O/S timer (wrap Linux gunk).
        void TimerDelete();


        // Static handler function that is invoked by (all) CmdTimerPosixImpl timers.
        static void TimerHandler(union sigval sigVal);

        //called by TimerHandler when timer fires
        //either executes command or
        //gives it to the multiplex
        void operator()();


    public:

        // Ctor. (with multiplex)
        explicit CmdTimerPosixImpl(Cmd::SharedPtr command, CmdTimerMultiplex * multiplex);

        // Ctor. (without multiplex)
        explicit CmdTimerPosixImpl(Cmd::SharedPtr command);

        // Ctor.
        explicit CmdTimerPosixImpl();

        // Ctor.
        explicit CmdTimerPosixImpl(CmdTimerMultiplex * command);


        // Copy ctor (non-copyable).
        CmdTimerPosixImpl(const CmdTimerPosixImpl&) = delete;


        // Assignment operator (non-assignable).
        CmdTimerPosixImpl& operator=(const CmdTimerPosixImpl&) = delete;


        // Dtor.
        ~CmdTimerPosixImpl();

        // Specify the Cmd to execute when the timer expires.
        void SetCmd(Cmd::SharedPtr cmdToInvoke);

        // Return the Cmd to execute when the timer expires.
        Cmd::SharedPtr GetCmd()
        {
        	return _cmdToInvoke;
        }

        // Starts the timer and sets it to trigger at a relative time
        void StartRelative(Time::Duration invocationDelay);


        // Starts the timer to trigger at an absolute time
        void StartAbsolute(Time::Timepoint invocationTime);


        // Turns off the timer.  You CAN restart it with Start*() and have it trigger.
        void Cancel();
    };



    // Pointer to shared implementation object.  Allows CmdTimerPosix objects to
    // be copied and assigned.  Underlying O/S timer will be cleaned up when
    // last CmdTimerPosix pointing to it is destroyed.
    std::shared_ptr<CmdTimerPosixImpl>               _pImpl;


public:

    // Ctor. (with multiplex)
    explicit CmdTimerPosix(Cmd::SharedPtr command, CmdTimerMultiplex * multiplex)
    : _pImpl(new CmdTimerPosixImpl(command, multiplex))
    {

    }

    // Ctor. (without multiplex)
    explicit CmdTimerPosix(Cmd::SharedPtr command)
    : _pImpl(new CmdTimerPosixImpl(command))
    {

    }

    // Ctor.
    explicit CmdTimerPosix()
    : _pImpl(new CmdTimerPosixImpl)
    {

    }

    // Ctor.
    explicit CmdTimerPosix(CmdTimerMultiplex * multiplex)
    : _pImpl(new CmdTimerPosixImpl(multiplex))
    {

    }


    // Copy ctor (copyable).
    CmdTimerPosix(const CmdTimerPosix&) = default;


    // Assignment operator (assignable).
    CmdTimerPosix& operator=(const CmdTimerPosix&) = default;


    // Specify the Cmd to execute when the timer expires.
    virtual void SetCmd(Cmd::SharedPtr cmdToInvoke)
    {
        _pImpl->SetCmd(cmdToInvoke);
    }

    // Return the Cmd to execute when the timer expires.
    virtual Cmd::SharedPtr GetCmd()
    {
        return _pImpl->GetCmd();
    }

    // Return a constant pointer to the timer's impl
    virtual CmdTimerPosixImpl * const GetImpl()
    {
    	return _pImpl.get();
    }

    // Starts the timer and sets it to trigger at a relative time
    virtual void StartRelative(Time::Duration invocationDelay)
    {
        _pImpl->StartRelative(invocationDelay);
    }


    // Starts the timer to trigger at an absolute time
    virtual void StartAbsolute(Time::Timepoint invocationTime)
    {
        _pImpl->StartAbsolute(invocationTime);
    }

    // Turns off the timer.  You CAN restart it with Start*() and have it trigger.
    virtual void Cancel()
    {
        _pImpl->Cancel();
    }
};

#endif /* CMD_TIMER_POSIX_H */
