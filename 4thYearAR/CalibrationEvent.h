#pragma once
#include "BaseEvent.h"

class CalibrationEvent: public BaseEvent
{
private:
	bool doCalibration;
public:
	CalibrationEvent(bool doCalibration);
	~CalibrationEvent(void);
	bool getDoCalibrate();

};

