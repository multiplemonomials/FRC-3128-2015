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

#include "CmdProcessor.h"
#include "Cmd.h"
#include "CmdTimerPosix.h"

//This class combines the functions of XControl and ListenerManager.
//It is constructed with one Joystick object, which confusingly seems
//to be the wpilib metaphor for an entire controller.
//It polls the controller at a set interval, and invokes listeners
//whenever a value they're set for has changed.  Listeners are run on
//the object's polling thread, and will need to be passed a reference
//to the listener manager somehow if the need control data
//You may register the same (shared ptr to a) listener as many
//times as you like, and each handler will only be invoked
//once per polling cycle.

//Enum which represents everything that can be read from the controller
enum Listenable
{
	ADOWN = 1,
	BDOWN = 2,
	XDOWN = 3,
	YDOWN = 4,
	LBDOWN = 5,
	RBDOWN = 6,
	BACKDOWN = 7,
	STARTDOWN = 8,
	L3DOWN = 9,
	R3DOWN = 10,
	JOY1X = 11,
	JOY2X = 12,
	TRIGGERS = 13,
	JOY1Y = 14,
	JOY2Y = 15,
	AUP = 21,
	BUP = 22,
	XUP = 23,
	YUP = 24,
	LBUP = 25,
	RBUP = 26,
	BACKUP = 27,
	STARTUP = 28,
	L3UP = 29,
	R3UP = 30,
};

class ListenerManager
{
public:

	typedef std::shared_ptr<boost::function<void()>> listenerFunctionType;

private:
	typedef std::unordered_multimap<Listenable, listenerFunctionType, std::hash<int>> ListenerMapType;

	//maps the listeners to the control inputs
	ListenerMapType _listeners;

	//wpilib object which represents a controller
	std::shared_ptr<Joystick> _joystick;

	std::vector<double> _joystickValues;

	std::vector<bool> _buttonValues;

	//serializes multithreaded input
	CmdProcessor _cmdProcessor;

	//timer for thread which polls the joystick and calls listeners if the values change
	CmdTimerPosix _threadTimer;

	//not assignable
	void operator=(ListenerManager) =  delete;

	//not copy constructible
	ListenerManager(ListenerManager&) = delete;

	std::pair<std::vector<bool>, std::vector<double>> pollControls();

	//run polling function via _cmdProcessor
	void operator()()
	{
		_cmdProcessor.EnqueueFront(&ListenerManager::operatorParenthesisImpl, boost::ref(*this));
	}

	//...
	void operatorParenthesisImpl();
	//Come on, don't tell me you didn't laugh a little inside

	void addListenerImpl(Listenable key, ListenerMapType::mapped_type listener);

	void removeAllListenersForControlImpl(Listenable listener);

	void removeAllListenersImpl();

public:

	//construct from existing Joystick
	ListenerManager(std::shared_ptr<Joystick> joystick);

	//add listener for given listenable
	//multiple listeners can be added for the same listenable
	void addListener(Listenable key, ListenerMapType::mapped_type listener)
	{
		_cmdProcessor.Enqueue(&ListenerManager::addListenerImpl, boost::ref(*this), key, listener);
	}

	//remove all listeners set for the given listener
	void removeAllListenersForControl(Listenable listener)
	{
		_cmdProcessor.Enqueue(&ListenerManager::removeAllListenersForControlImpl, boost::ref(*this), listener);
	}

	//remove all listeners, period
	void removeAllListeners()
	{
		_cmdProcessor.Enqueue(&ListenerManager::removeAllListenersImpl, boost::ref(*this));
	}

	//returns the boolean value of a button listenable (between A and R3).
	//Does bounds checking, throws if value is out of range.
	bool getRawBool(Listenable listenable);

	//returns the double value of an axis listenable (between JOY1X and TRIGGERS).
	//Does bounds checking, throws if value is out of range.
	double getRawDouble(Listenable listenable);

	virtual ~ListenerManager();
};

#endif /* LISTENERMANAGER_H_ */
