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
	LOG_INFO("DigitalInput::Get called.")
	return 0;
}

SensorBase::SensorBase()
{
	LOG_INFO("SensorBase::SensorBase() called.")
}

AnalogChannel::AnalogChannel(uint8_t moduleNumber, uint32_t channel)
:m_channel(channel),
 m_module(nullptr),
 m_accumulator(nullptr),
 m_accumulatorOffset(0),
 m_shouldUseVoltageForPID(false),
 m_table(nullptr)
{
	LOG_INFO("AnalogChannel::AnalogChannel(" << (short)(moduleNumber) << ", " << channel << ") called.")
}

float AnalogChannel::GetVoltage()
{
	LOG_INFO("AnalogChannel::GetVoltage() called.");
	return 0.0;
}

int32_t Encoder::GetRaw()
{
	LOG_INFO("Encoder::GetRaw() called.")
	return 0;
}

void Relay::Set(Value value)
{
	LOG_INFO("Relay::Set(" << value << ") called.")
}

Gyro::Gyro(uint8_t moduleNumber, uint32_t channel)
:m_analog(nullptr),
m_voltsPerDegreePerSecond(0),
m_offset(0),
m_channelAllocated(true),
m_center(0),
m_pidSource(PIDSource::PIDSourceParameter::kAngle),
m_table(nullptr)
{
	LOG_INFO("Gyro::Gyro(" << (short)(moduleNumber) << ", " << channel << ") called.")
}

Gyro::~Gyro()
{

}

float Gyro::GetAngle()
{
	LOG_INFO("Gyro::GetAngle() called.")
	return 0.0;
}

double Gyro::GetRate()
{
	LOG_INFO("Gyro::GetRate() called.")
	return 0.0;
}

void Gyro::Reset()
{
	LOG_INFO("Gyro::Reset() called.")
}

DigitalInput::DigitalInput(unsigned char module, unsigned int channel)
:m_channel(channel),
 m_module(nullptr),
 m_lastValue(false),
 m_table(nullptr)
{
	LOG_INFO("DigitalInput::DigitalInput(" << (short)(module) << ", " << channel << ") called.")
}

DigitalInput::~DigitalInput()
{

}

Talon::Talon(uint8_t moduleNumber, uint32_t channel)
{
	LOG_INFO("Talon::Talon(" << (short)moduleNumber << ", " << channel << ") called.")
}

void Talon::Set(float value, uint8_t syncGroup)
{
	LOG_INFO("Talon::Set(" << value << ") called.")
}

float Talon::Get()
{
	LOG_INFO("Talon::Get() called.")
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
	LOG_INFO("Relay::Relay(" << (short)moduleNumber << ", " << channel << ", " << direction << ") called.")
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
	LOG_INFO("Relay::Relay(" << port << ") called.")
}

Joystick::~Joystick()
{

}

bool Joystick::GetRawButton(uint32_t button)
{
	LOG_INFO("Joystick::GetRawButton(" << button << ") called.")
	return false;
}

float Joystick::GetRawAxis(uint32_t axis)
{
	LOG_INFO("Joystick::GetRawAxis(" << axis << ") called.")
	return 0.0;
}
