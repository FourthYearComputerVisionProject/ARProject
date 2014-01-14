/*
 * Manipulator.h
 * Header containing the interface for manipulators.
 * Manipulators manipulate (that is, make changes to) the current image that
  * will be drawn to the screen. They do not perform detection of any kind.
 */

#pragma once

#include "stdafx.h"

/*
 * The manipulator interface.
 */
class IManipulator {
public:
	/*
	 * Get the depth of this manipulator in the scene.
	 * This is used to decided the order in which manipulators draw to the screen image.
	 */
	virtual int getZDepth() = 0;
	/*
	 * This method is called once per frame, allowing this manipulator to draw something to the screen.
	 * Argument image: The image the manipulator is allowed to draw to.
	 */
	virtual void manipulate(cv::Mat leftImage, cv::Mat rightImage) = 0;
};