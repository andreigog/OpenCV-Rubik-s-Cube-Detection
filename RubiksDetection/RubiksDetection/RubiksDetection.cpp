// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <fstream>
#include <queue>
#include <random>
#include <iostream>
using namespace std;

#define M_PI 3.14159265358979323846


void testOpenImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image", src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName) == 0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName, "bmp");
	while (fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(), src);
		if (waitKey() == 27) //ESC pressed
			break;
	}
}

void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image

	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}

void testGrayAdditiveLevels()
{
	int addFactor = 0;
	printf("Give the additive factor: ");
	scanf("%d", &addFactor);
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				int newPixel = val + addFactor;
				if (newPixel > 255)
					newPixel = 255;
				if (newPixel < 0)
					newPixel = 0;
				dst.at<uchar>(i, j) = newPixel;
			}
		}
		imshow("input image", src);
		imshow("gray levels changed", dst);
		waitKey();
	}
}

void testGrayMultLevels()
{
	int multFactor = 0;
	printf("Give the multiplicative factor: ");
	scanf("%d", &multFactor);
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				int newPixel = val * multFactor;
				if (newPixel > 255)
					newPixel = 255;
				if (newPixel < 0)
					newPixel = 0;
				dst.at<uchar>(i, j) = newPixel;
			}
		}
		imwrite("output.bmp", dst);
		imshow("input image", src);
		imshow("gray levels changed", dst);
		waitKey();
	}
}

void BFSLabeling()
{
	int label = 0;
	int di[8] = { -1, -1, -1, 0, 0, +1, +1, +1 };
	int dj[8] = { -1, 0, +1, -1, +1, -1, 0, +1 };


	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat labels = Mat(height, width, CV_8UC1);
		Mat colored = Mat(height, width, CV_8UC3);
		std::queue<Point2i> Q;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				labels.at<uchar>(i, j) = 0;
			}
		}
		for (int i = 0; i < height - 1; i++)
		{
			for (int j = 0; j < width - 1; j++)
			{
				uchar val = src.at<uchar>(i, j);
				if ((val == 0) && (labels.at<uchar>(i, j) == 0))
				{
					label++;
					printf("the new label value is: %d\n", label);
					Point2i point;
					point.x = j;
					point.y = i;
					labels.at<uchar>(i, j) = label;
					Q.push(point);
					while (!Q.empty())
					{
						Point2i p = Q.front();
						Q.pop();
						int x = p.x;
						int y = p.y;
						for (int k = 0; k < 8; k++)
						{
							if (y + di[k] < height && y + di[k] >= 0 && x + dj[k] < width && x + dj[k] >= 0)
							{
								if (src.at<uchar>(y + di[k], x + dj[k]) == 0 && labels.at<uchar>(y + di[k], x + dj[k]) == 0)
								{
									labels.at<uchar>(y + di[k], x + dj[k]) = label;
									Point2i newp = Point2i(x + dj[k], y + di[k]);
									Q.push(newp);
								}
							}
						}
					}

				}

			}
		}
		printf("mnumber of objects: %d", label);
		Vec3b *randColor = new Vec3b[label];
		std::default_random_engine gen;
		std::uniform_int_distribution<int> d(0, 255);
		randColor[0] = Vec3b(255, 255, 255);
		for (int i = 1; i < label; i++)
		{
			uchar rand = d(gen);
			uchar b = d(gen);
			uchar g = d(gen);
			uchar r = d(gen);
			randColor[i] = Vec3b(b, g, r);
			printf("colors[%d]: %d %d %d \n", i, b, g, r);
		}
		for (int k = 0; k < label; k++)
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					if (labels.at<uchar>(i, j) == k)
					{
						colored.at<Vec3b>(i, j) = randColor[labels.at<uchar>(i, j)];
					}
				}
			}
			imshow("colored", colored);
			waitKey(0);
		}
	}
}

long mod(long a, long b)
{
	return (a%b + b) % b;
}

