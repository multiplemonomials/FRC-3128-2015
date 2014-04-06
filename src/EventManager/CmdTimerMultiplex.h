/*
 * CmdTimerMultiplex.h
 *
 *  Created on: Mar 16, 2014
 *      Author: jamie
 */

#ifndef CMDTIMERMULTIPLEX_H_
#define CMDTIMERMULTIPLEX_H_

#include "CmdProcessor.h"
#include "Cmd.h"
#include "CmdTimer.h"
#include <Util/Time.h>

/*
 * Each CmdTimer holds only one cmd, so this object
 * owns a horde of them and destroys them after they fire.
 */
class CmdTimerMultiplex
{
public:

	typedef std::map<CmdTimerImpl *, std::shared_ptr<CmdTimer>> CmdTimerMapType;

private:

	CmdTimerMapType _timerMap;

	boost::mutex _mutex;

public:

	//create relative timer
	void startTimer(Time::Duration time, Cmd::SharedPtr command);

	//create absolute timer
	void startTimer(Time::Timepoint time, Cmd::SharedPtr command);

	//called by timers when they go off
	void onTimerFire(CmdTimerImpl * timerPtr, Cmd::SharedPtr cmdToInvoke);

	CmdTimerMultiplex();

	virtual ~CmdTimerMultiplex();
};

#endif /* CMDTIMERMULTIPLEX_H_ */
