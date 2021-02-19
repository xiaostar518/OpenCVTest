#include "ErodeControl.h"

int erodeTest()
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	Mat s = getStructuringElement(MORPH_RECT, Size(3, 3));

	Mat E;
	erode(I, E, s, Point(-1, -1), 5);

	imshow("I", I);
	imshow("erode", E);

	waitKey(0);
	return 0;
}

int dilateTest()
{
	int r = 20;
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	imshow("I", I);

	Mat s = getStructuringElement(MORPH_RECT, Size(1, 2 * r + 1));


	Mat dst;
	dilate(I, dst, s);
	imshow("dilate", dst);

	waitKey(0);
	return 0;
}

int hatTest()
{
	int r = 20;
	int i = 10;

	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	imshow("I", I);
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * r + 1, 2 * r + 1));

	Mat d;
	morphologyEx(I, d, MORPH_BLACKHAT, element, Point(-1, -1), i);

	imshow("d", d);

	waitKey(0);
	return 0;
}