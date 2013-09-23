#include "stdafx.h"
/*
Simple Demo for GLSL
www.lighthouse3d.com
*/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "textfile.h"

GLint timeLoc;
GLuint vertShader, fragShader, fragShader2, program;

float lpos[4] = { 1.0, 0.0, 1.0, 0.0 };

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = 1.0f* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	glFrustum(-1, 1, -1, 1, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

float a = 0;

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0, 5.0, 5.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glRotatef(a, 0, 1, 0);

	glutSolidTeapot(1);
	a += 0.1f;

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}

void setShaders() {

	char *vs = NULL, *fs = NULL, *fs2 = NULL;

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader2 = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("toonf2.vert");
	fs = textFileRead("toonf2.frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(vertShader, 1, &vv, NULL);
	glShaderSource(fragShader, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	glUseProgram(program);
	timeLoc = glGetUniformLocation(program, "time");
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Lighthouse 3D");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//	glEnable(GL_CULL_FACE);

	glewInit();

	setShaders();

	glutMainLoop();

	return 0;
}

