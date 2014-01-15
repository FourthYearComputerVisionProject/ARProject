/*
 * HUDManipulator.h
 * Header file for HUDManipulator.cpp
 * Draws a box and moves it around to different positions upon receiving events.
 */

#pragma once

#include "stdafx.h"
#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "IManipulator.h"
#include "IEventListener.h"
#include "EventManager.h"

/*
 * A manipulator that draws a box.
 */
class HUDManipulator : public IManipulator, public IEventListener
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

	GLuint batteryTexture;
public:
	HUDManipulator();
	~HUDManipulator();
	void handleEvent(BaseEvent* evt);
	int getZDepth() { return 0; }
	void manipulate(cv::Mat leftImage, cv::Mat rightImage);
	void drawClock(cv::Mat leftImage, cv::Mat rightImage);
	std::stringstream getTime();
	void drawImage(GLuint texture);
};