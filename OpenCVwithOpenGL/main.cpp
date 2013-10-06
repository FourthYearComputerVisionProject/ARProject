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

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(640, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Capture Window");

	std::string *filename = NULL;
	const std::string imgTag = "-img";

	for(int i = 0; i < argc; i++){
		if(imgTag.compare(argv[i]) == 0){
			filename = new std::string(argv[++i]);
			continue;
		}
	}
	if(filename != NULL){
		viewer = new StereoViewer(*filename);
	}
	else{
		viewer = new StereoViewer(0, 0);
	}

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(reshapeCallback);
	glutCloseFunc(closeCallback);

	glewInit();
	//viewer->setShaders();

	glutMainLoop();

	delete(viewer);

	return 0;
}