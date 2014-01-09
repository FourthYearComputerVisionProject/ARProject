/**
	HandDection - detect the hand and provide recognition for gestures
	
	Author: Karen Madore
	Date:	Jan 2, 2013
	Version: 0.03

*/

#ifndef HAND_DETECTION_H
#define HAND_DETECTION_H

#include "stdafx.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

const int WINDOW_HEIGHT=480;
const int WINDOW_WIDTH=640;		

class HandDetection
{
	public:
		
		//constructors
		HandDetection();
		~HandDetection();
		void runDetection(Mat &camera);

	private:
		Mat camera;			//feed from the camera 
		Mat thresholdImage;	//threshold image
		vector< vector<Point> > contours;
		vector<Point> hullPoints;
		vector<int> hullIndexes;
		vector <Vec4i> hierarchy;
		vector<Point> cleanHullPoints; //cleaned up hull Points w/o duplicate points at the fingertips

		//minimum and maximum object area
		static const int MIN_OBJECT_AREA = 11047;
		static const int MAX_OBJECT_AREA = 83721;
		static const int MIN_DISTANCE_BETWEEN_HULLS = 15;
						
		int numFingers;

		//function declarations
		void removeNoise(Mat srcDstImage);
		void makeThreshold();
		void makeContours();
		void cleanUpContours(int index);
		void lookForHands(int index);
		void HandDetection::displayWindow();
			
		//util
		double distanceBetweenTwoPoints(Point from, Point to);
		Point averageTwoPoints(Point pt1, Point pt2);
		int angleFromThreePoints(Point ptO, Point ptQ, Point ptM);
		string intToString(int number);

}; //HandDetection

#endif