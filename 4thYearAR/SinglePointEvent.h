#pragma once

#include "stdafx.h"
#include "baseevent.h"
#include "opencv\cv.h"

using namespace cv;

class SinglePointEvent :
	public BaseEvent
{
protected:
	Point pRight, pLeft;

public:
	SinglePointEvent(Point newLeftPoint, Point newRightPoint);
	~SinglePointEvent(void);
	int getXR();
	int getYR();
	int getXL();
	int getYL();
};

