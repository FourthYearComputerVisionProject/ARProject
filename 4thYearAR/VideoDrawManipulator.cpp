#include "stdafx.h"

#include "VideoDrawManipulator.h"

VideoDrawManipulator::VideoDrawManipulator(std::string file, int x, int y) : x(x), y(y), source(file)
{
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

	leftXOffset = 0;
	leftYOffset = 0;
	leftZOffset = 2;

	rightXOffset = 0.5;
	rightYOffset = 0;
	rightZOffset = 2;
}

void VideoDrawManipulator::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	source.update();

	cv::Mat left = source.getLeftImage();
	cv::Mat right = source.getRightImage();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, leftTexture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(program, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, left.cols, left.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, left.data);

	glBindTexture(GL_TEXTURE_2D, leftTexture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + leftXOffset, 0.0 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.25 + leftXOffset, 0.0 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + leftXOffset, 0.5 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.25 + leftXOffset, 0.5 + leftYOffset, 0.0 + leftZOffset);

	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rightTexture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(program, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, right.cols, right.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, right.data);

	glBindTexture(GL_TEXTURE_2D, rightTexture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + rightXOffset, 0.0 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.25 + rightXOffset, 0.0 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + rightXOffset, 0.5 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.25 + rightXOffset, 0.5 + rightYOffset, 0.0 + rightZOffset);

	glEnd();
}