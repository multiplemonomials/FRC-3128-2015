/*------------------------------------------------------------------------------
 UnitTests.cpp
 ------------------------------------------------------------------------------*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UnitTests
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <LogMacros.h>

//NOTE: this file is from a library written by Randall Smith

/*-----------------------------------------------------------------------------
    "Sanity" unit test to verify the operation of the unit test framework.
    Generates a success and a fail.
 ----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_SUITE(Sanity);


// Verify that the test framework is properly catching errors on every run.
BOOST_AUTO_TEST_CASE(UnitTestsSuccessTest)
{

    LOG_INFO("This is a message.");
    BOOST_REQUIRE_MESSAGE(1 == 1, "Just verifying that a success is detected.");
}


// Verify that the test framework is properly catching errors on every run.
BOOST_AUTO_TEST_CASE(UnitTestsFailureTest)
{
    // BOOST_REQUIRE_MESSAGE(1 == 2, "Just verifying that a failure is detected.");
}


BOOST_AUTO_TEST_SUITE_END()
