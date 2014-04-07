/*
 * MagneticPotEncoder.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef MAGNETICPOTENCODER_H_
#define MAGNETICPOTENCODER_H_

#include <AnalogChannel.h>

#include "AbstractEncoder.h"

class MagneticPotEncoder : public AbstractEncoder
{
	AnalogChannel _enc;

    const double _offset;

public:

    //main constructor
	MagneticPotEncoder(double offset, int a, int b);

	//forwarding constructor that sets offset to 0
	MagneticPotEncoder(int a, int b);

	virtual ~MagneticPotEncoder();

	/**
	 * Gets the approximated angle from a magnetic encoder. It uses values which
	 * have been estimated to high accuracy from extensive tests. Unless need be
	 * , do not modify these values.
	 *
	 * @return the approximate angle from 0 to 360 degrees of the encoder
	 */
	double getAngle();

	double getRawValue();
};

#endif /* MAGNETICPOTENCODER_H_ */
