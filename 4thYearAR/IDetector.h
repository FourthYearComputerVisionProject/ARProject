/*
 * Detector.h
 * Header file containing the interface for a detector.
 * A detector performs some operation on an image to detect something in that
 * image. It does not modify the image in any way.
 */

#pragma once

#include "stdafx.h"

/*
 * The detector interface.
 */
class IDetector {
public:
	/*
	 * Perform the detector's logic. This is called once per frame.
	 * Argument image: The image to perform detection on.
	 */
	virtual void detect(cv::Mat leftImage, cv::Mat rightImage) = 0;
};