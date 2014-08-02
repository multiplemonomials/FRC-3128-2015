/*
 * InterruptibleWaiter.h
 *
 *  Created on: Jun 9, 2014
 *      Author: jamie
 */

#ifndef INTERRUPTIBLEWAITER_H_
#define INTERRUPTIBLEWAITER_H_

#include <mutex>
#include <chrono>
#include <condition_variable>

/*
 * NOTE: while there's nothing preventing multiple threads from using this object aat the same time,
 * interrupt() and awaken() will target one at random
 */

class InterruptibleWaiter
{

private:

	//used to communicate to the waiting thread that it was interrupted
	volatile bool _wasInterrupted;

	std::condition_variable _conditionVariable;

	std::mutex _mutex;


public:

	//wait until somebody calls awaken() or block() on the waiter
	//unlocks the supplied unique_lock, blocks, and then re-locks it
	void block(std::unique_lock<std::mutex> & callersLock);

	//wait until somebody calls awaken() or block() on the waiter
	void block();

	//wait for the provided time or until interrupt() or awaken() is called
	//returns true if the timer expired false if the thread was awoken
	bool wait(unsigned long milliseconds);

	//interrupt the blocking thread if there is one, throwing an interruptedexception
	//returns true if a thread was interrupted, false if not
	void interrupt();

	//awaken the wating or blocking thread.  Does not throw an interruptedexception
	void awaken();

	InterruptibleWaiter();

	virtual ~InterruptibleWaiter();
};

#endif /* INTERRUPTIBLEWAITER_H_ */
