#include "EdgeControl.h"

void roberts(InputArray src, OutputArray dst, int ddepth, int x, int y, int borderType)
{
	CV_Assert(!(x == 0 && y == 0));

	Mat roberts_1 = (Mat_<float>(2, 2) << 1, 0, 0, -1);
	Mat roberts_2 = (Mat_<float>(2, 2) << 0, 1, -1, 0);

	if (x != 0 && y == 0)
	{
		conv2D(src, roberts_1, dst, ddepth, Point(0, 0), borderType);
	}

	if (x == 0 && y != 0)
	{
		conv2D(src, roberts_2, dst, ddepth, Point(1, 0), borderType);
	}

}

int robertsTest()
{
	Mat image = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data)
		return -1;
	imshow("img", image);

	Mat img_roberts_1;
	roberts(image, img_roberts_1, CV_32FC1, 1, 0);

	Mat img_roberts_2;
	roberts(image, img_roberts_2, CV_32FC1, 0, 1);

	Mat abs_img_roberts_1, abs_img_roberts_2;
	convertScaleAbs(img_roberts_1, abs_img_roberts_1, 1, 0);
	convertScaleAbs(img_roberts_2, abs_img_roberts_2, 1, 0);

	imshow("135", abs_img_roberts_1);
	imshow("45", abs_img_roberts_2);

	Mat img_roberts_1_2, img_roberts_2_2;
	Mat edge;
	pow(img_roberts_1, 2.0, img_roberts_1_2);
	pow(img_roberts_2, 2.0, img_roberts_2_2);
	sqrt(img_roberts_1_2 + img_roberts_2_2, edge);

	edge.convertTo(edge, CV_8UC1);
	imshow("±ßÔµÇ¿¶È", edge);

	waitKey(0);
	return 0;
}