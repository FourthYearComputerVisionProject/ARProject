/*
 * StereoCapture.cpp
 * Functions for a stereo capture source.
 */

#include "stdafx.h"

#include "StereoCapture.h"

StereoCapture::StereoCapture(int leftDevice, int rightDevice)
{
	if(!leftCapture.open(leftDevice) || !rightCapture.open(rightDevice))
	{
		throw ERROR;
	}
	leftCapture.set(CV_CAP_PROP_FPS, 60.0);
	rightCapture.set(CV_CAP_PROP_FPS, 60.0);
}

bool StereoCapture::addDetector(IDetector* detector)
{
	for(auto it = detectorList.begin(); it != detectorList.end(); ++it)
	{
		if(detector == (*it))
		{
			return false;
		}
	}
	detectorList.push_back(detector);
	return true;
}

bool StereoCapture::removeDetector(IDetector* detector)
{
	for(auto it = detectorList.begin(); it != detectorList.end(); ++it)
	{
		if(detector == (*it))
		{
			detectorList.erase(it);
			return true;
		}
	}
	return false;
}

void StereoCapture::update()
{
	 leftCapture >> leftImage;
	 rightCapture >> rightImage;
	for(auto it = detectorList.begin(); it != detectorList.end(); ++it)
	{
		(*it)->detect(leftImage, rightImage);
	}
}