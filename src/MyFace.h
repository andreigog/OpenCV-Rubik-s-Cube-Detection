#ifndef MyFace_HEADER
#define MyFace_HEADER
#include "stdafx.h"
#include "common.h"


#pragma once
class MyFace
{
	Mat squares;
public:
	MyFace();
	Mat getSquares();
	void setSquare(int i, int j, Vec3b color);
	void printFace();
};

#endif
