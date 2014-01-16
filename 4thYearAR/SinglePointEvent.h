#pragma once

#include "stdafx.h"
#include "baseevent.h"
#include "opencv\cv.h"

using namespace cv;

class SinglePointEvent :
	public BaseEvent
{
protected:
	Point p;
public:
	SinglePointEvent(Point newPoint);
	~SinglePointEvent(void);
	int getX();
	int getY();
};

