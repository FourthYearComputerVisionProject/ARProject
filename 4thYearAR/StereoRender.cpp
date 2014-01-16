/*
 * StereoRender.cpp
 */

#include "stdafx.h"
#include <stdio.h>

#include "textfile.h"

#include "StereoRender.h"

void StereoRender::display(void) {

	source->update();

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

	cv::Mat left = source->getLeftImage();
	cv::Mat right = source->getRightImage();

	std::list<IManipulator*> toSortList;
	std::list<IManipulator*> depthSortedList;
	for(auto it = manipulatorList.begin(); it != manipulatorList.end(); ++it)
	{
		toSortList.push_back((*it));
	}
	int upperZDepth = 0;
	int zOffset = 0;
	bool sortedElement = false;
	while(!toSortList.empty())
	{
		for(auto it = toSortList.begin(); it != toSortList.end(); ++it)
		{
			if((*it)->getZDepth() == upperZDepth + zOffset)
			{
				upperZDepth = (*it)->getZDepth();
				depthSortedList.push_front((*it));
				toSortList.erase(it);
				zOffset = 0;
				sortedElement = true;
				break;
			}
		}
		if(!sortedElement)
		{
			zOffset++;
		}
		sortedElement = false;
	}
	
	for(auto it = depthSortedList.begin(); it != depthSortedList.end(); ++it)
	{
		(*it)->manipulate(left, right);
	}
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, leftTexture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(program, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, left.cols, left.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, left.data);

	glBindTexture(GL_TEXTURE_2D, leftTexture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + leftXOffset, 0.0 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.5 + leftXOffset, 0.0 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + leftXOffset, 1.0 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.5 + leftXOffset, 1.0 + leftYOffset, 0.0 + leftZOffset);

	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rightTexture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(program, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, right.cols, right.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, right.data);

	glBindTexture(GL_TEXTURE_2D, rightTexture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + rightXOffset, 0.0 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.5 + rightXOffset, 0.0 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + rightXOffset, 1.0 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.5 + rightXOffset, 1.0 + rightYOffset, 0.0 + rightZOffset);

	glEnd();

	glUseProgram(0);

	glutSwapBuffers();
	
}

bool StereoRender::addManipulator(IManipulator* manipulator)
{
	for(auto it = manipulatorList.begin(); it != manipulatorList.end(); ++it)
	{
		if(manipulator == (*it))
		{
			return false;
		}
	}
	manipulatorList.push_back(manipulator);
	return true;
}

bool StereoRender::removeManipulator(IManipulator* manipulator)
{
	for(auto it = manipulatorList.begin(); it != manipulatorList.end(); ++it)
	{
		if(manipulator == (*it))
		{
			manipulatorList.erase(it);
			return true;
		}
	}
	return false;
}

void StereoRender::reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -5, 5);
	glMatrixMode(GL_MODELVIEW);
}


void StereoRender::printShaderLog(int shader, char* shaderName) {
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


void StereoRender::setShader(char* filename, GLuint shader, GLenum type, GLuint program) {
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
StereoRender::StereoRender(ISteroSource* s) : source(s) {
	leftXOffset = 0;
	leftYOffset = 0;
	leftZOffset = 1;
	rightXOffset = 0.5;
	rightYOffset = 0;
	rightZOffset = 1;

	HUDManipulator* hudManip = new HUDManipulator();
	QRDetector* qr = new QRDetector();

	s->addDetector(qr);

	std::string vidSource = "http://dl.dropboxusercontent.com/u/31680566/Dragon.mp4";
	std::string localVidSource = "C:\\Users\\Damian\\Videos\\Janus Syndacite\\Back+to+School+with+Crackbone.mp4";

	//VideoDrawManipulator* vidManip = new VideoDrawManipulator(localVidSource, 100, 100);

	//HUDManipulator* boxManip = new DrawBoxManipulator();

	EventManager::getGlobal()->addListener(1, hudManip);

	addManipulator(hudManip);
	//addManipulator(vidManip);

	glGenTextures(1, &leftTexture); // Create The Texture

	glBindTexture(GL_TEXTURE_2D, leftTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &rightTexture); // Create The Texture

	glBindTexture(GL_TEXTURE_2D, rightTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	monochromeProgram = glCreateProgram();
	setShader("Shaders\\monochrome.vert", monochromeVertShader, GL_VERTEX_SHADER, monochromeProgram);
	setShader("Shaders\\monochrome.frag", monochromeFragShader, GL_FRAGMENT_SHADER, monochromeProgram);
	glLinkProgram(monochromeProgram);

	alphaProgram = glCreateProgram();
	setShader("Shaders\\empty.vert", alphaVertShader, GL_VERTEX_SHADER, alphaProgram);
	setShader("Shaders\\alphablend.frag", alphaFragShader, GL_FRAGMENT_SHADER, alphaProgram);
	glLinkProgram(alphaProgram);
}

StereoRender::~StereoRender() {
	glDeleteProgram(monochromeProgram);
	glDeleteShader(monochromeVertShader);
	glDeleteShader(monochromeFragShader);

	glDeleteProgram(alphaProgram);
	glDeleteShader(alphaFragShader);
	glDeleteShader(alphaVertShader);

	glDeleteTextures(1, &leftTexture);
	glDeleteTextures(1, &rightTexture);
}