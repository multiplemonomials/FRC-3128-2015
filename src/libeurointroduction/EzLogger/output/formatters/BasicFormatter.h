/*
 * BaseFormatter.h
 *
 *  Created on: Jan 3, 2014
 *      Author: jamie
 */

#ifndef BASICFORMATTER_H_
#define BASICFORMATTER_H_

#include <memory>
#include <sstream>
#include <boost/foreach.hpp>

#include <LogMessage.h>

struct BasicFormatter
{
	std::string operator()(std::shared_ptr<LogMessage> messagePtr)
	{
		std::ostringstream ostringstream;
		BOOST_FOREACH(LogMessage::TagMapElementType tag, messagePtr->getTags())
		{
			ostringstream << "[" << tag.first << ": " << tag.second << "] ";
		}

		ostringstream << messagePtr->getStream().str();

		return ostringstream.str();
	}
};



#endif /* BASICFORMATTER_H_ */
