#ifndef HAND_DETECTOR_H
#define HAND_DETECTOR_H

#pragma once

#include "stdafx.h"
#include "ChangeBoxLocationEvent.h"
#include "idetector.h"
#include "IEventListener.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

class HandDetector:
	public IDetector, public IEventListener
{
public:
	HandDetector(void);
	~HandDetector(void);
	void detect(cv::Mat leftImage, cv::Mat rightImage);
	void handleEvent(BaseEvent* evt);
private:
	cv::Rect boundingBox;
	bool doCalibration;
	bool debounce;
	bool doneCalibration;
	int hue_min, hue_max;
	int saturation_min, saturation_max;
	int value_min, value_max;

	Mat camera;			//feed from the camera 
	Mat thresholdImage;	//threshold image
	vector< vector<Point> > contours;
	vector<Point> hullPoints;
	vector<int> hullIndexes;
	vector <Vec4i> hierarchy;
	vector<Point> cleanHullPoints; //cleaned up hull Points w/o duplicate points at the fingertips

	//minimum and maximum object area
	static const int MIN_OBJECT_AREA = 200;
	static const int MAX_OBJECT_AREA = 80000;
			
	//function declarations
	Point runDetection(); //--
	void removeNoise(); //--
	void makeThreshold(); //--
	Point findCenterInContour(); //--
	//void cleanUpContours(int index);
	//void lookForHands(int index);
					
	//util
	Point centerOfMass(vector<Point> objContour); //--
	//string intToString(int number);

};
#endif
