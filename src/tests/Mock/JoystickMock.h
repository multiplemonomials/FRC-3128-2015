/*
 * JoystickMock.h
 *
 *  Created on: Apr 8, 2014
 *      Author: jamie
 */

#ifndef JOYSTICKMOCK_H_
#define JOYSTICKMOCK_H_

#include <Joystick.h>

#include <vector>
#include <boost/thread/mutex.hpp>

//Class that looks like a joystick, but allows
//unit test code to control what it returns

//NOTE: this class requires a modification to the WPIlib Joystick.h.  GetRawAxis and GetRawButton must be made virtual.

class JoystickMock: public Joystick
{
	std::vector<bool> _buttons;

	std::vector<float> _joysticks;

	boost::mutex _mutex;

public:

	JoystickMock();

	void setJoystick(int joystickNumber, float value);

	void setButton(int buttonNumber, bool value);

	virtual float GetRawAxis(uint32_t axis);

	virtual	bool GetRawButton(uint32_t button);

	virtual ~JoystickMock();
};

#endif /* JOYSTICKMOCK_H_ */
