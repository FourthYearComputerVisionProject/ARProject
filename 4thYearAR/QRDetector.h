/*
 * QRDetector.h
 * Header for a QR detector.
 */

#pragma once

#include "stdafx.h"

#include <map>
#include <list>
#include <string>

#include "zbar.h"

#include "IDetector.h"
#include "EventManager.h"
#include "QRCodeEvent.h"
#include "IEventListener.h"
#include "ChangeBoxLocationEvent.h"
#include "OpenVideoEvent.h"
#include "CloseVideoEvent.h"

class QRDetector : public IDetector, public IEventListener {
private:
	std::map<std::string, bool> history;
	std::list<std::string> historyOrder;
public:
	QRDetector();
	void detect(cv::Mat leftImage, cv::Mat rightImage);
	void handleEvent(BaseEvent* evt);
};