void borderTracing() {
	int di[8] = { 0, -1, -1, -1, 0, +1, +1, +1 };
	int dj[8] = { +1, +1, 0 , -1, -1, -1, 0, +1 };
	int values[8] = { 0,1,2,3,4,5,6,7 };
	Point2i* chain;
	int* chainCode;
	int* derivCode;

	int dir_init = 7;
	int dir;
	int a = -1;

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		//printf("%s", fname);
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat traced = Mat(height, width, CV_8UC1);
		int flag = 0;
		chain = (Point2i*)malloc(sizeof(Point2i)*(height*width));
		chainCode = (int*)malloc(sizeof(int)*(height*width));
		derivCode = (int*)malloc(sizeof(int)*(height*width));
		Point2i point;
		Point2i pointa;
		Point2i pointb;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				traced.at<uchar>(i, j) = 125;
			}
		}

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (src.at<uchar>(i, j) == 0) {
					point.x = j;
					point.y = i;
					a++;
					printf("gogandrei %d\n", a);
					chain[a] = point;
					derivCode[a] = dir_init;
					chainCode[a] = dir_init;
					printf("First detected point: ");
					printf("(%d, %d)\n", i, j);
					if (dir_init % 2 == 0) {
						dir_init = (dir_init + 7) % 8;
					}
					else {
						dir_init = (dir_init + 6) % 8;
					}
					for (int k = dir_init; k < dir_init + 8; k++)
					{
						if (src.at<uchar>(i + di[k % 8], j + dj[k % 8]) == 0) {
							traced.at<uchar>(i, j) = 0;
							point.x = j + dj[k % 8];
							point.y = i + di[k % 8];
							a++;
							chain[a] = point;
							dir = values[k % 8];
							derivCode[a] = mod((dir - dir_init), 8);
							dir_init = dir;
							chainCode[a] = dir;
							printf("second detected point: ");
							printf("(%d, %d)\n", point.y, point.x);
							break;
						}
					}
					flag = 1;
					break;
				}
			}
			if (flag)
				break;
		}
		if (dir % 2 == 0) {
			dir = (dir + 7) % 8;
		}
		else {
			dir = (dir + 6) % 8;
		}
		for (int k = dir; k < dir + 8; k++)
		{
			if (src.at<uchar>(chain[a].y + di[k % 8], chain[a].x + dj[k % 8]) == 0) {
				point.x = chain[a].x + dj[k % 8];
				point.y = chain[a].y + di[k % 8];
				traced.at<uchar>(point.y, point.x) = 0;
				a++;
				chain[a] = point;
				printf("Third detected point: ");
				printf("(%d, %d)\n", point.y, point.x);
				dir = values[k % 8];
				derivCode[a] = mod((dir - dir_init), 8);
				dir_init = dir;
				chainCode[a] = dir;
				break;
			}
		}
		pointa = point;
		pointb = point;

		while (chain[0] != pointa && chain[1] != pointb) {
			if (dir % 2 == 0) {
				dir = (dir + 7) % 8;
			}
			else {
				dir = (dir + 6) % 8;
			}
			for (int k = dir; k < dir + 8; k++)
			{
				if (src.at<uchar>(chain[a].y + di[k % 8], chain[a].x + dj[k % 8]) == 0) {
					point.x = chain[a].x + dj[k % 8];
					point.y = chain[a].y + di[k % 8];
					printf("Detected point: ");
					printf("(%d, %d)\n", point.y, point.x);
					traced.at<uchar>(point.y, point.x) = 0;
					a++;
					chain[a] = point;
					dir = values[k % 8];
					derivCode[a] = mod((dir - dir_init), 8);
					dir_init = dir;
					chainCode[a] = dir;
					break;
				}
			}
			pointa = pointb;
			pointb = point;
		}
		free(chain);
		printf("number of pixels is %d \n", a);
		printf("Chain code: ");
		for (int i = 0; i < a; i++)
		{
			printf("%d ", chainCode[i]);
		}
		printf("\nDifferential chain code: ");
		for (int i = 0; i < a; i++)
		{
			printf("%d ", derivCode[i]);
		}
		imshow("new image", traced);

		a = -1;
		waitKey();
	}
}

Mat dilation(Mat src) {
	int di[8] = { 0, -1, -1, -1, 0, +1, +1, +1 };
	int dj[8] = { +1, +1, 0 , -1, -1, -1, 0, +1 };

	int height = src.rows;
	int width = src.cols;
	Mat dest = Mat(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dest.at<uchar>(i, j) = 255;
		}
	}
	for (int i = 0; i < height - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			if (src.at<uchar>(i, j) == 0) {
				dest.at<uchar>(i, j) = 0;
				for (int k = 0; k < 8; k++)
				{
					dest.at<uchar>(i + di[k], j + dj[k]) = 0;
				}
			}
		}
	}
	return dest;
}

Mat erosion(Mat src) {
	int di[8] = { 0, -1, -1, -1, 0, +1, +1, +1 };
	int dj[8] = { +1, +1, 0 , -1, -1, -1, 0, +1 };

	int height = src.rows;
	int width = src.cols;
	Mat dest = Mat(height, width, CV_8UC1);
	int flag = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dest.at<uchar>(i, j) = src.at<uchar>(i, j);
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (src.at<uchar>(i, j) == 0) {
				for (int k = 0; k < 8; k++)
				{
					if (src.at<uchar>(i + di[k], j + dj[k]) != 0) {
						flag = 1;
					}
				}
				if (flag)
					dest.at<uchar>(i, j) = 255;
				flag = 0;
			}
		}
	}
	return dest;
}

void erode() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dest = Mat(height, width, CV_8UC1);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				dest.at<uchar>(i, j) = 255;
			}
		}
		dest = erosion(src);
		imshow("Source image", src);
		imshow("Eroded image", dest);
		waitKey();
	}
}

void dilate() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dest = Mat(height, width, CV_8UC1);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				dest.at<uchar>(i, j) = 255;
			}
		}
		dest = dilation(src);
		imshow("Source image", src);
		imshow("Dilated image", dest);
		waitKey();
	}
}

void closing() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dest = Mat(height, width, CV_8UC1);
		dest = dilation(src);
		dest = erosion(dest);
		imshow("Original image", src);
		imshow("Closed image", dest);
		waitKey();
	}
}

