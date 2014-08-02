/*
 * ThreadInterruptedException.cpp
 *
 *  Created on: Jun 9, 2014
 *      Author: jamie
 */

#include <InterruptibleWait/ThreadInterruptedException.h>

ThreadInterruptedException::ThreadInterruptedException()
{

}

const char* ThreadInterruptedException::what() const noexcept
{
	return "Thread interrupted";
}

ThreadInterruptedException::~ThreadInterruptedException()
{

}

