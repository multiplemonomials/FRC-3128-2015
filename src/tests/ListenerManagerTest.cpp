#include <string>
#include <memory>
#include <boost/test/unit_test.hpp>

#include <EventManager/ListenerManager.h>
#include "Mock/JoystickMock.h"
#include <Util/Time.h>
#include <Exception.h>
#include <LogMacros.h>


BOOST_AUTO_TEST_SUITE(ListenerManagerTestSuite)

class Handler
{
	boost::mutex _mutex;

	volatile bool _wasCalled;

public:
	void operator()()
	{
		LOG_DEBUG("Handler getting mutex lock..")
		boost::unique_lock<boost::mutex>();
		LOG_DEBUG("Handler being invoked..")
		_wasCalled = true;
	}

	bool wasCalled()
	{
		LOG_DEBUG("Handler polling function getting mutex lock...")
		boost::unique_lock<boost::mutex>();
		LOG_DEBUG("Handler polling function being invoked and returning " << (_wasCalled ? "true" : "false"))
		return _wasCalled;
	}
};


/*-----------------------------------------------------------------------------
    Test Case: registering a ButtonDown handler works
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(ButtonDown)
{

	//turn on logging
	boost::unit_test::unit_test_log_t::instance().set_threshold_level(boost::unit_test::log_messages);

    BOOST_MESSAGE("------------------------------------------------------------");


    // Construct.
    Handler            handler;

    std::shared_ptr<JoystickMock> joystickMock = std::make_shared<JoystickMock>();

    ListenerManager listenerManager(joystickMock);

    boost::this_thread::sleep(Time::Milliseconds(200));

    listenerManager.addListener(Listenable::ADOWN, Cmd::MakeShared(boost::ref(handler)));

    joystickMock->setButton(Listenable::ADOWN, true);

    boost::this_thread::sleep(Time::Milliseconds(200));

    BOOST_CHECK(handler.wasCalled());
}


/*-----------------------------------------------------------------------------
    Test Case: registering a ButtonUp handler works
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(ButtonUp)
{
    BOOST_MESSAGE("------------------------------------------------------------");


    // Construct.
    Handler            handler;

    std::shared_ptr<JoystickMock> joystickMock = std::make_shared<JoystickMock>();

    joystickMock->setButton(1, true);

    ListenerManager listenerManager(joystickMock);

    boost::this_thread::sleep(Time::Milliseconds(200));

    listenerManager.addListener(Listenable::AUP, Cmd::MakeShared(boost::ref(handler)));

    joystickMock->setButton(1, false);

    boost::this_thread::sleep(Time::Milliseconds(300));

    BOOST_CHECK(handler.wasCalled());
}

/*-----------------------------------------------------------------------------
    Test Case: registering a Joystick handler works
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(Joystick)
{
    BOOST_MESSAGE("------------------------------------------------------------");


	// Construct.
	Handler            handler;

	std::shared_ptr<JoystickMock> joystickMock = std::make_shared<JoystickMock>();

	ListenerManager listenerManager(joystickMock);

	listenerManager.addListener(Listenable::TRIGGERS, Cmd::MakeShared(boost::ref(handler)));

	joystickMock->setJoystick(3, 2.5);

	boost::this_thread::sleep(Time::Milliseconds(200));

	BOOST_CHECK(handler.wasCalled());
}

BOOST_AUTO_TEST_SUITE_END()
