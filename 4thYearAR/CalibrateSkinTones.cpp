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
		histRange r, r1, r2, r3, r4;
		clock_t time = clock();
		clock_t diff = time - enterTime;
		
		float timeSec = (float)diff / (float)CLOCKS_PER_SEC;
		if(timeSec>5.0f && timeSec<6.0){
			Mat hsv;
			
			cvtColor(leftImage, hsv, CV_RGB2HSV);
			//large area
			cout<<"Image width and height: " <<leftImage.size().width << leftImage.size().height<<endl;
			Mat roi(hsv, Rect(Point(leftImage.size().width*.4, leftImage.size().height*.3),Point(leftImage.size().width*.6, leftImage.size().height*.7)));
			r=makeHistogram(roi);
			ranges.push_back(HistRange(r.hue_min,r.hue_max, r.sat_min, r.sat_max, r.val_min, r.val_max));
			
			//region1
			Mat roi1(hsv, Rect(Point(leftImage.size().width*.5, leftImage.size().height*.4),Point(leftImage.size().width*.5+SIDE, leftImage.size().height*.4+SIDE)));
			r1=makeHistogram(roi1);
			ranges.push_back(HistRange(r1.hue_min,r1.hue_max, r1.sat_min, r1.sat_max, r1.val_min, r1.val_max));

			//region2
			Mat roi2(hsv, Rect(Point(leftImage.size().width*.6, leftImage.size().height*.5),Point(leftImage.size().width*.6+SIDE, leftImage.size().height*.5+SIDE)));
			r2=makeHistogram(roi2);
			ranges.push_back(HistRange(r2.hue_min,r2.hue_max, r2.sat_min, r2.sat_max, r2.val_min, r2.val_max));

			//region3
			Mat roi3(hsv, Rect(Point(leftImage.size().width*.5, leftImage.size().height*.4),Point(leftImage.size().width*.5+SIDE, leftImage.size().height*.4+SIDE)));
			r3=makeHistogram(roi3);
			ranges.push_back(HistRange(r3.hue_min,r3.hue_max, r3.sat_min, r3.sat_max, r3.val_min, r3.val_max));

			//region4
			Mat roi4(hsv, Rect(Point(leftImage.size().width*.7, leftImage.size().height*.6),Point(leftImage.size().width*.7+SIDE, leftImage.size().height*.6+SIDE)));
			r4=makeHistogram(roi4);
			ranges.push_back(HistRange(r4.hue_min,r4.hue_max, r4.sat_min, r4.sat_max, r4.val_min, r4.val_max));

			//fire coordinates off using event manager
			//CalibrationResultsEvent* calibrationEvent= new CalibrationResultsEvent(r.hue_min,r.hue_max, r.sat_min, r.sat_max, r.val_min, r.val_max );
			CalibrationResultsEvent* calibrationEvent= new CalibrationResultsEvent(ranges);
			EventManager::getGlobal()->fireEvent(calibrationEvent);
			
			doCalibration = false;
			
			CalibrationEvent* cTurnOffCalibration = new CalibrationEvent(false);
			EventManager::getGlobal()->fireEvent(cTurnOffCalibration);
		}
	}
}
//might remove this...
void CalibrateSkinTones::handleEvent(BaseEvent* evt)
{
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
	Mat hsv_min_max;
	//histRange range;
	histRange range;
		  
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
	uchar       h_stddev_pxl = h_stddev.val[0]*1.2; //1.2

	cv::meanStdDev ( s_hist, s_mean, s_stddev );
	uchar       s_mean_pxl = s_mean.val[0];
	uchar       s_stddev_pxl = s_stddev.val[0]*1.5;//1.5

	cv::meanStdDev ( v_hist, v_mean, v_stddev );
	uchar       v_mean_pxl = v_mean.val[0];
	uchar       v_stddev_pxl = v_stddev.val[0];//3.5

	range.hue_min= max(0, h_mean_pxl-h_stddev_pxl);
	range.hue_max=min(MAX_HUE, h_mean_pxl+h_stddev_pxl);  //putting limits on max hue to 100
	range.sat_min=max(0, s_mean_pxl-s_stddev_pxl);
	range.sat_max=min(255, s_mean_pxl+s_stddev_pxl);
	range.val_min=max(VALUE_MIN, v_mean_pxl-v_stddev_pxl); //putting limits on min value to be at least 25
	range.val_max=min(255, v_mean_pxl+v_stddev_pxl);
	//range.val_max=225;
	
	range.hue_min=findMin(img_hsv, 0, h_mean_pxl, 255, range);
	range.hue_max=findMax(img_hsv, 0, range.hue_max, range.hue_min, range);  //start from right and go to left
	
	range.sat_min=findMin(img_hsv, 1, s_mean_pxl, 255, range);
	range.sat_max=findMax(img_hsv, 1, 255, range.sat_min, range);

	range.val_min=findMin(img_hsv, 2, v_mean_pxl, 255, range);
	range.val_max=findMax(img_hsv, 2, 255, range.val_min, range);

	cout << endl;
	
	cout <<"H_mean: " << (int) h_mean_pxl<< " s.d.: " << (int) h_stddev_pxl << " H range: (" << range.hue_min << "," << range.hue_max <<")"<<endl;
	cout <<"S_mean: " << (int) s_mean_pxl<< " s.d.: " << (int) s_stddev_pxl << " S range: (" << range.sat_min << "," << range.sat_max <<")"<<endl;
	cout <<"V_mean: " << (int) v_mean_pxl<< " s.d.: " << (int) v_stddev_pxl << " V range: (" << range.val_min << "," << range.val_max <<")"<<endl;
	
	return range;
}


