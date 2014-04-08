/*
 * WPIMock.cpp
 *
 * Until the actual Linux-native WPILib is released,
 * this file is used to link and test the program.
 *
 * It holds replacements for the WPILib functions that are
 * invoked.
 */

#include <LogMacros.h>

#include <WPILib.h>

uint32_t DigitalInput::Get()
{
	LOG_INFO("DigitalInput::Get Called")
	return 0;
}

SensorBase::SensorBase()
{

}

AnalogChannel::AnalogChannel(uint8_t moduleNumber, uint32_t channel)
:m_channel(channel),
 m_module(nullptr),
 m_accumulator(nullptr),
 m_accumulatorOffset(0),
 m_shouldUseVoltageForPID(false),
 m_table(nullptr)
{

}

float AnalogChannel::GetVoltage()
{
	return 0.0;
}

int32_t Encoder::GetRaw()
{
	return 0;
}

void Relay::Set(Value value)
{

}

Gyro::Gyro(unsigned char, unsigned int)
:m_analog(nullptr),
m_voltsPerDegreePerSecond(0),
m_offset(0),
m_channelAllocated(true),
m_center(0),
m_pidSource(PIDSource::PIDSourceParameter::kAngle),
m_table(nullptr)
{

}

Gyro::~Gyro()
{

}

float Gyro::GetAngle()
{
	return 0.0;
}

double Gyro::GetRate()
{
	return 0.0;
}

void Gyro::Reset()
{

}

DigitalInput::DigitalInput(unsigned char module, unsigned int channel)
:m_channel(channel),
 m_module(nullptr),
 m_lastValue(false),
 m_table(nullptr)
{

}

DigitalInput::~DigitalInput()
{

}

Talon::Talon(uint8_t moduleNumber, uint32_t channel)
{

}

void Talon::Set(float value, uint8_t syncGroup)
{

}

float Talon::Get()
{
	return 0.0;
}

void Talon::PIDWrite(float output)
{
	LOG_INFO("Talon::PIDWrite(" << output << ") called.")
}

Talon::~Talon()
{

}

Relay::Relay(uint8_t moduleNumber, uint32_t channel, Direction direction)
:m_table(nullptr),
 m_channel(channel),
 m_direction(direction),
 m_module(nullptr)
{

}

Relay::~Relay()
{

}

Joystick::Joystick(unsigned int port)
:m_ds(nullptr),
 m_port(port),
 m_axes(nullptr),
 m_buttons(nullptr)
{

}

Joystick::~Joystick()
{

}

bool Joystick::GetRawButton(uint32_t button)
{
	return false;
}

float Joystick::GetRawAxis(uint32_t axis)
{
	return 0.0;
}
