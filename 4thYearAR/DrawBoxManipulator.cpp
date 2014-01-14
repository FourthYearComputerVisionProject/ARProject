
#include "stdafx.h"

#include "DrawBoxManipulator.h"
#include "ChangeBoxLocationEvent.h"

DrawBoxManipulator::DrawBoxManipulator()
{
	x = y = 0;
}

void DrawBoxManipulator::handleEvent(BaseEvent* evt)
{
	if(evt->getType() == 1)
	{
		ChangeBoxLocationEvent* cEvt = (ChangeBoxLocationEvent*)evt;
		x = cEvt->getX();
		y = cEvt->getY();
	}
}

void DrawBoxManipulator::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	cv::rectangle(leftImage, cv::Rect(x, y, 100, 100), cv::Scalar(1.0, 0.0, 1.0, 0.0), 5);
	cv::rectangle(rightImage, cv::Rect(x, y, 100, 100), cv::Scalar(1.0, 0.0, 1.0, 0.0), 5);
}