#pragma once

#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include "ConfidenceList.h"
#include "Experts.h"
#include "ExpertResult.h"
#include "Complex.h"
#include "Config.h"

using namespace cv;
using namespace std;

class STCTracker
{
//成员变量
private:

	ofstream out;
	int mFrameIndex;				//当前处理帧的索引
	int mIntervalNumber;	//每隔多少帧图片更新一次尺度参数
	double sigma;			//上下文先验概率公式中的尺度参数，即公式5
	double alpha;			//后验概率（即置信图）公式中的尺度缩放参数，即公式6
	double beta;			//后验概率（即置信图）公式中的控制图形尖锐度的参数，即公式6
	double mLearnRate;		//公式12中的学习参数

	Point mCenter;			//跟踪对象的中心点
	Size2d mRectSize;			//跟踪矩形框的尺寸
	Size mCxtRegionSize;	//上下文区域尺寸
	double mScale;			//尺度缩放比例
	double mScaleLearnRate;	//尺度缩放学习速率

	ConfidenceList *mConfidenceList;//保存邻近的置信度用于估计尺度参数

	Mat mCxtPostProMat;		//上下文后验概率矩阵，即标准置信图
	Mat mCxtPostProFFTMat;	//经过傅里叶转换后的后验概率矩阵

	Mat mFilterWindowMat;	//当前帧的滤波窗口，即公式4中还未乘I(z)时的矩阵内容，作为一个滤波窗口来看待


	Mat mHammingWinMat;		//汉明窗，用于弱化图像边缘带来的频率影响
	Mat mEucliDistMat;		//存储上下文区域中每一个像素点到跟踪对象中心点的欧几里得距离平方，即用于先验概率矩阵，也用于后验概率矩阵

	double mMinConfiPosi;

	Experts *mExperts;

public:
	STCTracker();
	virtual ~STCTracker();

public:
	//初始化方法
	void init(const Mat& mFrame, const Rect& mBox, int mFrameCount, Config *mTConfig);
	//进行每一帧的跟踪
	void track(const Mat& mFrame, Rect& mBox, Config& mTConfig);


private:
	//构造汉明窗
	void createHammingWindow();
	//初始化欧几里得距离矩阵和后验概率矩阵
	void createEucliDistMatAndCxtPostPro();

	void getFilterWindow();

};

