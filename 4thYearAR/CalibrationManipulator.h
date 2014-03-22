#ifndef CALIBRATION_MANIPULATOR_H
#define CALIBRATION_MANIPULATOR_H

#pragma once

#include "IManipulator.h"
#include "IEventListener.h"
#include "EventManager.h"

class CalibrationManipulator:public IManipulator, public IEventListener
{
private:
	bool doCalibration;
	clock_t enterTime;
	void CalibrationManipulator::drawCalibrationBox(int x, int y, cv::Mat img);
public:
	CalibrationManipulator(void);
	~CalibrationManipulator(void);
	void CalibrationManipulator::handleEvent(BaseEvent* evt);
	void CalibrationManipulator::manipulate(cv::Mat leftImage, cv::Mat rightImage);
	int CalibrationManipulator::getZDepth();
};
#endif
