#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImageChange.h"
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{

	//test();

	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	//Mat s = (Mat_<float>(2, 3) << 0.5, 0, 0, 0, 0.5, 0);
	//Mat dst1;
	//warpAffine(I, dst1, s, Size(I.cols / 2, I.rows / 2));

	//Mat dst2;
	//resize(I, dst2, Size(I.cols / 2, I.rows / 2), 0.5, 0.5);

	//imshow("I", I);
	//imshow("wrapAffine", dst1);
	//imshow("resize", dst2);


	Mat rImg;
	rotate(I, rImg, ROTATE_90_CLOCKWISE);
	//rotate(I, rImg, ROTATE_180);
	//rotate(I, rImg, ROTATE_90_COUNTERCLOCKWISE);


	imshow("Ô­Í¼", I);
	imshow("Ðý×ª", rImg);

	waitKey(0);

	return  cin.get();
}