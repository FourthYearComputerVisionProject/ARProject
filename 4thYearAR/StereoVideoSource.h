/*
 * StereoVideSource
 * A stereo source coming from a video.
 */

#pragma once

#include "stdafx.h"

#include <list>

#include "IStereoSource.h"

class StereoVideoSource : public ISteroSource
{
private:
	std::list<IDetector*> detectors;
	cv::Mat image;
	cv::VideoCapture capture;
public:
	StereoVideoSource(std::string file);
	bool addDetector(IDetector* detector);
	bool removeDetector(IDetector* detector);
	void update();
	cv::Mat getLeftImage() { return image; }
	cv::Mat getRightImage() { return image; }
};