#include "Complex.h"

void complexMultiply(const Mat& src1, const Mat& src2, Mat& dst)
{
	Mat *mAReal, *mAImag, *mBReal, *mBImag, *mRReal, *mRImag;

	vector<Mat> mAPlanes;
	split(src1, mAPlanes);
	mAReal = &mAPlanes[0];
	mAImag = &mAPlanes[1];

	vector<Mat> mBPlanes;
	split(src2, mBPlanes);
	mBReal = &mBPlanes[0];
	mBImag = &mBPlanes[1];

	dst.create(src1.rows, src1.cols, CV_64FC2);
	vector<Mat> mRPlanes;
	split(dst, mRPlanes);
	mRReal = &mRPlanes[0];
	mRImag = &mRPlanes[1];

	double *mARealData, *mAImagData, *mBRealData, *mBImagData, *mRRealData, *mRImagData;

	for (int i = 0; i < src1.rows; i++)
	{
		mARealData = (double *)(mAReal->data + mAReal->step * i);
		mAImagData = (double *)(mAImag->data + mAImag->step * i);
		mBRealData = (double *)(mBReal->data + mBReal->step * i);
		mBImagData = (double *)(mBImag->data + mBImag->step * i);
		mRRealData = (double *)(mRReal->data + mRReal->step * i);
		mRImagData = (double *)(mRImag->data + mRImag->step * i);

		for (int j = 0; j < src1.cols; j++)
		{
			mRRealData[j] = mARealData[j] * mBRealData[j] - mAImagData[j] * mBImagData[j];
			mRImagData[j] = mARealData[j] * mBImagData[j] + mAImagData[j] * mBRealData[j];
		}

	}
	merge(mRPlanes, dst);
}


void complexDivide(const Mat& src1, const Mat& src2, Mat& dst)
{
	Mat *mAReal, *mAImag, *mBReal, *mBImag, *mRReal, *mRImag;

	vector<Mat> mAPlanes;
	split(src1, mAPlanes);
	mAReal = &mAPlanes[0];
	mAImag = &mAPlanes[1];

	vector<Mat> mBPlanes;
	split(src2, mBPlanes);
	mBReal = &mBPlanes[0];
	mBImag = &mBPlanes[1];

	dst.create(src1.rows, src1.cols, CV_64FC2);
	vector<Mat> mRPlanes;
	split(dst, mRPlanes);
	mRReal = &mRPlanes[0];
	mRImag = &mRPlanes[1];

	double *mARealData, *mAImagData, *mBRealData, *mBImagData, *mRRealData, *mRImagData;

	for (int i = 0; i < src1.rows; i++)
	{
		mARealData = (double *)(mAReal->data + mAReal->step * i);
		mAImagData = (double *)(mAImag->data + mAImag->step * i);
		mBRealData = (double *)(mBReal->data + mBReal->step * i);
		mBImagData = (double *)(mBImag->data + mBImag->step * i);
		mRRealData = (double *)(mRReal->data + mRReal->step * i);
		mRImagData = (double *)(mRImag->data + mRImag->step * i);

		for (int j = 0; j < src1.cols; j++)
		{
			mRRealData[j] = (mARealData[j] * mBRealData[j] + mAImagData[j] * mBImagData[j]) / (mBRealData[j] * mBRealData[j] + mBImagData[j] * mBImagData[j]);
			mRImagData[j] = (mAImagData[j] * mBRealData[j] - mARealData[j] * mBImagData[j]) / (mBRealData[j] * mBRealData[j] + mBImagData[j] * mBImagData[j]);
		}

	}
	merge(mRPlanes, dst);
}