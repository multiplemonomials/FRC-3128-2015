
//NOTE: This file has been modified from a library written by Randall Smith

#ifndef RENDEZVOUS_BASE_H
#define RENDEZVOUS_BASE_H

#include <Util/Time.h>

/*-----------------------------------------------------------------------------
     Base class for a synchronization object that allows one thread
     to block until another thread does something.
 ----------------------------------------------------------------------------*/

class RendezvousBase
{

public:

    // Ctor.
    explicit RendezvousBase()
    {
        // Empty.
    }


    // Copy ctor (non-copyable).
    RendezvousBase(const RendezvousBase&) = delete;


    // Assignment operator (non-assignable).
    RendezvousBase& operator=(const RendezvousBase&) = delete;

    // Dtor.
    virtual ~RendezvousBase()
    {
        // Empty.
    }


    // Blocks until another thread calls one of the Notify() services.
    virtual void Wait() const = 0;


    // Blocks until another thread calls one of the Notify() services,
    // or the specified time elapses.  Returns false if the timeout expired
    // before notification occurred.
    virtual bool TimedWait(Time::Duration const & timeoutPeriod) const = 0;


    // Wake up one of the threads waiting on this barrier.
    virtual void NotifyOne() const = 0;


    // Wake up all of the threads waiting on this barrier.
    virtual void NotifyAll() const = 0;
};


#endif /* RENDEZVOUS_BASE_H */
