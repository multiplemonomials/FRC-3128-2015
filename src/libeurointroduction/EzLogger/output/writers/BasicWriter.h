/*
 * BasicWriter.h
 *
 * LogOutput writer that just prints to cout
 */

#ifndef BASICWRITER_H_
#define BASICWRITER_H_

struct BasicWriter
{
	void operator()(std::string & messageText)
	{
		std::cout << messageText << std::endl;
	}
};


#endif /* BASICWRITER_H_ */
