#include "stdafx.h"

#include "VideoDrawManipulatorFactory.h"

static VideoDrawManipulator* globalvid;	//temp so we can remove video, will be changed to location finding
static bool videoPlaying = false;

VideoDrawManipulatorFactory::VideoDrawManipulatorFactory(StereoRender* renderer)
{
	render = renderer;
	EventManager::getGlobal()->addListener(4, this);
	EventManager::getGlobal()->addListener(5, this);
	//EventManager::getGlobal()->addListener(2, this);
	//EventManager::getGlobal()->addListener(1, this);
}

void VideoDrawManipulatorFactory::handleEvent(BaseEvent* evt)
{
	float aspect = 640.0f / 480.0f;
	if(evt->getType() == 4)
	{
		OpenVideoEvent* vidEvt = (OpenVideoEvent*)evt;
		auto findIt = playing.find(vidEvt->getVideo());
		if(findIt == playing.end())
		{
			VideoDrawManipulator* manip = new VideoDrawManipulator(vidEvt->getVideo(), 200, 200, (int)(200.0f * aspect), 200);
			playing[vidEvt->getVideo()] = true;
			history[vidEvt->getVideo()] = manip;
			render->addManipulator(manip);
		}
	}
	else if(evt->getType() == 5)
	{
		CloseVideoEvent* vidEvt = (CloseVideoEvent*)evt;
		auto findIt = playing.find(vidEvt->getVideo());
		if(findIt != playing.end())
		{
			VideoDrawManipulator* manip = history[vidEvt->getVideo()];
			render->removeManipulator(manip);
			playing.erase(findIt);
			auto findIt2 = history.find(vidEvt->getVideo());
			if(findIt2 != history.end())
			{
				history.erase(findIt2);
			}
		}
	}
	/*if(evt->getType() == 2){
		QRCodeEvent* qEvt = (QRCodeEvent*)evt;
		std::string localVidSource = qEvt->getLocalVid();

		float aspect = 16.0f / 9.0f;
		VideoDrawManipulator* manip = new VideoDrawManipulator(localVidSource, 350, 300, (int)(150.0f * aspect), 150);
		render->addManipulator(manip);
	}
	else if(evt->getType() == 1)
	{
		if(globalvid!=NULL){
			if(videoPlaying==true){
				for(auto it = QRDetector::history.begin(); it != QRDetector::history.end(); ++it)
				{
					if((*it).compare(globalvid->getLocation()) == 0)
					{
						QRDetector::history.erase(it);
						break;
					}
				}
				render->removeManipulator(globalvid);
				globalvid = NULL;
				videoPlaying = false;
			}else{
				render->addManipulator(globalvid);
				videoPlaying = true;
			}
		}
	}*/
}