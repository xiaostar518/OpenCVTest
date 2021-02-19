#ifndef __POLAR__
#define __POLAR__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat polar(Mat I, Point2f center, Size size, float minr = 0, float mintheta = 0, float thetaStep = 1.0 / 4, float rStep = 1.0);

int polar_test();

#endif