//Find the min value for the given channel
int CalibrateSkinTones::findMin(Mat hsv, int channel, int startAt, int endAt, histRange r)
{
	double prevAreaDetected=0;
	double areaDetected;
	Mat thresholdImage;
	int returnVal = startAt;
		
	for(int i=startAt; i<endAt; i++)
	{
		switch(channel)
		{
		case 0:
			inRange(hsv,Scalar(i, r.sat_min, r.val_min ),Scalar(r.hue_max, r.sat_max, r.val_max),thresholdImage);

			break;
		case 1:
			inRange(hsv,Scalar(r.hue_min, i , r.val_min ),Scalar(r.hue_max, r.sat_max, r.val_max),thresholdImage);

			break;
		case 3:
			inRange(hsv,Scalar(r.hue_min, r.sat_min, i ),Scalar(r.hue_max, r.sat_max, r.val_max),thresholdImage);
			break;
		}	
		areaDetected = countNonZero(thresholdImage); //area
		//cout<<"Area Detected: " << areaDetected << " Channel: "<< channel<<  " Value_min: " << i<<endl;
		if(areaDetected<prevAreaDetected || areaDetected==0)
		{
			returnVal = i;
			break;
		}
		prevAreaDetected=areaDetected;
	}
	return returnVal;
}

int CalibrateSkinTones::findMax(Mat hsv, int channel, int startAt, int endAt, histRange r)
{
	double prevAreaDetected=0;
	double areaDetected;
	Mat thresholdImage;
	int returnVal = startAt;
	
	for(int i=startAt; i>(endAt+1); i--){
		switch(channel)
		{
		case 0: //hue max
			inRange(hsv,Scalar(r.hue_min, r.sat_min, r.val_min ),Scalar(i, r.sat_max, r.val_max),thresholdImage);

			break;
		case 1:	//saturation max
			inRange(hsv,Scalar(r.hue_min, r.sat_min , r.val_min ),Scalar(r.hue_max, i, r.val_max),thresholdImage);

			break;
		case 2: //value max
			inRange(hsv,Scalar(r.hue_min, r.sat_min, r.val_min ),Scalar(r.hue_max, r.sat_max, i),thresholdImage);
			break;
		}	
		
		areaDetected = countNonZero(thresholdImage); //area
		//cout<<"Area detected: " << areaDetected << " Channel: "<< channel<< " Value_max: " << i<<endl;
		if(prevAreaDetected>areaDetected){
			returnVal = i;
			break;
		}
		prevAreaDetected=areaDetected;
	}

	return returnVal;
}
