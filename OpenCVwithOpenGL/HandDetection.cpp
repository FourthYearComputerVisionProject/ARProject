/**
	HandDection - detect the hand and provide recognition for gestures
	
	Written by: Karen Madore
	Date:	Jan 2, 2013
	Version: 0.03

*/


#include "stdafx.h"
#include "HandDetection.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cmath>

using namespace cv;
using namespace std;

/*=============== HandDetection:: constructor ===============
	Initialize variables, perform skin detection, etc
*/
HandDetection::HandDetection()
{
	numFingers=0;
	
}

/*=============== HandDetection:: destructor ===============
	clean up allocated memory (if any)
*/
HandDetection::~HandDetection()
{

}

/*=============== HandDetection:: runDetection ===============
  Sets the camera matrix and runs the hand detection routine
*/
void HandDetection::runDetection(Mat &c)
{
	camera=c;
	makeThreshold();
	makeContours();
}
/*=============== HandDetection:: removeNoise ===============
  Remove noise from the image by using morphological operations
  Details: current settings uses a rectangular element area 3x3, open operation (erode, dialate), with two interations
*/
void HandDetection::removeNoise(Mat srcDstImage)
{
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Point anchor = Point(-1, -1); //default
	int iterations = 2;
    
	morphologyEx(srcDstImage, srcDstImage, CV_MOP_OPEN, element,anchor,iterations);
}

/*=============== HandDetection:: makeThreshold() ===============
  Create the threshold image
*/
void HandDetection::makeThreshold()
{
	//samples in my room in dim light at night
	int hue_min=0;
	int hue_max=32;
	int saturation_min=28;
	int saturation_max = 130;
	int value_min = 124;
	int value_max = 226;
	Mat image_HSV;
	
	//convert from RGB to HSV
	cvtColor(camera, image_HSV, COLOR_BGR2HSV);  
	
	//threshold via range
	inRange(image_HSV,Scalar(hue_min, saturation_min, value_min),Scalar(hue_max, saturation_max, value_max),thresholdImage);
	
	//remove noise from threshold
	removeNoise(thresholdImage);
}

/*=============== HandDetection:: makeContours() ===============
  Create the contours from the threshold image and if area of contour is within 
  area of hand then check for a hand

  hierarchy[i][0] next contour of the same hierarchical level
  hierarchy[i][1] previous contour " "
  hierarchy[i][2] parent contours
  hierarchy[i][3] nested contours
  Note: hierarchy[i] negative means none exists for that corresponding element

*/
void HandDetection::makeContours()
{
	Mat srcImage;
	thresholdImage.copyTo(srcImage); //preserve thresholdImage
	findContours(srcImage,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	if(hierarchy.size()>0){
		for (int index=0; index>=0; index=hierarchy[index][0]) {
			
			double area = contourArea(contours[index], false);

			if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA){  //first check: is size of area in range of hand
				cleanUpContours(index);
			}
		}
	}
}

/*=============== HandDetection:: calibrateSkinToneRangeToDetect() ===============
  When the program launches, this will be used to find the range of skin tones to detect
  and therefore customize it for the current ARUser under the specific lighting conditions
*/
void HandDetection::calibrateSkinToneRangeToDetect()
{
	
	
}

/*=============== HandDetection:: lookForHands() ===============
  Look through contours and see if it matches the size of our hands
*/
void HandDetection::lookForHands(int index)
{
	
	
}

