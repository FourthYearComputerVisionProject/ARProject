#pragma once

#include "stdafx.h"

#include <string>

#include "BaseEvent.h"

class VideoReadyEvent : public BaseEvent
{
private:
	bool ready;
public:
	VideoReadyEvent(bool vidReady);
	bool videoReady() { return ready; };
};