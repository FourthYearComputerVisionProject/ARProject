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

void mouseCallback(int button, int state, int x, int y)
{
	int height = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	int width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	if(x > width)
	{
		x -= width;
	}
	if(state == GLUT_UP)
	{
		return;
	}
	ChangeBoxLocationEvent* evt = new ChangeBoxLocationEvent(x, y);

	EventManager::getGlobal()->fireEvent(evt);
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

	//cv::setOpenGlContext("Capture Window");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, 0);
	glEnable(GL_ALPHA_TEST);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutMouseFunc(mouseCallback);

	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(reshapeCallback);
	glutCloseFunc(closeCallback);

	GLenum err = glewInit();

	StereoCapture* capture = new StereoCapture(0, 0);
	
	//add detectors
	ColouredBandDetector* cbDetector = new ColouredBandDetector();
	CalibrateSkinTones* calibration = new CalibrateSkinTones();
	HandDetector* handDetector = new HandDetector();
	if (argc > 1 && strcmp(argv[1], "-cb") == 0) {
		capture->addDetector(cbDetector);
	}
	else
	{
		capture->addDetector(handDetector);
		capture->addDetector(calibration);
	}
		
	render = new StereoRender(capture);//0,0 for one cam//0,2 for rift
	

	VideoDrawManipulatorFactory* fact = new VideoDrawManipulatorFactory(render);

	//add manipulators
	SinglePointManipulator* spManipulator = new SinglePointManipulator();
	CalibrationManipulator* calibrationManipulator = new CalibrationManipulator();
	render->addManipulator(spManipulator);
	render->addManipulator(calibrationManipulator);
	render->addManipulator(handDetector);
	EventManager::getGlobal()->addListener(3, spManipulator);
	EventManager::getGlobal()->addListener(8, handDetector);
	EventManager::getGlobal()->addListener(7, calibration);
	EventManager::getGlobal()->addListener(7, calibrationManipulator);
	
	
	//glewInit();

#ifdef _DEBUG
	std::cout << "GLSL version: "<< glGetString(GL_SHADING_LANGUAGE_VERSION);
#endif

	glutMainLoop();

	//delete(render);

	return 0;
}