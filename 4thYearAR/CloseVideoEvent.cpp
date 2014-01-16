#include "stdafx.h"

#include "CloseVideoEvent.h"

CloseVideoEvent::CloseVideoEvent(std::string vid) : BaseEvent(CLOSE_VIDEO_EVENT), video(vid)
{

}