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
	if(evt->getType() == 2)
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
	
}

int SinglePointManipulator::getZDepth()
{
	return 2;  //layer 2
}
//draws the icon to indicate a click

void SinglePointManipulator::drawClickIcon(int x, int y, Mat &img)
{
	Size s = img.size();

	circle(img,Point(x,y),3,Scalar(255,0,0),2);
    
    line(img,Point(x,y),Point(x,max(y-25,0)),Scalar(0,255,0),2);
   
    //if(y+25<FRAME_HEIGHT)
    line(img,Point(x,y),Point(x,min(y+25, s.height)),Scalar(0,255,0),2);
    //else line(img,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    //if(x-25>0)
    line(img,Point(x,y),Point(max(x-25, 0),y),Scalar(0,255,0),2);
    //else line(img,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    //if(x+25<FRAME_WIDTH)
    line(img,Point(x,y),Point(min(x+25, s.width),y),Scalar(0,255,0),2);
    //else line(img,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

}
	

