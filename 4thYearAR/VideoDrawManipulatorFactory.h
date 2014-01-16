#pragma once

#include "stdafx.h"

#include "IEventListener.h"
#include "StereoRender.h"

class VideoDrawManipulatorFactory : public IEventListener
{
private:
	StereoRender* render;
public:
	VideoDrawManipulatorFactory(StereoRender* renderer);
	void handleEvent(BaseEvent* evt);
};