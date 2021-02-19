#include "ThreshControl.h"

int threshTest()
{
	Mat src = (Mat_<uchar>(5, 3) << 123, 234, 68, 33, 51, 17, 48, 98, 234, 129, 89, 27, 45, 167, 134);
	//Mat src = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//if (!src.data)
	//	return -1;

	//imshow("img", src);


	double thresh = 150;
	Mat dst;
	threshold(src, dst, thresh, 255, THRESH_BINARY);
	cout << "dst = " << dst << endl;
	//imshow("dst", dst);

	double otsuThresh = 0;
	Mat dst_Otsu;
	otsuThresh = threshold(src, dst_Otsu, otsuThresh, 255, THRESH_OTSU + THRESH_BINARY);
	cout << "otsuThresh = " << otsuThresh << endl;
	cout << "dst_Otsu = " << dst_Otsu << endl;
	//imshow("dst_Otsu", dst_Otsu);

	double triThresh = 0;
	Mat dst_tri;
	triThresh = threshold(src, dst_tri, 0, 255, THRESH_TRIANGLE + THRESH_BINARY);
	cout << "triThresh = " << triThresh << endl;
	cout << "dst_tri = " << dst_tri << endl;
	//imshow("dst_tri", dst_tri);

	waitKey(0);
	return 0;
}

int threshTwoPeaks(const Mat & image, Mat & thresh_out)
{
	Mat histogram = calcGrayHist(image);

	Point firstPeakLoc;
	minMaxLoc(histogram, NULL, NULL, NULL, &firstPeakLoc);
	
	int firstPeak = firstPeakLoc.x;

	Mat measureDists = Mat::zeros(Size(256, 1), CV_32FC1);
	for (int k = 0; k < 256; k++)
	{
		int hist_k = histogram.at<int>(0, k);
		measureDists.at<float>(0, k) = pow(float(k - firstPeak), 2)*hist_k;
	}

	Point secondPeakLoc;
	minMaxLoc(measureDists, NULL, NULL, NULL, &secondPeakLoc);

	int secondPeak = secondPeakLoc.x;

	Point threshLoc;
	int thresh = 0;

	if (firstPeak < secondPeak)
	{
		minMaxLoc(histogram.colRange(firstPeak, secondPeak), NULL, NULL, &threshLoc);
		thresh = firstPeak + threshLoc.x + 1;
	}
	else
	{
		minMaxLoc(histogram.colRange(secondPeak, firstPeak), NULL, NULL, &threshLoc);
		thresh = secondPeak + threshLoc.x + 1;
	}

	threshold(image, thresh_out, thresh, 255, THRESH_BINARY);
	return thresh;
}

int threshTwoPeaksTest()
{
	Mat src = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!src.data)
		return -1;

	imshow("img", src);

	Mat dst;
	int thresh = threshTwoPeaks(src, dst);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

int otsu(const Mat & image, Mat & OtsuThreshImage)
{
	Mat histogram = calcGrayHist(image);

	Mat normHist;
	histogram.convertTo(normHist, CV_32FC1, 1.0 / (image.rows*image.cols), 0.0);

	Mat zeroCumuMoment = Mat::zeros(Size(256, 1), CV_32FC1);
	Mat oneCumuMoment = Mat::zeros(Size(256, 1), CV_32FC1);

	for (int i = 0; i < 256; i++)
	{
		if (i = 0)
		{
			zeroCumuMoment.at<float>(0, i) = normHist.at<float>(0, i);
			oneCumuMoment.at<float>(0, i) = i*normHist.at<float>(0, i);
		}
		else
		{
			zeroCumuMoment.at<float>(0, i) = zeroCumuMoment.at<float>(0, i - 1) + normHist.at<float>(0, i);
			oneCumuMoment.at<float>(0, i) = oneCumuMoment.at<float>(0, i - 1) + i*normHist.at<float>(0, i);
		}
	}

	Mat variance = Mat::zeros(Size(256, 1), CV_32FC1);

	float mean = oneCumuMoment.at<float>(0, 255);

	for (int i = 0; i < 255; i++)
	{
		if (zeroCumuMoment.at<float>(0, i) == 0 || zeroCumuMoment.at<float>(0, i) == 1)
		{
			variance.at<float>(0, i) = 0;
		}
		else
		{
			float cofficient = zeroCumuMoment.at<float>(0, i)*(1.0 - zeroCumuMoment.at<float>(0, i));
			variance.at<float>(0, i) = pow(mean*zeroCumuMoment.at<float>(0, i) - oneCumuMoment.at<float>(0, i), 2.0) / cofficient;
		}
	}

	Point maxLoc;
	minMaxLoc(variance, NULL, NULL, NULL, &maxLoc);
	int otsuThresh = maxLoc.x;

	threshold(image, OtsuThreshImage, otsuThresh, 255, THRESH_BINARY);

	return otsuThresh;
}

int otsuTest()
{
	Mat src = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!src.data)
		return -1;

	imshow("img", src);

	Mat OtsuThreshImage;
	int otsuThresh = otsu(src, OtsuThreshImage);
	//imshow("OtsuThreshImage", OtsuThreshImage);

	//cout << "otsuThresh = " << otsuThresh << endl;

	waitKey(0);
	return 0;
}

Mat adaptiveThresh(Mat I, int radius, float ratio, METHOD method)
{
	Mat smooth;
	switch (method)
	{
	case MEAN:
		boxFilter(I, smooth, CV_32FC1, Size(2 * radius + 1, 2 * radius + 1));
		break;
	case GAUSS:
		GaussianBlur(I, smooth, Size(2 * radius + 1, 2 * radius + 1), 0, 0);
		break;
	case MEDIAN:
		medianBlur(I, smooth, 2 * radius + 1);
		break;
	default:
		break;
	}

	I.convertTo(I, CV_32FC1);
	smooth.convertTo(smooth, CV_32FC1);

	Mat diff = I - (1.0 - ratio)*smooth;

	Mat out = Mat::zeros(diff.size(), CV_8UC1);
	for (int r = 0; r < out.rows; r++)
	{
		for (int c = 0; c < out.cols; c++)
		{
			if (diff.at<float>(r,c)>=0)
			{
				out.at<uchar>(r, c) = 255;
			}
		}
	}

	return out;
}

int adaptiveThreshTest()
{
	Mat src = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!src.data)
		return -1;

	imshow("img", src);


	Mat out = adaptiveThresh(src, 43, 0.2, MEDIAN);
	imshow("out", out);


	waitKey(0);
	return 0;
}

int and_or_Test()
{
	Mat src1 = Mat::zeros(Size(100, 100), CV_8UC1);
	cv::rectangle(src1, Rect(25, 25, 50, 50), Scalar(255), CV_FILLED);
	imshow("src1", src1);

	Mat src2 = Mat::zeros(Size(100, 100), CV_8UC1);
	cv::circle(src2, Point(75, 50), 25, Scalar(255), CV_FILLED);
	imshow("src2", src2);

	Mat dst_and;
	bitwise_and(src1, src2, dst_and);
	imshow("dst_and", dst_and);

	Mat dst_or;
	bitwise_or(src1, src2, dst_or);
	imshow("dst_or", dst_or);

	waitKey(0);
	return 0;
}