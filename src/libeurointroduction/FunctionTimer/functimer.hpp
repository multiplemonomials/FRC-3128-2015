
#ifndef _FUNCTIMER_H
#define _FUNCTIMER_H

#include <unistd.h>
#include <iostream>
#include <sys/time.h>


class scoped_function_timer
{

	struct timespec _startTime;

	std::string _funcName;
	
public:
	scoped_function_timer(std::string _funcName);

	~scoped_function_timer();

};

#define FUNC_TIMER scoped_function_timer __func_timer(__PRETTY_FUNCTION__)

#endif
