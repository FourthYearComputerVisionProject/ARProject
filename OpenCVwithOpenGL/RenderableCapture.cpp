#include "stdafx.h"
#include <stdlib.h>
#include "RenderableCapture.h"
#include "Manipulator.h"
#include <list>

RenderableCapture::RenderableCapture(int captureDevice, GLdouble xOffset, GLdouble yOffset, GLdouble zOffset)
	: xOffset(xOffset), yOffset(yOffset), zOffset(zOffset) {
	manips = *(new std::list<Manipulator*>());
	videoCapture.open(captureDevice);

	videoCapture.set(CV_CAP_PROP_FPS, DEFAULT_FRAMERATE);

	glGenTextures(1, &texture); // Create The Texture

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

RenderableCapture::~RenderableCapture(void){
	delete(&manips);
	glDeleteTextures(1, &texture);
	videoCapture.release();
}

void RenderableCapture::addManipulator(Manipulator *manip){
	for(std::list<Manipulator*>::iterator it = manips.begin(); it != manips.end(); it++){
		if(manip -> getUID() == (*it) -> getUID()){
			return;
		}
	}
	manips.push_back(manip);
}

void RenderableCapture::update(void){
	videoCapture >> capture;
	for(std::list<Manipulator*>::iterator it = manips.begin(); it != manips.end(); it++){
		if(((*it) -> getMode() & (char)0x01) != 0){
			(*it) -> update(capture);
		}
	}
}

void RenderableCapture::draw(void){
	for(std::list<Manipulator*>::iterator it = manips.begin(); it != manips.end(); it++){
		if(((*it) -> getMode() & (char)0x10) != 0){
			(*it) -> draw(capture);
		}
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(program, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, capture.cols, capture.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, capture.data);

	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + xOffset, 0.0 + yOffset, 0.0 + zOffset);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.5 + xOffset, 0.0 + yOffset, 0.0 + zOffset);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + xOffset, 1.0 + yOffset, 0.0 + zOffset);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.5 + xOffset, 1.0 + yOffset, 0.0 + zOffset);

	glEnd();
}