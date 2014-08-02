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

//master switch for the use of libjoystick to get real joystick values from usb
#undef LIBJOYSTICK_ENABLED

//port to connect to joystick on
#define LIBJOYSTICK_PORT "/dev/input/js2"

#ifdef LIBJOYSTICK_ENABLED

#include <libjoystick/Joystick.h>

#endif

uint32_t DigitalInput::Get()
{
	LOG_DEBUG("DigitalInput::Get called.")
	return 0;
}

SensorBase::SensorBase()
{
	LOG_DEBUG("SensorBase::SensorBase() called.")
}

AnalogChannel::AnalogChannel(uint8_t moduleNumber, uint32_t channel)
:m_channel(channel),
 m_module(nullptr),
 m_accumulator(nullptr),
 m_accumulatorOffset(0),
 m_shouldUseVoltageForPID(false),
 m_table(nullptr)
{
	LOG_DEBUG("AnalogChannel::AnalogChannel(" << (short)(moduleNumber) << ", " << channel << ") called.")
}

float AnalogChannel::GetVoltage()
{
	//LOG_DEBUG("AnalogChannel::GetVoltage() called.");
	return 0.0;
}

int32_t Encoder::GetRaw()
{
	LOG_DEBUG("Encoder::GetRaw() called.")
	return 0;
}

void Relay::Set(Value value)
{
	LOG_DEBUG("Relay::Set(" << value << ") called.")
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
	LOG_DEBUG("Gyro::Gyro(" << (short)(moduleNumber) << ", " << channel << ") called.")
}

Gyro::~Gyro()
{

}

float Gyro::GetAngle()
{
	LOG_DEBUG("Gyro::GetAngle() called.")
	return 0.0;
}

double Gyro::GetRate()
{
	LOG_DEBUG("Gyro::GetRate() called.")
	return 0.0;
}

void Gyro::Reset()
{
	LOG_DEBUG("Gyro::Reset() called.")
}

DigitalInput::DigitalInput(unsigned char module, unsigned int channel)
:m_channel(channel),
 m_module(nullptr),
 m_lastValue(false),
 m_table(nullptr)
{
	LOG_DEBUG("DigitalInput::DigitalInput(" << (short)(module) << ", " << channel << ") called.")
}

DigitalInput::~DigitalInput()
{

}

Talon::Talon(uint8_t moduleNumber, uint32_t channel)
{
	LOG_DEBUG("Talon::Talon(" << (short)moduleNumber << ", " << channel << ") called.")
}

void Talon::Set(float value, uint8_t syncGroup)
{
	LOG_DEBUG("Talon::Set(" << value << ") called.")
}

float Talon::Get()
{
	LOG_DEBUG("Talon::Get() called.")
	return 0.0;
}

void Talon::PIDWrite(float output)
{
	LOG_DEBUG("Talon::PIDWrite(" << output << ") called.")
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
	LOG_DEBUG("Relay::Relay(" << (short)moduleNumber << ", " << channel << ", " << direction << ") called.")
}

Relay::~Relay()
{

}

#ifdef LIBJOYSTICK_ENABLED

static joy::Joystick testingJoystick(std::string("/dev/input/js2"));

#endif

Joystick::Joystick(unsigned int port)
:m_ds(nullptr),
 m_port(port),
 m_axes(nullptr),
 m_buttons(nullptr)
{
	LOG_DEBUG("Joystick::Joystick(" << port << ") called.")
}

Joystick::~Joystick()
{

}

bool Joystick::GetRawButton(uint32_t button)
{
#ifdef LIBJOYSTICK_ENABLED

	bool rawValue =  testingJoystick.getButtonValue(button - 1);

	LOG_DEBUG("Joystick::GetRawButton(" << button << ") called, returning " << std::boolalpha << rawValue)

	return rawValue;

#else
	LOG_DEBUG("Joystick::GetRawButton(" << button << ") called.")
	return false;
#endif
}

float Joystick::GetRawAxis(uint32_t axis)
{
#ifdef LIBJOYSTICK_ENABLED

	float rawValue = (testingJoystick.getAxisValue(axis - 1) / 32768.0);

	LOG_DEBUG("Joystick::GetRawAxis(" << axis << ") called, returning " << rawValue)

	return rawValue;

#else
	LOG_DEBUG("Joystick::GetRawAxis(" << axis << ") called.")
	return 0.0;
#endif
}
