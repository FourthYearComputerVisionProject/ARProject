#ifndef RENDERABLEIMAGE_H
#define RENDERABLEIMAGE_H

#include "stdafx.h"

#include "cv.h"
#include "highgui.h"

#include <GL/glew.h>
#include <GL/glut.h>

class RenderableImage {

private:
	GLuint texture; //the array for our texture
	GLdouble xOffset;
	GLdouble yOffset;
	GLdouble zOffset;

public:
	void drawImage(void);

	RenderableImage(const std::string filename, GLdouble xOffset, GLdouble yOffset, GLdouble zOffset);
	~RenderableImage(void);
};

#endif