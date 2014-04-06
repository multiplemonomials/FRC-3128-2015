/*------------------------------------------------------------------------------
 UnitTests.cpp

 Top-level source file for the M2MSLAP unit tests.

 Copyright (c) 2013 Qualcomm, Inc. All rights reserved.
 Confidential and Proprietary.
 This software may be subject to U.S. and international export laws.
 Diversion contrary to U.S. and international laws is strictly prohibited.
 ------------------------------------------------------------------------------*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UnitTests
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <LogMacros.h>

namespace m2mslap
{

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


} /* namespace m2mslap */
