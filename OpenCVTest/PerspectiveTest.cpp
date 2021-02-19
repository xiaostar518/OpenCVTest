#include "PerspectiveTest.h"


Mat I;
Mat pI;
Point2f IPoint, pIPoint;

int i = 0, j = 0;
Point2f src[4];
Point2f dst[4];

void mouse_I(int event, int x, int y, int flags, void *param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		IPoint = Point2f(x, y);
		break;
	case CV_EVENT_LBUTTONUP:
		src[i] = IPoint;
		circle(I, src[i], 7, Scalar(0), 3);
		i += 1;
		break;
	default:
		break;
	}
}

void mouse_pI(int event, int x, int y, int flags, void *param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		pIPoint = Point2f(x, y);
		break;
	case CV_EVENT_LBUTTONUP:
		dst[j] = pIPoint;
		circle(pI, dst[j], 7, Scalar(0), 3);
		j += 1;
		break;
	default:
		break;
	}
}

int perTest()
{
	I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	pI = 255 * Mat::ones(I.size(), CV_8UC1);

	namedWindow("I", 1);
	setMouseCallback("I", mouse_I, NULL);

	namedWindow("pI", 1);
	setMouseCallback("pI", mouse_pI, NULL);

	imshow("I", I);
	imshow("pI", pI);

	while (!(i==4&&j==4))
	{
		imshow("I", I);
		imshow("pI", pI);
		if (waitKey(50) == 'q')
			break;
	}


	imshow("I", I);
	imshow("pI", pI);

	setMouseCallback("I", NULL, NULL);
	setMouseCallback("pI", NULL, NULL);

	Mat p = getPerspectiveTransform(src, dst);
	Mat result;
	warpPerspective(I, result, p, pI.size());

	imshow("投影后的效果", result);

	waitKey(0);

	return 0;
}