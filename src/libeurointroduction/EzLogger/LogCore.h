/*
 * LogCore.h
 *
 *  Created on: Dec 28, 2013
 *      Author: jamie
 */

#ifndef LOGCORE_H_
#define LOGCORE_H_

#include <boost/foreach.hpp>
#include <map>
#include <utility>
#include <output/LogOutput.h>
#include "LogMessage.h"

class LogCore
{
private:
	LogCore();

	LogCore(LogCore const &);

	void operator=(LogCore const &);

	typedef std::map<std::string, std::shared_ptr<LogOutputBaseClass> > outputMapType;

	outputMapType _outputs;

public:

	static LogCore & instance();

	//adds the message to the input queue of each LoggingOutput connected
	//each ThreadSafeQueue makes a copy of it
	void log(std::shared_ptr<LogMessage> message);

	//adds the LoggingOutput to the list of outputs to, ummm, output to
	void addOutput(std::string name, std::shared_ptr<LogOutputBaseClass> output);

	void removeOutput(std::string name);

	virtual ~LogCore();
};

#endif /* LOGCORE_H_ */
