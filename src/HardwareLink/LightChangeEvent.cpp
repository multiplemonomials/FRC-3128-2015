/*
 * LightChangeEvent.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: jamie
 */

#include "LightChangeEvent.h"
#include <Options.h>

void lightChange(Options::Alliance alliance, std::shared_ptr<RelayLink> redLights, std::shared_ptr<RelayLink> blueLights)
{
    if(Options::instance()._alliance == Options::Alliance::BLUE)
    {
    	redLights->setOff(); blueLights->setOn();
    }
    else
    {
    	redLights->setOn(); blueLights->setOff();
    }
}

void lightChange(std::shared_ptr<RelayLink> redLights, std::shared_ptr<RelayLink> blueLights)
{
	lightChange(Options::instance()._alliance, redLights, blueLights);
}


