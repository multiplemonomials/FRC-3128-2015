/*
 * BasicAcceptor.h
 *
 * LogOutput acceptor that accepts everything
 */

#ifndef BASICACCEPTOR_H_
#define BASICACCEPTOR_H_

#include <LogMessage.h>

struct BasicAcceptor
{
	bool operator()(std::shared_ptr<LogMessage>)
	{
		return true;
	}
};



#endif /* BASICACCEPTOR_H_ */
