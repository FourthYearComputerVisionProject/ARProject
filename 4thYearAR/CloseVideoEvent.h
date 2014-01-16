#pragma once

#include "stdafx.h"

#include <string>

#include "BaseEvent.h"

class CloseVideoEvent : public BaseEvent
{
private:
	std::string video;
public:
	CloseVideoEvent(std::string vid);
	std::string getVideo() { return video; }
};