// ARProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;

int main()
{
	//create an image to store the video screen grab 
	Mat leftImage;
	Mat	rightImage;

	//setup the video capture method using the left and right webcams
	VideoCapture left;
	left.open(0);

	VideoCapture right;
	right.open(0);

	//create the window that will show the video feed
	namedWindow("left eye", 1);
	namedWindow("right eye", 1);

	//create a loop to update the image with the webcam image capture
	while (1)
	{
		//grab a frame from the video cameras
		left >> leftImage;
		right >> rightImage;

		//show the image on the screen
		imshow("left eye", leftImage);
		imshow("right eye", rightImage);

		//create a 33ms delay
		waitKey(33);
	}

	return 0;
}