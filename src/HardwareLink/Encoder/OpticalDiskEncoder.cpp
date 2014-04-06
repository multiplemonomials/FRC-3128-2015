/*
 * OpticalDiskEncoder.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include <HardwareLink/Encoder/OpticalDiskEncoder.h>

OpticalDiskEncoder::OpticalDiskEncoder(std::shared_ptr<Encoder> enc)
:_enc(enc)
{

}

OpticalDiskEncoder::~OpticalDiskEncoder()
{

}

double OpticalDiskEncoder::getAngle()
{
	return _enc->GetRaw()/4.0;
}
