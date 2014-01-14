/*
 * StereoCapture.h
 * Header file for SteroCapture.cpp
 * Class definition for a capture from two cameras.
 */

#pragma once

#include "stdafx.h"
#include <list>

#include "IStereoSource.h"

/*
 * Class definition of a stero capture.
 * A stereo capture is a stereo image source that gets data
 * from cameras of some sort.
 */
class StereoCapture : public ISteroSource {
private:
	/*
	 * The list of detectors who are detecting on this
	 * source.
	 */
	std::list<IDetector*> detectorList;
	/*
	 * The left image of the capture.
	 */
	cv::Mat leftImage;
	/*
	 * The right image of the capture.
	 */
	cv::Mat rightImage;
	/*
	 * The left capture device.
	 */
	cv::VideoCapture leftCapture;
	/*
	 * The right capture device.
	 */
	cv::VideoCapture rightCapture;
public:
	/*
	 * Constructor.
	 * Argument leftDevice: The ID of the left capture device.
	 * Argument rightDevice: The ID of the right capture device.
	 */
	StereoCapture(int leftDevice, int rightDevice);
	/*
	 * Add a detector to this stereo image source.
	 * Argument detector: The detector to add.
	 */
	bool addDetector(IDetector* detector);
	/*
	 * Remove a detector from this stereo image source.
	 * Argument detector: The detector to remove.
	 */
	bool removeDetector(IDetector* detector);
	/*
	 * Capture a new frame from the input devices.
	 */
	void update();
	/*
	 * Get the left capture image.
	 */
	cv::Mat getLeftImage() { return leftImage; }
	/*
	 * Get the right capture image.
	 */
	cv::Mat getRightImage() { return rightImage; }
};