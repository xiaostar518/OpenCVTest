#ifndef __EDGE_CONTROL__
#define __EDGE_CONTROL__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "ImageControl.h"

using namespace std;
using namespace cv;

void roberts(InputArray src, OutputArray dst, int ddepth, int x = 1, int y = 0, int borderType = BORDER_DEFAULT);

int robertsTest();

#endif
