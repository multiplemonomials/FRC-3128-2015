#include "functimer.hpp"
#include <Util/DefineZeroedStruct.h>
	
scoped_function_timer::scoped_function_timer(std::string funcName)
:_funcName(funcName)
{
	clock_gettime(CLOCK_REALTIME, &_startTime);
}

scoped_function_timer::~scoped_function_timer()
{
	DEFINE_ZEROED_STRUCT(struct timespec, endTime);

	clock_gettime(CLOCK_REALTIME, &endTime);

	unsigned long long startTimeInNs = (_startTime.tv_sec * 100000000LL) + _startTime.tv_nsec;
	unsigned long long endTimeInNs = (endTime.tv_sec * 100000000LL) + endTime.tv_nsec;

	std::cout << "Function \"" << _funcName << "\" took " << ((endTimeInNs - startTimeInNs)/1000ULL) << " microseconds to run." << std::endl;
}

