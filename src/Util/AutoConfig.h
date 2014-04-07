/*
 * AutoConfig.h
 *
 * This class's initialize method is called by Global in order to configure the robot for automatic operation.
 */

#ifndef AUTOCONFIG_H_
#define AUTOCONFIG_H_

#include <LogMacros.h>
#include <Global.h>

class AutoConfig
{
	AutoConfig();

	static CmdProcessor _cmdProcessor;
public:

	static void initialize(Global & global);

	virtual ~AutoConfig();
};

#endif /* AUTOCONFIG_H_ */
