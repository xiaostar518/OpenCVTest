#include "FlipTest.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int flipTest()
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	Mat dst;
	flip(I, dst, -1);

	imshow("I", I);
	imshow("dst", dst);

	waitKey(0);

	return 0;
}