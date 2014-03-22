#include "stdafx.h"
#include "CalibrationManipulator.h"
#include "IEventListener.h"
#include "CalibrationEvent.h"
#include <iostream>

using namespace cv;
using namespace std;
 
/**
Calibration Manipulator: draws the calibration screen onto the display
*/

CalibrationManipulator::CalibrationManipulator(void)
{
	doCalibration=false;
}

void CalibrationManipulator::handleEvent(BaseEvent* evt)
{
	if(evt->getType() == CALIBRATION_EVENT) 
	{	
		enterTime=clock();
		CalibrationEvent* cEvt = (CalibrationEvent*) evt;
		doCalibration = cEvt->getDoCalibrate();
	}
}

void CalibrationManipulator::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	//cout<< "in calibration manipulator";
	if(doCalibration){
		//cout<<"in calibration";
		clock_t time = clock();
		clock_t diff = time - enterTime;
		float timeSec = (float)diff / (float)CLOCKS_PER_SEC;

		drawCalibrationBox(1, 1, leftImage);
		drawCalibrationBox(1, 1, rightImage);
		//cout<< "Time running:"<< timeSec ;
		putText(leftImage,std::to_string(timeSec),Point(leftImage.size().width*.43,leftImage.size().height*.75),1,1,Scalar(0,0,255),2);
		putText(rightImage,std::to_string(timeSec),Point(rightImage.size().width*.43,rightImage.size().height*.75),1,1,Scalar(0,0,255),2);
	}
}

int CalibrationManipulator::getZDepth()
{
	return 2;  //layer 2
}

//draws the calibration box onto the image img
void CalibrationManipulator::drawCalibrationBox(int x, int y, cv::Mat img)
{
	if(x == -1 || y == -1)
	{
		return;
	}
	//cout << "in draw";
	Size s = img.size();
	
	putText(img,"Calibration",Point(s.width*.42,100),1,1,Scalar(0,0,255),2);
	putText(img,"PLACE YOUR",Point(s.width*.42,s.height*.5),1,1,Scalar(0,0,255),2);
	putText(img,"HAND HERE",Point(s.width*.43,s.height*.6),1,1,Scalar(0,0,255),2);
	
	rectangle(img, Point(s.width*.4, s.height*.3),Point(s.width*.6, s.height*.7),Scalar(0,0,255),2);
}
	
CalibrationManipulator::~CalibrationManipulator(void)
{
}
