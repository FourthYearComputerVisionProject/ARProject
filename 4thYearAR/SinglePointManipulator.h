#pragma once
#include "imanipulator.h"
#include "IEventListener.h"

class SinglePointManipulator :
	public IManipulator, public IEventListener
{
public:
	SinglePointManipulator(void);
	~SinglePointManipulator(void);
	void manipulate(cv::Mat leftImage, cv::Mat rightImage);
	int getZDepth();
	void handleEvent(BaseEvent* evt);
private:
	int x, y;
	void drawClickIcon(int x, int y, cv::Mat &img);
};

