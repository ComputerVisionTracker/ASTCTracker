#include "CenterPoint.h"

CenterPoint::CenterPoint(const Size& mTCxtSize, const Size& mTFrameSize)
{
	mCxtSize = mTCxtSize;
	mFrameSize = mTFrameSize;

	mCxtX = new int[mCxtSize.width];
	mCxtY = new int[mCxtSize.height];
}

CenterPoint::CenterPoint(const CenterPoint& r)
{
	mCxtSize = r.mCxtSize;
	mFrameSize = r.mFrameSize;

	mCenterPoint = r.mCenterPoint;

	mCxtX = new int[mCxtSize.width];
	for (int i = 0; i < mCxtSize.width; i++)
	{
		mCxtX[i] = r.mCxtX[i];
	}
	mCxtY = new int[mCxtSize.height];
	for (int i = 0; i < mCxtSize.height; i++)
	{
		mCxtY[i] = r.mCxtY[i];
	}
}

CenterPoint::~CenterPoint()
{
	delete[] mCxtX;
	delete[] mCxtY;
}

CenterPoint& CenterPoint::operator=(const CenterPoint& r)
{
	if (this == &r)
	{
		return *this;
	}

	if (mCxtSize.width != r.mCxtSize.width || mCxtSize.height != r.mCxtSize.height)
	{
		mCxtSize = r.mCxtSize;
		mCxtX = new int[mCxtSize.width];
		mCxtY = new int[mCxtSize.height];
	}

	mFrameSize = r.mFrameSize;
	mCenterPoint = r.mCenterPoint;

	for (int i = 0; i < mCxtSize.width; i++)
	{
		mCxtX[i] = r.mCxtX[i];
	}
	for (int i = 0; i < mCxtSize.height; i++)
	{
		mCxtY[i] = r.mCxtY[i];
	}

	return *this;
}

void CenterPoint::setCenterPoint(const Point& mTCenterPoint)
{
	mCenterPoint = mTCenterPoint;

	int *temp = NULL;
	temp = mCxtX;
	for (int i = 0; i < mCxtSize.width; i++)
	{
		*temp = floor(mCenterPoint.x + (i - mCxtSize.width / 2.0));
		if (*temp < 0)
		{
			*temp = 0;
		}
		if (*temp >= mFrameSize.width)
		{
			*temp = mFrameSize.width - 1;
		}
		temp++;
	}

	temp = mCxtY;
	for (int i = 0; i < mCxtSize.height; i++)
	{
		*temp = floor(mCenterPoint.y + (i - mCxtSize.height / 2.0));
		if (*temp < 0)
		{
			*temp = 0;
		}
		if (*temp >= mFrameSize.height)
		{
			*temp = mFrameSize.height - 1;
		}
		temp++;
	}
}

Point& CenterPoint::getCenterPoint()
{
	return mCenterPoint;
}

int* CenterPoint::getCxtXPointer()
{
	return mCxtX;
}

int* CenterPoint::getCxtYPointer()
{
	return mCxtY;
}