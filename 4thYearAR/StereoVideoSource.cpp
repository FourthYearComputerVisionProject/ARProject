#include "stdafx.h"

#include "StereoVideoSource.h"

StereoVideoSource::StereoVideoSource(std::string file)
{
	capture.open(file);
	capture.set(CV_CAP_PROP_FPS, DEFAULT_FRAMERATE);
}

bool StereoVideoSource::addDetector(IDetector* detector)
{
	for(auto it = detectors.begin(); it != detectors.end(); ++it)
	{
		if((*it) == detector)
		{
			return false;
		}
	}
	detectors.push_back(detector);
	return true;
}

bool StereoVideoSource::removeDetector(IDetector* detector)
{
	for(auto it = detectors.begin(); it != detectors.end(); ++it)
	{
		if((*it) == detector)
		{
			detectors.erase(it);
			return true;
		}
	}
	return false;
}

void StereoVideoSource::update()
{
	capture >> image;
	for(auto it = detectors.begin(); it != detectors.end(); ++it)
	{
		(*it)->detect(image, image);
	}
}