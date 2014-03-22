#ifndef HAND_DETECTOR_H
#define HAND_DETECTOR_H

#pragma once

#include "stdafx.h"
#include "ChangeBoxLocationEvent.h"
#include "idetector.h"
#include "IEventListener.h"
#include "IManipulator.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

class HandDetector:
	public IDetector, public IEventListener, public IManipulator
{
public:
	HandDetector(void);
	~HandDetector(void);
	void detect(cv::Mat leftImage, cv::Mat rightImage);
	void handleEvent(BaseEvent* evt);
	void manipulate(cv::Mat leftImage, cv::Mat rightImage);
	int getZDepth();

private:
	cv::Rect boundingBox;

	bool doCalibration;
	bool doneCalibration;
	int hue_min, hue_max;
	int saturation_min, saturation_max;
	int value_min, value_max;

	int numberOfDefects;

	static const bool MANUAL_CALIBRATION = false;
	static const bool SHOW_WORK = true;

	Mat camera;			//feed from the camera 
	Mat thresholdImage;	//threshold image: used throughout HandDetector
	
	vector< vector<Point> > contours;
	vector <Vec4i> hierarchy;

	vector<Point> hullPoints;
	vector<int> hullIndexes;
	vector<Point> cleanHullPoints; //cleaned up hull Points w/o duplicate points at the fingertips
	//vector<vector<Vec4i>> convexDefects;
	//vector <Point> fingers;
		
	//minimum and maximum object area
	static const int MIN_OBJECT_AREA = 11047;
	static const int MAX_OBJECT_AREA = 83721;
				
	//function declarations
	Point runDetection(); //--

	void removeNoise(); //--
	void makeThreshold(); //--
	void drawHistogram(Mat &img_hsv);
	Point findCenterInContour(); //--
	Point findFingerTip(); //--
	
	//util
	float lengthPP(Point m, Point n);
	float angle(Point p, Point q, Point r);
	Point centerOfMass(vector<Point> objContour); //--
	
};
#endif
