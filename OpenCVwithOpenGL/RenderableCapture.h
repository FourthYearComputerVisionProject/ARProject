#ifndef RENDERCAPTURE_H
#define RENDERCAPTURE_H

#define DEFAULT_FRAMERATE 60

#include "stdafx.h"

#include <list>

#include "opencv\cv.h"
#include "opencv\highgui.h"

#include "GL\glew.h"
#include "GL\glut.h"

#include "Renderable.h"
#include "Manipulator.h"

using namespace cv;

class RenderableCapture : public Renderable{
private:
	GLuint texture; //the array for our texture
	GLdouble xOffset;
	GLdouble yOffset;
	GLdouble zOffset;
	VideoCapture videoCapture;
public:
	virtual void update(void);
	virtual void draw(void);
	virtual void addManipulator(Manipulator *manip);
	RenderableCapture(int captureDevice, GLdouble xOffset, GLdouble yOffset, GLdouble zOffset);
	~RenderableCapture(void);
};

#endif