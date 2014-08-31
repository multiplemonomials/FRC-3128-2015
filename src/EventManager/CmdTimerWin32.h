/*
 * CmdTimerWin32.h
 *
 *  Created on: Aug 18, 2014
 *      Author: jamie
 */

#ifndef CMDTIMERWIN32_H_
#define CMDTIMERWIN32_H_

#include "CmdTimer.h"
#include "CmdTimerMultiplex.h"

#include <windows.h>
#include <set>
#include <map>

/**
 * This class is used to execute a Cmd at a certain time, either relative or absolute.
 * It is the other half of the CmdTimer interface, used to work with Windows timers.
 * Compared to the Posix one, it is more resource-intensive to create, but much easier to kill.
 * Lastly, the resolution is 10ms and there can be exactly 4,294,967,295 ((max value of an uint32_t) - 1) timers running at the same time.
 */
class CmdTimerWin32 : public CmdTimer
{
 class CmdTimerWin32Impl : public CmdTimerImpl, public std::enable_shared_from_this<CmdTimerWin32Impl>
 {
	 	static UINT_PTR guaranteedUnusedID;

	 	//see, the problem with the timer ID's is that, while one is randomly generated each time we call SetTimer(), if
	 	//we pass one that is already used to the OS, instead of creating a new timer it will overwrite the settings for that one.
	 	//so we need to carefully map all of the used ID's and make sure
	 	//that we never use the same one twice
	 	static std::set<UINT_PTR> _usedIDs;

	 	static std::map<UINT_PTR, std::shared_ptr<CmdTimerWin32Impl>> _idToTimerMap;

	 	static std::mutex _mapMutex;

        //multiplex to report to when timer goes off
        //can be nullptr, which means no multiplex
        std::shared_ptr<CmdTimerMultiplex> 								_multiplex;

        Cmd::SharedPtr										_cmdToInvoke;

    private:

        // Handle to our timer instance being maintained by the O/S.
        UINT_PTR                                             _timerID;

        // Set an O/S timer (wrap Win32 gunk).
        // Timer expires this many milliseconds from now
        void TimerSetTime(UINT const & milliseconds);


        // Static handler function that is invoked by (all) CmdTimerWin32Impl timers.
        static void CALLBACK TimerHandler(HWND windowHandle, UINT uMsg, UINT_PTR idEvent, DWORD elapsedTime);

        //called by TimerHandler when timer fires
        //either executes command or
        //gives it to the multiplex
        void operator()();


    public:

        // Ctor.
        explicit CmdTimerWin32Impl(Cmd::SharedPtr command, Time::Duration timeRelative);

        // Ctor.
        explicit CmdTimerWin32Impl(Cmd::SharedPtr command);

        // Ctor.
        explicit CmdTimerWin32Impl();

        // Ctor.
        explicit CmdTimerWin32Impl(Cmd::SharedPtr command, std::shared_ptr<CmdTimerMultiplex> multiplex);


        // Copy ctor (non-copyable).
        CmdTimerWin32Impl(const CmdTimerWin32Impl&) = delete;


        // Assignment operator (non-assignable).
        CmdTimerWin32Impl& operator=(const CmdTimerWin32Impl&) = delete;


        // Dtor.
        ~CmdTimerWin32Impl();

        // Specify the Cmd to execute when the timer expires.
        void SetCmd(Cmd::SharedPtr cmdToInvoke);

        // Return the Cmd to execute when the timer expires.
        Cmd::SharedPtr GetCmd()
        {
        	return _cmdToInvoke;
        }

        //returns true if timer is running
        bool TimerIsRunning()
        {
        	return _timerID != 0;
        }

        // Starts the timer and sets it to trigger at a relative time
        void StartRelative(Time::Duration invocationDelay);


        // Starts the timer to trigger at an absolute time
        void StartAbsolute(Time::Timepoint invocationTime);

        // Deletes this timer from the OS
        void Cancel();
    };



    // Pointer to shared implementation object.  Allows CmdTimerPosix objects to
    // be copied and assigned.  Underlying O/S timer will be cleaned up when
    // last CmdTimerPosix pointing to it is destroyed.
    std::shared_ptr<CmdTimerWin32Impl>               _pImpl;


public:

    // Ctor. (with multiplex and duration included)
    //NOTE: Windows timers can't be set without a value.
    //hopefully no one ever waits more than 100 seconds between creating a timer and setting it
    explicit CmdTimerWin32(Cmd::SharedPtr command, Time::Duration timeRelative)
    : _pImpl(new CmdTimerWin32Impl(command, timeRelative))
    {

    }

    // Ctor. (with multiplex and duration included)
    explicit CmdTimerWin32(Cmd::SharedPtr command)
    : _pImpl(new CmdTimerWin32Impl(command))
    {

    }

    // Ctor. (with multiplex and duration included)
    explicit CmdTimerWin32(Cmd::SharedPtr command, std::shared_ptr<CmdTimerMultiplex> multiplex)
    : _pImpl(new CmdTimerWin32Impl(command, multiplex))
    {

    }

    // Ctor.
    explicit CmdTimerWin32()
    : _pImpl(new CmdTimerWin32Impl())
    {

    }


    // Copy ctor (copyable).
    CmdTimerWin32(const CmdTimerWin32&) = default;


    // Assignment operator (assignable).
    CmdTimerWin32& operator=(const CmdTimerWin32&) = default;


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
    virtual CmdTimerWin32Impl * const GetImpl()
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

    //returns true if timer is running
    bool TimerIsRunning()
    {
        return _pImpl->TimerIsRunning();
    }
};

#endif /* CMDTIMERWIN32_H_ */
