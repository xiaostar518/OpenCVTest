#include "GrayHist.h"



int calcGrayHist()
{
	Mat histogram = Mat::zeros(Size(256, 1), CV_32SC1);

	Mat image = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data)
		return -1;

	int rows = image.rows;
	int cols = image.cols;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int index = int(image.at<uchar>(r, c));
			histogram.at<int>(0, index) += 1;
		}
	}

	cout << "histogram = " << histogram << endl;

	return 0;
}

Mat calcGrayHist(Mat image)
{
	Mat histogram = Mat::zeros(Size(256, 1), CV_32SC1);

	int rows = image.rows;
	int cols = image.cols;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int index = int(image.at<uchar>(r, c));
			histogram.at<int>(0, index) += 1;
		}
	}

	//cout << "histogram = " << histogram << endl;

	return histogram;
}


int lineTest()
{

	Mat I = (Mat_<uchar>(2, 2) << 0, 200, 23, 4);
	Mat O;
	I.convertTo(O, CV_8UC1, 2.0, 0);

	cout << "I = " << I << endl;
	cout << "O = " << O << endl;

	return 0;
}

int lineTest1()
{
	Mat image = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data)
		return -1;

	Mat dst;

	convertScaleAbs(image, dst, 2, 0.0);


	imshow("image", image);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

int lineTest2()
{
	Mat image = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data)
		return -1;

	double Imax, Imin;
	minMaxLoc(image, &Imin, &Imax, NULL, NULL);

	double Omin = 0, Omax = 255;

	double a = (Omax - Omin) / (Imax - Imin);
	double b = Omin - a*Imin;

	Mat dst;

	convertScaleAbs(image, dst, a, b);


	imshow("image", image);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

int lineTest3()
{
	Mat image = imread("image1.png", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data)
		return -1;

	Mat dst;
	normalize(image, dst, 255, 0, NORM_MINMAX, CV_8U);
	imshow("image", image);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

int lineTest4()
{
	Mat image = imread("image1.png", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data)
		return -1;

	Mat fI;
	image.convertTo(fI, CV_64F, 1.0 / 255, 0);

	double gamma = 0.5;

	Mat O;
	pow(fI, gamma, O);

	imshow("image", image);

	imshow("O", O);


	O.convertTo(O, CV_8U, 255, 0);
	imwrite("O.jpg", 0);

	waitKey(0);
	return 0;
}

Mat equalHist(Mat image)
{
	CV_Assert(image.type() == CV_8UC1);

	int rows = image.rows;
	int cols = image.cols;

	Mat grayHist = calcGrayHist(image);

	Mat zeroCumuMoment = Mat::zeros(Size(256, 1), CV_32SC1);
	for (int p = 0; p < 256; p++)
	{
		if (p == 0)
			zeroCumuMoment.at<int>(0, p) = grayHist.at<int>(0, 0);
		else
			zeroCumuMoment.at<int>(0, p) = zeroCumuMoment.at<int>(0, p - 1) + grayHist.at<int>(0, p);
	}

	Mat outPut_q = Mat::zeros(Size(256, 1), CV_8UC1);
	float cofficient = 256.0 / (rows*cols);
	for (int p = 0; p < 256; p++)
	{
		float q = cofficient*zeroCumuMoment.at<int>(0, p) - 1;

		if (q >= 0)
			outPut_q.at<uchar>(0, p) = uchar(floor(q));
		else
			outPut_q.at<uchar>(0, p) = 0;
	}

	Mat equalHistImage = Mat::zeros(image.size(), CV_8UC1);
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int p = image.at<uchar>(r, c);
			equalHistImage.at<uchar>(r, c) = outPut_q.at<uchar>(0, p);
		}
	}

	return equalHistImage;
}

int GrayHistTest5()
{
	Mat image = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data)
		return -1;

	Mat dst = equalHist(image);

	imshow("image", image);

	imshow("dst", dst);

	waitKey(0);

	return 0;
}

int GrayHistTest6()
{
	Mat src = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!src.data)
		return -1;


	Ptr<CLAHE> clahe = createCLAHE(2.0, Size(8, 8));

	Mat dst;
	clahe->apply(src, dst);

	imshow("src", src);

	imshow("dst", dst);

	waitKey(0);

	return 0;
}