#include "stdafx.h"
#include "MyCube.h"


MyCube::MyCube()
{
	faceB = MyFace();
	faceG = MyFace();
	faceR = MyFace();
	faceY = MyFace();
	faceO = MyFace();
	faceW = MyFace();

}

MyFace MyCube::getFaceW() {
	return faceW;
}

MyFace MyCube::getFaceY() {
	return faceY;
}

MyFace MyCube::getFaceR() {
	return faceR;
}

MyFace MyCube::getFaceG() {
	return faceG;
}

MyFace MyCube::getFaceB() {
	return faceB;
}

MyFace MyCube::getFaceO() {
	return faceO;
}

void MyCube::printCube() {
	faceB.printFace();
	faceG.printFace();
	faceR.printFace();
	faceY.printFace();
	faceO.printFace();
	faceW.printFace();
}
