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
		boost::unique_lock<boost::mutex>();
		_wasCalled = true;
	}

	bool wasCalled()
	{
		boost::unique_lock<boost::mutex>();
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

/*-----------------------------------------------------------------------------
    Test Case: Values can be retrieved from the joystick
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE(ValueRetrieval)
{
    BOOST_MESSAGE("------------------------------------------------------------");


	// Construct.
	Handler            handler;

	std::shared_ptr<JoystickMock> joystickMock = std::make_shared<JoystickMock>();

	joystickMock->setButton(4, true);

	ListenerManager listenerManager(joystickMock);

	joystickMock->setJoystick(3, 2.5);

	joystickMock->setButton(8, true);

	boost::this_thread::sleep(Time::Milliseconds(200));

	BOOST_CHECK_EQUAL(listenerManager.getRawDouble(Listenable::TRIGGERS), 2.5);

	BOOST_CHECK_EQUAL(listenerManager.getRawBool(Listenable::YDOWN), true);

	//also check that it's able to hand;e the corner case of someone using an up handler to get a value
	BOOST_CHECK_EQUAL(listenerManager.getRawBool(Listenable::STARTUP), true);
}

BOOST_AUTO_TEST_SUITE_END()
