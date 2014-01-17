#include "stdafx.h"

#include "VideoReadyEvent.h"

VideoReadyEvent::VideoReadyEvent(bool vidReady) : BaseEvent(VIDEO_READY_EVENT)
{
	ready = vidReady;
}