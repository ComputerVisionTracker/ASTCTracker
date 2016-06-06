#include "Experts.h"

Experts::Experts(const Size& mTCxtSize, const Size& mTFrameSize, Config *mTConfig)
	:mNormalExpert(mTCxtSize, mTFrameSize), mTSnapshootExpert(mTCxtSize, mTFrameSize)
{
	mConfigPtr = mTConfig;
	mExpertsConfiArray = new double[(*mConfigPtr).mMaxExpertSize + 1];
	mSnapExpertsSize = 0;
}

Experts::Experts(const Experts& r)
	: mNormalExpert(r.mNormalExpert), mTSnapshootExpert(r.mTSnapshootExpert)
{
	mConfigPtr = r.mConfigPtr;

	mSnapExpertArr = r.mSnapExpertArr;
	mExpertResultArr = r.mExpertResultArr;

	mExpertsConfiArray = new double[(*mConfigPtr).mMaxExpertSize + 1];
	for (int i = (*mConfigPtr).mMaxExpertSize; i >= 0; i--)
	{
		mExpertsConfiArray[i] = r.mExpertsConfiArray[i];
	}

	mSnapExpertsSize = r.mSnapExpertsSize;
}

Experts::~Experts()
{
	delete[] mExpertsConfiArray;
}

Experts& Experts::operator=(const Experts& r)
{
	if (this == &r)
	{
		return *this;
	}

	mConfigPtr = r.mConfigPtr;

	mNormalExpert = r.mNormalExpert;
	mTSnapshootExpert = r.mTSnapshootExpert;

	mSnapExpertArr = r.mSnapExpertArr;
	mExpertResultArr = r.mExpertResultArr;

	for (int i = (*mConfigPtr).mMaxExpertSize; i >= 0; i--)
	{
		mExpertsConfiArray[i] = r.mExpertsConfiArray[i];
	}
	mSnapExpertsSize = r.mSnapExpertsSize;
	return *this;
}

void Experts::increaseSnapExpert(int mTFrameIndex)
{
	if (mSnapExpertsSize < (*mConfigPtr).mMaxExpertSize)
	{
		mSnapExpertsSize++;
	}

	while (mSnapExpertArr.size() >= (*mConfigPtr).mMaxExpertSize)
	{
		if ((*mConfigPtr).mLRUKickSnapexpert)
		{
			eraseASnapExpert();
		}
		else
		{
			mSnapExpertArr.erase(mSnapExpertArr.begin());
		}
	}
	mTSnapshootExpert.init(mTFrameIndex, mNormalExpert);
	mSnapExpertArr.push_back(mTSnapshootExpert);
}

bool Experts::eraseASnapExpert()
{
	int mTUpdateCount = mSnapExpertArr.at(0).getUpdateCount();
	for (int i = 1; i < mSnapExpertsSize; i++)
	{
		if (mTUpdateCount > mSnapExpertArr.at(i).getUpdateCount())
		{
			mTUpdateCount = mSnapExpertArr.at(i).getUpdateCount();
		}
	}

	vector<SnapshootExpert>::iterator mStartIterator;

	for (mStartIterator = mSnapExpertArr.begin(); mStartIterator != mSnapExpertArr.end(); mStartIterator++)
	{
		if (mStartIterator->getUpdateCount() == mTUpdateCount)
		{
			mSnapExpertArr.erase(mStartIterator);
			return true;
		}
	}
	return false;
}

ExpertResult& Experts::calculateExpertResult(const Mat& mFrame, const Mat& mFilterWindowMat, int mTFrameIndex)
{
	mExpertResultArr.clear();
	mExpertResultArr.push_back(mNormalExpert.calculateExpertResult(mFrame, mFilterWindowMat));
	
	for (int i = 0; i < mSnapExpertsSize; i++)
	{
		mExpertResultArr.push_back(mSnapExpertArr.at(i).calculateExpertResult(mFrame, mFilterWindowMat));
	}
	for (int i = 0; i <= mSnapExpertsSize; i++)
	{
		mExpertsConfiArray[i] = mExpertResultArr.at(i).getConfidence();
	}

	int index = 0;
	double maxconfi = mExpertsConfiArray[0];
	
	for (int i = 1; i <= mSnapExpertsSize; i++)
	{
		if (maxconfi < mExpertsConfiArray[i])
		{
			maxconfi = mExpertsConfiArray[i];
			index = i;
		}
	}

	//发生快照专家找出更好的结果，回滚覆盖普通专家的特征模板
	if (index != 0)
	{
		if ((maxconfi - mExpertsConfiArray[0]) / mExpertsConfiArray[0] >(*mConfigPtr).mResetThresholdBySnapexpert)
		{
			mSnapExpertArr.at(index - 1).accumulateUpdateCount();
			
			/*if (mTFrameIndex == 228)
			{
			for (int i = 0; i < mSnapExpertsSize; i++)
			{
			cout << mSnapExpertArr.at(i).getFrameIndex() << endl;
			}
			}*/

			//cout << "第" << mTFrameIndex << "发生快照回滚，" << mSnapExpertArr.at(index - 1).getFrameIndex() << endl;
			mNormalExpert.resetBySnapshootExpert(mSnapExpertArr.at(index - 1));
		}
	}
	
	return mExpertResultArr.at(index);
}

void Experts::setCenterPoint(const Point& mTPoint)
{
	mNormalExpert.setCenterPoint(mTPoint);
}

void Experts::getContext(const Mat& mFrame, const Mat& mFilterWindowMat)
{
	mNormalExpert.getContext(mFrame, mFilterWindowMat);
}

void Experts::setSTCModel(const Mat& mTSTCModel)
{
	mNormalExpert.setSTCModel(mTSTCModel);
}

Mat& Experts::calculateSCModel(const Mat& mTCxtPostProMatFFT)
{
	return mNormalExpert.calculateSCModel(mTCxtPostProMatFFT);
}

void Experts::updateSTCModel(double mTLearnRate)
{
	mNormalExpert.updateSTCModel(mTLearnRate);

}
