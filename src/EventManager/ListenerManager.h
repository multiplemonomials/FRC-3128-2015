/*
 * ListenerManager.h
 *
 *  Created on: Apr 4, 2014
 *      Author: jamie
 */

#ifndef LISTENERMANAGER_H_
#define LISTENERMANAGER_H_

#include <type_traits>
#include <unordered_map>
#include <Joystick.h>
#include <boost/thread.hpp>

#include "Cmd.h"

//This class combines the functions of XControl and ListenerManager.
//It is constructed with one Joystick object, which confusingly seems
//to be the wpilib metaphor for an entire controller.
//It polls the controller at a set interval, and invokes listeners
//whenever a value they're set for has changed.  Listeners are run on
//the object's polling thread, and must take a ListenerManager &
//as an argument, which they can use to get the data they require
//(which is cached in the object).
//You may register the same (shared ptr to a) listener as many
//times as you like, and each handler will only be invoked
//once per polling cycle.

//Enum which represents everything that can be read from the controller
enum Listenable
{
	A = 1,
	B = 2,
	X = 3,
	Y = 4,
	LB = 5,
	RB = 6,
	BACK = 7,
	START = 8,
	L3 = 9,
	R3 = 10,
	JOY1X = 11,
	JOY2X = 12,
	TRIGGERS = 13,
	JOY1Y = 14,
	JOY2Y = 15
};

class ListenerManager
{
	typedef std::unordered_multimap<Listenable, std::shared_ptr<boost::function<void(ListenerManager &)>>, std::hash<int>> ListenerMapType;

	//maps the listeners to the control inputs
	ListenerMapType _listeners;

	//wpilib object which represents a controller
	std::shared_ptr<Joystick> _joystick;

	std::vector<double> _joystickValues;

	std::vector<bool> _buttonValues;

	boost::mutex _mutex;

	//thread which polls the joystick and calls listeners if the values change
	boost::thread _thread;

	//not assignable
	void operator=(ListenerManager) =  delete;

	//not copy constructable
	ListenerManager(ListenerManager&) = delete;

	std::pair<std::vector<bool>, std::vector<double>> pollControls();

public:

	//construct from existing Joystick
	ListenerManager(std::shared_ptr<Joystick> joystick);

	//start thread
	void operator()();

	//add listener for given listenable
	//multiple listeners can be added for the same listenable
	void addListener(Listenable key, ListenerMapType::mapped_type listener);

	//remove all listeners set for the given listener
	void removeAllListeners(Listenable listener);

	//returns the boolean value of a button listenable (between A and R3).
	//Does bounds checking, throws if value is out of range.
	bool getRawBool(Listenable listenable);

	//returns the double value of an axis listenable (between JOY1X and TRIGGERS).
	//Does bounds checking, throws if value is out of range.
	double getRawDouble(Listenable listenable);

	virtual ~ListenerManager();
};

#endif /* LISTENERMANAGER_H_ */
