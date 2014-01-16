#include "stdafx.h"
#include "SinglePointEvent.h"
#include <opencv\cv.h>

using namespace std;
using namespace cv;

SinglePointEvent::SinglePointEvent(Point newPoint):BaseEvent(2)
{	
	p = newPoint;
}

SinglePointEvent::~SinglePointEvent(void)
{
}

int SinglePointEvent::getX() 
{ 
	return p.x; 
}

int SinglePointEvent::getY() 
{ 
	return p.y; 
}
