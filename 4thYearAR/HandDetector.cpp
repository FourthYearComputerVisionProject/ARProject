#include "stdafx.h"
#include "HandDetector.h"
#include "SinglePointEvent.h"
#include "EventManager.h"
#include "BaseEvent.h"
#include "CalibrationEvent.h"
#include "CalibrationResultsEvent.h"
#include "IEventListener.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <sstream>
#include <iostream>


HandDetector::HandDetector(void)
{
	doneCalibration = false;
	doCalibration = true;
}

void HandDetector::handleEvent(BaseEvent* evt)
{
	cout << "received calibration results"<< endl;
	
	if(evt->getType() == CALIBRATION_RESULTS_EVENT) 
	{	
		doneCalibration=true;

		CalibrationResultsEvent* cre = (CalibrationResultsEvent*) evt;

		hue_min = cre->getHueMin();
		hue_max = cre->getHueMax();
		saturation_min = cre->getSaturationMin();
		saturation_max = cre->getSaturationMax();
		value_min = cre->getValueMin();
		value_max = cre->getValueMax();
	}
}

void HandDetector::detect(cv::Mat leftImage, cv::Mat rightImage){
	if(doneCalibration){

		camera = leftImage;
		Point leftPoint = runDetection();
		camera = rightImage;
		Point rightPoint = runDetection();
		
		SinglePointEvent* spEvent = new SinglePointEvent(leftPoint, rightPoint);
		EventManager::getGlobal()->fireEvent(spEvent);

	}else{
		//fire it off only once and then wait for timeout
		if(doCalibration){
			//cout << "Starting calibration" <<endl;
			CalibrationEvent* calibrationEvent = new CalibrationEvent(true);
			EventManager::getGlobal()->fireEvent(calibrationEvent);
			doCalibration=false;
		}
	}
}

/*=============== HandDetector:: runDetection ===============
	Run the detection on the camera image
*/

Point HandDetector::runDetection()
{
	makeThreshold();
	removeNoise();
	return findCenterInContour();
}

/*=============== HandDetector:: removeNoise ===============
  Remove noise from the image by using morphological operations
  Details: current settings uses a rectangular element area 3x3, open operation (erode, dialate), with two interations
*/
void HandDetector::removeNoise()
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
void HandDetector::makeThreshold() //KMTODO: change this so it can handle other colours
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
	
	int hue_min=50;//42
	int hue_max=100;//100
	int saturation_min=61;
	int saturation_max = 181;
	int value_min = 88;
	int value_max = 200;//214
	*/


	Mat image_HSV;
	
	//convert from RGB to HSV
	cvtColor(camera, image_HSV, COLOR_RGB2HSV);  
	
	//threshold via range
	inRange(image_HSV,Scalar(hue_min, saturation_min, value_min),Scalar(hue_max, saturation_max, value_max),thresholdImage);
	
	//remove noise from threshold
	removeNoise();
	imshow("HSV", image_HSV);
	imshow("Threshold", thresholdImage);
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
Point HandDetector::findCenterInContour()
{
	Mat srcImage;
	Point objectFoundAt=0;
	thresholdImage.copyTo(srcImage); //preserve thresholdImage
	findContours(srcImage,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	if(hierarchy.size()>0){
		for (int index=0; index>=0; index=hierarchy[index][0]) {
			
			double area = contourArea(contours[index], false);

			if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA){  //first check: is size of area in range of hand
				//km temporary removed cleanUpContours(index); //for use with the hand detection code
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
Point HandDetector::centerOfMass( vector<Point> objContour)
{
	Moments m=moments(objContour,false);
	float area = m.m00;
	int x_bar= m.m10/area;
	int y_bar=(int) m.m01/area;
	return Point(x_bar, y_bar);
}

HandDetector::~HandDetector(void)
{
}