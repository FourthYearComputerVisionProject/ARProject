#include "stdafx.h"
#include "ColouredBandDetector.h"
#include "SinglePointEvent.h"
#include "EventManager.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cmath>

using namespace cv;
using namespace std;

/*=============== ColouredBandDetector:: constructor ===============
	Initialize variables
*/

ColouredBandDetector::ColouredBandDetector(void)
{
	debounce = false;
}

ColouredBandDetector::~ColouredBandDetector(void)
{
	
}

/*=============== ColouredBandDetector:: detect ===============
	Detect the brightly coloured band attached to the finger.
*/
void ColouredBandDetector::detect(cv::Mat leftImage, cv::Mat rightImage) 
{
	
	camera=leftImage;
	Point left = runDetection();
	camera=rightImage;
	Point right = runDetection();

	//fire coordinates off using event manager
	SinglePointEvent* spEvent= new SinglePointEvent(left, right);
	EventManager::getGlobal()->fireEvent(spEvent);

	if(left.x == -1 || left.y == -1)
	{
		return;
	}

	if(debounce)
	{
		clock_t time = clock();
		clock_t diff = time - enterTime;
		float timeSec = (float)diff / (float)CLOCKS_PER_SEC;
		if(timeSec > 0.25f)
		{
			debounce = false;
		}
		return;
	}

	if(boundingBox.x == -1 || boundingBox.y == -1)
	{
		boundingBox = cv::Rect(cv::Point(left.x - 25, right.y - 25), cv::Size(50, 50));
		enterTime = clock();
	}
	else
	{
		if(!boundingBox.contains(left))
		{
			cv::rectangle(leftImage, boundingBox, cv::Scalar(255, 0, 0, 255), 4);
			cv::rectangle(rightImage, boundingBox, cv::Scalar(255, 0, 0, 255), 4);
			boundingBox.x = -1;
			boundingBox.y = -1;
		}
		else
		{
			clock_t time = clock();
			clock_t diff = time - enterTime;
			float timeSec = (float)diff / (float)CLOCKS_PER_SEC;
			if(timeSec > 0.8f)
			{
				cv::rectangle(leftImage, boundingBox, cv::Scalar(0, 255, 0, 255), 4);
				cv::rectangle(rightImage, boundingBox, cv::Scalar(0, 255, 0, 255), 4);
				ChangeBoxLocationEvent* evt = new ChangeBoxLocationEvent(left.x, left.y);
				EventManager::getGlobal()->fireEvent(evt);
				boundingBox.x = -1;
				boundingBox.y = -1;
				enterTime = clock();
				debounce = true;
			}
			else
			{
				cv::rectangle(rightImage, boundingBox, cv::Scalar(255, 0, 0, 255), 4);
				cv::rectangle(leftImage, boundingBox, cv::Scalar(255, 0, 0, 255), 4);
			}
		}
	}
}

/*=============== ColouredBandDetector:: runDetection ===============
	Run the detection on the camera image
*/

Point ColouredBandDetector::runDetection()
{
	makeThreshold();
	removeNoise();
	return findCenterInContour();
}

/*=============== ColouredBandDetector:: removeNoise ===============
  Remove noise from the image by using morphological operations
  Details: current settings uses a rectangular element area 3x3, open operation (erode, dialate), with two interations
*/
void ColouredBandDetector::removeNoise()
{
	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	//Point anchor = Point(-1, -1); //default
	//int iterations = 4;
    
	//morphologyEx(srcDstImage, srcDstImage, CV_MOP_OPEN, element,anchor,iterations);

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
   	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(5,5));

	erode(thresholdImage,thresholdImage,erodeElement);
	erode(thresholdImage,thresholdImage,erodeElement);
	
	dilate(thresholdImage,thresholdImage,dilateElement);
	dilate(thresholdImage,thresholdImage,dilateElement);
}


/*=============== ColouredBandDetector:: makeThreshold() ===============
  Create the threshold image  
*/
void ColouredBandDetector::makeThreshold() //KMTODO: change this so it can handle other colours
{
	//samples in my room in dim light at night
	/*
	int hue_min=46;
	int hue_max=105;
	int saturation_min=61;
	int saturation_max = 181;
	int value_min = 88;
	int value_max = 214;
	*/

	/*
	//neon green in my room daylight
	int hue_min=47;
	int hue_max=105;
	int saturation_min=61;
	int saturation_max = 181;
	int value_min = 88;
	int value_max = 214;
	*/
	int hue_min=50;//42
	int hue_max=100;//100
	int saturation_min=61;
	int saturation_max = 181;
	int value_min = 88;
	int value_max = 200;//214
	
	Mat image_HSV;
	
	//convert from RGB to HSV
	cvtColor(camera, image_HSV, COLOR_RGB2HSV);  
	
	//threshold via range
	inRange(image_HSV,Scalar(hue_min, saturation_min, value_min),Scalar(hue_max, saturation_max, value_max),thresholdImage);
	
	//remove noise from threshold
	removeNoise();
	//imshow("Threshold", thresholdImage);
}

/*=============== ColouredBandDetector:: makeContours() ===============
  Create the contours from the threshold image and if area of contour is within 
  area of hand then check for a hand

  hierarchy[i][0] next contour of the same hierarchical level
  hierarchy[i][1] previous contour " "
  hierarchy[i][2] parent contours
  hierarchy[i][3] nested contours
  Note: hierarchy[i] negative means none exists for that corresponding element

*/
Point ColouredBandDetector::findCenterInContour()
{
	Mat srcImage;
	Point objectFoundAt=0;
	thresholdImage.copyTo(srcImage); //preserve thresholdImage
	findContours(srcImage,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	if(hierarchy.size()>0){
		for (int index=0; index>=0; index=hierarchy[index][0]) {
			
			double area = contourArea(contours[index], false);

			if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA){  //first check: is size of area in range of hand
				//cleanUpContours(index); //for use with the hand detection code
				objectFoundAt = centerOfMass(contours[index]);
				return objectFoundAt;
								
			}
		}
	}
	return Point(-1,-1);
}

/*=============== Point ColouredBandDetector::centerOfMass(vector< vector<Point> > objContour) ===============
  Find the center of mass given the object contour
*/
Point ColouredBandDetector::centerOfMass( vector<Point> objContour)
{
	Moments m=moments(objContour,false);
	float area = m.m00;
	int x_bar= m.m10/area;
	int y_bar=(int) m.m01/area;
	return Point(x_bar, y_bar);
}