void opening() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dest = Mat(height, width, CV_8UC1);
		dest = erosion(src);
		dest = dilation(dest);
		imshow("Original image", src);
		imshow("Opened image", dest);
		waitKey();
	}
}

boolean differentImages(Mat mat1, Mat mat2) {
	int height = mat1.rows;
	int width = mat2.cols;
	boolean ok = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (mat2.at<uchar>(i, j) != mat1.at<uchar>(i, j))
			{
				ok = 1;
			}
		}
	}
	return ok;
}

void boundaryExtraction() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat eroded = Mat(height, width, CV_8UC1);
		Mat dest = Mat(height, width, CV_8UC1);
		eroded = erosion(src);
		for (int i = 0; i < height - 1; i++)
		{
			for (int j = 0; j < width - 1; j++)
			{
				if (src.at<uchar>(i, j) == 0 && eroded.at<uchar>(i, j) == 255) {
					dest.at<uchar>(i, j) = 0;
				}
				else {
					dest.at<uchar>(i, j) = 255;
				}
			}
		}
		imshow("Original image", src);
		imshow("Extracted boundary", dest);
		waitKey();
	}
}

void regionFilling() {
	int di[8] = { 0, -1, -1, -1, 0, +1, +1, +1 };
	int dj[8] = { +1, +1, 0 , -1, -1, -1, 0, +1 };
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat final = Mat(height, width, CV_8UC1);
		Mat dilated1 = Mat(height, width, CV_8UC1);
		Mat dilated2 = Mat(height, width, CV_8UC1);
		boolean flag = 0;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				dilated1.at<uchar>(i, j) = 255;
				dilated2.at<uchar>(i, j) = 255;
				final.at<uchar>(i, j) = 255;
			}
		}
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				if (src.at<uchar>(i, j) == 255 && src.at<uchar>(i, j - 1) == 0 && src.at<uchar>(i - 1, j) == 0) {
					dilated1.at<uchar>(i, j) = 0;
					while (differentImages(dilated2, dilated1)) {
						dilated2 = dilated1;
						dilated1 = dilation(dilated1);

						for (int i = 0; i < height; i++)
						{
							for (int j = 0; j < width; j++)
							{
								if (src.at<uchar>(i, j) == 0 && dilated1.at<uchar>(i, j) == 0)
								{
									dilated1.at<uchar>(i, j) = 255;
								}
							}
						}
						imshow("Step by step", dilated1);
						waitKey();
					}
					flag = 1;
					break;
				}
			}
			if (flag)
				break;
		}

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (src.at<uchar>(i, j) == 0 || dilated1.at<uchar>(i, j) == 0) {
					final.at<uchar>(i, j) = 0;
				}
			}
		}
		imshow("Original image", src);
		imshow("Filled image", final);
		waitKey();
	}
}

void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
	Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

																		 //computes histogram maximum
	int max_hist = 0;
	for (int i = 0; i < hist_cols; i++)
		if (hist[i] > max_hist)
			max_hist = hist[i];
	double scale = 1.0;
	scale = (double)hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins colored in magenta
	}

	imshow(name, imgHist);
}

void computeHistograms() {
	int histogram[256];
	int cumulativeHistogram[256];
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
		cumulativeHistogram[i] = 0;
	}
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		int size = height * width;
		float meanValue = 0.0f;
		float standardDeviation = 0.0f;

		//historgram
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				histogram[src.at<uchar>(i, j)]++;
				meanValue = meanValue + src.at<uchar>(i, j);
			}
		}
		meanValue = (float)meanValue / size;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				standardDeviation += (src.at<uchar>(i, j) - meanValue) * (src.at<uchar>(i, j) - meanValue);
			}
		}
		standardDeviation = (float)standardDeviation / size;
		standardDeviation = (float)sqrt(standardDeviation);
		//cumulative histogram
		for (int i = 0; i < 256; i++)
			if (i == 0)
				cumulativeHistogram[i] = histogram[i];
			else
				cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];

		showHistogram("Histogram", histogram, width, height);
		showHistogram("Cumulative Histogram", cumulativeHistogram, width, height);
		printf("The standard deviation is: %f \n", standardDeviation);
		printf("Mean value is: %f\n", meanValue);
	}
}

