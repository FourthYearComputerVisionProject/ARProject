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
#include <math.h>

#define PI 3.14159
HandDetector::HandDetector(void)
{
	doneCalibration = false;
	doCalibration = true;
}

void HandDetector::handleEvent(BaseEvent* evt)
{
	if(evt->getType() == CALIBRATION_RESULTS_EVENT) 
	{	
		doneCalibration=true;

		CalibrationResultsEvent* cre = (CalibrationResultsEvent*) evt;
		if(MANUAL_CALIBRATION){
			/* from automatic calibration with face in medium daylight in my room
			hue_min = 0;
			hue_max = 57;
			saturation_min = 0;
			saturation_max = 136;
			value_min = 0;
			value_max = 154;
			
			//sample of hand in my room in dim light at night
			hue_min = 0;
			hue_max = 32;
			saturation_min =0;
			saturation_max = 255;
			value_min = 0;
			value_max = 255;
			*/
			//sample of hand in project room on desk far away from windows
			hue_min = 0;
			hue_max = 49;
			saturation_min =105;
			saturation_max = 255;
			value_min = 96;
			value_max = 255;

		}else{
			hue_min = cre->getHueMin();
			hue_max = cre->getHueMax();
			saturation_min = cre->getSaturationMin();
			saturation_max = cre->getSaturationMax();
			value_min = cre->getValueMin();
			value_max = cre->getValueMax();
		}
	}
}

void HandDetector::detect(cv::Mat leftImage, cv::Mat rightImage){
	if(doneCalibration)
	{
		//Run the detection routine only after calibration is done
		camera = leftImage;
		Point leftPoint = runDetection();
		//cout << "left point " << leftPoint <<endl;
		camera = rightImage;
		Point rightPoint = runDetection();

		//---- show a cursor or click icon if hand is detected ----
		SinglePointEvent* spEvent = new SinglePointEvent(leftPoint, rightPoint);
		EventManager::getGlobal()->fireEvent(spEvent);

	}else{
		// launch the calibration routine by firing a CalibrationEvent to 
		// the CalibrateSkinTone detector, CalibrationManipulator, ...
		if(doCalibration)
		{
			//cout << "Starting calibration" <<endl;
			CalibrationEvent* calibrationEvent = new CalibrationEvent(true);
			EventManager::getGlobal()->fireEvent(calibrationEvent);
			doCalibration=false;
		}
	}
}

/*=============== HandDetector:: runDetection ===============
	Contorl how the detection is run on the left/right image
	and return a point if we find the fingertip
*/
Point HandDetector::runDetection()
{
	Point fingerTipPos = Point(-1,-1);
	makeThreshold();
	fingerTipPos = findFingerTip();

	return fingerTipPos;
}

/*=============== HandDetector:: findFingerTips() ===============
	info on contours:
	hierarchy[i][0] next contour of the same hierarchical level
	hierarchy[i][1] previous contour " "
	hierarchy[i][2] parent contours
	hierarchy[i][3] nested contours
  Note: hierarchy[i] negative means none exists for that corresponding element
*/
Point HandDetector::findFingerTip(){
	Point fingerTipAt = Point(-1,-1); //init to default
	Mat srcImage;

	thresholdImage.copyTo(srcImage); //preserve thresholdImage
	findContours(srcImage,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );  //generate contours, hierarchy 

	if(hierarchy.size()>0)
	{
		//hopefully not too many objects
		for (int index=0; index>=0; index=hierarchy[index][0]) 
		{			
			double area = contourArea(contours[index], false);
			if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA)//first check: is size of area in range of hand
			{  	
				Point center = centerOfMass(contours[index]);
				//make hull points and indexes
				convexHull(contours[index], hullPoints, true, true);
				convexHull(contours[index], hullIndexes, true, false);
				//make defects
				//convexityDefects(contours[index], hullIndexes, convexDefects);  //generate convexDefects given the hullIndexes and contours.
				//int cdHullcount = (int)convexDefects.size();
				int numHulls = hullPoints.size();
				if (numHulls > 1)
				{
					float distanceFromCenter = 0;
					float distanceFromCenterNew = 0;
					fingerTipAt = hullPoints.at(0);
					for(int i = 0; i < numHulls-1; i++)
					{
						distanceFromCenterNew = lengthPP(hullPoints.at(i), hullPoints.at(i+1));
						if(distanceFromCenterNew > distanceFromCenter)
						{
							fingerTipAt = hullPoints.at(i+1);
							distanceFromCenter = distanceFromCenterNew;
						}
					}
				}
			}
		}
	}
	return fingerTipAt;
}

