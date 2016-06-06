#pragma once

//配置类
class Config
{
private:
	//快照专家更新间隔
	static const int UPDATE_EXPERT_INTERVAL;
	//最大的快照专家数量
	static const int MAX_EXPERT_SIZE;
	//学习速率
	static const double LEARN_RATE;

	//当快照专家发现了更好结果，询问是否需要覆盖正常专家时，
	//该快照专家提供的置信度应该超过普通专家置信度的阈值
	static const double RESET_THRESHOLD_BY_SNAPEXPERT;

	//是否使用最近未使用原则剔除快照专家，true为使用，false为不使用
	static const bool LRU_KICK_SNAPEXPERT;

	static const bool CAPTURE_FIRST_FRAME_SNAP;

public:
	friend class Experts;
	friend class STCTracker;

private:
	const int mUpdateExpertInterval;
	const int mMaxExpertSize;
	const double mResetThresholdBySnapexpert;
	const bool mLRUKickSnapexpert;
	const bool mCaptureFirstFrameSnap;

public:
	Config();
	Config(int mTUpdateExpertInteval, int mTMaxExpertSize, double mTResetThresholdBySnapexpert,
		bool mTLRUKickSnapexpert, bool mCapFirstSnap);
	Config(int mTUpdateExpertInteval, int mTMaxExpertSize, double mTResetThresholdBySnapexpert,
		bool mTLRUKickSnapexpert);
	Config(int mTUpdateExpertInteval, double mTResetThresholdBySnapexpert, bool mTLRUKickSnapexpert);
	Config(const Config& r);
	~Config();

public:
	Config& operator=(const Config& r);


};