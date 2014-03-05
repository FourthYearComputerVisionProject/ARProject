#include "stdafx.h"
#include "CalibrationEvent.h"


CalibrationEvent::CalibrationEvent(bool doCalibration): BaseEvent(CALIBRATION_EVENT)
{
	this->doCalibration=doCalibration;
}

bool CalibrationEvent::getDoCalibrate(){
	return doCalibration;
}
CalibrationEvent::~CalibrationEvent(void)
{
}
