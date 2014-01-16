#include "stdafx.h"

#include "VideoDrawManipulator.h"

#define BUF_SIZE 255

static HANDLE mutex;

VideoDrawManipulator::VideoDrawManipulator(std::string file, int x, int y) : x(x), y(y), source(file)
{
	mutex = CreateMutex(NULL, false, NULL);
	/*glBindTexture(GL_TEXTURE_2D, leftTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &rightTexture); // Create The Texture

	glBindTexture(GL_TEXTURE_2D, rightTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	leftXOffset = 0;
	leftYOffset = 0;
	leftZOffset = 2;

	rightXOffset = 0.5;
	rightYOffset = 0;
	rightZOffset = 2;*/
	thread = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
			bufferFunction,       // thread function name
            this,          // argument to thread function 
            0,                      // use default creation flags 
			&threadID);
}

VideoDrawManipulator::~VideoDrawManipulator()
{
	TerminateThread(thread, 0);
}

DWORD WINAPI VideoDrawManipulator::bufferFunction(LPVOID lpParam)
{
	HANDLE hStdout;

    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;

    // Make sure there is a console to receive output results. 

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;
	while(true)
	{
		WaitForSingleObject(mutex, INFINITE);
		if(((VideoDrawManipulator*)lpParam)->frameBuffer.size() < 240)
		{
			((VideoDrawManipulator*)lpParam)->source.update();
			cv::Mat leftImg, rightImg;
			cv::Mat leftResize;
			leftImg = ((VideoDrawManipulator*)lpParam)->source.getLeftImage();
			cv::resize(leftImg, leftResize, cv::Size(200, 200), CV_INTER_LINEAR);
			cv::cvtColor(leftResize, leftResize, CV_BGR2BGRA);
			//rightImg = ((VideoDrawManipulator*)lpParam)->source.getRightImage();
			((VideoDrawManipulator*)lpParam)->frameBuffer.push_back(leftResize);
			//((VideoDrawManipulator*)lpParam)->frameBuffer.push_back(rightImg);
		}
		ReleaseMutex(mutex);
	}
}

void VideoDrawManipulator::manipulate(cv::Mat leftImage, cv::Mat rightImage)
{
	if(!leftImage.data || !rightImage.data)
	{
		return;
	}
	WaitForSingleObject(mutex, INFINITE);
	if(frameBuffer.size() < 120)
	{
		ReleaseMutex(mutex);
		return;
	}
	//source.update();

	cv::Mat left = frameBuffer.front();
	frameBuffer.pop_front();
	//cv::Mat right = frameBuffer.front();
	//frameBuffer.pop_front();
	ReleaseMutex(mutex);

	cv::Mat resizeLeft;
	//cv::Mat resizeRight;

	//cv::resize(left, resizeLeft, cv::Size(200, 200), CV_INTER_LINEAR);
	//cv::resize(right, resizeRight, cv::Size(200, 200), CV_INTER_LINEAR);

	//cv::cvtColor(resizeRight, resizeRight, CV_BGR2BGRA);

	double alpha = 0.7;

	double beta = 1.0 - alpha;

	cv::Rect rect = cv::Rect(x, y, 400, 400);

	//leftImage.adjustROI(0 - y, 0 - (leftImage.rows - (y + resizeLeft.rows)), 0 - x, 0 - (leftImage.cols - (x + resizeLeft.cols)));
	//rightImage.adjustROI(0 - y, 0 - (rightImage.rows - (y + right.rows)), 0 - x, 0 - (rightImage.cols - (x + right.cols)));

	cv::Mat leftROI = leftImage(rect);
	cv::Mat rightROI = rightImage(rect);

	cv::addWeighted(left, alpha, leftROI, beta, 0.0, leftROI);
	cv::addWeighted(left, alpha, rightROI, beta, 0.0, rightROI);

	

	//leftImage.adjustROI(y, leftImage.rows - (y + resizeLeft.rows), x, leftImage.cols - (x + resizeLeft.cols));
	//rightImage.adjustROI(y, rightImage.rows - (y + right.rows), x, rightImage.cols - (x + right.cols));

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, leftTexture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(program, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, left.cols, left.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, left.data);

	glBindTexture(GL_TEXTURE_2D, leftTexture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + leftXOffset, 0.0 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.25 + leftXOffset, 0.0 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + leftXOffset, 0.5 + leftYOffset, 0.0 + leftZOffset);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.25 + leftXOffset, 0.5 + leftYOffset, 0.0 + leftZOffset);

	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rightTexture); //bind the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(program, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, right.cols, right.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, right.data);

	glBindTexture(GL_TEXTURE_2D, rightTexture); //bind the texture
	glBegin(GL_TRIANGLE_STRIP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexCoord2d(0.0, 1.0); glVertex3d(0.0 + rightXOffset, 0.0 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(1.0, 1.0); glVertex3d(0.25 + rightXOffset, 0.0 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0 + rightXOffset, 0.5 + rightYOffset, 0.0 + rightZOffset);
	glTexCoord2d(1.0, 0.0); glVertex3d(0.25 + rightXOffset, 0.5 + rightYOffset, 0.0 + rightZOffset);

	glEnd();*/
}