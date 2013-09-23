// OpenCVwithOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "StereoViewer.h"

using namespace cv;


void StereoViewer::display(void) {

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	StereoViewer::rightCapture->updateTexture();
	StereoViewer::leftCapture->updateTexture();

	StereoViewer::rightCapture->drawCapture();
	StereoViewer::leftCapture->drawCapture();

	//glTranslated(0.5, 0.5, 0.0);

	//glutSolidTeapot(0.25);

	//const GLint lensCenter = glGetUniformLocation(program, "lensCenter");
	//glUniform2fv(lensCenter, 1, {2.0,1.0});

	glutSwapBuffers();
}

void StereoViewer::reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void StereoViewer::setShaders() {

	oculusShader = glCreateShader(GL_FRAGMENT_SHADER);

	char *fs;
	fs = textFileRead("oculusDistort.frag");
	const char * ff = fs;
	glShaderSource(oculusShader, 1, &ff, NULL);
	free(fs);

	glCompileShader(oculusShader);

	program = glCreateProgram();

	glAttachShader(program, oculusShader);

	glLinkProgram(program);
	glUseProgram(program);
}

StereoViewer::StereoViewer(int leftDevice, int rightDevice) {
	StereoViewer::rightCapture = new RenderableVideoCapture(rightDevice, 0.5, 0, 0);
	StereoViewer::leftCapture = new RenderableVideoCapture(leftDevice, 0, 0, 0);
}

StereoViewer::~StereoViewer() {
	delete(rightCapture);
	delete(leftCapture);
}