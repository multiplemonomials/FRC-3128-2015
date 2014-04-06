/*
 * Global.h
 *
 *  Created on: Mar 25, 2014
 *      Author: jamie
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <wpilib/WPILib.h>

class Global
{
	Global();

public:

	//not assignable
	Global operator=(Global) = delete;

	//not copyable
	Global(Global&) = delete;

	static void initializeRobot();

	static void initializeDisabled();

	static void initializeAuto();

	static void initializeTeleop();

	static void robotStop();

	virtual ~Global();
};

#endif /* GLOBAL_H_ */
