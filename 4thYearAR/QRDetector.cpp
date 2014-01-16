/*
 * QRDetector.cpp
 * A QR Detector.
 */

#include "stdafx.h"

#include <list>
#include <string>

#include "QRDetector.h"

#define HISTORY_SIZE 10

static std::list<std::string> history;

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
		bool foundIt = false;
		for(auto it = history.begin(); it != history.end(); ++it)
		{
			if(it->compare(sym->get_data()) == 0)
			{
				foundIt = true;
				break;
			}
		}
		if(foundIt)
		{
			continue;
		}

		std::cout    << "decoded " << sym->get_type_name()
                    << " symbol \"" << sym->get_data() << '"' << std::endl;

		QRCodeEvent* evt = new QRCodeEvent(sym->get_data());
		EventManager::getGlobal()->fireEvent(evt);

		history.push_back(sym->get_data());
		if(history.size() > HISTORY_SIZE)
		{
			history.pop_front();
		}
		if (sym->get_location_size() == 4) {
            //rectangle(frame, Rect(symbol->get_location_x(i), symbol->get_location_y(i), 10, 10), Scalar(0, 255, 0));
            cv::line(leftImage, cv::Point(sym->get_location_x(0), sym->get_location_y(0)), cv::Point(sym->get_location_x(1), sym->get_location_y(1)), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::line(leftImage, cv::Point(sym->get_location_x(1), sym->get_location_y(1)), cv::Point(sym->get_location_x(2), sym->get_location_y(2)), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::line(leftImage, cv::Point(sym->get_location_x(2), sym->get_location_y(2)), cv::Point(sym->get_location_x(3), sym->get_location_y(3)), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::line(leftImage, cv::Point(sym->get_location_x(3), sym->get_location_y(3)), cv::Point(sym->get_location_x(0), sym->get_location_y(0)), cv::Scalar(0, 255, 0), 2, 8, 0);
        }
	}
}