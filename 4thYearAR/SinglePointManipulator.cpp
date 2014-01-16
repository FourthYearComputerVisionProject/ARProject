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
		x = spEvt->getX();
		y = spEvt->getY();
	}

}

void SinglePointManipulator::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	//cout<< "in manipulator";
	drawClickIcon(x, y, leftImage);
	drawClickIcon(x, y, rightImage);
	
}

int SinglePointManipulator::getZDepth()
{
	return 2;  //layer 2
}
//draws the icon to indicate a click

void SinglePointManipulator::drawClickIcon(int x, int y, Mat &img)
{
	//cout << "in draw";
	Size s = img.size();

	circle(img,Point(x,y),5,Scalar(255,0,0),1);
    
    line(img,Point(x,y),Point(x,max(y-15,0)),Scalar(0,255,0),1);
   
    line(img,Point(x,y),Point(x,min(y+15, s.height)),Scalar(0,255,0),1);
    
    line(img,Point(x,y),Point(max(x-15, 0),y),Scalar(0,255,0),1);
    
    line(img,Point(x,y),Point(min(x+15, s.width),y),Scalar(0,255,0),1);
    
}
	

