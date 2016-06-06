#include "ExpertResult.h"

ExpertResult::ExpertResult()
{
	mCenterX = mCenterY = 0;
	mConfidence = 0;
}

ExpertResult::ExpertResult(const ExpertResult& r)
{
	mCenterX = r.mCenterX;
	mCenterY = r.mCenterY;
	mConfidence = r.mConfidence;
}

ExpertResult::~ExpertResult()
{

}

ExpertResult& ExpertResult::operator=(const ExpertResult& r)
{
	if (this == &r)
	{
		return *this;
	}
	mCenterX = r.mCenterX;
	mCenterY = r.mCenterY;
	mConfidence = r.mConfidence;
	return *this;
}

void ExpertResult::setData(const Point& mPoint, double mTConfidence)
{
	mCenterX = mPoint.x;
	mCenterY = mPoint.y;
	mConfidence = mTConfidence;
}

void ExpertResult::setConfidence(double mTConfidence)
{
	mConfidence = mTConfidence;
}

int ExpertResult::getCenterX()
{
	return mCenterX;
}

int ExpertResult::getCenterY()
{
	return mCenterY;
}

double ExpertResult::getConfidence()
{
	return mConfidence;
}
