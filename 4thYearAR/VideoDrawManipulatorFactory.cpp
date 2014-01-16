#include "stdafx.h"

#include "VideoDrawManipulatorFactory.h"

VideoDrawManipulatorFactory::VideoDrawManipulatorFactory(StereoRender* renderer)
{
	render = renderer;
	EventManager::getGlobal()->addListener(2, this);
}

void VideoDrawManipulatorFactory::handleEvent(BaseEvent* evt)
{
	if(evt->getType() == 2){
		QRCodeEvent* qEvt = (QRCodeEvent*)evt;
		std::string localVidSource = qEvt->getLocalVid();
		float aspect = 640.0f / 480.0f;
		VideoDrawManipulator* manip = new VideoDrawManipulator(localVidSource, 200, 200, (int)(200.0f * aspect), 200);
		render->addManipulator(manip);
	}
}