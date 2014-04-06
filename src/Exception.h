#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <boost/exception/all.hpp>

//NOTE: This file has been modified from a library written by Randall Smith


/*-----------------------------------------------------------------------------
    Base class for all exceptions.

    Throw using the following syntax:

        BOOST_THROW_EXCEPTION(Exception());

     Embeds the following information (file, line number, function name) by default:

        ../unittest/src/ExceptionTest.cpp(34): Throw in function void ExceptionTestSuite::MoreComplicatedThrowerFunc()
        Dynamic exception type: frc3128::Exception
        std::exception::what: std::exception
 ----------------------------------------------------------------------------*/

struct RoboException :
    virtual std::exception,          // Standard C++ exception base class.
    virtual boost::exception         // Allow additions of data.
{
    // Ctor.
    RoboException()
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
 ----------------------------------------------------------------------------*/

#define THROW_EXCEPTION( __errorMessage) \
    BOOST_THROW_EXCEPTION(RoboException() << ErrMsg(__errorMessage))

#define THROW_CUSTOM_EXCEPTION(__exception, __errorMessage) \
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
