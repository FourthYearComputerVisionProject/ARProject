/*
 * IStereoSource.h
 * Header file defining the interface for a stereo image source.
 * A stereo source is a source of stereo images.
 */

#pragma once

#include "stdafx.h"

#include "IDetector.h"

/*
 * A source of stereo images
 */
class ISteroSource {
public:
	/*
	 * Add a detector to this stereo image source.
	 * Argument detector: The detector to add.
	 */
	virtual bool addDetector(IDetector* detector) = 0;
	/*
	 * Remove a detector from this stereo image source.
	 * Argument detector: The detector to remove.
	 */
	virtual bool removeDetector(IDetector* detector) = 0;
	/*
	 * Update the source, if needed.
	 */
	virtual void update() = 0;
	/*
	 * Get the left image of the capture.
	 */
	virtual cv::Mat getLeftImage() = 0;
	/*
	 * Get the right image of the capture.
	 */
	virtual cv::Mat getRightImage() = 0;
};