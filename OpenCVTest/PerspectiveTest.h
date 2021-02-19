#ifndef __PERSPECTIVE_TEST__
#define __PERSPECTIVE_TEST__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;




void mouse_I(int event, int x, int y, int flags, void *param);

void mouse_pI(int event, int x, int y, int flags, void *param);

int perTest();

#endif
