#include "stdafx.h"
#include "HistRange.h"
#include "CalibrationResultsEvent.h"

using namespace std;
//CalibrationResultsEvent::CalibrationResultsEvent(int h_min, int h_max, int s_min, int s_max, int v_min, int v_max):BaseEvent(CALIBRATION_RESULTS_EVENT)
CalibrationResultsEvent::CalibrationResultsEvent(vector<HistRange> newRanges):BaseEvent(CALIBRATION_RESULTS_EVENT)
{
	this->rangeResults = newRanges;
}

vector<HistRange> CalibrationResultsEvent::getHistRanges()
{
	return  rangeResults;
}


CalibrationResultsEvent::~CalibrationResultsEvent(void)
{
}