void globalThresholding() {
	int histogram[256];
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		int iMax, iMin;
		int size = height * width;
		int threshold = 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				histogram[src.at<uchar>(i, j)]++;
			}
		}
		int a = 0;
		while (histogram[a] == 0)
			a++;
		iMin = a;

		a = 255;
		while (histogram[a] == 0)
			a--;
		iMax = a;
		printf("min and max is %d   %d\n", iMin, iMax);
		threshold = (iMin + iMax) / 2;

		int mean1 = 0, mean2 = 0;

		for (int i = 0; i < src.rows; i++)
			for (int j = 0; j < src.cols; j++)
				if (src.at<uchar>(i, j) <= threshold)
					mean1 = mean1 + src.at<uchar>(i, j);
				else
					mean2 = mean2 + src.at<uchar>(i, j);

		int n1 = 0, n2 = 0;

		for (int i = 0; i < 256; i++)
		{
			if (i < threshold)
				n1 = n1 + histogram[i];
			else
				n2 = n2 + histogram[i];
		}


		mean1 = mean1 / n1;
		mean2 = mean2 / n2;

		//3
		threshold = (mean1 + mean2) / 2;

		//4
		for (int k = 0; k < 3; k++)
		{
			//2
			mean1 = 0;
			mean2 = 0;

			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					if (src.at<uchar>(i, j) <= threshold)
						mean1 = mean1 + src.at<uchar>(i, j);
					else
						mean2 = mean2 + src.at<uchar>(i, j);

			int n1 = 0, n2 = 0;

			for (int i = 0; i < 256; i++)
			{
				if (i < threshold)
					n1 = n1 + histogram[i];
				else
					n2 = n2 + histogram[i];
			}


			mean1 = mean1 / n1;
			mean2 = mean2 / n2;

			//3
			threshold = (mean1 + mean2) / 2;
		}

		//5
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				if (src.at<uchar>(i, j) < threshold)
					dst.at<uchar>(i, j) = 0;
				else
					dst.at<uchar>(i, j) = 255;
			}
		}
		imshow("input image", src);
		imshow("output image", dst);

		waitKey();

	}
}

void histogramTransformation() {
	int histogram[256];
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		int gInMin = 0;
		int gInMax = 255;
		int val = 0;
		printf("Strech/Shrink value:");
		scanf("%d", &val);
		int gOutMin = gInMin + val;
		int gOutMax = gInMax - val;

		int histo[256];
		for (int i = 0; i < 256; i++)
			histo[i] = 0;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				histo[src.at<uchar>(i, j)]++;
			}
		}

		int gIn;
		int gOut;
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				gIn = src.at<uchar>(i, j);
				gOut = gOutMin + (gIn - gInMin) *(gOutMax - gOutMin) / (gInMax - gInMin);
				dst.at<uchar>(i, j) = gOut;
			}
		}

		int newHisto[256];
		for (int i = 0; i < 256; i++)
			newHisto[i] = 0;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				newHisto[dst.at<uchar>(i, j)]++;
			}
		}

		showHistogram("Histogram", histo, 256, 256);
		showHistogram("NewHistogram", newHisto, 256, 256);
		imshow("Initial", src);
		imshow("Output", dst);
		waitKey(0);
	}
}

void gammaCorrection() {
	int histogram[256];
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		float gamma = 0;
		printf("Gamma:");
		scanf("%f", &gamma);

		int histo[256];
		for (int i = 0; i < 256; i++)
			histo[i] = 0;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				histo[src.at<uchar>(i, j)]++;
			}
		}

		int gIn;
		float inter;
		int gOut;
		int L = 255;
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				gIn = src.at<uchar>(i, j);
				inter = (float)gIn / L;
				gOut = L * pow(inter, gamma);
				if (gOut < 0)
					gOut = 0;
				if (gOut > 255)
					gOut = 255;
				dst.at<uchar>(i, j) = gOut;
			}
		}

		int newHisto[256];
		for (int i = 0; i < 256; i++)
			newHisto[i] = 0;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				newHisto[dst.at<uchar>(i, j)]++;
			}
		}

		showHistogram("Histogram", histo, 256, 256);
		showHistogram("NewHistogram", newHisto, 256, 256);
		imshow("Initial", src);
		imshow("Output", dst);
		waitKey(0);
	}
}

void brightnessChange() {
	int histogram[256];
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		float offset = 0;
		printf("Offset:");
		scanf("%f", &offset);

		int histo[256];
		for (int i = 0; i < 256; i++)
			histo[i] = 0;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				histo[src.at<uchar>(i, j)]++;
			}
		}

		int gIn;
		float inter;
		int gOut;
		int L = 255;
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				gIn = src.at<uchar>(i, j);
				gOut = gIn + offset;
				if (gOut < 0)
					gOut = 0;
				if (gOut > 255)
					gOut = 255;
				dst.at<uchar>(i, j) = gOut;
			}
		}

		int newHisto[256];
		for (int i = 0; i < 256; i++)
			newHisto[i] = 0;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				newHisto[dst.at<uchar>(i, j)]++;
			}
		}

		showHistogram("Histogram", histo, 256, 256);
		showHistogram("NewHistogram", newHisto, 256, 256);
		imshow("Initial", src);
		imshow("Output", dst);
		waitKey(0);
	}
}

void histogramEqualization() {
	int histo[256];
	float pdf[256];
	float cumPDF[256];
	//1
	for (int i = 0; i < 256; i++)
	{
		histo[i] = 0;
		pdf[i] = 0;
		cumPDF[i] = 0;
	}

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				histo[src.at<uchar>(i, j)]++;
			}
		}

		for (int i = 0; i < 256; i++)
		{
			pdf[i] = (float)histo[i] / (src.rows*src.cols);
		}

		//2
		for (int i = 0; i < 256; i++)
			if (i == 0)
				cumPDF[i] = pdf[i];
			else
				cumPDF[i] = cumPDF[i - 1] + pdf[i];

		printf("PDF\n");
		for (int i = 0; i < 256; i++)
		{
			printf("%f ", pdf[i]);
			printf("%f\n", cumPDF[i]);
		}


		float eqTable[256];

		//3
		int gIn;
		int gOut;
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				gIn = src.at<uchar>(i, j);
				gOut = 255 * cumPDF[gIn];
				//4
				dst.at<uchar>(i, j) = gOut;
			}
		}

		int newHisto[256];
		for (int i = 0; i < 256; i++)
			newHisto[i] = 0;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				newHisto[dst.at<uchar>(i, j)]++;
			}
		}

		showHistogram("Histogram", histo, 256, 256);
		showHistogram("NewHistogram", newHisto, 256, 256);
		imshow("Initial", src);
		imshow("Output", dst);
		waitKey(0);
	}
}

