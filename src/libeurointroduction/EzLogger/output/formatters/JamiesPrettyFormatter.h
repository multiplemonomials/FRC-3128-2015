/*
 * JamiesPrettyFormatter.h
 *
 *  Created on: Jan 3, 2014
 *      Author: jamie
 */

#ifndef JAMIESPRETTYFORMATTER_H_
#define JAMIESPRETTYFORMATTER_H_

#include <memory>
#include <sstream>
#include <boost/foreach.hpp>

#include <LogMessage.h>

struct JamiesPrettyFormatter
{
	std::string operator()(std::shared_ptr<LogMessage> messagePtr)
	{
		std::ostringstream ostringstream;
		BOOST_FOREACH(LogMessage::TagMapElementType tag, messagePtr->getTags())
		{
			ostringstream << "[" << tag.second << "] ";
		}

		ostringstream << messagePtr->getStream().str();

		return ostringstream.str();
	}
};

#endif /* JAMIESPRETTYFORMATTER_H_ */
