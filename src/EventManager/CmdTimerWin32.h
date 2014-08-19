/*
 * CmdTimerWin32.h
 *
 *  Created on: Aug 18, 2014
 *      Author: jamie
 */

#ifndef CMDTIMERWIN32_H_
#define CMDTIMERWIN32_H_

#include "CmdTimer.h"
#include <windows.h>

class CmdTimerWin32 : public CmdTimer
{
 class CmdTimerWin32Impl : public CmdTimerImpl
 {
	 	static UINT_PTR guaranteedUnusedID;

	 	//see, the problem with the timer ID's is that, while one is randomly generated each time we call SetTimer(),
	 	//we pass one that is already used to the OS, instead of creating a new timer it will overwrite the settings for that one.
	 	//so we need to carefully map all of the used ID's and make sure
	 	static std::set<UINT_PTR>

	 	static std::map<UINT_PTR, std::shared_ptr<CmdTimerMultiplex>;

        //multiplex to report to when timer goes off
        //can be nullptr, which means no multiplex
        std::shared_ptr<CmdTimerMultiplex> 								_multiplex;

        Cmd::SharedPtr										_cmdToInvoke;

    private:

        // Handle to our timer instance being maintained by the O/S.
        UINT_PTR                                             _timerId;


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


        // Static handler function that is invoked by (all) CmdTimerWin32Impl timers.
        static void TimerHandler(HWND windowHandle, UINT uMsg, UINT_PTR idEvent, DWORD elapsedTime);

        //called by TimerHandler when timer fires
        //either executes command or
        //gives it to the multiplex
        void operator()();


    public:

        // Ctor. (with multiplex)
        explicit CmdTimerWin32Impl(Cmd::SharedPtr command, std::shared_ptr<CmdTimerMultiplex> multiplex);

        // Ctor. (without multiplex)
        explicit CmdTimerWin32Impl(Cmd::SharedPtr command);

        // Ctor.
        explicit CmdTimerWin32Impl();

        // Ctor.
        explicit CmdTimerWin32Impl(CmdTimerMultiplex * command);


        // Copy ctor (non-copyable).
        CmdTimerWin32Impl(const CmdTimerWin32Impl&) = delete;


        // Assignment operator (non-assignable).
        CmdTimerWin32Impl& operator=(const CmdTimerWin32Impl&) = delete;


        // Dtor.
        ~CmdTimerWin32Impl();

        //check with the OS if the timer is running
        bool TimerIsRunning();

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
    std::shared_ptr<CmdTimerWin32Impl>               _pImpl;


public:

    // Ctor. (with multiplex)
    explicit CmdTimerWin32(Cmd::SharedPtr command, CmdTimerMultiplex * multiplex)
    : _pImpl(new CmdTimerWin32Impl(command, multiplex))
    {

    }

    // Ctor. (without multiplex)
    explicit CmdTimerWin32(Cmd::SharedPtr command)
    : _pImpl(new CmdTimerWin32Impl(command))
    {

    }

    // Ctor.
    explicit CmdTimerWin32()
    : _pImpl(new CmdTimerWin32Impl)
    {

    }

    // Ctor.
    explicit CmdTimerWin32(std::shared_ptr<CmdTimerMultiplex> multiplex)
    : _pImpl(new CmdTimerWin32Impl(multiplex))
    {

    }


    // Copy ctor (copyable).
    CmdTimerWin32(const CmdTimerPosix&) = default;


    // Assignment operator (assignable).
    CmdTimerPosix& operator=(const CmdTimerPosix&) = default;

    //returns true if timer is running
    bool TimerIsRunning()
    {
        return _pImpl->TimerIsRunning();
    }


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
};

#endif /* CMDTIMERWIN32_H_ */
