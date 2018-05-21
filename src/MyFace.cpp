#include "stdafx.h"
#include "MyFace.h"


MyFace::MyFace()
{
	squares = Mat(3, 3, CV_8UC3);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			squares.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
		}
	}
}

Mat MyFace::getSquares() {
	return squares;
}

void MyFace::setSquare(int i, int j, Vec3b color) {
	//squares.at<Vec3b>(i, j) = color;
	if (squares.at<Vec3b>(i, j)[0] == 255 && squares.at<Vec3b>(i, j)[1] == 255 && squares.at<Vec3b>(i, j)[2] == 255) {
		squares.at<Vec3b>(i, j) = color;
	}
}

void MyFace::printFace() {
	printf("Face:\n");
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vec3b centerColor = squares.at<Vec3b>(i, j);
			//red face
			if (centerColor[0] == 0 && centerColor[1] == 0 && centerColor[2] == 255)
			{
				printf("RED ");
			}
			//green face
			if (centerColor[0] == 0 && centerColor[1] == 255 && centerColor[2] == 0)
			{
				printf("GREEN ");
			}
			//blue face
			if (centerColor[0] == 204 && centerColor[1] == 0 && centerColor[2] == 0)
			{
				printf("BLUE ");
			}
			//yellow face
			if (centerColor[0] == 0 && centerColor[1] == 255 && centerColor[2] == 255)
			{
				printf("YELLOW ");
			}
			//orange face
			if (centerColor[0] == 0 && centerColor[1] == 128 && centerColor[2] == 255)
			{
				printf("ORANGE ");
			}
			//white face
			if (centerColor[0] == 255 && centerColor[1] == 255 && centerColor[2] == 255)
			{
				printf("WHITE ");
			}
		}
		printf("\n");
	}
}