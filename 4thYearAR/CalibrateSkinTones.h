#ifndef CALIBRATE_SKINTONES
#define CALIBRATE_SKINTONES

#pragma once
#include "idetector.h"
#include "IManipulator.h"
#include "EventManager.h"
#include <opencv\cv.h>

#define MAX_HUE 100
#define VALUE_MIN 25

using namespace cv; 

class CalibrateSkinTones:
	public IDetector, public IEventListener
{
public:
	
	CalibrateSkinTones();
	~CalibrateSkinTones(void);
	void detect(cv::Mat leftImage, cv::Mat rightImage);
	void handleEvent(BaseEvent* evt);
	/*
	void manipulate(cv::Mat leftImage, cv::Mat rightImage);
	int CalibrateSkinTones::getZDepth();
	*/
	
private:
	struct histRange
	{
		int hue_min, hue_max;
		int sat_min, sat_max;
		int val_min, val_max;
	};
	clock_t enterTime;
	bool doCalibration;
	//void drawCalibrateRectangle();
	histRange makeHistogram(Mat &img_hsv);
	void drawCalibrationBox(int x, int y, cv::Mat img);

};
#endif

