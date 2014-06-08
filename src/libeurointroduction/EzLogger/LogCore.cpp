/*
 * LogCore.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: jamie
 */

#include "LogCore.h"

LogCore::LogCore()
:_outputs()
{

}

LogCore& LogCore::instance()
{
	//will be created on first use
	static LogCore localInstance;

	return localInstance;
}

void LogCore::log(std::shared_ptr<LogMessage> message)
{

	for(outputMapType::iterator it = _outputs.begin(); it != _outputs.end(); ++it)
	{
		it->second->enqueueMessage(message);
	}
}

void LogCore::addOutput(std::string name, std::shared_ptr<LogOutputBaseClass> output)
{
	_outputs[name] = output;
}

void LogCore::removeOutput(std::string name)
{
	_outputs.erase(name);
}

LogCore::~LogCore()
{

}

