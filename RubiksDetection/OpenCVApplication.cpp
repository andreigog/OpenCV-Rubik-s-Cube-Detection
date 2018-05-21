#include "stdafx.h"
#include "MyCube.h"
#include "MyFace.h"
#include <fstream>
#include <queue>
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define M_PI 3.14159265358979323846
MyCube myCube = MyCube();

struct faceSquare {
	int label;
	Point center;
	Vec3b color;
	int area;
};


void cubeTo2D()
{
	int label = 0;
	int di[8] = { -1, -1, -1, 0, 0, +1, +1, +1 };
	int dj[8] = { -1, 0, +1, -1, +1, -1, 0, +1 };
	char* fname = "C:\\Users\\andrei\\Desktop\\sablon.png";

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
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			if (src.at<uchar>(i, j) == 255 && src.at<uchar>(i, j - 1) == 0 && src.at<uchar>(i - 1, j) == 0 && (labels.at<uchar>(i, j) == 0))
			{
				label++;
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
							if (src.at<uchar>(y + di[k], x + dj[k]) == 255 && labels.at<uchar>(y + di[k], x + dj[k]) == 0)
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
	Vec3b colors[54];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			colors[1 + (i * 3 + j)] = myCube.getFaceB().getSquares().at<Vec3b>(i, j);
		}

	}
	colors[10] = myCube.getFaceO().getSquares().at<Vec3b>(0, 0);
	colors[11] = myCube.getFaceO().getSquares().at<Vec3b>(0, 1);
	colors[12] = myCube.getFaceO().getSquares().at<Vec3b>(0, 2);
	colors[22] = myCube.getFaceO().getSquares().at<Vec3b>(1, 0);
	colors[23] = myCube.getFaceO().getSquares().at<Vec3b>(1, 1);
	colors[24] = myCube.getFaceO().getSquares().at<Vec3b>(1, 2);
	colors[34] = myCube.getFaceO().getSquares().at<Vec3b>(2, 0);
	colors[35] = myCube.getFaceO().getSquares().at<Vec3b>(2, 1);
	colors[36] = myCube.getFaceO().getSquares().at<Vec3b>(2, 2);

	colors[13] = myCube.getFaceW().getSquares().at<Vec3b>(0, 0);
	colors[14] = myCube.getFaceW().getSquares().at<Vec3b>(0, 1);
	colors[15] = myCube.getFaceW().getSquares().at<Vec3b>(0, 2);
	colors[25] = myCube.getFaceW().getSquares().at<Vec3b>(1, 0);
	colors[26] = myCube.getFaceW().getSquares().at<Vec3b>(1, 1);
	colors[27] = myCube.getFaceW().getSquares().at<Vec3b>(1, 2);
	colors[37] = myCube.getFaceW().getSquares().at<Vec3b>(2, 0);
	colors[38] = myCube.getFaceW().getSquares().at<Vec3b>(2, 1);
	colors[39] = myCube.getFaceW().getSquares().at<Vec3b>(2, 2);

	colors[16] = myCube.getFaceR().getSquares().at<Vec3b>(0, 0);
	colors[17] = myCube.getFaceR().getSquares().at<Vec3b>(0, 1);
	colors[18] = myCube.getFaceR().getSquares().at<Vec3b>(0, 2);
	colors[28] = myCube.getFaceR().getSquares().at<Vec3b>(1, 0);
	colors[29] = myCube.getFaceR().getSquares().at<Vec3b>(1, 1);
	colors[30] = myCube.getFaceR().getSquares().at<Vec3b>(1, 2);
	colors[40] = myCube.getFaceR().getSquares().at<Vec3b>(2, 0);
	colors[41] = myCube.getFaceR().getSquares().at<Vec3b>(2, 1);
	colors[42] = myCube.getFaceR().getSquares().at<Vec3b>(2, 2);

	colors[19] = myCube.getFaceY().getSquares().at<Vec3b>(0, 0);
	colors[20] = myCube.getFaceY().getSquares().at<Vec3b>(0, 1);
	colors[21] = myCube.getFaceY().getSquares().at<Vec3b>(0, 2);
	colors[31] = myCube.getFaceY().getSquares().at<Vec3b>(1, 0);
	colors[32] = myCube.getFaceY().getSquares().at<Vec3b>(1, 1);
	colors[33] = myCube.getFaceY().getSquares().at<Vec3b>(1, 2);
	colors[43] = myCube.getFaceY().getSquares().at<Vec3b>(2, 0);
	colors[44] = myCube.getFaceY().getSquares().at<Vec3b>(2, 1);
	colors[45] = myCube.getFaceY().getSquares().at<Vec3b>(2, 2);

	colors[46] = myCube.getFaceG().getSquares().at<Vec3b>(0, 0);
	colors[47] = myCube.getFaceG().getSquares().at<Vec3b>(0, 1);
	colors[48] = myCube.getFaceG().getSquares().at<Vec3b>(0, 2);
	colors[49] = myCube.getFaceG().getSquares().at<Vec3b>(1, 0);
	colors[50] = myCube.getFaceG().getSquares().at<Vec3b>(1, 1);
	colors[51] = myCube.getFaceG().getSquares().at<Vec3b>(1, 2);
	colors[52] = myCube.getFaceG().getSquares().at<Vec3b>(2, 0);
	colors[53] = myCube.getFaceG().getSquares().at<Vec3b>(2, 1);
	colors[54] = myCube.getFaceG().getSquares().at<Vec3b>(2, 2);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			colored.at<Vec3b>(i, j) = colors[labels.at<uchar>(i, j)];
		}
	}
	imshow("colored", colored);
	waitKey(0);
}

