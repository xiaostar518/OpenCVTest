#include "ImageControl.h"

void sepConv2D_X_Y(InputArray src, OutputArray src_kerX_kerY, int ddepth, InputArray kernelY, InputArray kernerX, Point anchor, int borderType)
{
	Mat src_kerX;
	conv2D(src, kernelY, src_kerX, ddepth, anchor, borderType);

	conv2D(src_kerX, kernerX, src_kerX_kerY, ddepth, anchor, borderType);
}


void sepConv2D_Y_X(InputArray src, OutputArray src_kerY_kerX, int ddepth, InputArray kernelY, InputArray kernerX, Point anchor, int borderType)
{
	Mat src_kerY;
	conv2D(src, kernelY, src_kerY, ddepth, anchor, borderType);

	conv2D(src_kerY, kernerX, src_kerY_kerX, ddepth, anchor, borderType);
}

void conv2D(InputArray src, InputArray kernel, OutputArray dst, int ddepth, Point anchor, int borderType)
{
	Mat kernelFlip;
	flip(kernel, kernelFlip, -1);

	filter2D(src, dst, ddepth, kernelFlip, anchor, 0.0, borderType);
}

void imageControlTest()
{
	Mat I = (Mat_<float>(5, 5) <<
		1, 2, 3, 10, 12,
		32, 43, 12, 4, 190,
		12, 234, 78, 0, 12,
		43, 90, 32, 8, 90,
		71, 12, 4, 98, 123
		);

	Mat kernel = (Mat_<float>(3, 3) <<
		1, 0, -1,
		1, 0, -1,
		1, 0, -1
		);

	Mat c_same;
	conv2D(I, kernel, c_same, CV_32FC1, Point(-1, -1), 4);

	cout << "c_same = " << c_same << endl;

	Mat kernel1 = (Mat_<float>(1, 3) <<
		1, 0, -1
		);

	Mat kernel2 = (Mat_<float>(3,1) <<
		1, 
		1, 
		1
		);

	Mat c_same_copy_X_Y;
	sepConv2D_X_Y(I, c_same_copy_X_Y, CV_32FC1, kernel1, kernel2);
	cout << "c_same_copy_X_Y = " << c_same_copy_X_Y << endl;

	Mat c_same_copy_Y_X;
	sepConv2D_Y_X(I, c_same_copy_Y_X, CV_32FC1, kernel2, kernel1);
	cout << "c_same_copy_Y_X = " << c_same_copy_Y_X << endl;
}

Mat gaussBlur(const Mat &image, Size winSize, float sigma, int ddepth, Point anchor, int borderType)
{
	CV_Assert(winSize.width % 2 == 1 && winSize.height % 2 == 1);

	Mat gK_y = getGaussianKernel(winSize.height, sigma,  CV_64F);

	Mat gK_x = getGaussianKernel(winSize.width, sigma, CV_64F);

	gK_x = gK_x.t();

	Mat blurImage;

	sepConv2D_Y_X(image, blurImage, ddepth, gK_y, gK_x, Point(-1, -1));

	return blurImage;
}

int gaussTest()
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	imshow("I", I);
	//cout << " I  = " << I << endl;

	Mat dst;
	GaussianBlur(I, dst, Size(51, 51), 2);

	//Mat dst = gaussBlur(I, Size(51, 51), 2);

	imshow("dst", dst);

	waitKey(0);

	return 0;
}

Mat fastMeanBlur(Mat img, Size winSize, int borderType, Scalar value)
{
	int hei = winSize.height;
	int wid = winSize.width;
	CV_Assert(hei % 2 == 1 && wid % 2 == 1);

	int hh = (hei - 1) / 2;
	int ww = (wid - 1) / 2;

	float area = float(hei*wid);

	Mat paddImg;
	copyMakeBorder(img, paddImg, hh, hh, ww, ww, borderType, value);

	Mat inte;
	integral(paddImg, inte, CV_32FC1);

	int rows = img.rows;
	int cols = img.cols;

	int r = 0, c = 0;
	Mat meanImg = Mat::zeros(img.size(), CV_32FC1);

	for (int h = hh; h < hh+rows; h++)
	{
		for (int w = ww; w < ww + cols; w++)
		{
			float bottomRight = inte.at<float>(h + hh + 1, w + ww + 1);
			float topLeft = inte.at<float>(h - hh, w - ww);
			float topRight = inte.at<float>(h - hh, w + ww + 1);
			float bottomLeft = inte.at<float>(h + hh + 1, w - ww);

			meanImg.at<float>(r, c) = (bottomRight + topLeft - topRight - bottomLeft) / area;
			c++;
		}
		r++;
		c = 0;
	}

	return meanImg;
}


