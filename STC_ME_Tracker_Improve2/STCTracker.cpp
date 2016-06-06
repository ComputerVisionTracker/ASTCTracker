#include "STCTracker.h"


STCTracker::STCTracker()
{
	out.open("STC_Result.txt", ios::out);

	mFrameIndex = 0;
	mIntervalNumber = 5;
	alpha = 2.25;
	beta = 1;
	mLearnRate = 0.075;
	mScale = 1.0;
	mScaleLearnRate = 0.25;


	mMinConfiPosi = 1;

}


STCTracker::~STCTracker()
{
	out.flush();
	out.close();
	if (mConfidenceList != NULL)
	{
		delete mConfidenceList;
	}
	if (mExperts != NULL)
	{
		delete mExperts;
	}
}


void STCTracker::createHammingWindow()
{
	int mCols = mHammingWinMat.cols;
	int mRows = mHammingWinMat.rows;

	double *mHammingValue = new double[mRows];
	double *mHanningValue = new double[mCols];

	for (int i = 0; i < mRows; i++)
	{
		mHammingValue[i] = 0.54 - 0.46*cos(2 * CV_PI*i / (mRows - 1));
	}

	for (int i = 0; i < mCols; i++)
	{
		mHanningValue[i] = 0.5 - 0.5*cos(2 * CV_PI*i / (mCols - 1));
	}
	
	double *mData = NULL;

	for (int i = 0; i < mRows; i++)
	{
		mData = (double *)(mHammingWinMat.data + i * mHammingWinMat.step);
		for (int j = 0; j < mCols; j++)
		{
			mData[j] = mHanningValue[j] * mHammingValue[i];
		}

	}

	delete[] mHammingValue;
	delete[] mHanningValue;
}

void STCTracker::createEucliDistMatAndCxtPostPro()
{
	double *mXArray, *mYArray, *mXTempPtr, *mYTempPtr;

	mXArray = new double[mCxtRegionSize.width];
	mYArray = new double[mCxtRegionSize.height];

	double mXMiddle = floor((mCxtRegionSize.width - 1 + 0.000001) / 2);
	double mYMiddle = floor((mCxtRegionSize.height - 1 + 0.000001) / 2);

	mXTempPtr = mXArray;
	for (int i = 0; i < mCxtRegionSize.width; i++)
	{
		*mXTempPtr = (i - mXMiddle);
		mXTempPtr++;
	}

	mYTempPtr = mYArray;
	for (int i = 0; i < mCxtRegionSize.height; i++)
	{
		*mYTempPtr = (i - mYMiddle);
		mYTempPtr++;
	}
	double *mEucData = NULL;
	double *mCxtPostProData = NULL;
	double mPostSum = 0;
	for (int i = 0; i < mCxtRegionSize.height; i++)
	{
		mEucData = (double *)(mEucliDistMat.data + mEucliDistMat.step * i);
		mCxtPostProData = (double *)(mCxtPostProMat.data + mCxtPostProMat.step * i);

		mXTempPtr = mXArray;
		mYTempPtr = mYArray + i;
		for (int j = 0; j < mCxtRegionSize.width; j++)
		{
			mEucData[j] = *mYTempPtr * *mYTempPtr + *mXTempPtr * *mXTempPtr;
			mXTempPtr++;

			mCxtPostProData[j] = exp(-pow(0.5 * sqrt(mEucData[j]) / alpha, beta));
			mPostSum += mCxtPostProData[j];
		}
	}
	mCxtPostProMat.convertTo(mCxtPostProMat, -1, 1.0 / mPostSum);
	
	//预先计算出后验概率，即标准置信图
	Mat planes1[] = { mCxtPostProMat, Mat::zeros(mCxtPostProMat.size(), CV_64FC1) };
	merge(planes1, 2, mCxtPostProFFTMat);
	dft(mCxtPostProFFTMat, mCxtPostProFFTMat);

	delete[] mXArray;
	delete[] mYArray;
}

/*
 * 因为sigma每帧都在变化，所以滤波窗每一帧都需计算
 */
void STCTracker::getFilterWindow()
{
	double *mFilterData = NULL, *mEucliData = NULL, *mHammData = NULL, mSum = 0;

	for (int i = 0; i < mCxtRegionSize.height; i++)
	{
		mFilterData = (double *)(mFilterWindowMat.data + mFilterWindowMat.step * i);
		mEucliData = (double *)(mEucliDistMat.data + mEucliDistMat.step * i);
		mHammData = (double *)(mHammingWinMat.data + mHammingWinMat.step * i);
		for (int j = 0; j < mCxtRegionSize.width; j++)
		{
			mFilterData[j] = mHammData[j] * exp(-mEucliData[j] / (2 * sigma * sigma));
			mSum += mFilterData[j];
		}

	}
	mFilterWindowMat.convertTo(mFilterWindowMat, -1, 1 / mSum);

}