void applyKernel() {
	int kernelSize;
	float values[25];
	int di[9] = { 0, -1, -1, -1, 0, +1, +1, +1, 0 };
	int dj[9] = { +1, +1, 0 , -1, -1, -1, 0, +1, 0 };
	int sum = 0;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				dst.at<uchar>(i, j) = src.at<uchar>(i, j);
			}
		}
		/*
		printf("Given the kernel size: ");
		scanf("%d", &kernelSize);
		printf("Give the kernel values: ");
		for (int i = 0; i < kernelSize*kernelSize; i++)
		{
		scanf("%f", &values[i]);
		}
		Mat kernel(kernelSize, kernelSize, CV_32FC1, values);*/
		//float valori[9] = { -1.0f, -1.0f, -1.0f, -1.0f, 8.0f, -1.0f, -1.0f, -1.0f, -1.0f };
		float valori[9] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		kernelSize = 3;
		Mat kernel(kernelSize, kernelSize, CV_32FC1, valori);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				printf("%f ", kernel.at<float>(i, j));
				kernel.at<float>(i, j) = kernel.at<float>(i, j) / 9.0f;
			}
		}
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				for (int k = 0; k < 9; k++)
				{
					sum = sum + src.at<uchar>(i + di[k], j + dj[k])*kernel.at<float>(1 + di[k], 1 + dj[k]);
				}
				if (sum < 0)
					dst.at<uchar>(i, j) = 0;
				else
					if (sum > 255)
						dst.at<uchar>(i, j) = 255;
					else
						dst.at<uchar>(i, j) = sum;
				sum = 0;
			}
		}
		imshow("Initial", src);
		imshow("Output", dst);
		waitKey(0);
	}
}

void centering_transform(Mat img) {
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			img.at<float>(i, j) = ((i + j) & 1) ? -img.at<float>(i, j) : img.at<float>(i, j);
		}
	}
}

Mat generic_frequency_domain_filter(Mat src) {
	Mat srcf;
	src.convertTo(srcf, CV_32FC1);

	centering_transform(srcf);

	Mat fourier;
	dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

	Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(),CV_32F) };
	split(fourier, channels);

	Mat mag, phi;
	magnitude(channels[0], channels[1], mag);
	phase(channels[0], channels[1], phi);

	//
	//

	Mat dst, dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

	centering_transform(dstf);

	normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);

	return dst;

}

void borderToImage() {
	char* fname = "C:\\Users\\andrei\\Documents\\Visual Studio 2017\\Projects\\OpenCV\\Images\\files_border_tracing\\gray_background.bmp";
	Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
	int height = src.rows;
	int width = src.cols;
	Mat output = src;
	int di[8] = { 0, -1, -1, -1, 0, +1, +1, +1 };
	int dj[8] = { +1, +1, 0 , -1, -1, -1, 0, +1 };
	int values[8] = { 0,1,2,3,4,5,6,7 };
	std::fstream myfile("C:\\Users\\andrei\\Documents\\Visual Studio 2017\\Projects\\OpenCV\\Images\\files_border_tracing\\reconstruct.txt", std::ios_base::in);
	Point2i point;
	int length;
	int dir;
	myfile >> point.y >> point.x >> length;
	output.at<uchar>(point.y, point.x) = 0;
	for (int i = 0; i < length; i++)
	{
		myfile >> dir;
		point.y += di[dir];
		point.x += dj[dir];
		output.at<uchar>(point.y, point.x) = 0;
	}
	myfile.close();
	imshow("Generated image from the read code", output);
	waitKey();
	scanf("%d", &dir);
}

void createImage()
{
	Mat img(256, 256, CV_8UC3);
	int height = img.rows;
	int width = img.cols;
	Vec3b pixel;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i <= 128 && j <= 128)
			{
				pixel[0] = 255;
				pixel[1] = 255;
				pixel[2] = 255;
				img.at<Vec3b>(i, j) = pixel;
			}
			if (i > 128 && j <= 128)
			{
				pixel[0] = 0;
				pixel[1] = 0;
				pixel[2] = 255;
				img.at<Vec3b>(i, j) = pixel;
			}
			if (i <= 128 && j > 128)
			{
				pixel[0] = 0;
				pixel[1] = 255;
				pixel[2] = 0;
				img.at<Vec3b>(i, j) = pixel;
			}
			if (i > 128 && j > 128)
			{
				pixel[0] = 100;
				pixel[1] = 255;
				pixel[2] = 255;
				img.at<Vec3b>(i, j) = pixel;
			}
		}
	}
	imshow("new image", img);
	waitKey();
}

