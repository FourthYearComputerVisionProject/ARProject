#ifndef STEREOVIEWER_H
#define STEREOVIEWER_H

#include "stdafx.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include <ctype.h>
#include "cv.h"
#include "highgui.h"
#include "textfile.h"
#include "RenderableVideoCapture.h"

class StereoViewer{
private:
	GLuint oculusShader;
	GLuint program;

	RenderableVideoCapture *rightCapture;
	RenderableVideoCapture *leftCapture;
public:
	void setShaders(void);
	void reshape(int w, int h);
	void display(void);
	
	StereoViewer(int leftDevice, int rightDevice);
	~StereoViewer();
};

#endif