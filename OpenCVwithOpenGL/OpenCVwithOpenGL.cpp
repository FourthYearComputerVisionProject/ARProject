// OpenCVwithOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include <windows.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include <ctype.h>
#include "cv.h"
#include "highgui.h"

using namespace cv;

GLuint rightTexture; //the array for our texture
GLuint leftTexture; //the array for our texture
Mat rightCamImage;
Mat leftCamImage;
VideoCapture rightCamCapture;
VideoCapture leftCamCapture;


void videoToTexture(VideoCapture *capture, GLuint *texture) {
	Mat image;
	*capture >> image;
	glBindTexture(GL_TEXTURE_2D, *texture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.data);
}

void plane(GLuint *texture, GLfloat x, GLfloat y, GLfloat z) {
	glBegin(GL_QUADS);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + x, 0.0 + y, 0.0 + z); //with our vertices we have to assign a texcoord
	glTexCoord2d(1.0, 1.0); glVertex3d(0.5 + x, 0.0 + y, 0.0 + z); //so that our texture has some points to draw to
	glTexCoord2d(1.0, 0.0); glVertex3d(0.5 + x, 1.0 + y, 0.0 + z);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + x, 1.0 + y, 0.0 + z);

	glEnd();
}

void display(void) {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	videoToTexture(&leftCamCapture, &leftTexture);
	plane(&rightTexture, 0, 0, 0);
	videoToTexture(&rightCamCapture, &rightTexture);
	plane(&rightTexture, 0.5, 0, 0);

	glutSwapBuffers();
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);

}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1280, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("A basic OpenGL Window");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);


	rightCamCapture;
	rightCamCapture.open(0);

	leftCamCapture;
	leftCamCapture.open(0);

	glGenTextures(1, &rightTexture); // Create The Texture
	glGenTextures(1, &leftTexture);

	glBindTexture(GL_TEXTURE_2D, rightTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, leftTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glutMainLoop();

	//Free our texture
	FreeTexture(rightTexture);

	return 0;
}