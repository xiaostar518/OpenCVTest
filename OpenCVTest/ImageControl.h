#ifndef __IMAGE_CONTROL__
#define __IMAGE_CONTROL__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void sepConv2D_X_Y(InputArray src, OutputArray src_kerX_kerY, int ddepth, InputArray kernelY, InputArray kernerX, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT);

void sepConv2D_Y_X(InputArray src, OutputArray src_kerY_kerX, int ddepth, InputArray kernelY, InputArray kernerX, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT);

void conv2D(InputArray src, InputArray kernel, OutputArray dst, int ddepth, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT);

void imageControlTest();

Mat gaussBlur(const Mat &image, Size winSize, float sigma, int ddepth = CV_64F, Point anchor = Point(-1,-1), int borderType = BORDER_DEFAULT);

int gaussTest();

Mat fastMeanBlur(Mat img, Size winSize, int borderType, Scalar value = Scalar());

int blurTest();

Mat medianSmooth(const Mat &I, Size size, int borderType = BORDER_DEFAULT);

int medianSmoothTest();

Mat getClosenessWeight(double sigma_g, Size size);

Mat bfltGray(const Mat & image, Size winSize, float sigma_g, float sigma_d);

int bfltGaryTest();

Mat joinBLF(Mat I, Size size, float sigma_g, float sigma_d, int borterType = BORDER_DEFAULT);

int joinBLFTest();

Mat guidedFilter(Mat I, Mat p, int r, float eps, float s);

int guidedFilterTest();

#endif
