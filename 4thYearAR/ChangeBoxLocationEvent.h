/*
 * ChangeBoxLocationEvent.h
 * Header file for ChangeBoxLocationEvent
 * Defines an event to change the location of a DrawBoxManipulator's box.
 */

#pragma once

#include "stdafx.h"

#include "BaseEvent.h"

/*
 * An event to change the location of a draw box manipulator.
 */
class ChangeBoxLocationEvent : public BaseEvent
{
private:
	int x, y;
public:
	//NOTE: The constructor hard-codes the event type so that we don't need to provide it
	//each time we make a new event
	ChangeBoxLocationEvent(int newX, int newY) : BaseEvent(1) { x = newX; y = newY; }
	int getX() { return x; }
	int getY() { return y; }
};