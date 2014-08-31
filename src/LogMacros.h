/*
 * LogMacros.h
 *
 *  Created on: Jul 31, 2014
 *      Author: jamie
 */

#ifndef LOGMACROS_H_
#define LOGMACROS_H_

//if we are running unit tests, we want the output to print in real time
//so forward the log messages to Boost Test
#ifndef UNIT_TEST

#include <EzLogger/LogMessage.h>
#include <EzLogger/LogCore.h>
#include <memory>
#include <EzLogger/Tags.h>

#define LOG_DEBUG(args) \
{ \
std::shared_ptr<LogMessage> logMessage(new LogMessage({{"time", currentTime()}, {"severity", "Debug"}})); \
logMessage->stream() << args;\
LogCore::instance().log(logMessage); \
}

#define LOG_INFO(args) \
{ \
std::shared_ptr<LogMessage> logMessage(new LogMessage({{"time", currentTime()}, {"severity", "Info"}})); \
logMessage->stream() << args;\
LogCore::instance().log(logMessage); \
}

#define LOG_UNUSUAL(args) \
{ \
std::shared_ptr<LogMessage> logMessage(new LogMessage({{"time", currentTime()}, {"severity", "Unusual"}})); \
logMessage->stream() << args;\
LogCore::instance().log(logMessage); \
}


#define LOG_RECOVERABLE(args) \
{ \
std::shared_ptr<LogMessage> logMessage(new LogMessage({{"time", currentTime()}, {"severity", "Recoverable"}})); \
logMessage->stream() << args;\
LogCore::instance().log(logMessage); \
}


#define LOG_FATAL(args) \
{ \
std::shared_ptr<LogMessage> logMessage(new LogMessage({{"time", currentTime()}, {"severity", "Fatal"}})); \
logMessage->stream() << args;\
LogCore::instance().log(logMessage); \
std::this_thread::sleep_for(std::chrono::milliseconds(100));\
exit(2); \
}

#else

#include <boost/test/unit_test.hpp>

#define LOG_DEBUG(args) \
{ \
std::stringstream stream; \
stream << "[Debug] " << args; \
BOOST_MESSAGE(stream.str()); \
}

#define LOG_INFO(args) \
{ \
std::stringstream stream; \
stream << "[Info] " << args; \
BOOST_MESSAGE(stream.str()); \
}

#define LOG_UNUSUAL(args) \
{ \
std::stringstream stream; \
stream << "[Unusual] " << args; \
BOOST_MESSAGE(stream.str()); \
}


#define LOG_RECOVERABLE(args) \
{ \
std::stringstream stream; \
stream << "[Recoverable] " << args; \
BOOST_MESSAGE(stream.str()); \
}


#define LOG_FATAL(args) \
{ \
std::stringstream stream; \
stream << "[Fatal] " << args; \
BOOST_MESSAGE(stream.str()); \
}

#endif


#endif /* LOGMACROS_H_ */
