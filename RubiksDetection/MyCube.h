#ifndef MyCube_HEADER
#define MyCube_HEADER
#include "MyFace.h"

#pragma once
class MyCube
{
	MyFace faceW;
	MyFace faceR;
	MyFace faceB;
	MyFace faceG;
	MyFace faceY;
	MyFace faceO;
public:
	MyCube();
	MyFace getFaceW();
	MyFace getFaceO();
	MyFace getFaceG();
	MyFace getFaceB();
	MyFace getFaceY();
	MyFace getFaceR();
	void printCube();
};

#endif