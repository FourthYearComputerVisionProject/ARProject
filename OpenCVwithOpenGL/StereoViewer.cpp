// OpenCVwithOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "StereoViewer.h"
#include "BoxManip.h"

using namespace cv;


void StereoViewer::display(void) {

	//TODO Do update on captures
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D); //enable 2D texturing

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glUseProgram(0); //set no shader

	StereoViewer::rightImg->update();
	StereoViewer::leftImg->update();

	StereoViewer::rightImg->draw();
	StereoViewer::leftImg->draw();

	glUseProgram(0);

	glutSwapBuffers();
}

void StereoViewer::reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -5, 5);
	glMatrixMode(GL_MODELVIEW);
}


void StereoViewer::printShaderLog(int shader, char* shaderName) {
	int len = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 1)
	{
		int n = 0;
		char* buffer = (char *) malloc(len);
		if (!buffer) std::cout << shaderName << (":\nCannot allocate %d bytes of text for shader log\n", len);
		glGetShaderInfoLog(shader, len, &n, buffer);

#ifdef _DEBUG
		std::cout << shaderName << ":\n" << buffer;
#endif

		free(buffer);
	}
	glGetShaderiv(shader, GL_COMPILE_STATUS, &len);
}


void StereoViewer::setShader(char* filename, GLuint shader, GLenum type, GLuint program) {
	shader = glCreateShader(type);

	char* file = textFileRead(filename);
	const char* constFile = file;

	glShaderSource(shader, 1, &constFile, NULL);
	free(file);
	glCompileShader(shader);

#ifdef _DEBUG
	printShaderLog(shader, filename); //check for errors
#endif

	glAttachShader(program, shader);

	//glLinkProgram(program);

	//glDeleteShader(shader); //flag shader for deletion once it is detached from the program
}
StereoViewer::StereoViewer(int leftDevice, int rightDevice) {

	StereoViewer::rightImg = new RenderableCapture(rightDevice, 0.5, 0, 1);
	StereoViewer::leftImg = new RenderableCapture(leftDevice, 0, 0, 1);

	BoxManip *manipLeft = new BoxManip(StereoViewer::leftImg);
	BoxManip *manipRight = new BoxManip(StereoViewer::rightImg);

	StereoViewer::rightImg -> addManipulator(manipRight);
	StereoViewer::leftImg -> addManipulator(manipLeft);
	
	monochromeProgram = glCreateProgram();
	setShader("Shaders\\monochrome.vert", monochromeVertShader, GL_VERTEX_SHADER, monochromeProgram);
	setShader("Shaders\\monochrome.frag", monochromeFragShader, GL_FRAGMENT_SHADER, monochromeProgram);
	glLinkProgram(monochromeProgram);

	alphaProgram = glCreateProgram();
	setShader("Shaders\\empty.vert", alphaVertShader, GL_VERTEX_SHADER, alphaProgram);
	setShader("Shaders\\alphablend.frag", alphaFragShader, GL_FRAGMENT_SHADER, alphaProgram);
	glLinkProgram(alphaProgram);
}

StereoViewer::~StereoViewer() {
	delete(rightImg);
	delete(leftImg);

	glDeleteProgram(monochromeProgram);
	glDeleteShader(monochromeVertShader);
	glDeleteShader(monochromeFragShader);

	glDeleteProgram(alphaProgram);
	glDeleteShader(alphaFragShader);
	glDeleteShader(alphaVertShader);
}