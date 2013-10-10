#ifndef RENDERABLEVIDEOCAPTURE_H
#define RENDERABLEVIDEOCAPTURE_H

#define DEFAULT_FRAMERATE 60

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <GL/glew.h>
#include <GL/glut.h>

class RenderableVideoCapture {

private:
	GLuint texture; //the array for our texture
	cv::VideoCapture capture;
	GLdouble xOffset;
	GLdouble yOffset;
	GLdouble zOffset;

public:
	void updateTexture(GLuint program);
	void drawCapture(void);

	RenderableVideoCapture(int captureDevice, GLdouble xOffset, GLdouble yOffset, GLdouble zOffset);
	RenderableVideoCapture(const std::string filename, GLdouble xOffset, GLdouble yOffset, GLdouble zOffset);
	~RenderableVideoCapture(void);
};

#endif