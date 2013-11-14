#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "stdafx.h"

#include "opencv\cv.h"
#include "opencv\highgui.h"

class Manipulator {
private:
public:
	virtual unsigned char getMode(void) = 0;
	//Update the manipulator using the given image as a data source
	virtual void update(cv::Mat image) = 0;
	//Draw any information to the provided image
	virtual void draw(cv::Mat image) = 0;

	virtual long getUID(void) = 0;

	Manipulator(void) {/*Do nothing*/}
	~Manipulator(void) {/*Do nothing*/}
};

#endif