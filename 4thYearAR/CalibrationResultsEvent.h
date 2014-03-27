#pragma once
#include "stdafx.h"
#include "BaseEvent.h"
#include "HistRange.h"

using namespace std;

class CalibrationResultsEvent:public BaseEvent
{
private:
	int h_min, h_max;
	int s_min, s_max;
	int v_min, v_max;
	vector<HistRange> rangeResults;
public:
	
	//CalibrationResultsEvent(int h_min, int h_max, int s_min, int s_max, int v_min, int v_max);
	CalibrationResultsEvent(vector <HistRange> ranges);
	~CalibrationResultsEvent(void);
	vector<HistRange> getHistRanges();
};

