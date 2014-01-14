/*
 * QRDetector.h
 * Header for a QR detector.
 */

#pragma once

#include "stdafx.h"

#include "zbar.h"

#include "IDetector.h"

class QRDetector : public IDetector{
public:
	void detect(cv::Mat leftImage, cv::Mat rightImage);
};