#ifndef BOXMANIP_H
#define BOXMANIP_H

#include "stdafx.h"

#include "opencv\cv.h"
#include "opencv\highgui.h"

#include "Manipulator.h"
#include "Renderable.h"

class BoxManip : public Manipulator {
private:
	Renderable *parent;
public:
	unsigned char getMode(void) {return 0x10;}
	virtual void update(cv::Mat image);
	virtual void draw(cv::Mat image);
	long getUID(void) {return 1;}

	BoxManip(Renderable *myParent);
	virtual ~BoxManip(void);
};

#endif