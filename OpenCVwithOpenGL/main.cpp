
#include "stdafx.h"
#include "main.h"

StereoViewer *viewer;

void displayCallback() {
	viewer->display();
}

void idleCallback() {
	viewer->display();
}

void reshapeCallback(int w, int h) {
	viewer->reshape(w, h);
}

void closeCallback(){
	glutLeaveMainLoop();
	
}

int main(int argc, char **argv) {
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

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(reshapeCallback);
	glutCloseFunc(closeCallback);

	GLenum err = glewInit();

	//glewInit();

	viewer = new StereoViewer(0, 0);

#ifdef _DEBUG
	std::cout << "GLSL version: "<< glGetString(GL_SHADING_LANGUAGE_VERSION);
#endif

	glutMainLoop();

	delete(viewer);

	return 0;
}