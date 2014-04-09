#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <boost/exception/all.hpp>
#include <sstream>

#include <LogMacros.h>

//NOTE: This file has been modified from a library written by Randall Smith

/*-----------------------------------------------------------------------------
    Use to attach an error message to an exception.  E.g.:

        BOOST_THROW_EXCEPTION(Exception() << ErrMsg("Setting name does not match any supported types."));
 ----------------------------------------------------------------------------*/

typedef boost::error_info<struct tag_error_msg, std::string> ErrMsg;


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

    virtual const char* what()
    {
    	LOG_DEBUG("Calling RoboException what()")
    	return (*(boost::get_error_info<ErrMsg>(*this))).c_str();
    }

    virtual ~RoboException()
    {

    }
};

/*-----------------------------------------------------------------------------
 *	Use to extract the message string from a RoboException
 *
 *	Usage:
 *
 *	catch(RoboException & error)
 *  {
 *  	BOOST_ERROR(GET_ROBOEXCPTION_MESSAGE(error));
 *  }
 *
 *---------------------------------------------------------------------------*/

#define GET_ROBOEXCPTION_MESSAGE(error) (*(boost::get_error_info<ErrMsg>(error)))


/*-----------------------------------------------------------------------------
    Shorthand to throw an Exception (or derivative) macro.

    Usage:

        // Throws |RoboException| object.
        THROW_EXCEPTION("An unknown error occurred.);

        // Throws my derivative of |RoboException|.
        THROW_MY_EXCEPTION(MyException() "An unknown error occurred.);
 ----------------------------------------------------------------------------*/

#define THROW_EXCEPTION( __errorMessage) \
	std::stringstream stringstream;		\
	stringstream <<  __errorMessage;	\
    BOOST_THROW_EXCEPTION(RoboException() << ErrMsg(stringstream.str()))

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
    if (!(__value)) BOOST_THROW_EXCEPTION(RoboException() << ErrMsg("Assertion failed."))


#endif /* EXCEPTION_H_ */
