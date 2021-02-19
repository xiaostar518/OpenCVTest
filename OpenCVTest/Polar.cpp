#include "Polar.h"


Mat polar(Mat I, Point2f center, Size size, float minr, float mintheta, float thetaStep, float rStep)
{

	Mat ri = Mat::zeros(Size(1, size.height), CV_32FC1);
	for (int i = 0; i < size.height; i++)
	{
		ri.at<float>(i, 0) = minr + i*rStep;
	}

	Mat r = repeat(ri, 1, size.width);

	Mat thetaj = Mat::zeros(Size(size.width, 1), CV_32FC1);
	for (int j = 0; j < size.width; j++)
	{
		thetaj.at<float>(0, j) = mintheta + j*thetaStep;
	}

	Mat theta = repeat(thetaj, size.height, 1);

	Mat x, y;
	polarToCart(r, theta, x, y, true);

	x += center.x;
	y += center.y;

	Mat dst = 125 * Mat::ones(size, CV_8UC1);
	for (int i = 0; i < size.height; i++)
	{
		for (int j = 0; j < size.width; j++)
		{
			float xij = x.at<float>(i, j);
			float yij = y.at<float>(i, j);

			int nearestx = int(round(xij));
			int nearesty = int(round(yij));

			if ((0 <= nearestx&&nearestx < I.cols) && (0 <= nearesty&&nearesty < I.rows))
			{
				dst.at<uchar>(i, j) = I.at<uchar>(nearesty, nearestx);
			}

		}
	}

	return dst;
}

int polar_test()
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	float thetaStep = 1.0 / 4;
	float minr = 270;

	Size size(int(360 / thetaStep), 70);

	Mat dst = polar(I, Point2f(508, 503), size, minr);

	flip(dst, dst, 0);

	imshow("I", I);
	imshow("������任", dst);

	waitKey(0);

	return 0;
}