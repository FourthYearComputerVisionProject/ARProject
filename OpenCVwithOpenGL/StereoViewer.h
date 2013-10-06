#ifndef STEREOVIEWER_H
#define STEREOVIEWER_H

#include "stdafx.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include <ctype.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "textfile.h"
#include "RenderableVideoCapture.h"
#include "RenderableImage.h"

class StereoViewer{
private:
	GLuint oculusShader;
	GLuint program;

	RenderableVideoCapture *rightCapture;
	RenderableVideoCapture *leftCapture;

	RenderableImage *rightImage;
	RenderableImage *leftImage;

	int mode;
public:
	void setShaders(void);
	void reshape(int w, int h);
	void display(void);
	
	StereoViewer(int leftDevice, int rightDevice);
	StereoViewer::StereoViewer(const std::string filename);
	~StereoViewer();
};

#endif