#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

class CenterPoint
{
private:
	Size mCxtSize;	//上下文大小
	Size mFrameSize;	//帧大小

	Point mCenterPoint;
	//分别存储的是对应的局部空间上下文区域子图像的横纵坐标的数组
	int *mCxtX;
	int *mCxtY;

public:
	CenterPoint(const Size& mTCxtSize, const  Size& mTFrameSize);
	CenterPoint(const CenterPoint& r);
	~CenterPoint();

public:
	CenterPoint& operator=(const CenterPoint& r);

public:

	void setCenterPoint(const Point& mTCenterPoint);

	Point& getCenterPoint();

	int* getCxtXPointer();

	int* getCxtYPointer();

};