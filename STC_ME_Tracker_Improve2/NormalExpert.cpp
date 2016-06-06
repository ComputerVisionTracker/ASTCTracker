#include "NormalExpert.h"

NormalExpert::NormalExpert(const Size& mTCxtSize, const Size& mTFrameSize)
{
	mCxtSize = mTCxtSize;
	mFrameSize = mTFrameSize;

	mCxtX = new int[mCxtSize.width];
	mCxtY = new int[mCxtSize.height];

	mCxtMat = Mat::zeros(mCxtSize, CV_64FC1);
	mCxtPriProMat = Mat::zeros(mCxtSize, CV_64FC1);
	mCxtPriProMatFFT = Mat::zeros(mCxtSize, CV_64FC2);

	mSCModelFFT = Mat::zeros(mCxtSize, CV_64FC2);
	mSTCModelFFT = Mat::zeros(mCxtSize, CV_64FC2);
}

NormalExpert::NormalExpert(const NormalExpert& r)
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

	mCxtMat = Mat::zeros(mCxtSize, CV_64FC1);
	r.mCxtMat.copyTo(mCxtMat);
	mCxtPriProMat = Mat::zeros(mCxtSize, CV_64FC1);
	r.mCxtPriProMat.copyTo(mCxtPriProMat);
	mCxtPriProMatFFT = Mat::zeros(mCxtSize, CV_64FC2);
	r.mCxtPriProMatFFT.copyTo(mCxtPriProMatFFT);

	mSCModelFFT = Mat::zeros(mCxtSize, CV_64FC2);
	r.mSCModelFFT.copyTo(mSCModelFFT);
	mSTCModelFFT = Mat::zeros(mCxtSize, CV_64FC2);
	r.mSTCModelFFT.copyTo(mSTCModelFFT);

	mExpertResult = r.mExpertResult;

	r.mPredictionConfidenceMap.copyTo(mPredictionConfidenceMap);

}

NormalExpert::~NormalExpert()
{
	delete[] mCxtX;
	delete[] mCxtY;
}

NormalExpert& NormalExpert::operator=(const NormalExpert& r)
{
	if (this == &r)
	{
		return *this;
	}

	mCenterPoint = r.mCenterPoint;

	if (mCxtSize.width != r.mCxtSize.width || mCxtSize.height != r.mCxtSize.height)
	{
		mCxtSize = r.mCxtSize;

		if (mCxtX != NULL)
		{
			delete[] mCxtX;
		}
		if (mCxtY != NULL)
		{
			delete[] mCxtY;
		}

		mCxtMat = Mat::zeros(mCxtSize, CV_64FC1);
		mCxtPriProMat = Mat::zeros(mCxtSize, CV_64FC1);
		mCxtPriProMatFFT = Mat::zeros(mCxtSize, CV_64FC2);

		mSCModelFFT = Mat::zeros(mCxtSize, CV_64FC2);
		mSTCModelFFT = Mat::zeros(mCxtSize, CV_64FC2);
	}

	for (int i = 0; i < mCxtSize.width; i++)
	{
		mCxtX[i] = r.mCxtX[i];
	}
	for (int i = 0; i < mCxtSize.height; i++)
	{
		mCxtY[i] = r.mCxtY[i];
	}

	r.mCxtMat.copyTo(mCxtMat);
	r.mCxtPriProMat.copyTo(mCxtPriProMat);
	r.mCxtPriProMatFFT.copyTo(mCxtPriProMatFFT);

	r.mSCModelFFT.copyTo(mSCModelFFT);
	r.mSTCModelFFT.copyTo(mSTCModelFFT);

	mExpertResult = r.mExpertResult;

	r.mPredictionConfidenceMap.copyTo(mPredictionConfidenceMap);

	return *this;
}

