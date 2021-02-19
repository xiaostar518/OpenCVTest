#ifndef __GRAY_HIST__
#define __GARY_HIST__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;


int calcGrayHist();

Mat calcGrayHist(Mat image);

int lineTest();

int lineTest1();

int lineTest2();

int lineTest3();

int lineTest4();

Mat equalHist(Mat image);

int GrayHistTest5();

int GrayHistTest6();


#endif // !__GRAY_HIST__