#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "stdafx.h"
#include <list>

#include "opencv\cv.h"
#include "opencv\highgui.h"

#include "Manipulator.h"

class Renderable{
protected:
	std::list<Manipulator*> manips;
	cv::Mat capture;
public:
	Manipulator* getManipulator(long uid){
		for(std::list<Manipulator*>::iterator it = manips.begin(); it != manips.end(); it++){
			if((*it)->getUID() == uid){
				return (*it);
			}
		}
		return NULL;
	}

	//Fetch a new capture and update the manips
	virtual void update(void) = 0;

	//Draw the image
	virtual void draw(void) = 0;

	virtual void addManipulator(Manipulator *manip) = 0;
};

#endif