void createMatrix()
{
	float values[9] = { 3, 0, 0, 0, 3, 0, 0, 0, 3 };
	Mat M(3, 3, CV_32FC1, values);
	Mat inv = M.inv();
	std::cout << inv << std::endl;
	waitKey(0);
	int x;
	scanf("%d", &x);
}

void threeGrayscale()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		int height = src.rows;
		int width = src.cols;
		Mat g1 = Mat(height, width, CV_8UC1);
		Mat g2 = Mat(height, width, CV_8UC1);
		Mat g3 = Mat(height, width, CV_8UC1);
		Vec3b pixel;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				pixel = src.at<Vec3b>(i, j);
				g1.at<uchar>(i, j) = pixel[0];
				g2.at<uchar>(i, j) = pixel[1];
				g3.at<uchar>(i, j) = pixel[2];
			}
		}
		imshow("input image", src);
		imshow("g1", g1);
		imshow("g2", g2);
		imshow("g3", g3);

		waitKey();
	}
}

void convertToGrayscale()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Vec3b pixel;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				pixel = src.at<Vec3b>(i, j);
				dst.at<uchar>(i, j) = (pixel[0] + pixel[1] + pixel[2]) / 3;
			}
		}
		imshow("input image", src);
		imshow("output image", dst);

		waitKey();
	}
}

void convertToBinary()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		uchar value;
		int threshold = 128;
		printf("Give an threshold value between 0 and 255: ");
		scanf("%d", &threshold);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				value = src.at<uchar>(i, j);
				if (value < threshold)
				{
					dst.at<uchar>(i, j) = 0;
				}
				else
				{
					dst.at<uchar>(i, j) = 255;
				}

			}
		}
		imshow("input image", src);
		imshow("output image", dst);

		waitKey();
	}
}

void convertToHSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat m1 = Mat(height, width, CV_8UC1);
		Mat m2 = Mat(height, width, CV_8UC1);
		Mat m3 = Mat(height, width, CV_8UC1);
		Vec3b pixel;
		float r, g, b, max, min, c;
		float h, s, v;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				pixel = src.at<Vec3b>(i, j);
				b = (float)pixel[0] / 255;
				g = (float)pixel[1] / 255;
				r = (float)pixel[2] / 255;
				max = max(r, max(g, b));
				min = min(r, min(g, b));
				c = (float)max - min;
				v = max;
				if (c) {
					s = c / v;
				}
				else
				{
					s = 0;
				}
				if (c) {
					if (max == r)
						h = (float)60 * (g - b) / c;
					if (max == g)
						h = (float)120 + (float)60 * (b - r) / c;
					if (max == b)
						h = (float)240 + (float)60 * (r - g) / c;
				}
				else
				{
					h = 0;
				}
				if (h < 0)
					h += 360;
				h = h * 255 / 360;
				s *= 255;
				v *= 255;
				m1.at<uchar>(i, j) = h;
				m2.at<uchar>(i, j) = s;
				m3.at<uchar>(i, j) = v;
			}
		}
		imshow("input image", src);
		imshow("H", m1);
		imshow("S", m2);
		imshow("V", m3);

		waitKey();
	}
}

void detectYellow()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Vec3b pixel;
		float r, g, b, max, min, c;
		float h, s, v;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				pixel = src.at<Vec3b>(i, j);
				b = (float)pixel[0] / 255;
				g = (float)pixel[1] / 255;
				r = (float)pixel[2] / 255;
				max = max(r, max(g, b));
				min = min(r, min(g, b));
				c = (float)max - min;
				v = max;
				if (c) {
					s = c / v;
				}
				else
				{
					s = 0;
				}
				if (c) {
					if (max == r)
						h = (float)60 * (g - b) / c;
					if (max == g)
						h = (float)120 + (float)60 * (b - r) / c;
					if (max == b)
						h = (float)240 + (float)60 * (r - g) / c;
				}
				else
				{
					h = 0;
				}
				if (h < 0)
					h += 360;
				if (h > 50 && h<70 && s>0.3)
					dst.at<uchar>(i, j) = 0;
				else
					dst.at<uchar>(i, j) = 255;

			}
		}
		imshow("input image", src);
		imshow("output image", dst);

		waitKey();
	}
}

void wavyImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float a = (float)(j / 30.0) * 360.0;
				float angle = a * M_PI / 180.0;
				float newI = i + 10 * sin(angle);
				if (newI < 0)
					newI = i;
				if (newI > height)
					newI = i;
				dst.at<uchar>(i, j) = src.at<uchar>((int)newI, j);
			}
		}

		imshow("input image", src);
		imshow("wavy image", dst);
		waitKey();
	}
}

void testNegativeImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				uchar neg = 255 - val;
				dst.at<uchar>(i, j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("negative image", dst);
		waitKey();
	}
}

void testParcurgereSimplaDiblookStyle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();

		double t = (double)getTickCount(); // Get the current time [s]

										   // the fastest approach using the “diblook style”
		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;
		int w = (int)src.step; // no dword alignment is done !!!
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("negative image", dst);
		waitKey();
	}
}

