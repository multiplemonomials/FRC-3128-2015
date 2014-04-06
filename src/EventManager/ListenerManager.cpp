/*
 * ListenerManager.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#include <EventManager/ListenerManager.h>
#include <LogMacros.h>
#include <Util/Time.h>
#include <FunctionTimer/functimer.hpp>
#include <Exception.h>

#include <unordered_set>

ListenerManager::ListenerManager(std::shared_ptr<Joystick> joystick)
:_listeners(),
 _joystick(joystick),
 _joystickValues(),
 _buttonValues(),
 _mutex(),
 _thread(boost::ref(*this))
{


}

ListenerManager::~ListenerManager()
{
	_thread.interrupt();
}

void ListenerManager::addListener(Listenable key, ListenerMapType::mapped_type listener)
{
	boost::unique_lock<boost::mutex>(_mutex);
	_listeners.insert(std::make_pair(key, listener));
}

bool ListenerManager::getRawBool(Listenable listenable)
{
	//check that this listenable is one of the boolean valued ones
	if(listenable >= Listenable::A && listenable <= Listenable::R3)
	{
		boost::unique_lock<boost::mutex>(_mutex);
		try
		{
			return _buttonValues.at(listenable);
		}
		catch(std::out_of_range & error)
		{
			LOG_RECOVERABLE("Attempt to read data from ListenerManager whose thread has not finished starting")

			return false;
		}
	}
	else
	{
		THROW_EXCEPTION("Attempt to get boolean value of control listenable " << listenable << " which is not a boolean");
	}
}

double ListenerManager::getRawDouble(Listenable listenable)
{
	//check that this listenable is one of the double valued ones
	if(listenable >= Listenable::JOY1X && listenable <= Listenable::JOY2Y)
	{
		boost::unique_lock<boost::mutex>(_mutex);
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
	}
}

std::pair<std::vector<bool>, std::vector<double>> ListenerManager::pollControls()
{
	std::vector<bool> buttonValues;
	std::vector<double> joystickValues;

	boost::unique_lock<boost::mutex>(_mutex);

	//read button values
	for(int counter = Listenable::A; counter <= Listenable::R3 ; counter++)
	{
		buttonValues[counter] = _joystick->GetRawButton(counter);
	}

	//read joystick values
	for(int counter = Listenable::JOY1X; counter <= Listenable::JOY2Y; counter++)
	{
		joystickValues[counter] = _joystick->GetRawAxis(counter);
	}

	return std::make_pair(buttonValues, joystickValues);
}

void ListenerManager::operator()()
{
	LOG_DEBUG("ListenerManager Thread Starting");

	while(true)
	{
		FUNC_TIMER;

		auto newValues = pollControls();

		//test if values have changed
		if((newValues.first != _buttonValues) || (newValues.second != _joystickValues))
		{
			std::unordered_set<ListenerMapType::mapped_type> listenersToInvoke;

			//loop through button values
			for(int counter = Listenable::A; counter <= Listenable::R3 ; counter++)
			{
				//has this particular value changed?
				if(_buttonValues[counter] != newValues.first[counter])
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
				boost::unique_lock<boost::mutex>(_mutex);
				_buttonValues = newValues.first;
				_joystickValues = newValues.second;
			}

			//invoke handlers
			for(std::unordered_set<ListenerMapType::mapped_type>::value_type listener: listenersToInvoke)
			{
				try
				{
					(*listener)(*this);
				}
				catch(RoboException & error)
				{
					LOG_RECOVERABLE("Caught a RoboException from a control listener: \n" << error.what());
				}
				catch(...)
				{
					LOG_RECOVERABLE("Caught... something from a control listener")
				}
			}
		}

		try
		{
			//TODO configuration setting for time
			Time::Sleep(Time::Milliseconds(50));
		}
		catch(boost::thread_interrupted & interrupt)
		{
			return;
		}
	}
}

