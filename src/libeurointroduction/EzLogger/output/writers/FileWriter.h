/*
 * FileWriter.h
 *
 *  Created on: Jan 3, 2014
 *      Author: jamie
 */

#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <fstream>
#include <iostream>

class FileWriter
{
	std::ofstream _ofstream;

public:
	FileWriter(const char* filename)
	:_ofstream(filename)
	{
		if(!_ofstream)
		{
			std::cerr << "FileWriter: file \"" << filename << "\" could not be opened.";
		}
	}

	void operator()(const std::string & text)
	{
		_ofstream << text << std::endl;
	}

};

#endif /* FILEWRITER_H_ */