void testColor2Gray()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);

		int height = src.rows;
		int width = src.cols;

		Mat dst = Mat(height, width, CV_8UC1);

		// Asa se acceseaaza pixelii individuali pt. o imagine RGB 24 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec3b v3 = src.at<Vec3b>(i, j);
				uchar b = v3[0];
				uchar g = v3[1];
				uchar r = v3[2];
				dst.at<uchar>(i, j) = (r + g + b) / 3;
			}
		}

		imshow("input image", src);
		imshow("gray image", dst);
		waitKey();
	}
}

void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;

		// Componentele d eculoare ale modelului HSV
		Mat H = Mat(height, width, CV_8UC1);
		Mat S = Mat(height, width, CV_8UC1);
		Mat V = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* lpH = H.data;
		uchar* lpS = S.data;
		uchar* lpV = V.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);

		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int hi = i * width * 3 + j * 3;
				int gi = i * width + j;

				lpH[gi] = hsvDataPtr[hi] * 510 / 360;		// lpH = 0 .. 255
				lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
				lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", H);
		imshow("S", S);
		imshow("V", V);

		waitKey();
	}
}

void testResize()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		Mat dst1, dst2;
		//without interpolation
		resizeImg(src, dst1, 320, false);
		//with interpolation
		resizeImg(src, dst2, 320, true);
		imshow("input image", src);
		imshow("resized image (without interpolation)", dst1);
		imshow("resized image (with interpolation)", dst2);
		waitKey();
	}
}

void testCanny()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src, dst, gauss;
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		double k = 0.4;
		int pH = 50;
		int pL = (int)k*pH;
		GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
		Canny(gauss, dst, pL, pH, 3);
		imshow("input image", src);
		imshow("canny", dst);
		waitKey();
	}
}

void testVideoSequence()
{
	VideoCapture cap("Videos/rubic.avi"); // off-line video from file
										  //VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey(0);
		return;
	}

	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
		Canny(grayFrame, edges, 40, 100, 3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = cvWaitKey(0);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n");
			break;  //ESC pressed
		};
	}
}

void testSnap()
{
	VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
	if (!cap.isOpened()) // openenig the video device failed
	{
		printf("Cannot open video capture device.\n");
		return;
	}

	Mat frame;
	char numberStr[256];
	char fileName[256];

	// video resolution
	Size capS = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Display window
	const char* WIN_SRC = "Src"; //window for the source frame
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Snapped"; //window for showing the snapped frame
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, capS.width + 10, 0);

	char c;
	int frameNum = -1;
	int frameCount = 0;

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("End of the video file\n");
			break;
		}

		++frameNum;

		imshow(WIN_SRC, frame);

		c = cvWaitKey(10);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished");
			break;  //ESC pressed
		}
		if (c == 115) { //'s' pressed - snapp the image to a file
			frameCount++;
			fileName[0] = NULL;
			sprintf(numberStr, "%d", frameCount);
			strcat(fileName, "Images/A");
			strcat(fileName, numberStr);
			strcat(fileName, ".bmp");
			bool bSuccess = imwrite(fileName, frame);
			if (!bSuccess)
			{
				printf("Error writing the snapped image\n");
			}
			else
				imshow(WIN_DST, frame);
		}
	}

}

void shapeSpecs(Vec3b color, Mat* img)
{
	int area = 0;
	int height = (int)(*img).rows;
	int width = (int)(*img).cols;
	Mat newImg(height, width, CV_8UC3);
	Mat proj(height, width, CV_8UC3);
	Vec3b val, valN, valS, valE, valW;
	int c = 0;
	int r = 0;
	float denom = 0, nom1 = 0, nom2 = 0;
	int perim = 0;
	bool ok = false;
	int firstI = 0;
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{

			val = (Vec3b)(*img).at<Vec3b>(i, j);
			if (color == val) {
				area++;
				r += i;
				c += j;
			}
			valN = (Vec3b)(*img).at<Vec3b>(i - 1, j);
			valS = (Vec3b)(*img).at<Vec3b>(i + 1, j);
			valE = (Vec3b)(*img).at<Vec3b>(i, j + 1);
			valW = (Vec3b)(*img).at<Vec3b>(i, j - 1);
			if ((val == color) && ((color != valN) || (color != valS) || (color != valE) || (color != valW))) {
				perim++;
				newImg.at<Vec3b>(i, j) = color;
			}
			else
			{
				newImg.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
		}
	}
	int rr = r / area;
	int cc = c / area;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			val = (Vec3b)(*img).at<Vec3b>(i, j);
			if (color == val) {
				denom = denom + (i - rr)*(j - cc);
				nom1 = nom1 + (j - cc)*(j - cc);
				nom2 = nom2 + (i - rr)*(i - rr);
			}
		}
	}
	for (int i = 0; i < height; i++)
	{
		ok = false;
		for (int j = 0; j < width; j++)
		{
			val = (Vec3b)(*img).at<Vec3b>(i, j);
			if (color == val) {
				if (!ok) {
					firstI = j;
					ok = true;
				}
				proj.at<Vec3b>(i, j - firstI) = color;
			}
		}
	}
	firstI = 0;
	for (int i = 0; i < width; i++)
	{
		ok = false;
		for (int j = height - 1; j > 0; j--)
		{
			val = (Vec3b)(*img).at<Vec3b>(j, i);
			if (color == val) {
				if (!ok) {
					firstI = j;
					ok = true;
				}
				proj.at<Vec3b>(i, j + (height - firstI)) = color;
			}
		}
	}
	float angle = atan2(2 * denom, nom1 - nom2) / 2;
	float dAngle = angle / M_PI * 180;
	printf("Area is: %d \n", area);
	printf("The center of mass is at: (%d, %d) \n", rr, cc);
	printf("The angle is: %f \n", dAngle);
	printf("The perimeter is: %d \n", perim);
	float ratio = (float)M_PI * 4 * area / (perim*perim);
	newImg.at<Vec3b>(rr, cc) = Vec3b(0, 0, 0);
	Point start = Point(cc, rr);
	Point end = Point(cc + 100 * cos(angle), rr + 100 * sin(angle));
	line(newImg, start, end, CV_RGB(0, 0, 0), 2);
	printf("Thiness ratio is: %lf \n", ratio);
	imshow("New Iamge", newImg);
	imshow("Projection", proj);
	waitKey(0);
}

