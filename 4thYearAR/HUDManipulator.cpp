
#include "stdafx.h"

#include "HUDManipulator.h"
#include "ChangeBoxLocationEvent.h"

HUDManipulator::HUDManipulator()
{
	batteryTexture = cv::imread("Images\\battery.png", -1);
	cv::cvtColor(batteryTexture, batteryTexture, CV_RGBA2BGRA);

	alpha = 0.5;

	//cv::imshow("t", batteryTexture);
	/*
	glGenTextures(1, &batteryTexture); // Create The Texture

	glBindTexture(GL_TEXTURE_2D, batteryTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, battery.cols, battery.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, battery.data);
	*/
	x = y = 0;
}

void HUDManipulator::handleEvent(BaseEvent* evt)
{
	if(evt->getType() == 1)
	{
		ChangeBoxLocationEvent* cEvt = (ChangeBoxLocationEvent*)evt;
		x = cEvt->getX();
		y = cEvt->getY();
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

	drawClock(leftImage, rightImage);
}

void HUDManipulator::drawClock(cv::Mat leftImage, cv::Mat rightImage) {
	cv::putText(leftImage, getTime().str(), cv::Point(350, 100), cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(0, 0, 255, 255), 2,CV_AA);
	cv::putText(rightImage, getTime().str(), cv::Point(325, 100), cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(0, 0, 255, 255), 2);
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