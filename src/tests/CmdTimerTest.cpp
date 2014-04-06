/*------------------------------------------------------------------------------
	CmdTimerTest.cpp

	<summary>

	Copyright (c) 2013 Qualcomm, Inc. All rights reserved.  
	Confidential and Proprietary.
	This software may be subject to U.S. and international export laws.
	Diversion contrary to U.S. and international laws is strictly prohibited.
------------------------------------------------------------------------------*/

#include <string>
#include <memory>
#include <boost/test/unit_test.hpp>

#include <string>
#include <memory>
#include <boost/test/unit_test.hpp>
#include <unistd.h>

#include <EventManager/CmdTimer.h>
#include <EventManager/CmdTimerPosix.h>


BOOST_AUTO_TEST_SUITE(CmdTimerTestSuite)

/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

struct MyHandler
{
private:
    bool _wasCalled;

public:

    // Ctor.
    MyHandler()
        : _wasCalled(false)
    {

    }

    // Handler function.
    void operator()()
    {
        _wasCalled = true;
        BOOST_MESSAGE("MyHandler() was called");
    }

    bool wasCalled()
    {
        return _wasCalled;
    }
};


/*-----------------------------------------------------------------------------

 ----------------------------------------------------------------------------*/

struct MyMultiHandler
{
private:
    int _timesCalled;

public:

    // Ctor.
    MyMultiHandler()
        : _timesCalled(0)
    {

    }

    // Handler function.
    void operator()()
    {
        ++_timesCalled;
        BOOST_MESSAGE("MyMultiHandler() was called (" << _timesCalled << " time).");
    }

    int timesCalled()
    {
        return _timesCalled;
    }
};


/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(Basic)
{
	//turn on logging
	boost::unit_test::unit_test_log_t::instance().set_threshold_level(boost::unit_test::log_messages);

    BOOST_MESSAGE("------------------------------------------------------------");

    MyHandler       handler;
    CmdTimerPosix   handlerCmdTimer{Cmd::MakeShared(boost::ref(handler))};

    handlerCmdTimer.StartRelative(Time::Seconds(3));

    Time::Sleep(Time::Seconds(5));


    // Verify that handler was called.
    BOOST_CHECK(handler.wasCalled());
}

/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(AssertIfSetCmdNotCalled)
{
    BOOST_MESSAGE("------------------------------------------------------------");

    MyHandler       handler;
    CmdTimerPosix   handlerCmdTimer{};

    // NOT DONE!  handlerCmdTimer.SetCmd(Cmd::MakeShared(boost::ref(handler)));

    BOOST_CHECK_THROW(handlerCmdTimer.StartRelative(Time::Seconds(3)), std::exception);
}

/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(Absolute)
{
    BOOST_MESSAGE("------------------------------------------------------------");

    MyHandler       handler;
    CmdTimerPosix   handlerCmdTimer{};
    handlerCmdTimer.SetCmd(Cmd::MakeShared(boost::ref(handler)));

    handlerCmdTimer.StartAbsolute(Time::GetTime() + Time::Seconds(3));

    Time::Sleep(Time::Seconds(6));

    BOOST_CHECK(handler.wasCalled());
}

/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(Canceling)
{
    BOOST_MESSAGE("------------------------------------------------------------");

    MyHandler       handler;
    CmdTimerPosix   handlerCmdTimer{};
    handlerCmdTimer.SetCmd(Cmd::MakeShared(boost::ref(handler)));

    handlerCmdTimer.StartRelative(Time::Seconds(3));
    handlerCmdTimer.Cancel();

    Time::Sleep(Time::Seconds(6));

    BOOST_CHECK(!handler.wasCalled());

}

/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(Deleting)
{
    BOOST_MESSAGE("------------------------------------------------------------");

    MyHandler           handler;
    {
        CmdTimerPosix   handlerCmdTimer{};
        handlerCmdTimer.SetCmd(Cmd::MakeShared(boost::ref(handler)));
        handlerCmdTimer.StartRelative(Time::Seconds(3));
        //let it go out of scope
    }

    Time::Sleep(Time::Seconds(6));

    BOOST_CHECK(!handler.wasCalled());
}


/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(Multiple)
{
    BOOST_MESSAGE("------------------------------------------------------------");

    MyMultiHandler      handler;

    CmdTimerPosix       handlerCmdTimer1{};
    CmdTimerPosix       handlerCmdTimer2{};
    CmdTimerPosix       handlerCmdTimer3{};

    handlerCmdTimer1.SetCmd(Cmd::MakeShared(boost::ref(handler)));
    handlerCmdTimer2.SetCmd(Cmd::MakeShared(boost::ref(handler)));
    handlerCmdTimer3.SetCmd(Cmd::MakeShared(boost::ref(handler)));

    handlerCmdTimer1.StartRelative(Time::Seconds(1));
    handlerCmdTimer2.StartRelative(Time::Seconds(2));
    handlerCmdTimer3.StartRelative(Time::Seconds(3));

    Time::Sleep(Time::Seconds(6));

    BOOST_CHECK(handler.timesCalled() == 3);
}


/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(CopyAndAssign)
{
    BOOST_MESSAGE("------------------------------------------------------------");

    MyMultiHandler      handler;
    CmdTimerPosix       handlerCmdTimer3{};
    handlerCmdTimer3.SetCmd(Cmd::MakeShared(boost::ref(handler)));

    {
        // Create a timer & start it.
        CmdTimerPosix   handlerCmdTimer1{};
        handlerCmdTimer1.SetCmd(Cmd::MakeShared(boost::ref(handler)));
        handlerCmdTimer1.StartRelative(Time::Seconds(3));


        // Copy-construct second wrapper.
        CmdTimerPosix   handlerCmdTimer2(handlerCmdTimer1);


        // Assign 3rd wrapper (deletes original timer assigned to this wrapper, which was never started).
        handlerCmdTimer3 = handlerCmdTimer2;
    }

    // Underlying timer should still be in existence at this point & fire normally.

    Time::Sleep(Time::Seconds(6));

    BOOST_CHECK(handler.timesCalled() == 1);
}


BOOST_AUTO_TEST_SUITE_END()