void NormalExpert::setCenterPoint(const Point& mTPoint)
{
	mCenterPoint = mTPoint;

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

void NormalExpert::getContext(const Mat& mFrame, const Mat& mFilterWindowMat)
{
	double *mData = NULL;
	uchar *mFrameData = NULL;
	double mPxSum = 0, mPxAverage = 0;

	for (int i = 0; i < mCxtSize.height; i++)
	{

		mData = (double *)(mCxtMat.data + mCxtMat.step * i);
		mFrameData = (uchar *)(mFrame.data + mFrame.step * mCxtY[i]);
		for (int j = 0; j < mCxtSize.width; j++)
		{
			mData[j] = mFrameData[mCxtX[j]];
			mPxSum += mData[j];
		}

	}
	mPxAverage = mPxSum / (mCxtSize.width * mCxtSize.height);

	mCxtMat.convertTo(mCxtMat, -1, 1, -mPxAverage);

	mCxtPriProMat = mFilterWindowMat.mul(mCxtMat);

	Mat priorFourier;
	Mat planes[] = { mCxtPriProMat, Mat::zeros(mCxtPriProMat.size(), CV_64FC1) };
	merge(planes, 2, priorFourier);
	dft(priorFourier, mCxtPriProMatFFT);
}



void NormalExpert::setSTCModel(const Mat& mTSTCModel)
{
	mTSTCModel.copyTo(mSTCModelFFT);
}

void NormalExpert::updateSTCModel(double mTLearnRate)
{
	addWeighted(mSTCModelFFT, 1.0 - mTLearnRate, mSCModelFFT, mTLearnRate, 0.0, mSTCModelFFT);
}

Mat& NormalExpert::calculateSCModel(const Mat& mTCxtPostProMatFFT)
{
	complexDivide(mTCxtPostProMatFFT, mCxtPriProMatFFT, mSCModelFFT);
	return mSCModelFFT;
}

ExpertResult& NormalExpert::calculateExpertResult(const Mat& mFrame, const Mat& mFilterWindowMat)
{
	getContext(mFrame, mFilterWindowMat);

	Mat mConditionalFourier;
	complexMultiply(mSTCModelFFT, mCxtPriProMatFFT, mConditionalFourier);
	dft(mConditionalFourier, mPredictionConfidenceMap, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

	Point point;
	minMaxLoc(mPredictionConfidenceMap, 0, 0, 0, &point);
	if (point.x == -1 || point.y == -1)
	{
		cout << "找不到最大置信度的点，程序已经无法继续执行，不在进行预测" << endl;
		mExpertResult.setConfidence(0.001);
		return mExpertResult;
	}

	Point mNewPoint;
	//这里要加一个像素的偏移量，是因为point得到的是矩阵中元素的坐标，尺度范围为0到(size-1)，而mCxtRegionSize的宽高除二得到的中心坐标的尺度范围为1到size
	//故这里加上一个像素的偏移量，使两种坐标落到一个尺度范围内，便可得到中心坐标移动的偏移量
	mNewPoint.x = mCenterPoint.x + point.x + 1 - mCxtSize.width / 2;
	mNewPoint.y = mCenterPoint.y + point.y + 1 - mCxtSize.height / 2;

	setCenterPoint(mNewPoint);
	getContext(mFrame, mFilterWindowMat);

	complexMultiply(mSTCModelFFT, mCxtPriProMatFFT, mConditionalFourier);
	dft(mConditionalFourier, mPredictionConfidenceMap, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	minMaxLoc(mPredictionConfidenceMap, 0, 0, 0, &point);

	mExpertResult.setData(mCenterPoint, *(((double *)(mPredictionConfidenceMap.data + point.y * mPredictionConfidenceMap.step)) + point.x));

	return mExpertResult;
}

void NormalExpert::resetBySnapshootExpert(SnapshootExpert& mTSnapExpert)
{
	setCenterPoint(mTSnapExpert.mNewCenterPoint.getCenterPoint());

	mTSnapExpert.mCxtPriProMatFFT.copyTo(mCxtPriProMatFFT);
	mTSnapExpert.mSTCModelFFT.copyTo(mSTCModelFFT);

	mExpertResult = mTSnapExpert.mExpertResult;
}