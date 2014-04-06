/*
 * Tags.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: jamie
 */

#include "Tags.h"

std::string currentTime()
{
	std::time_t time = std::time(0);
	char cstr[128];
	std::strftime(cstr, sizeof(cstr), "%I:%M:%S", std::localtime(&time));

	return cstr;
}

std::string currentDate()
{
	std::time_t time = std::time(0);
	char cstr[128];
	std::strftime(cstr, sizeof(cstr), "%D", std::localtime(&time));

	return cstr;
}
