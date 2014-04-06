/*------------------------------------------------------------------------------
	Exception.h

	<summary>

	Copyright (c) 2013 Qualcomm, Inc. All rights reserved.  
	Confidential and Proprietary.
	This software may be subject to U.S. and international export laws.
	Diversion contrary to U.S. and international laws is strictly prohibited.
------------------------------------------------------------------------------*/

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <boost/exception/all.hpp>


/*-----------------------------------------------------------------------------
    Base class for all exceptions.

    Throw using the following syntax:

        BOOST_THROW_EXCEPTION(Exception());

     Embeds the following information (file, line number, function name) by default:

        ../unittest/src/ExceptionTest.cpp(34): Throw in function void ExceptionTestSuite::MoreComplicatedThrowerFunc()
        Dynamic exception type: frc3128::Exception
        std::exception::what: std::exception
 ----------------------------------------------------------------------------*/

struct Exception :
    virtual std::exception,          // Standard C++ exception base class.
    virtual boost::exception         // Allow additions of data.
{
    // Ctor.
    Exception()
    {

    }
};


/*-----------------------------------------------------------------------------
    Use to attach an error message to an exception.  E.g.:

        BOOST_THROW_EXCEPTION(Exception() << ErrMsg("Setting name does not match any supported types."));
 ----------------------------------------------------------------------------*/

typedef boost::error_info<struct tag_error_msg, std::string> ErrMsg;


/*-----------------------------------------------------------------------------
    Use to attach an error code to an exception.  E.g.:

        BOOST_THROW_EXCEPTION(Exception() << ErrCode(43));
 ----------------------------------------------------------------------------*/

typedef boost::error_info<struct tag_error_code, int> ErrCode;


/*-----------------------------------------------------------------------------
    Shorthand to throw an Exception (or derivative) macro.

    Usage:

        // Throws |Exception| object.
        THROW_EXCEPTION(M2M_ERR_UNKNOWN_ERROR, "An unknown error occurred.);

        // Throws my derivative of |Exception|.
        THROW_MY_EXCEPTION(MyException(), M2M_ERR_UNKNOWN_ERROR, "An unknown error occurred.);

      Note:  The error code values should be drawn from M2mErrorCodes.h; these
      are frequently returned to M2M applications, and so should track this file.
 ----------------------------------------------------------------------------*/

#define THROW_EXCEPTION(__errorCode, __errorMessage) \
    BOOST_THROW_EXCEPTION(Exception() << ErrMsg(__errorMessage))

#define THROW_MY_EXCEPTION(__exception, __errorCode, __errorMessage) \
    BOOST_THROW_EXCEPTION(__exception << ErrMsg(__errorMessage))


/*-----------------------------------------------------------------------------
    ASSERT macro.  Evaluates the specified predicate and throws if it's not true.

    E.g.:

    {
        bool mustBeTrue;

        // ...

        ASSERT(mustBeTrue);     // Throws if not true.
    }

    Always enabled.
 ----------------------------------------------------------------------------*/

#define ASSERT(__value) \
    if (!(__value)) BOOST_THROW_EXCEPTION(Exception() << ErrMsg("Assertion failed."))


#endif /* EXCEPTION_H_ */
