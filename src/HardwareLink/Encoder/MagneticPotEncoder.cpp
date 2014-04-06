/*
 * MagneticPotEncoder.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include <HardwareLink/Encoder/MagneticPotEncoder.h>

MagneticPotEncoder::MagneticPotEncoder(double offset, int a, int b)
:_enc(a, b),
_offset(offset)
{

}

MagneticPotEncoder::MagneticPotEncoder(int a, int b)
:MagneticPotEncoder(a, b, 0)
{

}

MagneticPotEncoder::~MagneticPotEncoder()
{

}

double MagneticPotEncoder::getAngle()
{
	double voltage = 0;

	//average out the voltage over 10 reads
	for(char i = 0; i<10; i++)
	{
		voltage += _enc.GetVoltage();
	}
	voltage /= 10;

	return (voltage/5.0*360.0) + _offset;
}

double MagneticPotEncoder::getRawValue()
{
	return _enc.GetVoltage();
}
