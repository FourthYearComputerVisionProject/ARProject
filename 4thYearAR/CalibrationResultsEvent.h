#pragma once
#include "BaseEvent.h"
class CalibrationResultsEvent:public BaseEvent
{
private:
	int h_min, h_max;
	int s_min, s_max;
	int v_min, v_max;
public:
	public:
	CalibrationResultsEvent(int h_min, int h_max, int s_min, int s_max, int v_min, int v_max);
	~CalibrationResultsEvent(void);
	int getHueMin();
	int getHueMax();
	int getSaturationMin();
	int getSaturationMax();
	int getValueMin();
	int getValueMax();

};

