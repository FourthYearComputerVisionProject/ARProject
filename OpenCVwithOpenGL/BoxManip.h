#ifndef BOXMANIP_H
#define BOXMANIP_H

#include "stdafx.h"

#include "opencv\cv.h"
#include "opencv\highgui.h"

#include "Manipulator.h"

class BoxManip : public Manipulator {
private:
public:
	int getMode(void);
	void update(cv::Mat image);
	void draw(cv::Mat image);
	long getUID(void);

	BoxManip(void);
	~BoxManip(void) {}
};

#endif