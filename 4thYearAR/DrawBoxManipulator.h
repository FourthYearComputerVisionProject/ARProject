/*
 * DrawBoxManipulator.h
 * Header file for DrawBoxManipulator.cpp
 * Draws a box and moves it around to different positions upon receiving events.
 */

#pragma once

#include "stdafx.h"

#include "IManipulator.h"
#include "IEventListener.h"
#include "EventManager.h"

/*
 * A manipulator that draws a box.
 */
class DrawBoxManipulator : public IManipulator, public IEventListener
{
private:
	/*
	 * The X position of the box.
	 */
	int x;
	/*
	 * The Y position of the box.
	 */
	int y;
public:
	DrawBoxManipulator();
	void handleEvent(BaseEvent* evt);
	int getZDepth() { return 0; }
	void manipulate(cv::Mat leftImage, cv::Mat rightImage);
};