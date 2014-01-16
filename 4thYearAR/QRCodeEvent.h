/*
 * QRCodeEvent.h
 * Header file for QRCodeEvent
 * Defines an event to play a video from the QRCode
 */

#pragma once

#include "stdafx.h"

#include "BaseEvent.h"

class QRCodeEvent : public BaseEvent
{
private:
	std::string location;
public:
	//NOTE: The constructor hard-codes the event type so that we don't need to provide it
	//each time we make a new event
	QRCodeEvent(std::string loc) : BaseEvent(2) { location = loc;}
	std::string getLocation() { return location; }
};