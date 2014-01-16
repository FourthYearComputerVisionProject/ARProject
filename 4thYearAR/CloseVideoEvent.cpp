#include "stdafx.h"

#include "CloseVideoEvent.h"

CloseVideoEvent::CloseVideoEvent(std::string vid) : BaseEvent(5L), video(vid)
{

}