#include "stdafx.h"

#include "VideoDrawManipulatorFactory.h"

static VideoDrawManipulator* globalvid;	//temp so we can remove video, will be changed to location finding
static bool videoPlaying = false;

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
<<<<<<< HEAD
		float aspect = 16.0f / 9.0f;
		VideoDrawManipulator* manip = new VideoDrawManipulator(localVidSource, 350, 300, (int)(150.0f * aspect), 150);
		render->addManipulator(manip);
=======
		float aspect = 640.0f / 480.0f;
		VideoDrawManipulator* manip = new VideoDrawManipulator(localVidSource, 200, 200, (int)(200.0f * aspect), 200);
		globalvid = manip;
		//render->addManipulator(manip);
	}
	else if(evt->getType() == 1)
	{
		if(globalvid!=NULL){
			if(videoPlaying==true){
				render->removeManipulator(globalvid);
				globalvid = NULL;
				videoPlaying = false;
			}else{
				render->addManipulator(globalvid);
				videoPlaying = true;
			}
		}
>>>>>>> 1ebc3b77e7f7c1c0284f86af2284ded98a503229
	}
}