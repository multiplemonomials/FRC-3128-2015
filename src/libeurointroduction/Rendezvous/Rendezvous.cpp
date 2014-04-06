//NOTE: This file has been modified from a library written by Randall Smith


#include "Rendezvous.h"


/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

Rendezvous::Rendezvous()
:   _mutex(),
    _waiters(),
    _notified(false)
{
    // Empty.
}


/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void Rendezvous::Wait() const
{
    boost::unique_lock<boost::mutex>    lock(_mutex);

    // If Notify has not been called...
    while (!_notified)
    {
        // Wait here until one of the Notify services is called.
        //
        // (NB: wait() releases _mutex while waiting, so others can call
        // a Notify service on this object, e.g., then re-acquires it
        // before it returns.)
        _waiters.wait(lock);
    }


    // Reset the bool for the next waiter (we have the lock again now!)
    // Each call to Notify will thus release one waiting thread.
    _notified = false;
}


/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

bool Rendezvous::TimedWait(Time::Duration const & timeoutPeriod) const
{
    boost::unique_lock<boost::mutex>    lock(_mutex);

    // If Notify has not been called...
    while (!_notified)
    {
        // Wait here until one of the Notify services is called.
        //
        // (NB: wait() releases _mutex while waiting, so others can call
        // a Notify service on this object, e.g., then re-acquires it
        // before it returns.)
        if (!_waiters.timed_wait(lock, timeoutPeriod))
        {
            // Returning because we timed out before Notify() was called.
            return false;
        }
    }


    // Reset the notification indicator for the next waiter (we have the lock again now!)
    // Each call to Notify will thus release one waiting thread.
    _notified = false;


    // Returning normally (not because of a timeout).
    return true;
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void Rendezvous::NotifyOne() const
{
    // Synchronously update the you-have-been-notified flag.
    {
        boost::unique_lock<boost::mutex>    lock(_mutex);

        // If this object is already in the notified state when someone
        // calls Notify, the notification will effectively be lost.  If this
        // happens, there's probably a problem somewhere.
        //
        // Technically, we could allow multiple advance notifications to be "saved up",
        // but this object does not presently support this and's it not clear
        // what the utility of this would be.
        assert(!_notified);

        _notified = true;
    }


    // Wake up one of the other waiting threads.
    _waiters.notify_one();
}

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

void Rendezvous::NotifyAll() const
{
    // Synchronously update the you-have-been-notified flag.
    {
        boost::unique_lock<boost::mutex>    lock(_mutex);

        // If this object is already in the notified state when someone
        // calls Notify, the notification will effectively be lost.  If this
        // happens, there's probably a problem somewhere.
        //
        // Technically, we could allow multiple advance notifications to be "saved up",
        // but this object does not presently support this and's it not clear
        // what the utility of this would be.
        assert(!_notified);

        _notified = true;
    }


    // Wake up *all* of the other waiting threads (they compete for the lock).
    _waiters.notify_all();
}
