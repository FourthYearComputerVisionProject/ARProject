// ARProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>
//#include "OVR.h"

using namespace cv;

int main()
{
	int FPS = 30;
	int videoHeight = 480;
	int videoWidth = 640;
	int renderHeight = 800;
	int renderWidth = 1280;

	//create an image to store the video screen grab 
	//Mat combinedImage(videoWidth*2,videoHeight,CV_8U);
	Mat combinedImage(1500,15000, CV_8U);
	Mat leftROI;
	Mat	rightROI;

	//setup the video capture method using the left and right webcams
	VideoCapture leftCapture;
	leftCapture.open(0);
	//leftCapture.set(CV_CAP_PROP_FRAME_WIDTH, videoWidth);
	//leftCapture.set(CV_CAP_PROP_FRAME_HEIGHT, videoHeight);
	leftCapture.set(CV_CAP_PROP_FPS, FPS);
	std::cout << "the width: " << leftCapture.get(CV_CAP_PROP_FRAME_WIDTH) << "\n";
	std::cout << "the height: " << leftCapture.get(CV_CAP_PROP_FRAME_HEIGHT) << "\n";
	std::cout << "the fps: " << leftCapture.get(CV_CAP_PROP_FPS) << "\n";

	VideoCapture rightCapture;
	rightCapture.open(0);
	//rightCapture.set(CV_CAP_PROP_FRAME_WIDTH, videoWidth);
	//rightCapture.set(CV_CAP_PROP_FRAME_HEIGHT, videoHeight);
	rightCapture.set(CV_CAP_PROP_FPS, FPS);

	//create the window that will show the video feed
	namedWindow("right", CV_WINDOW_NORMAL);

	//create the window that will show the video feed
	namedWindow("left", CV_WINDOW_NORMAL);

	//create a loop to update the image with the webcam image capture
	while (1)
	{

		//grab a frame from the video cameras
		leftCapture >> leftROI;
		rightCapture >> rightROI;

		//leftROI.copyTo(combinedImage,combinedImage(Rect(0, 0, videoWidth, videoHeight)));
		//rightROI.copyTo(combinedImage,combinedImage(Rect(videoWidth, 0, videoWidth, videoHeight)));

		//show the image on the screen
		imshow("right", rightROI);
		imshow("left", leftROI);
		//create a 33ms delay
		waitKey(1);
	}

	return 0;
}