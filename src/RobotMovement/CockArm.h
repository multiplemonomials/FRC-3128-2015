/*
 * CockArm.h
 *
 * Class to represent the shooter arm
 */

#ifndef COCKARM_H_
#define COCKARM_H_
#include <DigitalInput.h>
#include <mutex>

#include <HardwareLink/Motor/MotorLink.h>
#include <EventManager/CmdTimerPosix.h>

class CockArm
{

	volatile bool _warned = false;
    volatile bool _cockArmActive = true;

    std::shared_ptr<DigitalInput> _shooterTSensor;

    std::shared_ptr<MotorLink> _mShooter;

    std::mutex _mutex;

    CmdTimerPosix _timer;

	void execute();

public:
	CockArm(std::shared_ptr<DigitalInput> shooterTSensor, std::shared_ptr<MotorLink> mShooter);

	virtual ~CockArm();

	void stopArmCock();

	void startArmCock();

	//Stops the internal arm-control loop if it's running
	void cancel();

	//Starts the internal arm-control loop if it's not already running
	void start();
};

#endif /* COCKARM_H_ */