int blurTest() 
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;
	imshow("img", I);

	Mat dst;
	blur(I, dst, Size(11, 11), Point(1, 2));
	imshow("dst", dst);

	waitKey(0);

	return 0;
}

Mat medianSmooth(const Mat &I, Size size, int borderType)
{
	CV_Assert(I.type() == CV_8UC1);

	int H = size.height;
	int W = size.width;

	CV_Assert(H > 0 && W > 0);
	CV_Assert(H % 2 == 1 && W % 2 == 1);

	int h = (H - 1) / 2;
	int w = (W - 1) / 2;

	Mat Ip;
	copyMakeBorder(I, Ip, h, h, w, w, borderType);

	int rows = I.rows;
	int cols = I.cols;

	Mat medianI(I.size(), CV_8UC1);
	int i = 0, j = 0;

	int index = (H*W - 1) / 2;
	for (int r = h; r < h+rows; r++)
	{
		for (int c = w; c < w+cols; c++)
		{
			Mat region = Ip(Rect(c - w, r - h, W, H)).clone();
			region = region.reshape(1, 1);
			cv::sort(region, region, CV_SORT_EVERY_ROW);

			uchar mValue = region.at<uchar>(0, index);
			medianI.at<uchar>(i, j) = mValue;
			j++;
		}
		i++;
		j = 0;
	}

	return medianI;
}

int medianSmoothTest()
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;
	imshow("img", I);

	Mat medianImage = medianSmooth(I, Size(7, 7));

	//Mat medianImage;
	//medianBlur(I, medianImage,7);


	imshow("dst", medianImage);

	waitKey(0);

	return 0;
}

Mat getClosenessWeight(double sigma_g, Size size)
{
	int W = size.width;
	int H = size.height;

	int cW = (W - 1) / 2;
	int cH = (H - 1) / 2;

	Mat closenessWeight = Mat::zeros(size, CV_64FC1);
	for (int r = 0; r < H; r++)
	{
		for (int c = 0; c < W; c++)
		{
			double norm2 = pow(double(r - cH), 2.0) + pow(double(c - cW), 2.0);
			double sigma_g2 = 2.0*pow(sigma_g, 2.0);

			closenessWeight.at<double>(r, c) = exp(-norm2 / sigma_g2);
		}
	}

	return closenessWeight;
}

Mat bfltGray(const Mat & image, Size winSize, float sigma_g, float sigma_d)
{
	int winH = winSize.height;
	int winW = winSize.width;

	CV_Assert(winH > 0 && winW > 0);
	CV_Assert(winH % 2 == 1 && winW % 2 == 1);

	if (winH == 1 && winW == 1)
		return image;

	int half_winW = (winW - 1) / 2;
	int half_winH = (winH - 1) / 2;

	Mat closenessWeight = getClosenessWeight(sigma_g, winSize);
	int rows = image.rows;
	int cols = image.cols;

	Mat blfImage = Mat::ones(image.size(), CV_32FC1);
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			double pixel = image.at<double>(r, c);

			int rTop = (r - half_winH) < 0 ? 0 : r - half_winH;
			int rBottom = (r + half_winH) > rows - 1 ? rows - 1 : r + half_winH;
			int cLeft = (c - half_winW) < 0 ? 0 : c - half_winW;
			int cRight = (c + half_winW) > cols - 1 ? cols - 1 : c + half_winW;

			Mat region = image(Rect(Point(cLeft, rTop), Point(cRight + 1, rBottom + 1))).clone();

			Mat similarityWeight;
			pow(region - pixel, 2.0, similarityWeight);

			exp(-0.5*similarityWeight / pow(sigma_d, 2), similarityWeight);
			similarityWeight /= pow(sigma_d, 2);

			Rect regionRect = Rect(Point(cLeft - c + half_winW, rTop - r + half_winH), Point(cRight - c + half_winW + 1, rBottom - r + half_winH + 1));
			Mat closenessWightTemp = closenessWeight(regionRect).clone();
			
			Mat weightTemp = closenessWightTemp.mul(similarityWeight);
			weightTemp = weightTemp / sum(weightTemp)[0];

			Mat result = weightTemp.mul(region);
			blfImage.at<float>(r, c) = sum(result)[0];
		}
	}

	return blfImage;
}

int bfltGaryTest()
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;


	Mat fI, blfI;
	//I.convertTo(fI, CV_64FC1, 1.0 / 255, 0);
	//blfI = bfltGray(fI, Size(33, 33), 19, 0.5);

	bilateralFilter(I, blfI,33,19,5);

	imshow("img", I);
	imshow("dst", blfI);

	//blfI.convertTo(blfI, CV_8U, 255, 0);
	//imwrite("blf.jpg", blfI);

	waitKey(0);
	return 0;
}

