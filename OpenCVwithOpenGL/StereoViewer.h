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
#include "Renderable.h"
#include "RenderableCapture.h"

class StereoViewer {
private: 
	GLuint monochromeVertShader;
	GLuint monochromeFragShader;
	GLuint monochromeProgram;

	GLuint alphaVertShader;
	GLuint alphaFragShader;
	GLuint alphaProgram;

	GLuint frameBuffer;
	GLuint frameBuffer_texture;
	GLuint depthBuffer;

	int width;
	int height;

	int mode;

	Renderable *rightImg;
	Renderable *leftImg;

	void printShaderLog(int shader, char* shaderName);
	void stereoWarp(GLuint outFBO);
	
public:
	void setShader(char* filename, GLuint shader, GLenum type, GLuint program);
	void reshape(int w, int h);
	void display(void);
	
	StereoViewer(int leftDevice, int rightDevice);
	~StereoViewer();
};

#endif