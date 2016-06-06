#pragma once

#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "ExpertResult.h"
#include "SnapshootExpert.h"
#include "NormalExpert.h"
#include "Config.h"

using namespace std;
using namespace cv;

//专家组类
class Experts
{
private:
	Config *mConfigPtr;

private:
	NormalExpert mNormalExpert;

	//临时快照专家
	SnapshootExpert mTSnapshootExpert;
	vector<SnapshootExpert> mSnapExpertArr;

	vector<ExpertResult> mExpertResultArr;
	double* mExpertsConfiArray;	//0号专家为普通专家，1及其以后的为快照专家
	int mSnapExpertsSize;	//快照专家数量
	
	
public:
	Experts(const Size& mTCxtSize, const Size& mTFrameSize, Config *mTConfig);
	Experts(const Experts& r);
	~Experts();

public:
	Experts& operator=(const Experts& r);

private:
	//擦除一个快照专家
	bool eraseASnapExpert();


public:
	//增加一个快照专家
	void increaseSnapExpert(int mTFrameIndex);

	
	ExpertResult& calculateExpertResult(const Mat& mFrame, const Mat& mFilterWindowMat, int mTFrameIndex);

	void setCenterPoint(const Point& mTPoint);

	//根据提供的当前帧数据矩阵和滤波窗口矩阵计算当前帧的上下文数据矩阵
	void getContext(const Mat& mFrame, const Mat& mFilterWindowMat);

	//设置经过FFT变换的时空上下文模型
	void setSTCModel(const Mat& mTSTCModel);

	void updateSTCModel(double mTLearnRate);

	//计算当前专家的经过FFT变换的空间上下文模型
	Mat& calculateSCModel(const Mat& mTCxtPostProMatFFT);

};