#include "stdafx.h"
#include "BoxManip.h"

BoxManip::BoxManip(void){
	
}

int BoxManip::getMode(void){
	return 0x0100;
}

long BoxManip::getUID(void){
	return 1;
}

void BoxManip::update(cv::Mat image){
	//Do nothing
}

void BoxManip::draw(cv::Mat image){
	cv::Rect rectangle = *(new cv::Rect(0, 0, 180, 60));
	cv::Scalar color = *(new cv::Scalar(0, 0, 255));
	cv::rectangle(image, rectangle, color, 5);
}