#include "stdafx.h"

#include "OpenVideoEvent.h"

OpenVideoEvent::OpenVideoEvent(std::string vid) : BaseEvent(OPEN_VIDEO_EVENT), video(vid)
{

}