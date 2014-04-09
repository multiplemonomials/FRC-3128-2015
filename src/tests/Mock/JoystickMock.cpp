/*
 * JoystickMock.cpp
 *
 *  Created on: Apr 8, 2014
 *      Author: jamie
 */

#include <tests/Mock/JoystickMock.h>
#include <Exception.h>

JoystickMock::JoystickMock()
:Joystick(1),
 _buttons(11),
 _joysticks(16)
{

}

JoystickMock::~JoystickMock()
{

}

void JoystickMock::setJoystick(int joystickNumber, float value)
{
	boost::unique_lock<boost::mutex>(_mutex);
	if((joystickNumber >= 1) && (joystickNumber <= 5))
	{
		_joysticks[joystickNumber] = value;
	}
	else
	{
		THROW_EXCEPTION("setJoystick called with invalid joystick number " << joystickNumber);
	}
}

void JoystickMock::setButton(int buttonNumber, bool value)
{
	boost::unique_lock<boost::mutex>(_mutex);
	if((buttonNumber >= 1) && (buttonNumber <= 10))
	{
		_buttons[buttonNumber] = value;
	}
	else
	{
		THROW_EXCEPTION("setJoystick called with invalid button number " << buttonNumber);
	}
}

float JoystickMock::GetRawAxis(uint32_t axis)
{
	boost::unique_lock<boost::mutex>(_mutex);

	//don't check validity on purpose.
	//nothing that accesses a joystick should let invalid data get this far
	return _joysticks[axis];
}

bool JoystickMock::GetRawButton(uint32_t button)
{
	boost::unique_lock<boost::mutex>(_mutex);
	return _buttons[button];
}