/*=============== HandDetector:: findCenterInContour() ===============
	1. Generate contours and hierarchy.
	2. Find the center of the object using the contour.
*/ //MAY DELETE THIS....
Point HandDetector::findCenterInContour()
{
	Point centerFoundAt = Point(-1,-1); //init to default
		
	if(hierarchy.size()>0)
	{
		for (int index=0; index>=0; index=hierarchy[index][0]) 
		{			
			double area = contourArea(contours[index], false);
			
			if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA)  //first check: is size of area in range of hand
			{					
				centerFoundAt = centerOfMass(contours[index]);
			}
		}
	}
	return centerFoundAt;
}

/*=============== HandDetector:: makeHulls() ===============
	Make the hullPoints and hullIndexes for the HandDetector.

void HandDetector::makeHulls(int index){
	convexHull(contours[index], hullPoints, true, true);
	convexHull(contours[index], hullIndexes, true, false);
	int hullcount = (int)hullPoints.size();
	/*
	if(hullcount>0)
	{

		if(SHOW_WORK){ //DEBUGGING
			Point pt0 = hullPoints[hullcount-1];
			for(int i = 0; i < hullcount; i++ )
			{
				Point pt = hullPoints[i];
			
				circle(camera, pt,3,Scalar(0, 0, 255), 1, CV_AA);
				line(camera, pt0, pt, Scalar(0, 255, 0), 1, CV_AA);
				pt0 = pt;
			}
		} //END SHOW_WORK
	}
*/


/*=============== HandDetector:: makeDefects() ===============
	Generate the convexDefects given the hullIndexes and contours

void HandDetector::makeDefects(int index)
{
	// cv::Vec4i): (start_index, end_index, farthest_pt_index, fixpt_depth), 
	// where indices are 0-based indices in the original contour of the convexity
	// defect beginning, end and the farthest point, and fixpt_depth is fixed-point approximation
	// (with 8 fractional bits) of the distance between the farthest contour point and the hull. That
	// is, to get the floating-point value of the depth will be fixpt_depth/256.0

	convexityDefects(contours[index], hullIndexes, convexDefects);  //generate convexDefects given the hullIndexes and contours.
	/* does not show unless it is a manipulator
	int cdHullcount = (int)convexDefects.size();
					
	if(cdHullcount>0)
	{
		if(SHOW_WORK)
		{
			for(int idx = 0; idx < cdHullcount; idx++)
			{
				int cdIndex = convexDefects[idx][2];
				Point cdPoint = contours[index][cdIndex];
				circle(camera, cdPoint ,3,Scalar(230, 250, 42), 1, CV_AA);
			}
		}//END SHOW_WORK
	}
	*/

/*=============== HandDetector:: removeNoise ===============
  Remove noise from the image by using morphological operations
  Details: current settings uses a rectangular element area 3x3, open operation (erode, dialate), with two interations
*/
void HandDetector::removeNoise()
{
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
   	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(5,5));

	erode(thresholdImage,thresholdImage,erodeElement);
	erode(thresholdImage,thresholdImage,erodeElement);
	
	dilate(thresholdImage,thresholdImage,dilateElement);
	dilate(thresholdImage,thresholdImage,dilateElement);
}

/*=============== HandDetector:: makeThreshold() ===============
  Create the threshold image and clean up the resulting image
*/
void HandDetector::makeThreshold() //KMTODO: change this so it can handle other colours
{
	Mat image_HSV;
	
	//convert from RGB to HSV
	cvtColor(camera, image_HSV, COLOR_RGB2HSV);  
	
	//threshold via range
	inRange(image_HSV,Scalar(hue_min, saturation_min, value_min),Scalar(hue_max, saturation_max, value_max),thresholdImage);
	
	//remove noise from threshold
	removeNoise();

	if(SHOW_WORK){
		//imshow("HSV", image_HSV);
		drawHistogram(image_HSV);
		imshow("Threshold", thresholdImage);
	}
}
/*=============== HandDetector:: manipulate() ===============
	Draw fingertips and convex defects
*/
void HandDetector::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	if(SHOW_WORK){
		
		int hullCount = hullIndexes.size();
		if(hullCount>0)
		{
			Point pt0 = hullPoints[hullCount-1];
			for(int i = 0; i < hullCount; i++ )
			{
				Point pt = hullPoints[i];

				circle(leftImage, pt,3,Scalar(0, 0, 255), 1, CV_AA);
				line(leftImage, pt0, pt, Scalar(0, 255, 0), 1, CV_AA);
				pt0 = pt;
			}
		}
	} //END SHOW_WORK
}

