#include "stdafx.h"
#include "CalibrationResultsEvent.h"

CalibrationResultsEvent::CalibrationResultsEvent(int h_min, int h_max, int s_min, int s_max, int v_min, int v_max):BaseEvent(CALIBRATION_RESULTS_EVENT)
{
	this->h_min = h_min;
	this->h_max = h_max;
	this->s_min = s_min;
	this->s_max = s_max;
	this->v_min = v_min;
	this->v_max = v_max;
}

int CalibrationResultsEvent::getHueMin()
{
	return h_min;
}

int CalibrationResultsEvent::getHueMax()
{
	return h_max;
}

int CalibrationResultsEvent::getSaturationMin()
{
	return s_min;
}

int CalibrationResultsEvent::getSaturationMax()
{
	return s_max;
}

int CalibrationResultsEvent::getValueMin()
{
	return v_min;
}

int CalibrationResultsEvent::getValueMax()
{
	return v_max;
}

CalibrationResultsEvent::~CalibrationResultsEvent(void)
{
}
