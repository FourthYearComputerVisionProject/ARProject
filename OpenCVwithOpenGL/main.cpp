
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

int main(int argc, char **argv) {

	viewer = new StereoViewer(0, 0);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(640, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Capture Window");
	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(reshapeCallback);

	glewInit();
	//viewer->setShaders();

	glutMainLoop();

	delete(viewer);

	return 0;
}