/************************************************************************/
/* 首先初始化一个够长的数组用于存储每一帧对应的置信度，用于处理尺度缩放
/************************************************************************/
void STCTracker::init(const Mat& mFrame, const Rect& mBox, int mFrameCount, Config *mTConfig)
{
	mFrameIndex = 0;

	if (mConfidenceList != NULL)
	{
		delete mConfidenceList;
	}
	mConfidenceList = new ConfidenceList(mIntervalNumber);

	sigma = 0.5 * (mBox.width + mBox.height);
	mCenter.x = mBox.x + mBox.width / 2;
	mCenter.y = mBox.y + mBox.height / 2;

	mRectSize.width = mBox.width;
	mRectSize.height = mBox.height;

	mCxtRegionSize.width = mBox.width * 2;
	mCxtRegionSize.height = mBox.height * 2;

	if (mCxtRegionSize.width > mFrame.cols)
	{
		mCxtRegionSize.width = mFrame.cols;
	}

	if (mCxtRegionSize.width < mBox.width)
	{
		mCxtRegionSize.width = mBox.width;
	}

	if (mCxtRegionSize.height >= mFrame.rows)
	{
		mCxtRegionSize.height = mFrame.rows;
	}

	if (mCxtRegionSize.height < mBox.height)
	{
		mCxtRegionSize.height = mBox.height;
	}


	if (mExperts != NULL)
	{
		delete mExperts;
	}
	mExperts = new Experts(mCxtRegionSize, Size(mFrame.cols, mFrame.rows), mTConfig);

	mCxtPostProMat = Mat::zeros(mCxtRegionSize.height, mCxtRegionSize.width, CV_64FC1);
	mFilterWindowMat = Mat::zeros(mCxtRegionSize.height, mCxtRegionSize.width, CV_64FC1);
	mEucliDistMat = Mat::zeros(mCxtRegionSize.height, mCxtRegionSize.width, CV_64FC1);
	createEucliDistMatAndCxtPostPro();

	mHammingWinMat = Mat::zeros(mCxtRegionSize.height, mCxtRegionSize.width, CV_64FC1);
	createHammingWindow();

	getFilterWindow();
	
	mExperts->setCenterPoint(mCenter);
	mExperts->getContext(mFrame, mFilterWindowMat);
	Mat mTSCModel = mExperts->calculateSCModel(mCxtPostProFFTMat);
	mExperts->setSTCModel(mTSCModel);

	if (mTConfig->mCaptureFirstFrameSnap)
	{
		mExperts->increaseSnapExpert(mFrameIndex + 1);
	}
	
}


void STCTracker::track(const Mat& mFrame, Rect& mBox, Config& mTConfig)
{
	mFrameIndex++;

	sigma = sigma * mScale;

	getFilterWindow();

	ExpertResult mExpertResult = mExperts->calculateExpertResult(mFrame, mFilterWindowMat, mFrameIndex + 1);

	double mTempConfi = mExpertResult.getConfidence();
	if (mTempConfi > 0.001)
	{
		if (mTempConfi < mMinConfiPosi)
		{
			mMinConfiPosi = mTempConfi;
		}
	}
	else
	{
		mTempConfi = mMinConfiPosi;
	}
	
	mConfidenceList->addConfidence(mTempConfi);

	if (mFrameIndex % (mIntervalNumber + 1) == 0)
	{
		mScale = (1 - mScaleLearnRate)*mScale + mScaleLearnRate*(mConfidenceList->calculateEstimateScale());
	}
	
	mRectSize.width = mRectSize.width * mScale;
	mRectSize.height = mRectSize.height * mScale;
	mBox.width = (int)mRectSize.width;
	mBox.height = (int)mRectSize.height;
	mBox.x = mExpertResult.getCenterX() - mBox.width / 2;
	mBox.y = mExpertResult.getCenterY() - mBox.height / 2;

	mExperts->calculateSCModel(mCxtPostProFFTMat);
	mExperts->updateSTCModel(mLearnRate);

	if (mFrameIndex % mTConfig.mUpdateExpertInterval == 0)
	{
		mExperts->increaseSnapExpert(mFrameIndex + 1);
	}
}
