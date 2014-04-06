/*
 * LogMessage.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: jamie
 */

#include "LogMessage.h"

LogMessage::LogMessage(InitializerListType const & list)
:_stream(),
 _tags(list)
{

}

LogMessage::LogMessage()
:_stream(),
 _tags()
{

}

LogMessage::LogMessage(LogMessage const & message)
:_stream(),
 _tags(message._tags)
{
	//copy message._stream's buffer
	_stream << message._stream.rdbuf();
}
