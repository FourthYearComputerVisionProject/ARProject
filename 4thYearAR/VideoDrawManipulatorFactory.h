#pragma once

#include "stdafx.h"

#include <map>

#include "IEventListener.h"
#include "StereoRender.h"
#include "OpenVideoEvent.h"
#include "CloseVideoEvent.h"

class VideoDrawManipulatorFactory : public IEventListener
{
private:
	std::map<std::string, VideoDrawManipulator*> history;
	std::map<std::string, bool> playing;
	StereoRender* render;
public:
	VideoDrawManipulatorFactory(StereoRender* renderer);
	void handleEvent(BaseEvent* evt);
};