/**
 * Formats messages like this:
 * [Debug] 06:54:17 TestEzLogger.cpp:232 ----------------------------
 * This is a debug message.
 */

#ifndef RANDALLSCUSTOMFORMATTER_H_
#define RANDALLSCUSTOMFORMATTER_H_

#include <memory>
#include <sstream>
#include <boost/foreach.hpp>

#include <LogMessage.h>

struct RandallsCustomFormatter
{
	std::string operator()(std::shared_ptr<LogMessage> messagePtr)
	{
		std::ostringstream ostringstream;

		//make things a bit shorter
		LogMessage::TagMapType & tags = messagePtr->getTags();

		//the __FILE__ macro includes part of the path to the file
		const char* incorrect_filename = tags["filename"].c_str();

		const char* true_filename = strrchr(incorrect_filename, '/') ? strrchr(incorrect_filename, '/') + 1 : incorrect_filename;

		ostringstream << "[" << tags["severity"] << "] "
					  << tags["time"] << " "
		              << true_filename << ":"
		              << tags["line_number"]
		              << " ----------------------------"
		              << std::endl;


		ostringstream << messagePtr->getStream().str() << std::endl; //add an extra newline

		return ostringstream.str();
	}
};



#endif /* RANDALLSCUSTOMFORMATTER_H_ */
