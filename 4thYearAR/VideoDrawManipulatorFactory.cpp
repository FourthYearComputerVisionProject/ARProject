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
		float aspect = 16.0f / 9.0f;
		VideoDrawManipulator* manip = new VideoDrawManipulator(localVidSource, 350, 300, (int)(150.0f * aspect), 150);
		render->addManipulator(manip);
	}
}