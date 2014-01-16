#include "stdafx.h"

#include "VideoDrawManipulatorFactory.h"

VideoDrawManipulatorFactory::VideoDrawManipulatorFactory(StereoRender* renderer)
{
	render = renderer;
	EventManager::getGlobal()->addListener(2, this);
	EventManager::getGlobal()->addListener(1, this);
}

void VideoDrawManipulatorFactory::handleEvent(BaseEvent* evt)
{
	if(evt->getType() == 2){
		QRCodeEvent* qEvt = (QRCodeEvent*)evt;
		std::string localVidSource = qEvt->getLocalVid();
		VideoDrawManipulator* manip = new VideoDrawManipulator(localVidSource, 100, 100);
		render->addManipulator(manip);
	}
	else if(evt->getType() == 1)
	{
		std::string localVidSource = "C:\\Users\\Damian\\Videos\\Janus Syndacite\\Back+to+School+with+Crackbone.mp4";
		VideoDrawManipulator* manip = new VideoDrawManipulator(localVidSource, 100, 100);
		render->addManipulator(manip);
	}
}