/*
 * VideoDrawManipulator.h
 * Header for a manpilator which draws videos.
 */

#pragma once

#include "stdafx.h"

#include <windows.h>
#include <list>

#include "GL\glew.h"
#include "GL\glut.h"

#include "IManipulator.h"
#include "StereoVideoSource.h"

class VideoDrawManipulator : public IManipulator
{
private:
	std::list<cv::Mat> frameBuffer;
	/*
	 * The texture used to render the left side of the
	 * stereo image.
	 */
	GLuint leftTexture;
	/*
	 * The X offset of the left texture.
	 */
	GLdouble leftXOffset;
	/*
	 * The Y offset of the left texture.
	 */
	GLdouble leftYOffset;
	/*
	 * The Z offset of the left texture.
	 */
	GLdouble leftZOffset;
	/*
	 * The texture used to render the right side of the
	 * stereo image.
	 */
	GLuint rightTexture;
	/*
	 * The X offset of the right texture.
	 */
	GLdouble rightXOffset;
	/*
	 * The Y offset of the right texture.
	 */
	GLdouble rightYOffset;
	/*
	 * The Z offset of the right texture.
	 */
	GLdouble rightZOffset;
	int x, y;
	StereoVideoSource source;
	HANDLE thread;
	DWORD threadID;
public:
	VideoDrawManipulator(std::string file, int x, int y);
	~VideoDrawManipulator();
	int getZDepth() { return 1; }
	static DWORD WINAPI bufferFunction(LPVOID lpParam);
	void manipulate(cv::Mat leftImage, cv::Mat rightImage);
};