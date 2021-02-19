#ifndef __THRESH_CONTROL_
#define __THRESH_CONTROL_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "GrayHist.h"

using namespace std;
using namespace cv;


int threshTest();

int threshTwoPeaks(const Mat & image, Mat & thresh_out);

int threshTwoPeaksTest();

//error
int otsu(const Mat & image, Mat & OtsuThreshImage);

int otsuTest();

enum METHOD
{
	MEAN,
	GAUSS,
	MEDIAN
};

Mat adaptiveThresh(Mat I, int radius, float ratio, METHOD method = MEAN);

int adaptiveThreshTest();

int and_or_Test();

#endif
