/*------------------------------------------------------------------------------
	CmdTimerTest.cpp

	<summary>

	Copyright (c) 2013 Qualcomm, Inc. All rights reserved.
	Confidential and Proprietary.
	This software may be subject to U.S. and international export laws.
	Diversion contrary to U.S. and international laws is strictly prohibited.
------------------------------------------------------------------------------*/

#include <boost/test/unit_test.hpp>

#include <EventManager/CmdTimerMultiplex.h>


BOOST_AUTO_TEST_SUITE(CmdTimerMultiplexTestSuite)

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
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(Basic)
{
	//turn on logging
	boost::unit_test::unit_test_log_t::instance().set_threshold_level(boost::unit_test::log_messages);

    BOOST_MESSAGE("------------------------------------------------------------");

    CmdTimerMultiplex cmdTimerMultiplex{};

    MyHandler       handler;
    cmdTimerMultiplex.startTimer(Time::Seconds(2), Cmd::MakeShared(boost::ref(handler)));

    Time::Sleep(Time::Seconds(3));

    // Verify that handler was called.
    BOOST_CHECK(handler.wasCalled());

}

/*-----------------------------------------------------------------------------
    Test Case:
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(TimersStopWhenContainerIsDestroyed)
{
    BOOST_MESSAGE("------------------------------------------------------------");

    auto cmdTimerMultiplexPtr = std::make_shared<CmdTimerMultiplex>();

    MyHandler       handler;
    cmdTimerMultiplexPtr->startTimer(Time::Seconds(2), Cmd::MakeShared(boost::ref(handler)));

    Time::Sleep(Time::Seconds(1));

    cmdTimerMultiplexPtr.reset();

    Time::Sleep(Time::Seconds(2));

    // Verify that handler was called.
    BOOST_CHECK(!handler.wasCalled());

}

BOOST_AUTO_TEST_SUITE_END()
