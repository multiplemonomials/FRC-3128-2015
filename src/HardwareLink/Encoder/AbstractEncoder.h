/*
 * AbstractEncoder.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef ABSTRACTENCODER_H_
#define ABSTRACTENCODER_H_

/*
 * Base class for the two types of encoder
 */
class AbstractEncoder
{
public:
	virtual ~AbstractEncoder();

	virtual double getAngle() = 0;

	virtual double getRawValue() = 0;
};

#endif /* ABSTRACTENCODER_H_ */
