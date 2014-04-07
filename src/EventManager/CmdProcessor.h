#ifndef CMDPROCESSOR_H_
#define CMDPROCESSOR_H_

#include <future>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <memory>

#include "ThreadSafeQueue/ThreadSafeQueue.h"
#include "Cmd.h"

//NOTE: This file has been modified from a library written by Randall Smith


/*-----------------------------------------------------------------------------
 * Active object that maintains a queue of command objects and executes them
 * in its own thread.
 *
 * Note:  The thread starts up when the object is constructed, and
 * stops when the object is destroyed.  I.e., the thread lifetime and the
 * object lifetime are identical.  This makes things really simple when
 * constructing and destroying the object.
 ----------------------------------------------------------------------------*/

class CmdProcessor
{
    // Human-readable name of thread.  Used for logging.
    std::string                                 _name;


    // Command queue.  Goal of this object is to take things out of
    // this queue and invoke them.
    ThreadSafeQueue<Cmd::SharedPtr>             _queue;


    // True if the thread should continue running.
    // Note: mutual exclusion not needed since it's only modified from
    // a message handler executing in the thread.
    bool                                        _keepRunning;


    // Thread.
    boost::thread                               _thread;

    // Cause the message processing loop to shut down.
    //
    // Private since destroying the object is the proper way to
    // shut down the thread.  (Not sure what it would mean to shut down the thread
    // but leave this object in place.)
    void ShutDown();


public:

    // Ctor.  Accepts human-readable name of thread.
    // Thread is running after construction.
    CmdProcessor
    (
        std::string const &                 name
    );


    // Need explicit dtor to shut down thread.
    ~CmdProcessor();


    // Builds a Cmd with the specified ctor arguments, then puts it on the processing queue.
    template<typename... BindArgs>
    void Enqueue(BindArgs... bindArgs)
    {
        _queue.Enqueue(std::make_shared<Cmd::Functor>(boost::bind<void>(std::forward<BindArgs>(bindArgs)...)));
    }

    // Builds a Cmd with the specified ctor arguments, then puts it on the processing queue.
    void EnqueueLambda(Cmd::Functor function)
    {
        _queue.Enqueue(std::shared_ptr<Cmd::Functor>(&function));
    }


    // Builds a Cmd with the specified ctor arguments, then puts it on the front (i.e. next to execute) on the processing queue.
    template<typename... BindArgs>
    void EnqueueFront(BindArgs... bindArgs)
    {
        _queue.Enqueue(std::make_shared<Cmd::Functor>(boost::bind<void>(std::forward<BindArgs>(bindArgs)...)), true);
    }


    // Body of the message processing thread.
    // Making this public since boost::thread can't see it otherwise.  There's
    // probably a better way.
    void operator()
    (
        std::string                         threadName
    );
};



#endif /* CMDPROCESSOR_H_ */
