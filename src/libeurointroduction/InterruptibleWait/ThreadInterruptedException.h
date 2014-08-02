/*
 * ThreadInterruptedException.h
 *
 * Exception produced by InterruptibleWait if interrupted
 */

#ifndef THREADINTERRUPTEDEXCEPTION_H_
#define THREADINTERRUPTEDEXCEPTION_H_

#include <exception>
#include <string>

class ThreadInterruptedException : std::exception
{
public:
	ThreadInterruptedException();

	virtual const char* what() const noexcept;

	virtual ~ThreadInterruptedException();
};

#endif /* THREADINTERRUPTEDEXCEPTION_H_ */
