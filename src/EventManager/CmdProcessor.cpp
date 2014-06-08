#include "CmdProcessor.h"

#include <LogMacros.h>

//NOTE: This file has been modified from a library written by Randall Smith

/*-----------------------------------------------------------------------------
    Note:  The initialization of _thread starts the thread running.
 ----------------------------------------------------------------------------*/

CmdProcessor::CmdProcessor
(
    std::string const &                 name
)
:   _name(name + "CmdProcessor"),
    _queue(),
    _keepRunning(true),
    _thread(&CmdProcessor::operator(), this, _name)
{
    LOG_INFO("CmdProcessor(): Constructor completed for '" << _name << "'.");
}

/*-----------------------------------------------------------------------------
    Note:  Presently, this dtor will block until the thread actually shuts down,
    which makes cleaning up this active object really clean.  It does
    mean that destroying the object will take unbounded time, which
    may or may not be OK.  We can figure out something else if this is a
    problem.
 ----------------------------------------------------------------------------*/

CmdProcessor::~CmdProcessor()
{
    // Signal the thread to shut down.
    ShutDown();


    // Wait for the thread to shut down.
   // _thread.join();


    LOG_INFO("~CmdProcessor(): Shut down thread '" << _name << "'.");
}

/*-----------------------------------------------------------------------------
    Note:  This approach to shutting down the thread will cause all
    the currently-queued messages to be skipped.  This could be a good or
    bad thing.
 ----------------------------------------------------------------------------*/

void CmdProcessor::ShutDown()
{
    // Enqueue the shutdown command.
	 //_thread.interrupt();

    LOG_INFO("CmdProcessor::ShutDown(): Shutting down thread '" << _name << "'.");
}

/*-----------------------------------------------------------------------------
    This is the main body of the message processing thread.  It's fed
    to the boost::thread object at construction time, at which point
    the thread becomes active.
 ----------------------------------------------------------------------------*/

void CmdProcessor::operator()
(
    std::string                         threadName
)
{
	LOG_INFO("Thread " << threadName << " starting up");

    // Keep processing until told to stop (see ShutDownHandler()).
    do
    {
        try
        {
            // Pull a message off the queue and execute it.
            Cmd::SharedPtr  cmdPtr(_queue.Dequeue());
            (*cmdPtr)();
        }
//        //thread shutdown signal
//        catch(boost::thread_interrupted & interrupt)
//        {
//        	return;
//        }
        catch(std::exception & error)
        {
        	LOG_RECOVERABLE("Error processsing event: " << error.what())
        }

    } while (_keepRunning);

}




