#ifndef COLOURED_BAND_DETECTOR_H
#define COLOURED_BAND_DETECTOR_H

#pragma once

#include "stdafx.h"

#include "idetector.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

class ColouredBandDetector :
	public IDetector
{
public:
	ColouredBandDetector(void);
	~ColouredBandDetector(void);
	void detect(cv::Mat leftImage, cv::Mat rightImage);

	private:
		Mat camera;			//feed from the camera 
		Mat thresholdImage;	//threshold image
		vector< vector<Point> > contours;
		vector<Point> hullPoints;
		vector<int> hullIndexes;
		vector <Vec4i> hierarchy;
		vector<Point> cleanHullPoints; //cleaned up hull Points w/o duplicate points at the fingertips

		//minimum and maximum object area
		static const int MIN_OBJECT_AREA = 15;
		static const int MAX_OBJECT_AREA = 80000;
			
		//function declarations
		void runDetection();
		void removeNoise();
		void makeThreshold();
		void findCenterInContour();
		void cleanUpContours(int index);
		void lookForHands(int index);
					
		//util
		Point centerOfMass(vector<Point> objContour);
		string intToString(int number);

}; //ColouredBandDetector

#endif