void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	Mat* src = (Mat*)param;
	Vec3b rez;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
			x, y,
			(int)(*src).at<Vec3b>(y, x)[2],
			(int)(*src).at<Vec3b>(y, x)[1],
			(int)(*src).at<Vec3b>(y, x)[0]);
		rez = (*src).at<Vec3b>(y, x);
		shapeSpecs(rez, src);

	}
}

void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}

/* Histogram display function - display a histogram using bars (simlilar to L3 / PI)
Input:
name - destination (output) window name
hist - pointer to the vector containing the histogram values
hist_cols - no. of bins (elements) in the histogram = histogram image width
hist_height - height of the histogram image
Call example:
showHistogram ("MyHist", hist_dir, 255, 200);
*/

int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		printf(" 5 - Resize image\n");
		printf(" 6 - Canny edge detection\n");
		printf(" 7 - Edges in a video sequence\n");
		printf(" 8 - Snap frame from live video\n");
		printf(" 9 - Mouse callback demo\n");
		printf(" 10 - Change the gray levels by an additive factor \n");
		printf(" 11 - Change the gray levels by a multiplicative factor \n");
		printf(" 12 - Create new image \n");
		printf(" 13 - Create new matrix \n");
		printf(" 14 - Create wavy \n");
		printf(" 15 - Create three grayscale images \n");
		printf(" 16 - Convert to grayscale image \n");
		printf(" 17 - Convert to binary image \n");
		printf(" 18 - Convert to HSV image \n");
		printf(" 19 - Detect Yellow \n");
		printf(" 20 - BFS labeling \n");
		printf(" 21 - Border tracing \n");
		printf(" 22 - Border file to image \n");
		printf(" 23 - Dilation \n");
		printf(" 24 - Erosion \n");
		printf(" 25 - Boundary extraction \n");
		printf(" 26 - Fill image \n");
		printf(" 27 - Show histogram \n");
		printf(" 28 - Global thresholding \n");
		printf(" 29 - Stretch/shrink histogram \n");
		printf(" 30 - Gama correction \n");
		printf(" 31 - Brightness changing \n");
		printf(" 32 - Histogram equalization \n");
		printf(" 33 - Apply filter \n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d", &op);
		switch (op)
		{
		case 1:
			testOpenImage();
			break;
		case 2:
			testOpenImagesFld();
			break;
		case 3:
			testParcurgereSimplaDiblookStyle(); //diblook style
			break;
		case 4:
			//testColor2Gray();
			testBGR2HSV();
			break;
		case 5:
			testResize();
			break;
		case 6:
			testCanny();
			break;
		case 7:
			testVideoSequence();
			break;
		case 8:
			testSnap();
			break;
		case 9:
			testMouseClick();
			break;
		case 10:
			testGrayAdditiveLevels();
			break;
		case 11:
			testGrayMultLevels();
			break;
		case 12:
			createImage();
			break;
		case 13:
			createMatrix();
			break;
		case 14:
			wavyImage();
			break;
		case 15:
			threeGrayscale();
			break;
		case 16:
			convertToGrayscale();
			break;
		case 17:
			convertToBinary();
			break;
		case 18:
			convertToHSV();
			break;
		case 19:
			detectYellow();
			break;
		case 20:
			BFSLabeling();
			break;
		case 21:
			borderTracing();
			break;
		case 22:
			borderToImage();
			break;
		case 23:
			dilate();
			break;
		case 24:
			erode();
			break;
		case 25:
			boundaryExtraction();
			break;
		case 26:
			regionFilling();
			break;
		case 27:
			computeHistograms();
			break;
		case 28:
			globalThresholding();
			break;
		case 29:
			histogramTransformation();
			break;
		case 30:
			gammaCorrection();
			break;
		case 31:
			brightnessChange();
			break;
		case 32:
			histogramEqualization();
			break;
		case 33:
			applyKernel();
			break;
		}
	} while (op != 0);
	return 0;
}