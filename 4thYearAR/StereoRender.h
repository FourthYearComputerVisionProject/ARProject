/*
 * StereoRender.h
 * Header file for StereoRender.cpp
 * Defines a renderer for rendering stereo images.
 */

#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <list>

#include <GL/glew.h>
#include <GL/glut.h>

#include "IStereoSource.h"
#include "IManipulator.h"
#include "HUDManipulator.h"

/*
 * Definition of a stereo renderer.
 */
class StereoRender {
private:
	//FIXME Matt, document these variables
	GLuint monochromeVertShader;
	GLuint monochromeFragShader;
	GLuint monochromeProgram;

	//FIXME Matt, document these variables
	GLuint alphaVertShader;
	GLuint alphaFragShader;
	GLuint alphaProgram;

	//FIXME Matt, document these variables
	GLuint frameBuffer;
	GLuint frameBuffer_texture;
	GLuint depthBuffer;
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
	/*
	 * The list of manipulators who want to manipulate the
	 * output before it's rendered.
	 */
	std::list<IManipulator*> manipulatorList;
	//FIXME Matt, document these variables
	int width;
	int height;
	/*
	 * The source of the stereo images.
	 */
	ISteroSource* source;

	//FIXME Matt, document these functions
	void printShaderLog(int shader, char* shaderName);
	void stereoWarp(GLuint outFBO);
	
public:
	/*
	 * Add a manipulator to this renderer.
	 * Argument manipulator: The manipulator to add.
	 * Return: True if the manipulator was added, false otherwise.
	 */
	bool addManipulator(IManipulator* manipulator);
	/*
	 * Remove a manipulator to this renderer.
	 * Argument manipulator: The manipulator to remove.
	 * Return: True if the manipulator was removed, false otherwise.
	 */
	bool removeManipulator(IManipulator* manipulator);

	//FIXME Matt, document these functions
	void setShader(char* filename, GLuint shader, GLenum type, GLuint program);
	void reshape(int w, int h);
	/*
	 * Perform a display cycle.
	 * A display cycle consists of updating the stereo source,
	 * letting manipulators perform their operations, and
	 * then drawing to the screen.
	 */
	void display(void);
	
	/*
	 * Constructor
	 * Argument s: The source of stereo images
	 */
	StereoRender(ISteroSource* s);
	/*
	 * Deconstructor
	 */
	~StereoRender();
};