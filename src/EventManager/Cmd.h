#ifndef CMD_H_
#define CMD_H_

#include <boost/function.hpp>
#include <boost/bind.hpp>

//NOTE: This file has been modified from a library written by Randall Smith

/*-----------------------------------------------------------------------------
    A key idiom is the Command pattern, where
    a command handler plus arguments are bundled into an object for later
    execution.

    This shows up in at least a couple of places:

    (1) CmdProcessor, which supports the enqueueing of commands for
    serialized execution in a separate thread.  This is the principal
    (and preferred) means of implementing active objects in the code.
    Typically, a public function creates the command object and enqueues
    it, and a private command handler function executes it within the
    context of the processor's thread.  This eliminates the need for most
    locking, automatically serializes requests from concurrent clients,
    and minimizes deadlock problems.

    (2) CmdTimer, which allows command to be queued for execution
    at a future time.  This is a nice way of doing periodic or deferred
    operations.  The command handler can re-queue another command if periodic
    invocations are needed.

    It's really flexible and requires almost no infrastructure.

    The downside of this otherwise-great approach is the need to get the
    correct syntax for building the commands, which can be tricky.  See
    www.boost.org/doc/libs/1_54_0/libs/bind/bind.html for a general discussion.

    Examples:

    (1) To invoke the following command handler without waiting for a response:

            void RegistrationRequestCmd
            (
                ApplicationIdType const &           applicationId
            );

        To enqueue the command:

            _cmdProcessor.Enqueue
            (
                &AppRegistry::RegistrationRequestCmd,   // MFP of handler.
                boost::ref(*this),                      // always pass |this| by non-const ref.
                applicationId                           // copy arg into command; we will be gone by the time it runs.
            );


    (2) To get something back from the following command handler, use a non-const reference
    argument and a Rendezvous object:

            void AppRegistry::GetRegistrationCountCmd
            (
                uint32_t &      count,
                Rendezvous &    rendezvous
            ) const
            {
                count = GetCount();
                rendezvous.NotifyOne();
            }

        To enqueue the command and wait for a response:

            uint32_t    returnValue;
            Rendezvous  rendezvous;

            _cmdProcessor.Enqueue
            (
                &AppRegistry::GetRegistrationCountCmd,  // MFP of handler.
                boost::ref(*this),                      // always pass |this| by non-const ref.
                boost::ref(returnValue),                // pass by non-const ref.
                boost::ref(rendezvous)                  // pass by non-const ref.
            );

            rendezvous.Wait();      // Block here until handler notifies us.
            return returnValue;     // returnValue now has valid content.

 ----------------------------------------------------------------------------*/

struct Cmd
{
    /*-----------------------------------------------------------------------------
        Command object that is executed by a command processor.  Technically
        a "nullary functor" object, one that implements the operator() member
        function, invoked with no arguments.
     ----------------------------------------------------------------------------*/

    typedef boost::function<void ()>        Functor;

    /*-----------------------------------------------------------------------------
        Shared pointer to command.  Commands on the command queue awaiting
        processing are managed by this type.
     ----------------------------------------------------------------------------*/

    typedef std::shared_ptr<Cmd::Functor>   SharedPtr;


    /*----------------------------------------------------------------------------
        Helper function that creates a Cmd object on the heap and returns a shared_ptr
        to it.  Uses std::forward to pass supplied arguments through to a
        boost::bind constructor.
     ----------------------------------------------------------------------------*/

    template<typename... BindArgs>
    SharedPtr static MakeShared(BindArgs... bindArgs)
    {
        return std::make_shared<Cmd::Functor>(boost::bind<void>(std::forward<BindArgs>(bindArgs)...));

    }

    /*----------------------------------------------------------------------------
        Helper function that creates a Cmd object on the heap and returns a shared_ptr
        to it.  Recieves a lambda as a boost::function.
     ----------------------------------------------------------------------------*/

    SharedPtr static MakeSharedLambda(boost::function<void()> function)
    {
        return std::shared_ptr<decltype(function)>(&function);

    }
};


#endif /* CMD_H_ */
