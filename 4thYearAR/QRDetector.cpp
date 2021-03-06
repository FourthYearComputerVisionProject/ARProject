/*
 * QRDetector.cpp
 * A QR Detector.
 */

#include "stdafx.h"

#include "QRDetector.h"

#define HISTORY_SIZE 10

QRDetector::QRDetector()
{
	EventManager::getGlobal()->addListener(1, this);
	EventManager::getGlobal()->addListener(CLOSE_VIDEO_EVENT, this);
	videoPlaying = false;
}

void QRDetector::handleEvent(BaseEvent* evt)
{
	if(evt->getType() == 1)
	{
		ChangeBoxLocationEvent* chEvt = (ChangeBoxLocationEvent*)evt;
		if(history.size() != 0)
		{
			std::string str = historyOrder.front();
			auto findIt = history.find(str);
			if(findIt != history.end() && findIt->second)
			{
				CloseVideoEvent evt = CloseVideoEvent(findIt->first);
				EventManager::getGlobal()->fireEvent(&evt);
				/*history.erase(findIt);
				historyOrder.pop_front();*/
				if (history.size() != 0) { // if another video is ready to load
					VideoReadyEvent vrevt = VideoReadyEvent(true);
					EventManager::getGlobal()->fireEvent(&vrevt);
					videoPlaying = false;
				}
			}
			else if(findIt != history.end())
			{
				OpenVideoEvent ovevt = OpenVideoEvent(findIt->first);
				EventManager::getGlobal()->fireEvent(&ovevt);
				history[findIt->first] = true;

				VideoReadyEvent vrevt = VideoReadyEvent(false);
				EventManager::getGlobal()->fireEvent(&vrevt);
				videoPlaying = true;
			}
		}
	}
	else if(evt->getType() == CLOSE_VIDEO_EVENT)
	{
		std::string str = historyOrder.front();
		auto findIt = history.find(str);
		if(findIt != history.end() && findIt->second)
		{
			history.erase(findIt);
			historyOrder.pop_front();
			if (history.size() != 0) { // if another video is ready to load
				VideoReadyEvent vrevt = VideoReadyEvent(true);
				EventManager::getGlobal()->fireEvent(&vrevt);
			}
		}
		videoPlaying = false;
	}
}

void QRDetector::detect(cv::Mat leftImage, cv::Mat rightImage)
{
	if(!leftImage.data)
	{
		return;
	}
	cv::Mat grayScale;
	zbar::ImageScanner scanner;
	scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
	cv::cvtColor(leftImage, grayScale, CV_RGBA2GRAY);
	int width, height;
	width =  grayScale.cols;
	height = grayScale.rows;
	uchar *raw = (uchar*)(grayScale.data);

	zbar::Image image(width, height, "Y800", raw, width * height);

	scanner.scan(image);

	int counter = 0;

	for(zbar::Image::SymbolIterator sym = image.symbol_begin();
		sym != image.symbol_end(); ++sym)
	{
		auto findIt = history.find(sym->get_data());
		if(findIt != history.end())
		{
			continue;
		}

		std::cout    << "decoded " << sym->get_type_name()
                    << " symbol \"" << sym->get_data() << '"' << std::endl;

		//QRCodeEvent* evt = new QRCodeEvent(sym->get_data());
		//EventManager::getGlobal()->fireEvent(evt);

		history[sym->get_data()] = false;
		historyOrder.push_back(sym->get_data());
		if (sym->get_location_size() == 4) {
            //rectangle(frame, Rect(symbol->get_location_x(i), symbol->get_location_y(i), 10, 10), Scalar(0, 255, 0));
            cv::line(leftImage, cv::Point(sym->get_location_x(0), sym->get_location_y(0)), cv::Point(sym->get_location_x(1), sym->get_location_y(1)), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::line(leftImage, cv::Point(sym->get_location_x(1), sym->get_location_y(1)), cv::Point(sym->get_location_x(2), sym->get_location_y(2)), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::line(leftImage, cv::Point(sym->get_location_x(2), sym->get_location_y(2)), cv::Point(sym->get_location_x(3), sym->get_location_y(3)), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::line(leftImage, cv::Point(sym->get_location_x(3), sym->get_location_y(3)), cv::Point(sym->get_location_x(0), sym->get_location_y(0)), cv::Scalar(0, 255, 0), 2, 8, 0);
        }

		if (!videoPlaying) {
			VideoReadyEvent vrevt = VideoReadyEvent(true);
			EventManager::getGlobal()->fireEvent(&vrevt); //a video was added, inform the user
		}
	}
}