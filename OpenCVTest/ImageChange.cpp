#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImageChange.h"
#include <iostream>

using namespace std;
using namespace cv;

int test()
{

	//Mat img = imread(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
	//if (!img.data)
	//	return -1;
	//imshow("原图", img);
	//waitKey(0);

	Mat mat = Mat(2, 3, CV_32FC1);
	Mat(Size(3, 2), CV_32FC1);
	Mat m;
	m.create(2, 3, CV_32FC1);
	Mat ones = Mat::ones(2, 3, CV_32FC1);
	Mat zeros = Mat::zeros(2, 3, CV_32FC1);
	Mat arr = (Mat_<int>(2, 3) << 1, 2, 3, 4, 5, 6);
	cout << "行数： " << mat.rows << endl;
	cout << "列数： " << mat.cols << endl;

	Size size = arr.size();
	cout << "尺寸:" << size << endl;
	cout << "通道数:" << arr.channels() << endl;
	cout << "面积:" << arr.total() << endl;
	cout << "维数:" << arr.dims << endl;



	for (int row = 0; row < arr.rows; row++)
	{
		const int* ptr = arr.ptr<int>(row);
		for (int col = 0; col < arr.cols; col++)
		{
			//cout << arr.at<int>(row, col) << ",";

			//cout << ptr[col] << ",";
			cout << *((int*)(arr.data + arr.step[0] * row + arr.step[1] * col)) << ",";
		}
	}

	cout << endl;


	if (arr.isContinuous())
	{
		int* ptr = arr.ptr<int>(0);
		for (int n = 0; n <arr.rows* arr.cols; n++)
		{
			cout << ptr[n] << ",";
		}
	}

	cout << endl;

	Vec<int, 3> vi(21, 23, 56);

	//cout << "向量的行数: " << vi.rows << endl;
	//cout << "向量的列数: " << vi.cols << endl;

	cout << "vi[0]: " << vi[0] << endl;
	cout << "vi(3): " << vi(2) << endl;

	Mat src1 = (Mat_<uchar>(2, 3) << 23, 15, 251, 55, 44, 66);
	Mat src2 = (Mat_<float>(2, 3) << 7, 5, 4, 33, 22, 11);
	Mat dst;
	add(src1, src2, dst, Mat(), CV_64FC1);

	cout << "dst: " << dst << endl;

	Mat src3 = (Mat_<Vec2f>(2, 1) << Vec2f(1, 2), Vec2f(3, 4));
	Mat src4 = (Mat_<Vec2f>(1, 2) << Vec2f(10, 20), Vec2f(5, 15));
	Mat dst1 = src3 * src4;
	cout << "dst1: " << dst1 << endl;

	Mat src5 = (Mat_<uchar>(2, 2) << 4, 25, 16, 49);
	Mat dst2;
	pow(src5, 2, dst2);
	cout << "src5: " << src5 << endl;
	cout << "dst2: " << dst2 << endl;


	//Mat img = imread(argv[1], CV_LOAD_IMAGE_ANYCOLOR);
	//if (img.empty())
	//	return -1;

	//imshow("BGR", img);

	//vector<Mat> planes;
	//split(img, planes);

	//imshow("B", planes[0]);
	//imshow("G", planes[0]);
	//imshow("R", planes[0]);
	//waitKey(0);

	Mat src6 = (Mat_<float>(3, 2) << 0, 0, 200, 0, 0, 200);
	Mat dst6 = (Mat_<float>(3, 2) << 0, 0, 100, 0, 0, 100);

	Mat A = getAffineTransform(src6, dst6);
	cout << "A = " << A << endl;

	Mat B = getRotationMatrix2D(Point2d(40, 50), 30, 0.5);
	cout << "B = " << B << endl;

	return 0;
}