long mod(long a, long b)
{
	return (a%b + b) % b;
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
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
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
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
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

Vec3b getFlatColor(Vec3b pixel)
{
	float r, g, b, max, min, c;
	float h, s, v;
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
	//red
	if ((h < 10 || h>350) && s > 0.5)
		return Vec3b(0, 0, 255);
	//green
	if (h > 150 && h<170 && s>0.5)
		return Vec3b(0, 255, 0);
	//blue
	if (h > 220 && h<250 && s>0.5)
		return Vec3b(204, 0, 0);
	//yellow
	if (h > 45 && h<75 && s>0.5)
		return Vec3b(0, 255, 255);
	//orange
	if (h > 10 && h<30 && s>0.5)
		return Vec3b(0, 128, 255);
	//white
	return Vec3b(255, 255, 255);
}

Mat convertToNegative(Mat src) {
	int height = src.rows;
	int width = src.cols;
	Mat dst = Mat(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (src.at<uchar>(i, j) == 0)
				dst.at<uchar>(i, j) = 255;
			else
				dst.at<uchar>(i, j) = 0;
		}
	}
	return dst;
}

Mat convertToGrayscale(Mat src) {
	int height = src.rows;
	int width = src.cols;
	Mat dst = Mat(height, width, CV_8UC3);
	Vec3b pixel;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pixel = src.at<Vec3b>(i, j);
			dst.at<Vec3b>(i, j) = Vec3b((pixel[0] + pixel[1] + pixel[2]) / 3, (pixel[0] + pixel[1] + pixel[2]) / 3, (pixel[0] + pixel[1] + pixel[2]) / 3);
		}
	}
	return dst;
}

Mat binarize(Mat src, int threshold) {
	int height = src.rows;
	int width = src.cols;
	Mat dst = Mat(height, width, CV_8UC1);
	uchar value;
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
	return dst;
}

inline bool operator<(const faceSquare& lhs, const faceSquare& rhs)
{
	return lhs.label < rhs.label;
}

bool sortByX(const faceSquare &a, faceSquare &b)
{
	return a.center.x < b.center.x;
}

bool sortByY(const faceSquare &a, faceSquare &b)
{
	return a.center.y < b.center.y;
}

