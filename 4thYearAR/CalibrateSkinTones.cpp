#include "stdafx.h"
#include "CalibrateSkinTones.h"
#include "CalibrationResultsEvent.h"
#include "CalibrationEvent.h"
#include "IManipulator.h"
#include <opencv\cv.h>
#include "EventManager.h"

using namespace cv;
using namespace std;

CalibrateSkinTones::CalibrateSkinTones()
{
	doCalibration = false;
}

CalibrateSkinTones::~CalibrateSkinTones(void)
{
}

/*=============== ColouredBandDetector:: detect ===============
	Draw the calibration screen. Wait for 2 minutes and grab the 
	range values, put them into an event and fire it off.  
	Skin detector should grab these values and set them.
*/
void CalibrateSkinTones::detect(cv::Mat leftImage, cv::Mat rightImage){
	if(doCalibration){	
		histRange r;
		clock_t time = clock();
		clock_t diff = time - enterTime;
		float timeSec = (float)diff / (float)CLOCKS_PER_SEC;
		if(timeSec>5.0f && timeSec<6.0){
			r=makeHistogram(leftImage);
			//fire coordinates off using event manager
			CalibrationResultsEvent* calibrationEvent= new CalibrationResultsEvent(r.hue_min,r.hue_max, r.sat_min, r.sat_max, r.val_min, r.val_max );
			EventManager::getGlobal()->fireEvent(calibrationEvent);
			
			//cout<<"done calibration";
			doCalibration = false;
			
			CalibrationEvent* cTurnOffCalibration = new CalibrationEvent(false);
			EventManager::getGlobal()->fireEvent(cTurnOffCalibration);
		}
	}
}
//might remove this...
void CalibrateSkinTones::handleEvent(BaseEvent* evt)
{
	cout<< "in CalibrateSkinTones- handlEvent"<<endl;
	if(evt->getType() == CALIBRATION_EVENT) 
	{
		CalibrationEvent* spEvt = (CalibrationEvent*) evt;
		doCalibration= spEvt->getDoCalibrate();
		enterTime = clock();
		
	}
}
/*
void CalibrateSkinTones::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	if(doCalibration){
		clock_t time = clock();
		clock_t diff = time - enterTime;
		float timeSec = (float)diff / (float)CLOCKS_PER_SEC;

		drawCalibrationBox(1, 1, leftImage);
		drawCalibrationBox(1, 1, rightImage);
		//cout<< "Time running:"<< timeSec ;
		putText(leftImage,std::to_string(timeSec),Point(leftImage.size().width*.43,leftImage.size().height*.75),1,1,Scalar(0,0,255),2);
		putText(rightImage,std::to_string(timeSec),Point(rightImage.size().width*.43,rightImage.size().height*.75),1,1,Scalar(0,0,255),2);
	}
}

int CalibrateSkinTones::getZDepth()
{
	return 2;  //layer 2
}

//draws the calibration box onto the image img
void CalibrateSkinTones::drawCalibrationBox(int x, int y, cv::Mat img)
{
	if(x == -1 || y == -1)
	{
		return;
	}
	//cout << "in draw";
	Size s = img.size();
	
	putText(img,"Calibration",Point(s.width*.42,100),1,1,Scalar(0,0,255),2);
	putText(img,"PLACE YOUR",Point(s.width*.42,s.height*.5),1,1,Scalar(0,0,255),2);
	putText(img,"HAND HERE",Point(s.width*.43,s.height*.6),1,1,Scalar(0,0,255),2);
	
	rectangle(img, Point(s.width*.4, s.height*.3),Point(s.width*.6, s.height*.7),Scalar(0,0,255),2);
}
/*=============== CalibrateSkinTones:: makeHistogram ===============
	Make a histogram of the image, calculate the mean and std dev, 
	return the range 
*/
CalibrateSkinTones::histRange CalibrateSkinTones::makeHistogram(Mat &img_hsv)
{
	Mat histogram;
	//Mat img_HSV;
	Mat hsv_min_max;
	histRange range;

	//cvtColor(cameraFeed, image_HSV, COLOR_RGB2HSV);  //convert to HSV
  
	int h_bins = 255; //number of bins to use in histogram for hue, saturation, value
	int s_bins = 255; 
	int v_bins = 255;

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
	calcHist( &hsv_planes[0], 1, 0, Mat(), h_hist, 1, &h_bins, &ranges[0], true, false );
	calcHist( &hsv_planes[1], 1, 0, Mat(), s_hist, 1, &s_bins, &ranges[1], true, false );
	calcHist( &hsv_planes[2], 1, 0, Mat(), v_hist, 1, &v_bins, &ranges[2], true, false );

	normalize( h_hist, h_hist, 0, 255, NORM_MINMAX, -1, Mat() );
	normalize( s_hist, s_hist, 0, 255, NORM_MINMAX, -1, Mat() );
	normalize( v_hist, v_hist, 0, 255, NORM_MINMAX, -1, Mat() );
	
	// calculate the mean of the histogram and take k std dev from mean
	Scalar     h_mean, s_mean, v_mean;
	Scalar     h_stddev, s_stddev, v_stddev;
	
	cv::meanStdDev ( h_hist, h_mean, h_stddev );
	uchar       h_mean_pxl = h_mean.val[0];
	uchar       h_stddev_pxl = h_stddev.val[0]*2.2;

	cv::meanStdDev ( s_hist, s_mean, s_stddev );
	uchar       s_mean_pxl = s_mean.val[0];
	uchar       s_stddev_pxl = s_stddev.val[0] * 2.5;

	cv::meanStdDev ( v_hist, v_mean, v_stddev );
	uchar       v_mean_pxl = v_mean.val[0];
	uchar       v_stddev_pxl = v_stddev.val[0]*4;

	
	range.hue_min= max(0,h_mean_pxl-h_stddev_pxl);
	range.hue_max=min(255,h_mean_pxl+h_stddev_pxl);
	range.sat_min=max(0,s_mean_pxl-s_stddev_pxl);
	range.sat_max=min(255,s_mean_pxl+s_stddev_pxl);
	range.val_min=max(0,v_mean_pxl-v_stddev_pxl);
	range.val_max=min(255,v_mean_pxl+v_stddev_pxl);
	
	
	cout << endl;
	cout <<"H_mean: " << (int) h_mean_pxl<< " s.d.: " << (int) h_stddev_pxl << "(" << (int) max(0,h_mean_pxl-h_stddev_pxl) << "," << (int)  min(255,h_mean_pxl+h_stddev_pxl) <<")"<<endl;
	cout <<"S_mean: " << (int) s_mean_pxl<< " s.d.: " << (int) s_stddev_pxl << "(" << (int) max(0,s_mean_pxl-s_stddev_pxl) << "," << (int)  min(255,s_mean_pxl+s_stddev_pxl) <<")"<<endl;
	cout <<"V_mean: " << (int) v_mean_pxl<< " s.d.: " << (int) v_stddev_pxl << "(" << (int) max(0,v_mean_pxl-v_stddev_pxl) << "," << (int)  min(255,v_mean_pxl+v_stddev_pxl) <<")"<<endl;
	
	return range;
}

