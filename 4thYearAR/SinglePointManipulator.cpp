#include "stdafx.h"
#include "SinglePointManipulator.h"
#include "SinglePointEvent.h"
#include "IEventListener.h"
#include <iostream>

using namespace cv;
using namespace std;

SinglePointManipulator::SinglePointManipulator()
{
	
}


SinglePointManipulator::~SinglePointManipulator(void)
{
}

void SinglePointManipulator::handleEvent(BaseEvent* evt)
{
	//cout<< "in manipulator handlEvent";
	if(evt->getType() == 3)
	{
		SinglePointEvent* spEvt = (SinglePointEvent*) evt;
		xR = spEvt->getXR();
		yR = spEvt->getYR();

		xL = spEvt->getXL();
		yL = spEvt->getYL();
	}
}

void SinglePointManipulator::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	//cout<< "in manipulator";
	drawClickIcon(xL, yL, leftImage);
	drawClickIcon(xR, yR, rightImage);
	
}

int SinglePointManipulator::getZDepth()
{
	return 2;  //layer 2
}
//draws the icon to indicate a click

void SinglePointManipulator::drawClickIcon(int x, int y, Mat &img)
{
	if(x == -1 || y == -1)
	{
		return;
	}
	//cout << "in draw";
	Size s = img.size();

	circle(img,Point(x,y),5,Scalar(0,0,255, 255),1);
    
	line(img, Point(x, y), Point(x, max(y - 15, 0)), Scalar(255, 0, 0, 255), 2);
   
	line(img, Point(x, y), Point(x, min(y + 15, s.height)), Scalar(255, 0, 0, 255), 2);
    
	line(img, Point(x, y), Point(max(x - 15, 0), y), Scalar(255, 0, 0, 255), 2);
    
	line(img, Point(x, y), Point(min(x + 15, s.width), y), Scalar(255, 0, 0, 255), 2);
    
}
	

