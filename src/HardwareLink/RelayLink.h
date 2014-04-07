/*
 * RelayLink.h
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#ifndef RELAYLINK_H_
#define RELAYLINK_H_

#include <Relay.h>

class RelayLink
{
    std::shared_ptr<Relay> _rel;

public:
    /**
     * Instantiates a new relay.
     *
     * @param rel the relay to be linked
     */
    RelayLink(std::shared_ptr<Relay> rel)
	{
    	_rel = rel;
	}

    virtual ~RelayLink()
    {

    }

    /**
     * Sets the relay on.
     */
    void setOn()
    {
    	_rel->Set(Relay::Value::kForward);
    }

    /**
     * Sets the relay off.
     */
    void setOff()
    {
    	_rel->Set(Relay::Value::kReverse);
    }
};

#endif /* RELAYLINK_H_ */