Mat joinBLF(Mat I, Size size, float sigma_g, float sigma_d, int borterType)
{
	Mat closeWeight = getClosenessWeight(sigma_g, size);

	Mat Ig;
	GaussianBlur(I, Ig, size, sigma_g);

	int cH = (size.height - 1) / 2;
	int cW = (size.width - 1) / 2;

	Mat Ip, Igp;
	copyMakeBorder(I, Ip, cH, cH, cW, cW, borterType);
	copyMakeBorder(Ig, Igp, cH, cH, cW, cW, borterType);

	int rows = I.rows;
	int cols = I.cols;

	int i = 0, j = 0;

	Mat jblf = Mat::zeros(I.size(), CV_64FC1);
	for (int r = cH; r < cH + rows; r++)
	{
		for (int c = cW; c < cW+cols; c++)
		{
			double pixel = Igp.at<double>(r, c);

			Mat region = Igp(Rect(c - cW, r - cH, size.width, size.height));
			Mat similarityWeight;
			pow(region - pixel, 2.0, similarityWeight);
			exp(-0.5*similarityWeight / pow(sigma_d, 2.0), similarityWeight);

			Mat weight = closeWeight.mul(similarityWeight);
			weight = weight / sum(weight)[0];

			Mat Iregion = Ip(Rect(c - cW, r - cH, size.width, size.height));
			jblf.at<double>(i, j) = sum(Iregion.mul(weight))[0];

			j += 1;
		}
		j = 0;
		i += 1;
	}

	return jblf;
}

int joinBLFTest()
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	Mat fI;
	I.convertTo(fI, CV_64F, 1.0, 0);
	Mat jblf = joinBLF(fI, Size(33, 33), 7, 2);

	Mat jblf8U;
	jblf.convertTo(jblf8U, CV_8U, 1, 0);


	imshow("img", I);
	imshow("dst", jblf8U);

	waitKey(0);
	return 0;
}

Mat guidedFilter(Mat I, Mat p, int r, float eps, float s)
{
	int rows = I.rows;
	int cols = I.cols;

	Mat small_I, small_p;
	Size smallSize(int(round(s*cols)), int(round(s*rows)));
	resize(I, small_I, smallSize, 0, 0, CV_INTER_CUBIC);
	resize(p, small_p, smallSize, 0, 0, CV_INTER_CUBIC);

	int small_r = int(round(r*s));
	Size winSize(2 * small_r + 1, 2 * small_r + 1);

	Mat mean_small_I, mean_small_p;
	boxFilter(small_I, mean_small_I, CV_64FC1, winSize);
	boxFilter(small_p, mean_small_p, CV_64FC1, winSize);

	Mat small_Ip = small_I.mul(small_p);
	Mat mean_small_Ip;
	boxFilter(small_Ip, mean_small_Ip, CV_64FC1, winSize);

	Mat cov_small_Ip = mean_small_Ip - mean_small_I.mul(mean_small_p);

	Mat mean_small_II;
	boxFilter(small_I.mul(small_I), mean_small_II, CV_64FC1, winSize);

	Mat var_small_I = mean_small_II - mean_small_I.mul(mean_small_I);
	Mat small_a = cov_small_Ip / (var_small_I + eps);
	Mat small_b = mean_small_p - small_a.mul(mean_small_I);

	Mat mean_small_a, mean_small_b;
	boxFilter(small_a, mean_small_a, CV_64FC1, winSize);
	boxFilter(small_b, mean_small_b, CV_64FC1, winSize);

	Mat mean_a, mean_b;
	resize(mean_small_a, mean_a, I.size(), 0, 0, CV_INTER_LINEAR);
	resize(mean_small_b, mean_b, I.size(), 0, 0, CV_INTER_LINEAR);

	Mat q = mean_a.mul(I) + mean_b;
	return q;
}

int guidedFilterTest()
{
	Mat I = imread("cat1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!I.data)
		return -1;

	imshow("I", I);

	Mat fI;
	I.convertTo(fI, CV_64FC1, 1.0 / 255);
	Mat q = guidedFilter(fI, fI, 7, 0.04, 0.3);
	imshow("dst", q);

	Mat I_enhanced = (fI - q) * 5 + q;
	normalize(I_enhanced, I_enhanced, 1, 0, NORM_MINMAX, CV_32FC1);
	imshow("I_enhanced", I_enhanced);

	waitKey(0);
	return 0;
}