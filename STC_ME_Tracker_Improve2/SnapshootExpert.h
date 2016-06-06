#pragma once

#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "ExpertResult.h"
#include "Complex.h"
#include "CenterPoint.h"

using namespace std;
using namespace cv;

class NormalExpert;

//快照专家类
class SnapshootExpert
{
public:
	friend class NormalExpert;

private:
	int mFrameIndex;
	//更新计数
	int mUpdateCount;

	Size mCxtSize;	//上下文大小
	Size mFrameSize;	//帧大小

	Point mInnatePoint;	//此为快照专家的固定中心点数据
	CenterPoint mInnateCenterPoint;	//此为快照专家对应的固定中心点坐标数组包装类
	CenterPoint mNewCenterPoint;	//临时保存计算得到的新的中心点数据以及坐标包装类

	Mat mCxtMat;	//上下文数据矩阵
	Mat mCxtPriProMat;	//经过处理后得到的上下文先验概率矩阵
	Mat mCxtPriProMatFFT;	//经过FFT变换后的上下文先验概率矩阵

	Mat mSTCModelFFT;	//经过FFT变换的时空上下文模型矩阵

	ExpertResult mExpertResult;

	Mat mPredictionConfidenceMap;	//用于存储当前帧的预测用的置信图

public:
	SnapshootExpert(const Size& mTCxtSize, const Size& mTFrameSize);
	SnapshootExpert(const SnapshootExpert& r);
	~SnapshootExpert();

public:
	SnapshootExpert& operator=(const SnapshootExpert& r);

public:
	int getFrameIndex();

	void init(int mTFrameIndex, const NormalExpert& mTNormalExpert);

	//根据提供的当前帧数据矩阵和滤波窗口矩阵计算当前帧的上下文数据矩阵
	void getContext(const Mat& mFrame, const Mat& mFilterWindowMat, CenterPoint& mTCenterPoint);

	ExpertResult& calculateExpertResult(const Mat& mFrame, const Mat& mFilterWindowMat);

	//累加更新计数
	void accumulateUpdateCount();

	int getUpdateCount();

};