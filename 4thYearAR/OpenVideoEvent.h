#pragma once

#include "stdafx.h"

#include <string>

#include "BaseEvent.h"

class OpenVideoEvent : public BaseEvent
{
private:
	std::string video;
public:
	OpenVideoEvent(std::string vid);
	std::string getVideo() { return video; }
};