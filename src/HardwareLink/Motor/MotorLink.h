/*
 * MotorLink.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef MOTORLINK_H_
#define MOTORLINK_H_

#include <Talon.h>

#include <HardwareLink/Encoder/AbstractEncoder.h>
#include <LogMacros.h>
#include <EventManager/CmdProcessor.h>
/**
 * MotorLink is the class used in this code to represent a motor.
 * It can operate with or without an encoder and with several
 * different varieties of speed control or none at all.
 *
 * Since it might be being controlled by several threads at once,
 * it uses a CmdProcessor to serialize calls to its methods
 * that change class-scope values.
 */

//prevent circular inclusion
class MotorControl;

class MotorLink
{

	std::shared_ptr<Talon> _talon;

    std::shared_ptr<AbstractEncoder> _encoder;

    std::shared_ptr<MotorControl> _spdControl;

    volatile bool _spdControlEnabled;
    volatile bool _motorReversed = false;
    volatile double _speedScalar = 1;

    std::string _name;

    CmdProcessor _cmdProcessor;

	void setSpeedImpl(double pow);

	void setEncoderImpl(std::shared_ptr<AbstractEncoder> enc);

	void reverseMotorImpl();

	void setSpeedScalarImpl(double powScl);

	void startSpeedControlImpl(double target);

	void setSpeedControllerImpl(std::shared_ptr<MotorControl> spdControl);

	void stopSpeedControlImpl();

public:
	MotorLink(std::string name, std::shared_ptr<Talon> talon)
	: MotorLink(name, talon, std::shared_ptr<AbstractEncoder>(), std::shared_ptr<MotorControl>(), 1)
	{

	}

	MotorLink(std::string name, std::shared_ptr<Talon> talon, double powscl)
	:MotorLink(name, talon, std::shared_ptr<AbstractEncoder>(), std::shared_ptr<MotorControl>(), powscl)
	{

	}

	MotorLink(std::string name, std::shared_ptr<Talon> talon, std::shared_ptr<AbstractEncoder> enc)
	:MotorLink(name, talon, enc, std::shared_ptr<MotorControl>(), 1)
	{

	}

	MotorLink(std::string name, std::shared_ptr<Talon> talon, std::shared_ptr<AbstractEncoder> enc, double powscl)
	:MotorLink(name, talon, enc, std::shared_ptr<MotorControl>(), 1)
	{

	}

	MotorLink(std::string name, std::shared_ptr<Talon> talon, std::shared_ptr<AbstractEncoder> enc, std::shared_ptr<MotorControl> spdControl)
	:MotorLink(name, talon, enc, spdControl, 1)
	{

	}

	MotorLink(std::string name, std::shared_ptr<Talon> talon, std::shared_ptr<AbstractEncoder> enc, std::shared_ptr<MotorControl> spdControl, double powscl)
	:_talon(talon),
	_encoder(enc),
	_spdControl(spdControl),
	_spdControlEnabled(false),
	_motorReversed(false),
	_speedScalar(powscl),
	_name(name),
	_cmdProcessor("MotorLinkCmdProcessor")
	{

	}

	virtual ~MotorLink();

	void reverseMotor()
	{
		_cmdProcessor.Enqueue(&MotorLink::reverseMotorImpl, boost::ref(*this));
	}

	void setSpeedScalar(double powScl)
	{
		_cmdProcessor.Enqueue(&MotorLink::setSpeedScalarImpl, boost::ref(*this), powScl);
	}

	//sets the motor speed, except if a speed controller is active
	void setSpeed(double pow);

	//sets the motor speed whether or not a speed controller is active
	void setSpeedForce(double pow);

	void setEncoder(std::shared_ptr<AbstractEncoder> enc)
	{
		_cmdProcessor.Enqueue(&MotorLink::setEncoderImpl, boost::ref(*this), enc);
	}

	void setSpeedController(std::shared_ptr<MotorControl> spdControl)
	{
		_cmdProcessor.Enqueue(&MotorLink::setSpeedControllerImpl, boost::ref(*this), spdControl);
	}

	double getSpeedScalar()
	{
		return _speedScalar;
	}

	double getSpeed()
	{
		return _talon->Get();
	}

    double getEncoderAngle()
    {
        if(!_encoder)
        {
            LOG_FATAL("Something attempted to get the encoder value, but no encoder exists.");
            return -1;
        }
        return _encoder->getAngle();
    }

    void setControlTarget(double target);

    bool isSpeedControlRunning()
    {
    	return _spdControlEnabled;
    }

    void startSpeedControl(double target)
	{
		_cmdProcessor.Enqueue(&MotorLink::startSpeedControlImpl, boost::ref(*this), target);
	}

    void stopSpeedControl()
	{
		_cmdProcessor.Enqueue(&MotorLink::stopSpeedControlImpl, boost::ref(*this));
	}

};

#endif /* MOTORLINK_H_ */
