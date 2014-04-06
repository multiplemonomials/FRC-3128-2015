/*------------------------------------------------------------------------------
	CmdProcessorTest.cpp

	<summary>

	Copyright (c) 2013 Qualcomm, Inc. All rights reserved.  
	Confidential and Proprietary.
	This software may be subject to U.S. and international export laws.
	Diversion contrary to U.S. and international laws is strictly prohibited.
------------------------------------------------------------------------------*/

#include <string>
#include <memory>
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread/future.hpp>

#include <Exception.h>
#include <EventManager/CmdProcessor.h>
#include <Rendezvous/Rendezvous.h>


BOOST_AUTO_TEST_SUITE(CmdProcessorTestSuite)


// Test handlers
void HandlerFunction(int arg)
{
    BOOST_MESSAGE("HandlerFunction() invoked.  arg = '" << arg << "'.");
    THROW_EXCEPTION(0, "error");
}

struct HandlerClass
{

    // operator() function.
    void operator()(int const & arg)
    {
        BOOST_MESSAGE("HandlerClass::operator() invoked.  arg = '" << arg << "'.");
        THROW_EXCEPTION(0, "error");
    }

    // Takes no arguments.
    void HandlerFunc1()
    {
        BOOST_MESSAGE("HandlerClass::HandlerFunc1 invoked.");
        THROW_EXCEPTION(0, "error");
    }

    // Takes two (const ref) arguments of different types.
    void HandlerFunc2(int const & arg1, double const & arg2)
    {
        BOOST_MESSAGE("HandlerClass::HandlerFunc2 invoked.  arg1 = '" << arg1 << "', arg2 = '" << arg2 << "'.");
        THROW_EXCEPTION(0, "error");
    }

    // Takes two arguments of different types.  One is used to provide input, the other to receive output.
    void HandlerFunc3(std::string const & input, std::string & output)
    {
        BOOST_MESSAGE("HandlerClass::HandlerFunc3 invoked.  input = '" << input << "'.");
        output = input;
    }


    // Takes two arguments of different types.  One is used to provide input, the other to receive output.
    // Note:  We also use a rendezvous object to synchronize with the originator of the command.
    // This is the standard approach when a command handler needs to return data.
    void HandlerFunc4
    (
        std::string const & input,
        std::string & output,
        Rendezvous & rendezvous
    )
    {
        BOOST_MESSAGE("HandlerClass::HandlerFunc3 invoked.  input = '" << input << "'.");
        output = input;
        rendezvous.NotifyOne();
    }


    bool _handlerFunc5Called;

    // Takes two (const ref) arguments of different types.   "Remembers" that it's been called.
    void HandlerFunc5(int const & arg1, double const & arg2)
    {
        BOOST_MESSAGE("HandlerClass::HandlerFunc5 invoked.  arg1 = '" << arg1 << "', arg2 = '" << arg2 << "'.");
        _handlerFunc5Called = true;
    }


    bool _handlerFunc6Called;

    // Takes two (const ref) arguments of different types.   "Remembers" that it's been called.
    int HandlerFunc6(int const & arg1, int const & arg2)
    {
        BOOST_MESSAGE("HandlerClass::HandlerFunc6 invoked.  arg1 = '" << arg1 << "', arg2 = '" << arg2 << "'.");
        _handlerFunc6Called = true;

        return arg1 + arg2;
    }
};


/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(Basic)
{
    BOOST_MESSAGE("------------------------------------------------------------");


    // Construct.
    HandlerClass            handler;

    // Free (C-style) function handler.
    Cmd::Functor            cmd1(boost::bind<void>(HandlerFunction, 81));
    BOOST_CHECK_THROW(cmd1(), Exception);

    // Functor handler.
    Cmd::Functor            cmd2(boost::bind<void>(handler, 44));
    BOOST_CHECK_THROW(cmd2(), Exception);

    // Ptr-to-member-function.
    Cmd::Functor            cmd3(boost::bind<void>(&HandlerClass::HandlerFunc1, handler));
    BOOST_CHECK_THROW(cmd3(), Exception);

    // Ptr-to-member-function.
    Cmd::Functor            cmd4(boost::bind<void>(&HandlerClass::HandlerFunc2, handler, 999, 1.27));
    BOOST_CHECK_THROW(cmd4(), Exception);

    // Ptr-to-member-function.
    std::string             inputValue("foo");
    std::string             returnValue;
    Cmd::Functor            cmd5(boost::bind<void>(&HandlerClass::HandlerFunc3, handler, boost::cref(inputValue), boost::ref(returnValue)));
    cmd5();
    BOOST_CHECK_EQUAL(returnValue, inputValue);
    BOOST_MESSAGE("returnValue = '" << returnValue << "'.");
}


