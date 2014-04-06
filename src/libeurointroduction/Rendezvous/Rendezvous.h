
//NOTE: This file has been modified from a library written by Randall Smith

#ifndef RENDEZVOUS_H_
#define RENDEZVOUS_H_

#include <boost/thread/thread.hpp>

#include "RendezvousBase.h"
#include <Util/Time.h>

/*-----------------------------------------------------------------------------
     General-purpose synchronization object to be used when one thread
     needs to wait for another to do something.  General scenario:

     (1) Thread A constructs a Rendezvous.
     (2) Thread A kicks off thread B, passing it a reference to the Rendezvous object.
     (3) Thread A calls Rendezvous.Wait(), causing it to suspend.
     (4) Thread B, completing it's work, calls Rendezvous.NotifyOne().
     (5) Thread A wakes up, from Rendezvous.Wait(), and proceeds.

     There are minor variations with TimedWait() and NotifyAll().

     Multiple threads can Wait(); one waiting thread will be released on each call
     to Notify().

     If Notify() is called before Wait(), Wait() will return immediately.
     (Multiple Notifications will not be counted--this could be fixed if needed.)
 ----------------------------------------------------------------------------*/

class Rendezvous : public RendezvousBase
{
    // Mutex and synchronizer used to coordinate things.
    mutable boost::mutex                            _mutex;
    mutable boost::condition_variable               _waiters;


    // True if Notify() has been called since construction.
    mutable bool                                    _notified;


public:

    // Ctor.
    explicit Rendezvous();


    // Copy ctor (non-copyable).
    Rendezvous(const Rendezvous&) = delete;


    // Assignment operator (non-assignable).
    Rendezvous& operator=(const Rendezvous&) = delete;


    // Dtor (silence compiler warning).
    virtual ~Rendezvous()
    {
        // Empty.
    }


    // Blocks until another thread calls one of the Notify() services.
    virtual void Wait() const;


    // Blocks until another thread calls one of the Notify() services,
    // or the specified time elapses.  Returns false if the timeout expired
    // before notification occurred.
    virtual bool TimedWait(Time::Duration const & timeoutPeriod) const;


    // Wake up one of the threads waiting on this barrier.
    virtual void NotifyOne() const;


    // Wake up all of the threads waiting on this barrier.
    virtual void NotifyAll() const;
};

#endif /* RENDEZVOUS_H_ */
