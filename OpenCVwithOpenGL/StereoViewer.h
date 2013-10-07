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
#include "RenderableImage.h"

class StereoViewer{
private:
	GLuint oculusVertShader;
	GLuint oculusFragShader;
	//GLuint oculusRightGeomShader;
	//GLuint oculusLeftGeomShader;
	GLuint program;
	//GLuint leftShaderProgram;
	//GLuint rightShaderProgram;
	//GLuint frameBuffer;
	//GLuint frameBuffer_texture;
	//GLuint depthBuffer;

	int width;
	int height;

	RenderableVideoCapture *rightCapture;
	RenderableVideoCapture *leftCapture;

	RenderableImage *rightImage;
	RenderableImage *leftImage;

	void printShaderLog(int shader, char* shaderName);
	void stereoWarp(GLuint outFBO);
	
public:
	void setShaders(void);
	void reshape(int w, int h);
	void display(void);
	
	StereoViewer(int leftDevice, int rightDevice);
	~StereoViewer();
};

#endif