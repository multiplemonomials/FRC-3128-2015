/*
 * LightChangeEvent.h
 *
 * Just a single function for changing the lights in autonomous
 */

#ifndef LIGHTCHANGEEVENT_H_
#define LIGHTCHANGEEVENT_H_

#include <memory>

#include "RelayLink.h"
#include <Options.h>

//Sets the lights to those of the current Alliance, gotten from Options
void lightChange(std::shared_ptr<RelayLink> redLights, std::shared_ptr<RelayLink> blueLights);

//Sets the lights to the color of the provided Alliance
void lightChange(Options::Alliance alliance, std::shared_ptr<RelayLink> redLights, std::shared_ptr<RelayLink> blueLights);

#endif /* LIGHTCHANGEEVENT_H_ */
