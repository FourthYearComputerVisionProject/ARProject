/*
 * main.cpp
 * Program entry point
 */

#include "stdafx.h"

#include "main.h"

StereoRender *render;

void displayCallback() {
	render->display();
}

void idleCallback() {
	render->display();
}

void reshapeCallback(int w, int h) {
	render->reshape(w, h);
}

void closeCallback(){
	glutLeaveMainLoop();
	
}

int main(int argc, char **argv) {
	EventManager(true);

	int screenWidth = 1366;
	int screenHeight = 768;

	int windowWidth = 1024; 
	int windowHeight = 576;

	// Center the window on the screen
	int windowPosX = (screenWidth / 2) - (windowWidth / 2);
	int windowPosY = (screenHeight / 2) - (windowHeight / 2);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutCreateWindow("Capture Window");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, 0);
	glEnable(GL_ALPHA_TEST);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(reshapeCallback);
	glutCloseFunc(closeCallback);

	GLenum err = glewInit();

	StereoCapture* capture = new StereoCapture(0, 0);

	render = new StereoRender(capture);//0,0 for one cam//0,2 for rift

	ChangeBoxLocationEvent* evt = new ChangeBoxLocationEvent(100, 100);

	EventManager::getGlobal()->fireEvent(evt);

	//glewInit();

#ifdef _DEBUG
	std::cout << "GLSL version: "<< glGetString(GL_SHADING_LANGUAGE_VERSION);
#endif

	glutMainLoop();

	delete(render);

	return 0;
}