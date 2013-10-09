// OpenCVwithOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "StereoViewer.h"

using namespace cv;


void StereoViewer::display(void) {

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

	StereoViewer::rightCapture->updateTexture();
	StereoViewer::leftCapture->updateTexture();

	StereoViewer::rightCapture->drawCapture();
	StereoViewer::leftCapture->drawCapture();

	glUseProgram(alphaProgram);
	if (mode == 0) {		

		glPushMatrix();
			glTranslated(0.125, 0.25, 0);
			glScaled(0.5, 0.28, 1);
			StereoViewer::image->drawImage();
		glPopMatrix();

		glPushMatrix();
			glTranslated(0.625, 0.25, 0);
			glScaled(0.5, 0.28, 1);
			StereoViewer::image->drawImage();
		glPopMatrix();

	}
	else {
		StereoViewer::video->updateTexture();
		glPushMatrix();
			glTranslated(0.25, 0.25, 0);
			glScaled(0.4, 0.3, 1);
			StereoViewer::video->drawCapture();
		glPopMatrix();
		glPushMatrix();
			glTranslated(0.75, 0.25, 0);
			glScaled(0.4, 0.3, 1);
			StereoViewer::video->drawCapture();
		glPopMatrix();
	}
	glUseProgram(0);
	
	/*
	//glBindVertexArray();
	int tLoc = 0;
	glViewport(0, 0, width, height);

	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffer_texture);

	// bind the shader for the right eye //
	glUseProgram(rightShaderProgram);
	tLoc = glGetUniformLocation(oculusRightGeomShader, "Texture");
	glUniform1i(tLoc, 0);
	glDrawArrays(GL_POINTS, 0, 1);

	// bind the shader for the left eye //
	glUseProgram(leftShaderProgram);
	tLoc = glGetUniformLocation(oculusLeftGeomShader, "Texture");
	glUniform1i(tLoc, 0);
	glDrawArrays(GL_POINTS, 0, 1);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
	*/
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

	// Create back-buffer, used for post-processing //
	/*
	// Texture //
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &frameBuffer_texture);
	glBindTexture(GL_TEXTURE_2D, frameBuffer_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Depth Buffer //
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Framebuffer to link everything together //
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "glCheckFramebufferStatus: error %p", status);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// onReshape //
	// Rescale FBO and RBO as well
	glBindTexture(GL_TEXTURE_2D, frameBuffer_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	*/
	StereoViewer::mode = 1;
	if (StereoViewer::mode == 0){
		StereoViewer::image = new RenderableImage("Images/n4g.png", 0, 0, 2);
	}
	else {
		//StereoViewer::video = new RenderableVideoCapture("http://dl.dropboxusercontent.com/u/31680566/Episode.flv", 0, 0, 2);
		StereoViewer::video = new RenderableVideoCapture("Images/gits.avi", 0, 0, 2);
	}
	
	StereoViewer::rightCapture = new RenderableVideoCapture(rightDevice, 0.5, 0, 1);
	StereoViewer::leftCapture = new RenderableVideoCapture(leftDevice, 0, 0, 1);
	
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
	delete(rightCapture);
	delete(leftCapture);
	delete(image);

	glDeleteProgram(monochromeProgram);
	glDeleteShader(monochromeVertShader);
	glDeleteShader(monochromeFragShader);

	glDeleteProgram(alphaProgram);
	glDeleteShader(alphaFragShader);
	glDeleteShader(alphaVertShader);
}