vector<faceSquare> sort2D(vector<faceSquare> face) {
	std::sort(face.begin(), face.end(), sortByY);
	std::sort(face.begin(), face.begin() + 3, sortByX);
	std::sort(face.begin() + 3, face.begin() + 6, sortByX);
	std::sort(face.begin() + 6, face.begin() + 9, sortByX);
	return face;
}

Mat labelImage(Mat src, Mat original) {
	Mat grayscale = convertToGrayscale(original);
	int label = 0;
	int area = 0;
	int perim = 0;
	int rowsCount = 0;
	int colsCount = 0;
	int valN, valS, valE, valW;
	float ratio;
	int di[8] = { -1, -1, -1, 0, 0, +1, +1, +1 };
	int dj[8] = { -1, 0, +1, -1, +1, -1, 0, +1 };

	int height = src.rows;
	int width = src.cols;
	Mat labels = Mat(height, width, CV_32S);
	Mat colored = Mat(height, width, CV_8UC3);
	Mat avgColor = Mat(height, width, CV_8UC3);
	std::queue<Point2i> Q;
	std::set<faceSquare> squaresSet;
	std::vector<faceSquare> squaresVector;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			labels.at<int>(i, j) = 0;
			colored.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			avgColor.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			if (src.at<uchar>(i, j) == 255 && src.at<uchar>(i, j - 1) == 0 && src.at<uchar>(i - 1, j) == 0 && (labels.at<int>(i, j) == 0))
			{
				area = 0;
				perim = 0;
				rowsCount = 0;
				colsCount = 0;
				label++;
				Point2i point;
				point.x = j;
				point.y = i;
				labels.at<int>(i, j) = label;
				Q.push(point);
				while (!Q.empty())
				{
					area++;
					Point2i p = Q.front();
					Q.pop();
					int x = p.x;
					int y = p.y;
					avgColor.at<Vec3b>(y, x) = Vec3b((avgColor.at<Vec3b>(y, x)[0] + original.at<Vec3b>(y, x)[0]) / 2,
						(avgColor.at<Vec3b>(y, x)[1] + original.at<Vec3b>(y, x)[1]) / 2,
						(avgColor.at<Vec3b>(y, x)[2] + original.at<Vec3b>(y, x)[2]) / 2);

					valN = src.at<uchar>(y - 1, x);
					valS = src.at<uchar>(y + 1, x);
					valE = src.at<uchar>(y, x + 1);
					valW = src.at<uchar>(y, x - 1);
					rowsCount += y;
					colsCount += x;
					if ((valN != 255) || (valS != 255) || (valE != 255) || (valW != 255)) {
						perim++;
					}
					for (int k = 0; k < 8; k++)
					{
						if (y + di[k] < height && y + di[k] >= 0 && x + dj[k] < width && x + dj[k] >= 0)
						{
							if (src.at<uchar>(y + di[k], x + dj[k]) == 255 && labels.at<int>(y + di[k], x + dj[k]) == 0)
							{
								labels.at<int>(y + di[k], x + dj[k]) = label;
								Point2i newp = Point2i(x + dj[k], y + di[k]);
								Q.push(newp);
							}
						}
					}
				}
				rowsCount = rowsCount / area;
				colsCount = colsCount / area;
				Point center = Point(colsCount, rowsCount);
				Vec3b color = avgColor.at<Vec3b>(rowsCount, colsCount);
				color = getFlatColor(color);
				Scalar scalar = Scalar(color[0], color[1], color[2]);
				ratio = (float)M_PI * 4 * area / (perim*perim);
				if (ratio > 0.9f && ratio < 1.1f && area>1000) {
					faceSquare square;
					square.label = label;
					square.center = center;
					square.color = color;
					square.area = area;
					squaresVector.push_back(square);
					squaresSet.insert(square);
					circle(grayscale, center, 10, scalar, -1, 8);
					cv::putText(grayscale,
						std::to_string(square.center.y) + ", " + std::to_string(square.center.x),
						square.center, // Coordinates
						cv::FONT_HERSHEY_SIMPLEX, // Font
						0.4, // Scale. 2.0
						cv::Scalar(0, 0, 0), // Color
						1, // Thickness
						1); // Anti-alias
				}
			}
		}
	}

	boolean allSquares = false;;
	std::vector<faceSquare>::iterator it;

	if (squaresVector.size() == 9) {
		allSquares = true;
		int myArea = squaresVector[0].area;
		Point myCenter1 = squaresVector[0].area;
		Point myCenter2 = squaresVector[3].area;
		Point myCenter3 = squaresVector[6].area;
		for (it = squaresVector.begin(); it != squaresVector.begin()+3; ++it) {
			if ((it->center.x - myCenter1.x > 10) || (myCenter1.x - it->center.x < -10)) {
				allSquares = false;
			}
		}
		for (it = squaresVector.begin()+3; it != squaresVector.begin() + 6; ++it) {
			if ((it->center.x - myCenter2.x > 10) || (myCenter2.x - it->center.x < -10)) {
				allSquares = false;
			}
		}
		for (it = squaresVector.begin()+6; it != squaresVector.begin() + 9; ++it) {
			if ((it->center.x - myCenter3.x > 10) || (myCenter3.x - it->center.x < -10)) {
				allSquares = false;
			}
		}
		for (it = squaresVector.begin(); it != squaresVector.end(); ++it) {
			if ((it->area - myArea) > 200 || (myArea - it->area) < -200) {
				allSquares = false;
			}
		}
	}
	if (allSquares) {
		squaresVector = sort2D(squaresVector);
		faceSquare square = squaresVector[4];
		Vec3b centerColor = square.color;
		//red face
		if (centerColor[0] == 0 && centerColor[1] == 0 && centerColor[2] == 255)
		{
			printf("Reading RED face\n");
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					myCube.getFaceR().setSquare(i, j, squaresVector[i * 3 + j].color);
				}
			}
		}
		//green face
		if (centerColor[0] == 0 && centerColor[1] == 255 && centerColor[2] == 0)
		{
			printf("Reading GREEN face\n");
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					myCube.getFaceG().setSquare(i, j, squaresVector[i * 3 + j].color);
				}
			}
		}
		//blue face
		if (centerColor[0] == 204 && centerColor[1] == 0 && centerColor[2] == 0)
		{
			printf("Reading BLUE face\n");
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					myCube.getFaceB().setSquare(i, j, squaresVector[i * 3 + j].color);
				}
			}
		}
		//yellow face
		if (centerColor[0] == 0 && centerColor[1] == 255 && centerColor[2] == 255)
		{
			printf("Reading YELLOW face\n");
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					myCube.getFaceY().setSquare(i, j, squaresVector[i * 3 + j].color);
				}
			}
		}
		//orange face
		if (centerColor[0] == 0 && centerColor[1] == 128 && centerColor[2] == 255)
		{
			printf("Reading ORANGE face\n");
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					myCube.getFaceO().setSquare(i, j, squaresVector[i * 3 + j].color);
				}
			}
		}
		//white face
		if (centerColor[0] == 255 && centerColor[1] == 255 && centerColor[2] == 255)
		{
			printf("Reading WHITE face\n");
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					myCube.getFaceW().setSquare(i, j, squaresVector[i * 3 + j].color);
				}
			}
		}
	}
	return grayscale;
}

int main()
{
	VideoCapture cap(0); 
	if (!cap.isOpened())  
		return -1;
	Mat edges;
	Mat dst;
	namedWindow("Rubik's Cube Detection", 1);
	while (true)
	{
		Mat frame;
		cap >> frame;
		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		edges = convertToNegative(edges);
		edges = dilation(edges);
		edges = labelImage(edges, frame);
		imshow("Rubik's Cube Detection", edges);
		if (waitKey(30) >= 0) break;
	}


	myCube.printCube();
	cubeTo2D();
	int x;
	scanf("%d", &x);
	return 0;
}