/*=============== HandDetection:: cleanUpContours() ===============
  Generate hulls and convexityDefects.  Generate a new set of cleaned up hullPoints

	cv::Vec4i): (start_index, end_index, farthest_pt_index, fixpt_depth), 
	where indices are 0-based indices in the original contour of the convexity
	defect beginning, end and the farthest point, and fixpt_depth is fixed-point approximation
	(with 8 fractional bits) of the distance between the farthest contour point and the hull. That
	is, to get the floating-point value of the depth will be fixpt_depth/256.0
*/		
void HandDetection::cleanUpContours(int index)
{
	vector<Vec4i> convexDefects;

	convexHull(contours[index], hullPoints, true, true);  //generate the hullPoints
	convexHull(contours[index], hullIndexes, true, false); //generate the hullIndexes of the contour

	int hullcount = (int)hullPoints.size();
	int distance =0;

	if(hullcount>0)
	{
		Point pt0 = hullPoints[hullcount-1];
				
		for(int i = 0; i < hullcount; i++ )
		{
			Point pt = hullPoints[i];

			//second check: generate new cleanHullPoints by checking the minimum distance between hull points
			distance = distanceBetweenTwoPoints(pt0, pt);
			
			circle(camera, pt,3,Scalar(0, 0, 255), 1, CV_AA);  //these need to move
			line(camera, pt0, pt, Scalar(0, 255, 0), 1, CV_AA); //these need to move
				
			pt0 = pt;
		}
	}

	convexityDefects(contours[index], hullIndexes, convexDefects); //generate convexDefects indexes

	int cdHullcount = (int)convexDefects.size();
					
	if(cdHullcount>0){
		for(int idx = 0; idx < cdHullcount; idx++)
		{
			int cdIndex = convexDefects[idx][2];
			Point cdPoint = contours[index][cdIndex];
			circle(camera, cdPoint ,3,Scalar(230, 250, 42), 1, CV_AA);
		}
	}

}

/*=============== HandDetection:: findOutlier(Mat image) ===============
  Find the outliers in the image.  Used to find the fingertip.
*/

Point findOutlier(Mat contour)
{
	
}
/*=============== HandDetection:: drawClickIcon(int x, int y, Mat drawTo) ===============
  draws the icon at the x,y coordinates to indicate a click at that location
*/

void drawClickIcon(int x, int y, Mat drawTo)
{
	circle(drawTo,Point(x,y),3,Scalar(255,0,0),2);
    
    line(drawTo,Point(x,y),Point(x,max(y-25,0)),Scalar(0,255,0),2);
    line(drawTo,Point(x,y),Point(x,min(y+25, WINDOW_HEIGHT)),Scalar(0,255,0),2);
    line(drawTo,Point(x,y),Point(max(x-25, 0),y),Scalar(0,255,0),2);
    line(drawTo,Point(x,y),Point(min(x+25, WINDOW_WIDTH),y),Scalar(0,255,0),2);
}

/*=============== HandDetection:: distanceBetweenTwoPoints(Point from, Point to) ===============*/
double HandDetection::distanceBetweenTwoPoints(Point from, Point to)
{
	double distance=sqrt( (to.x-from.x)*(to.x-from.x)+(to.y-from.y)*(to.y-from.y) );
	return distance;
}

/*=============== HandDetection:: averageTwoPoints(Point pt1, Point pt2) ===============*/
Point HandDetection::averageTwoPoints(Point pt1, Point pt2)
{
	int x = (pt1.x+pt2.x)/2;
	int y = (pt1.y+pt2.y)/2;
	return Point(x,y);
}
/*=============== HandDetection:: angleBetweenTwoLines(Point ptO, Point ptQ, Point ptM) ===============
  Angle is between Q-O-M  :  theta = arcCos(a.b / |a||b|)

*/
int HandDetection::angleFromThreePoints(Point ptO, Point ptQ, Point ptM)
{
	int dotProduct;
	int magnitudeOM, magnitudeOQ;
	int angle;
	int xOM, yOM, xOQ, yOQ;
	
	xOM = ptM.x-ptO.x;
	yOM = ptM.y-ptO.y;
	xOQ = ptQ.x-ptO.x;
	yOQ = ptQ.y-ptO.y;
	dotProduct = xOM*xOQ + yOM*yOQ;
	magnitudeOM = sqrt(xOM*xOM + yOM*yOM);
	magnitudeOQ = sqrt(xOQ*xOQ + yOQ*yOQ);

	angle = acos(dotProduct/(magnitudeOM*magnitudeOQ));
	return angle;
}

/*=============== HandDetection:: intToString() ===============*/
string intToString(int number){
	stringstream sStream;
	sStream << number;
	return sStream.str();
}

/*=============== HandDetection:: main ===============
  For testing HandDection program
*/

int main_HandDetection(int argc, char **argv) {
	VideoCapture video;
	Mat camera;
	HandDetection hd;

	video.open(0);
	video.set(CV_CAP_PROP_FRAME_WIDTH,WINDOW_WIDTH);
	video.set(CV_CAP_PROP_FRAME_HEIGHT,WINDOW_HEIGHT);
	
	while(1){
		video.read(camera);	
		hd.runDetection(camera);
		
		//process hand detection here
		imshow("Hand Detection", camera);
		waitKey(30);
	}
}