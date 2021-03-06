#ifndef MAIN_H
#define MAIN_H

#include "stdafx.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "StereoViewer.h"

void displayCallback();
void idleCallback();
void reshapeCallback(int w, int h);
void closeCallback();
int main(int argc, char **argv);

#endif