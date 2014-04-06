/*
 * LogMessage.h
 *
 *  Created on: Dec 28, 2013
 *      Author: jamie
 */

#ifndef LOGMESSAGE_H_
#define LOGMESSAGE_H_

#include <memory>
#include <map>
#include <sstream>
/**
 * A LogMessage contains all of the data for one message that will be written to the log.
 * It can hold an infinite number of tags and one text message
 * LoggerOutput takes care of the formatting
 */
class LogMessage
{
	// User-supplied portion of the log message.
	std::ostringstream 											_stream;

public:
	typedef std::map<std::string, std::string> 					TagMapType;
	typedef std::pair<std::string, std::string> 				TagMapElementType;
	typedef std::initializer_list<std::pair<const std::string, std::string>> InitializerListType;

private:
	TagMapType 													_tags;

public:

	// Construct from C++ initializer list.
	LogMessage(InitializerListType const & list);

	//construct with no tags
	LogMessage();

	// Copyable.
	LogMessage(LogMessage const & message);


	// assignable
	LogMessage & operator=(LogMessage const &) = default;


	// Equality operator.
	bool operator==(LogMessage const & rhs)
	{
		return rhs._stream == _stream && rhs._tags == _tags;
	}


	// Return reference to _stream--aloows user to supply content.
	std::ostringstream & stream()
	{
		return _stream;
	}

	const std::ostringstream& getStream() const
	{
		return _stream;
	}

	TagMapType& getTags()
	{
		return _tags;
	}
};

#endif /* LOGMESSAGE_H_ */
