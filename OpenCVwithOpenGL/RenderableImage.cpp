#include "stdafx.h"
#include "RenderableImage.h"

void RenderableImage::drawImage() {
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

RenderableImage::RenderableImage(const std::string filename, GLdouble xOffset, GLdouble yOffset, GLdouble zOffset)
	: xOffset(xOffset), yOffset(yOffset), zOffset(zOffset)
{
	cv::Mat material = cv::imread(filename, -1);

	glGenTextures(1, &texture); // Create The Texture

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, material.cols, material.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, material.data);
}

RenderableImage::~RenderableImage() {
	glDeleteTextures(1, &texture);
}