/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(CommandProcessor)
{
    BOOST_MESSAGE("------------------------------------------------------------");


    // Construct.
    CmdProcessor                        cmdProcessor("CmdProcessor");
    HandlerClass                        handler;

    // Ptr-to-member-function returning a value.  Use Rendezvous object
    // to synchronize handler.
    std::string             inputValue("lksajdhflksajdhflakjsdhflaskjhdflasjf");
    std::string             returnValue;
    Rendezvous              rendezvous;
    cmdProcessor.Enqueue
    (
        &HandlerClass::HandlerFunc4,
        handler,
        boost::cref(inputValue),
        boost::ref(returnValue),
        boost::ref(rendezvous)
    );
    rendezvous.Wait();
    BOOST_CHECK_EQUAL(returnValue, inputValue);
    BOOST_MESSAGE("returnValue = '" << returnValue << "'.");
}


/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

#if 0

// I sort of like this idea, but it's taking up too much time to investigate.
// Looks like not all of the future<> stuff from braddock.com made it into
// the std:: version, so we would need to augment a bit.  Maybe later.


/*-----------------------------------------------------------------------------
    Convenience wrapper for std::promise objects.

    Taken from http://braddock.com/~braddock/future/
 ----------------------------------------------------------------------------*/

template<typename R> class future_wrapper
{
    boost::function<R(void)>    fn_;
    std::promise<R>             ft_;

public:

    // stores fn and ft
    future_wrapper
    (
        const boost::function<R(void)> &    fn,
        const std::promise<R> &             ft
    )
    : fn_(fn),
      ft_(ft)
    {
    }


    // executes fn() and places the outcome into ft
    void operator()() throw()
    {
        try
        {
            ft_.set(fn_());
        }
        catch (...)
        {
            ft_.set_exception(std::current_exception());
        }
    }

    std::future<R> get_future() const
    {
        return std::future < R > (ft_);
    }
};


class CmdProcessorWithFutures : public CmdProcessor
{
public:

    CmdProcessorWithFutures
    (
        std::string const &                 name
    )
    : CmdProcessor(name)
    {

    }


    template<typename RET_TYPE, typename... BindArgs>
    std::future<RET_TYPE> EnqueueAndReturnResponse(BindArgs... bindArgs)
    {
        // Build a functor from the specified args.
        Cmd::Functor functor(boost::bind<void>(std::forward<BindArgs>(bindArgs)...));

        // Wrap functor and associated promise together and queue for processing.
        std::promise<RET_TYPE>      promise;
        _queue.Enqueue(std::make_shared<future_wrapper<RET_TYPE>>(functor, promise));

        // Return a future (associated with the promise) to caller for retrieving result.
        return std::future<RET_TYPE>(promise);
    }
};


BOOST_AUTO_TEST_CASE(ReturnDataUsingFutures)
{
    BOOST_MESSAGE("------------------------------------------------------------");


    // Construct.
    CmdProcessorWithFutures             cmdProcessor("CmdProcessor");
    HandlerClass                        handler;

    BOOST_CHECK(!handler._handlerFunc6Called);
    std::future<int> response = cmdProcessor.EnqueueAndReturnResponse<int>(&HandlerClass::HandlerFunc6, boost::ref(handler), 2, 3);

    // response.get() blocks until processing is complete.
    BOOST_CHECK(response.get() == 5);
    BOOST_CHECK(handler._handlerFunc6Called);
}

#endif


BOOST_AUTO_TEST_SUITE_END()
