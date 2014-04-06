
//NOTE: This file has been modified from a library written by Randall Smith

#ifndef RENDEZVOUSMOCK_H_
#define RENDEZVOUSMOCK_H_

#include <boost/thread/thread.hpp>

#include "RendezvousBase.h"

/*-----------------------------------------------------------------------------
     Mock rendezvous object to be used in function code.

     Wait() service does not block.  Notify services don't do anything.
 ----------------------------------------------------------------------------*/

class RendezvousMock : public RendezvousBase
{

public:

    // Ctor.
    explicit RendezvousMock()
    {
        // Empty.
    }


    // Copy ctor (non-copyable).
    RendezvousMock(const RendezvousMock&) = delete;


    // Assignment operator (non-assignable).
    RendezvousMock& operator=(const RendezvousMock&) = delete;


    // Dtor (silence compiler warning).
    virtual ~RendezvousMock()
    {
        // Empty.
    }


    // Blocks until another thread calls one of the Notify() services.
    virtual void Wait()
    {
        // Empty.
    }


    // Wake up one of the threads waiting on this barrier.
    virtual void NotifyOne()
    {
        // Empty.
    }


    // Wake up all of the threads waiting on this barrier.
    virtual void NotifyAll()
    {
        // Empty.
    }
};

#endif /* RENDEZVOUSMOCK_H_ */
