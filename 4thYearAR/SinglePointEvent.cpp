#include "stdafx.h"
#include "SinglePointEvent.h"
#include <opencv\cv.h>

using namespace std;
using namespace cv;

SinglePointEvent::SinglePointEvent(Point newLeftPoint, Point newRightPoint):BaseEvent(SINGLE_POINT_EVENT)
{	
	pRight = newRightPoint;
	pLeft = newLeftPoint;

}

SinglePointEvent::~SinglePointEvent(void)
{
}

int SinglePointEvent::getXR() 
{ 
	return pRight.x; 
}

int SinglePointEvent::getYR() 
{ 
	return pRight.y; 
}

int SinglePointEvent::getXL() 
{ 
	return pLeft.x; 
}

int SinglePointEvent::getYL() 
{ 
	return pLeft.y; 
}
