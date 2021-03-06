/*
 * ListenerManager.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#include <EventManager/ListenerManager.h>
#include <LogMacros.h>
#include <Util/Time.h>
#include <FunctionTimer/functimer.h>
#include <Exception.h>

#include <unordered_set>

ListenerManager::ListenerManager(std::shared_ptr<Joystick> joystick)
:_controlValuesMutex(),
 _listeners(),
 _joystick(joystick),
 _joystickValues(16),
 _buttonValues(11),
 _waiter(),
 _thread(&ListenerManager::run, this)
{

}

ListenerManager::~ListenerManager()
{
	_waiter.interrupt();
	while(!_thread.joinable())
	{

	}
	_thread.join();
}

void ListenerManager::addListener(Listenable key, ListenerMapType::mapped_type listener)
{
	std::unique_lock<std::mutex> lock(_listenersMutex);
	_listeners.insert(std::make_pair(key, listener));
}

void ListenerManager::removeAllListeners()
{
	std::unique_lock<std::mutex> lock(_listenersMutex);
	_listeners.clear();
}

void ListenerManager::removeAllListenersForControl(Listenable listener)
{
	std::unique_lock<std::mutex> lock(_listenersMutex);
	_listeners.erase(listener);
}

bool ListenerManager::getRawBool(Listenable listenable)
{
	//check that this listenable is one of the boolean valued ones
	if(listenable >= Listenable::ADOWN && listenable <= Listenable::R3DOWN)
	{
		std::unique_lock<std::mutex>(_controlValuesMutex);
		try
		{
			return _buttonValues.at(listenable);
		}
		catch(std::out_of_range & error)
		{
			LOG_RECOVERABLE("Attempt to read data from ListenerManager whose thread has not finished starting")
		}

		return false;
	}
	//if that came up empty, try the up values of those same controls
	else if(listenable >= Listenable::AUP && listenable <= Listenable::R3UP)
	{
		std::unique_lock<std::mutex>(_controlValuesMutex);
		try
		{
			return _buttonValues.at(listenable - 20);
		}
		catch(std::out_of_range & error)
		{
			LOG_RECOVERABLE("Attempt to read data from ListenerManager whose thread has not finished starting")
		}

		return false;
	}
	else
	{
		THROW_EXCEPTION("Attempt to get boolean value of control listenable " << listenable << " which is not a boolean");

		return false;
	}
}

double ListenerManager::getRawDouble(Listenable listenable)
{
	//check that this listenable is one of the double valued ones
	if(listenable >= Listenable::JOY1X && listenable <= Listenable::JOY2Y)
	{
		std::unique_lock<std::mutex>(_controlValuesMutex);
		try
		{
			return _joystickValues.at(listenable);
		}
		catch(std::out_of_range & error)
		{
			LOG_RECOVERABLE("Attempt to read data from ListenerManager whose thread has not finished starting")

			return 0.0;
		}
	}
	else
	{
		THROW_EXCEPTION("Attempt to get double value of control listenable " << listenable << " which is not a double");
		return 0.0;
	}
}

std::pair<std::vector<bool>, std::vector<double>> ListenerManager::pollControls()
{
	std::vector<bool> buttonValues(11);
	std::vector<double> joystickValues(16);

	std::unique_lock<std::mutex>(_controlValuesMutex);

	//read button values
	for(int counter = Listenable::ADOWN; counter <= Listenable::R3DOWN ; counter++)
	{
		buttonValues[counter] = _joystick->GetRawButton(counter);
	}

	//read joystick values
	for(int counter = Listenable::JOY1X; counter <= Listenable::JOY2Y; counter++)
	{
		joystickValues[counter] = _joystick->GetRawAxis(counter - 10);
	}

	return std::make_pair(buttonValues, joystickValues);
}

void ListenerManager::run()
{
	while(true)
	{
		auto newValues = pollControls();

		//test if values have changed
		if((newValues.first != _buttonValues) || (newValues.second != _joystickValues))
		{
			std::unordered_set<ListenerMapType::mapped_type> listenersToInvoke;

			//loop through button values
			for(int counter = Listenable::ADOWN; counter <= Listenable::R3DOWN ; counter++)
			{
				//has this button been pressed?
				if((!_buttonValues[counter]) && (newValues.first[counter]))
				{
					//get all its registered listeners
					std::pair<ListenerMapType::const_iterator, ListenerMapType::const_iterator> found_listeners = _listeners.equal_range((Listenable)(counter));

					if(found_listeners.first != found_listeners.second)
					{
						//loop through them
						for(ListenerMapType::const_iterator iterator = found_listeners.first; iterator != found_listeners.second; iterator++)
						{
							listenersToInvoke.insert((*iterator).second);
						}
					}

				}
				//has this button just stopped being pressed?
				if((!_buttonValues[counter]) && (newValues.first[counter]))
				{
					//get all its registered listeners
					//increment counter by 20 to get button up listeners
					std::pair<ListenerMapType::const_iterator, ListenerMapType::const_iterator> found_listeners = _listeners.equal_range((Listenable)(counter + 20));

					if(found_listeners.first != found_listeners.second)
					{
						//loop through them
						for(ListenerMapType::const_iterator iterator = found_listeners.first; iterator != found_listeners.second; iterator++)
						{
							listenersToInvoke.insert((*iterator).second);
						}
					}

				}
			}

			//loop through joystick values
			for(int counter = Listenable::JOY1X; counter <= Listenable::JOY2Y ; counter++)
			{
				//has this particular value changed?
				if(_joystickValues[counter] != newValues.second[counter])
				{
					//get all its registered listeners
					std::pair<ListenerMapType::const_iterator, ListenerMapType::const_iterator> found_listeners = _listeners.equal_range((Listenable)(counter));

					if(found_listeners.first != found_listeners.second)
					{
						//loop through them
						for(ListenerMapType::const_iterator iterator = found_listeners.first; iterator != found_listeners.second; iterator++)
						{
							listenersToInvoke.insert((*iterator).second);
						}
					}

				}
			}


			//update class variables to match new data
			{
				std::unique_lock<std::mutex>(_controlValuesMutex);
				_buttonValues = newValues.first;
				_joystickValues = newValues.second;
			}

			//invoke handlers
			for(std::unordered_set<ListenerMapType::mapped_type>::value_type listener: listenersToInvoke)
			{
				try
				{
					(*listener)();
				}
				catch(RoboException & error)
				{
					LOG_RECOVERABLE("Caught a RoboException from a control listener: \n" << GET_ROBOEXCEPTION_MESSAGE(error));
				}
				catch(...)
				{
					LOG_RECOVERABLE("Caught... something from a control listener")
				}
			}
		}

		//TODO configuration setting for time
		try
		{
			_waiter.wait(std::chrono::milliseconds(50));
		}
		catch(ThreadInterruptedException & error)
		{
			return;
		}
	}
}