int HandDetector::getZDepth()
{
	return 3;  //layer 3
}

void HandDetector::drawHistogram(Mat &img_hsv)
{
	bool do_manual_calibration = true;
	Mat histogram;
	Mat hsv_min_max;
		  
	int h_bins = 255; //number of bins to use in histogram for hue, saturation, value
	int s_bins = 255; 
	int v_bins = 255;
	bool uniform=true; bool accumulate=false;

	int histogramSize[] = { h_bins, s_bins, v_bins };
	
	float h_range[] = { 0, 255 };
	float s_range[] = { 0, 255 };
	float v_range[] = { 0, 255 };

	const float* histRange = (h_range);
	const float* ranges[] = { h_range, s_range, v_range };

	int channels[] = { 0, 1, 2 };
	
	//separate the image in 3 planes (H, S, and V)
	vector<Mat> hsv_planes;
	split(img_hsv, hsv_planes);
				
	/// Get the Histogram and normalize it
	
	vector<Point> histogramMean;
	vector<Point> histogramStdDev;
	
	Mat h_hist, s_hist, v_hist;
	int histSize= 255;

	//calcHist( &img_hsv, 1, channels, Mat() ,histogram, 2, histogramSize, ranges, true, false );
	calcHist( &hsv_planes[0], 1, 0, Mat(), h_hist, 1, &h_bins, &ranges[0], uniform, accumulate );
	calcHist( &hsv_planes[1], 1, 0, Mat(), s_hist, 1, &s_bins, &ranges[1], uniform, accumulate );
	calcHist( &hsv_planes[2], 1, 0, Mat(), v_hist, 1, &v_bins, &ranges[2], uniform, accumulate );

	normalize( h_hist, h_hist, 0, 255, NORM_MINMAX, -1, Mat() );
	normalize( s_hist, s_hist, 0, 255, NORM_MINMAX, -1, Mat() );
	normalize( v_hist, v_hist, 0, 255, NORM_MINMAX, -1, Mat() );
	
	// calculate the mean of the histogram and take k std dev from mean
	Scalar     h_mean, s_mean, v_mean;
	Scalar     h_stddev, s_stddev, v_stddev;
	
	cv::meanStdDev ( h_hist, h_mean, h_stddev );
	uchar       h_mean_pxl = h_mean.val[0];
	uchar       h_stddev_pxl = h_stddev.val[0]*1.2;

	cv::meanStdDev ( s_hist, s_mean, s_stddev );
	uchar       s_mean_pxl = s_mean.val[0];
	uchar       s_stddev_pxl = s_stddev.val[0] * 1.5;

	cv::meanStdDev ( v_hist, v_mean, v_stddev );
	uchar       v_mean_pxl = v_mean.val[0];
	uchar       v_stddev_pxl = v_stddev.val[0]*1.5;

	// Draw for each channel
	
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(h_hist.at<float>(i-1)) ) ,
					Point( bin_w*(i), hist_h - cvRound(h_hist.at<float>(i)) ),
					Scalar( 255, 0, 0), 2, 8, 0  );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(s_hist.at<float>(i-1)) ) ,
					Point( bin_w*(i), hist_h - cvRound(s_hist.at<float>(i)) ),
					Scalar( 0, 255, 0), 2, 8, 0  );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(v_hist.at<float>(i-1)) ) ,
					Point( bin_w*(i), hist_h - cvRound(v_hist.at<float>(i)) ),
					Scalar( 0, 0, 255), 2, 8, 0  );
	}

    imshow("Histogram", histImage );

}

/*=============== Point HandDetector::centerOfMass(vector< vector<Point> > objContour) ===============
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

/*=============== Point HandDetector::lenthPP(Point m, Point n) ===============
	Return the distance between two points
*/
float HandDetector::lengthPP(Point m, Point n){
	return sqrt(fabs( pow(m.x-n.x,2) + pow(m.y-n.y,2) )) ;  
}

/*=============== float HandDetector::angle(Point p, Point q, Point r) ===============
*/
float HandDetector::angle(Point p, Point q, Point r){
	float l1 = lengthPP(q,p);
	float l2 = lengthPP(q,r);
	float dot=(p.x-q.x)*(r.x-q.x) + (p.y-q.y)*(r.y-q.y);
	float angle = acos(dot/(l1*l2));
	angle=angle*180/PI;
	return angle;
}

HandDetector::~HandDetector(void)
{
}