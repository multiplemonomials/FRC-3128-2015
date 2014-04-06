/*
 * OpticalDiskEncoder.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef OPTICALDISKENCODER_H_
#define OPTICALDISKENCODER_H_

class OpticalDiskEncoder
{
	std::shared_ptr<Encoder> _enc;

public:

	OpticalDiskEncoder(std::shared_ptr<Encoder> enc);

	double getAngle();

	virtual ~OpticalDiskEncoder();

	double getRawValue()
	{
		return _enc->GetRaw();
	}

};

#endif /* OPTICALDISKENCODER_H_ */
