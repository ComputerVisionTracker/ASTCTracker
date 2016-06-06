#include "Config.h"

const int Config::UPDATE_EXPERT_INTERVAL = 50;
const int Config::MAX_EXPERT_SIZE = 4;
const double Config::LEARN_RATE = 0.075;

const double Config::RESET_THRESHOLD_BY_SNAPEXPERT = 0.1;

const bool Config::LRU_KICK_SNAPEXPERT = false;

const bool Config::CAPTURE_FIRST_FRAME_SNAP = false;

Config::Config()
	: mUpdateExpertInterval(UPDATE_EXPERT_INTERVAL), mMaxExpertSize(MAX_EXPERT_SIZE),
	mResetThresholdBySnapexpert(RESET_THRESHOLD_BY_SNAPEXPERT), mLRUKickSnapexpert(LRU_KICK_SNAPEXPERT),
	mCaptureFirstFrameSnap(CAPTURE_FIRST_FRAME_SNAP)
{
}

Config::Config(int mTUpdateExpertInteval, int mTMaxExpertSize, double mTResetThresholdBySnapexpert,
	bool mTLRUKickSnapexpert, bool mCapFirstSnap)
	: mUpdateExpertInterval(mTUpdateExpertInteval), mMaxExpertSize(mTMaxExpertSize),
	mResetThresholdBySnapexpert(mTResetThresholdBySnapexpert), mLRUKickSnapexpert(mTLRUKickSnapexpert),
	mCaptureFirstFrameSnap(mCapFirstSnap)
{
}

Config::Config(int mTUpdateExpertInteval, int mTMaxExpertSize, double mTResetThresholdBySnapexpert, bool mTLRUKickSnapexpert)
	: mUpdateExpertInterval(mTUpdateExpertInteval), mMaxExpertSize(mTMaxExpertSize),
	mResetThresholdBySnapexpert(mTResetThresholdBySnapexpert), mLRUKickSnapexpert(mTLRUKickSnapexpert),
	mCaptureFirstFrameSnap(CAPTURE_FIRST_FRAME_SNAP)
{
}

Config::Config(int mTUpdateExpertInteval, double mTResetThresholdBySnapexpert, bool mTLRUKickSnapexpert)
	: mUpdateExpertInterval(mTUpdateExpertInteval), mMaxExpertSize(MAX_EXPERT_SIZE),
	mResetThresholdBySnapexpert(mTResetThresholdBySnapexpert), mLRUKickSnapexpert(mTLRUKickSnapexpert),
	mCaptureFirstFrameSnap(CAPTURE_FIRST_FRAME_SNAP)
{
}

Config::Config(const Config& r)
	: mUpdateExpertInterval(r.mUpdateExpertInterval), mMaxExpertSize(r.mMaxExpertSize),
	mResetThresholdBySnapexpert(r.mResetThresholdBySnapexpert), mLRUKickSnapexpert(r.mLRUKickSnapexpert),
	mCaptureFirstFrameSnap(r.mCaptureFirstFrameSnap)
{
}

Config::~Config()
{
}

Config& Config::operator=(const Config& r)
{
	if (this == &r)
	{
		return *this;
	}

	return *this;
}
