#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

class ExpertResult
{
private:
	int mCenterX;
	int mCenterY;
	double mConfidence;

public:
	ExpertResult();
	ExpertResult(const ExpertResult& r);
	~ExpertResult();

public:
	ExpertResult& operator=(const ExpertResult& r);

public:
	void setData(const Point& mPoint, double mTConfidence);
	void setConfidence(double mTConfidence);

public:
	int getCenterX();
	int getCenterY();
	double getConfidence();

};