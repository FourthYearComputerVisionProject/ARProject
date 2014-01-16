
#include "stdafx.h"

#include "HUDManipulator.h"
#include "ChangeBoxLocationEvent.h"

HUDManipulator::HUDManipulator()
{
	EventManager::getGlobal()->addListener(VIDEO_READY_EVENT, this); //subscribe to video ready events

	batteryTexture = cv::imread("Images\\battery.png", -1);
	cv::cvtColor(batteryTexture, batteryTexture, CV_RGBA2BGRA);
	videoAvailable = false;

	alpha = 0.5;
	x = y = 0;
}

void HUDManipulator::handleEvent(BaseEvent* evt)
{
	if(evt->getType() == CLICK_EVENT)
	{
		ChangeBoxLocationEvent* cEvt = (ChangeBoxLocationEvent*)evt;
		x = cEvt->getX();
		y = cEvt->getY();
	}
	else if (evt->getType() == VIDEO_READY_EVENT) {
		VideoReadyEvent vrevt = (VideoReadyEvent*) evt;
		videoAvailable = vrevt.videoReady();
	}
}

void HUDManipulator::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	cv::Rect roi = cv::Rect(300, 50, batteryTexture.cols, batteryTexture.rows);
	cv::Mat leftROI = leftImage(roi);
	cv::addWeighted(batteryTexture, alpha, leftROI, 1 - alpha, 0.0, leftROI);

	roi = cv::Rect(275, 50, batteryTexture.cols, batteryTexture.rows);
	cv::Mat rightROI = rightImage(roi);
	cv::addWeighted(batteryTexture, alpha, rightROI, 1 - alpha, 0.0, rightROI);

	if (videoAvailable) {
		drawMessage("Video Available",leftImage,rightImage);
	}

	drawClock(leftImage, rightImage);

	//drawCrosshair(leftImage);
	//drawCrosshair(rightImage);
}

void HUDManipulator::drawCrosshair(cv::Mat img) {
	cv::Size s = img.size();
	cv::Point p = cv::Point(s.width / 2, s.height / 2);
	cv::circle(img, p, 5, cv::Scalar(0, 0, 255, 255), 1);
	cv::line(img, p, cv::Point(p.x, cv::max(p.y - 15, 0)), cv::Scalar(255, 0, 0, 255), 1);
	line(img, p, cv::Point(p.x, cv::min(p.y + 15, s.height)), cv::Scalar(255, 0, 0, 255), 1);
	line(img, p, cv::Point(cv::max(p.x - 15, 0), p.y), cv::Scalar(255, 0, 0, 255), 1);
	line(img, p, cv::Point(cv::min(p.x + 15, s.width), p.y), cv::Scalar(255, 0, 0, 255), 1);
}

void HUDManipulator::drawMessage(std::string message,cv::Mat leftImage, cv::Mat rightImage) {
	cv::putText(leftImage, message, cv::Point(250, 350), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 255, 255), 2, CV_AA);
	cv::putText(rightImage, message, cv::Point(225, 350), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 255, 255), 2, CV_AA);
}

void HUDManipulator::drawClock(cv::Mat leftImage, cv::Mat rightImage) {
	cv::putText(leftImage, getTime().str(), cv::Point(350, 100), cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(0, 0, 255, 255), 2, CV_AA);
	cv::putText(rightImage, getTime().str(), cv::Point(325, 100), cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(0, 0, 255, 255), 2, CV_AA);
}

std::stringstream HUDManipulator::getTime() {
	std::time_t now = std::time(0);
	std::tm  *localtime = std::localtime(&now);
	std::stringstream time;
	time << std::setw(2) << std::setfill('0') << localtime->tm_hour << ":";
	time << std::setw(2) << std::setfill('0') << localtime->tm_min;
	return time;
}

void HUDManipulator::drawImage(GLuint texture) {
	
	/*glBindTexture(GL_TEXTURE_2D, texture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + 0.0, 0.0 + 0, 0.0 + 1.0);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.5 + 0.0, 0.0 + 0, 0.0 + 1.0);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + 0.0, 1.0 + 0, 0.0 + 1.0);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.5 + 0.0, 1.0 + 0, 0.0 + 1.0);
	glEnd();*/
}

HUDManipulator::~HUDManipulator() {
	//glDeleteTextures(1, &batteryTexture);
}