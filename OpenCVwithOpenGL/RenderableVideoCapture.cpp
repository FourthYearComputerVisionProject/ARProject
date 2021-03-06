
#include "stdafx.h"
#include "RenderableVideoCapture.h"

void RenderableVideoCapture::updateTexture() {
	cv::Mat image;
	capture >> image;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(program, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
}
	
void RenderableVideoCapture::drawCapture() {
	//MOVED TO RenderableAbst//
	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + xOffset, 0.0 + yOffset, 0.0 + zOffset);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.5 + xOffset, 0.0 + yOffset, 0.0 + zOffset);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + xOffset, 1.0 + yOffset, 0.0 + zOffset);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.5 + xOffset, 1.0 + yOffset, 0.0 + zOffset);
	glEnd();
	//END MOVED//
}

RenderableVideoCapture::RenderableVideoCapture(int captureDevice, GLdouble xOffset, GLdouble yOffset, GLdouble zOffset)
	: xOffset(xOffset), yOffset(yOffset), zOffset(zOffset) 
{
	capture.open(captureDevice);
	//capture.set(CV_CAP_PROP_EXPOSURE, 0);
	capture.set(CV_CAP_PROP_FPS, DEFAULT_FRAMERATE);

	glGenTextures(1, &texture); // Create The Texture
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

RenderableVideoCapture::RenderableVideoCapture(const std::string filename, GLdouble xOffset, GLdouble yOffset, GLdouble zOffset)
	: xOffset(xOffset), yOffset(yOffset), zOffset(zOffset)
{
	capture.open(filename);
	//capture.set(CV_CAP_PROP_EXPOSURE, 0);
	capture.set(CV_CAP_PROP_FPS, DEFAULT_FRAMERATE);

	glGenTextures(1, &texture); // Create The Texture

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


RenderableVideoCapture::~RenderableVideoCapture() {
	glDeleteTextures(1, &texture);
	capture.release();
}
